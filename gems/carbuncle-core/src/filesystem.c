#include "carbuncle/core.h"
#include "carbuncle/filesystem.h"

#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "physfs.h"

#include <string.h>

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/error.h>
#include <mruby/string.h>
#include <mruby/compile.h>
#include <mruby/variable.h>

#include "raylib.h"

#ifdef __EMSCRIPTEN__

EM_JS(void, mrb_file_carbuncle_console_log, (const char *a, const char *b), {
  console.log(UTF8ToString(a) + ' ' + UTF8ToString(b));
});

void
mrb_carbuncle_fetch_file(mrb_state *mrb, const char *filename)
{
  if (PHYSFS_exists(filename))
  {
    return;
  }
  if (strstr(filename, "https://") == filename)
  {
    emscripten_wget(filename, filename);
    mrb_file_carbuncle_console_log(filename, "was loaded.");
    return;
  }
  char *host = emscripten_run_script_string("/.*(?=\\/\\w*)/.exec(window.location.href).join() + '/'");
  char *endpoint = mrb_alloca(mrb, strlen(host) + strlen(filename) + 1);
  strcpy(endpoint, host);
  strcat(endpoint, filename);
  mrb_file_carbuncle_console_log("Loading filename", filename);
  emscripten_wget(endpoint, filename);
  mrb_file_carbuncle_console_log(filename, "was loaded.");
}
#endif

#include "external/stb_image.h"

#define E_FILE_ERROR get_class_by_code(mrb, code)
#define ERROR_MESSAGE PHYSFS_getErrorByCode(code)
#define LOAD_PATHS mrb_intern_cstr(mrb, "#_load_paths_")

#define SEPARATOR mrb_str_new_cstr(mrb, "/")

static struct RClass *
get_class_by_code(mrb_state *mrb, PHYSFS_ErrorCode code)
{
  switch (code)
  {
  case PHYSFS_ERR_OUT_OF_MEMORY:
    return mrb_class_get(mrb, "NoMemoryError");
  case PHYSFS_ERR_NOT_INITIALIZED:
    return mrb_carbuncle_class_get(mrb, "FilesystemNotInitialized");
  case PHYSFS_ERR_IS_INITIALIZED:
    return mrb_carbuncle_class_get(mrb, "FilesystemNotInitialized");
  case PHYSFS_ERR_ARGV0_IS_NULL:
    return mrb_carbuncle_class_get(mrb, "FilesystemNotInitialized");
  case PHYSFS_ERR_UNSUPPORTED:
    return E_NOTIMP_ERROR;
  case PHYSFS_ERR_NOT_FOUND:
    return mrb_carbuncle_class_get(mrb, "FileNotExists");
  case PHYSFS_ERR_NOT_A_FILE:
    return mrb_carbuncle_class_get(mrb, "NotAFile");
  default:
  {
    break;
  }
  }
  return mrb_carbuncle_class_get(mrb, "FileError");
}

static void
raise_physfs_error(mrb_state *mrb, const char *action, const char* file)
{
  PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
  if (file)
  {
    mrb_raisef(mrb, E_FILE_ERROR, "Failed to %s file \"%s\" (%s).", action, file, ERROR_MESSAGE);
  }
  else
  {
    mrb_raisef(mrb, E_FILE_ERROR, "Failed to %s file (%s).", action, ERROR_MESSAGE);
  }
}

static void
mrb_file_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
  {
    PHYSFS_File *file = ptr;
    if (!PHYSFS_close(file))
    {
      raise_physfs_error(mrb, "close", NULL);
    }
  }
}

static const struct mrb_data_type file_data_type = {
    "Carbuncle::File", mrb_file_free};

static PHYSFS_File *
get_file(mrb_state *mrb, mrb_value self)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, self, &file_data_type, PHYSFS_File);
}

static mrb_value
mrb_file_run_block(mrb_state *mrb, mrb_value self)
{
  return mrb_funcall(mrb, mrb_ary_entry(self, 1), "call", 1, mrb_ary_entry(self, 0));
}

static mrb_value
mrb_file_initialize(mrb_state *mrb, mrb_value self)
{
  PHYSFS_File *ptr;
  const char *file, *mode;
  mrb_value block = mrb_nil_value();
  if (mrb_get_args(mrb, "z|z&", &file, &mode, &block) < 2)
  {
    mode = "r";
  }
  if (!strcmp(mode, "r"))
  {
#ifdef __EMSCRIPTEN__
    mrb_carbuncle_fetch_file(mrb, file);
#endif    
    ptr = PHYSFS_openRead(file);
  }
  else if (!strcmp(mode, "w+"))
  {
    ptr = PHYSFS_openAppend(file);
  }
  else if (!strcmp(mode, "+"))
  {
    ptr = PHYSFS_openAppend(file);
  }
  else if (!strcmp(mode, "w"))
  {
    ptr = PHYSFS_openWrite(file);
  }
  else
  {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "Unknown File mode: %s.", mode);
  }
  if (!ptr)
  {
    PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
    mrb_raisef(mrb, E_FILE_ERROR, "File '%s' could not be opened (%s)", file, ERROR_MESSAGE);
  }
  DATA_PTR(self) = ptr;
  DATA_TYPE(self) = &file_data_type;
  if (mrb_carbuncle_test(block))
  {
    mrb_value error;
    mrb_bool raised;
    mrb_value values[2] = {self, block};
    error = mrb_protect(mrb, mrb_file_run_block, mrb_ary_new_from_values(mrb, 2, values), &raised);
    if (!PHYSFS_flush(ptr))
    {
      PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
      mrb_raisef(mrb, E_FILE_ERROR, "Failed to close file (%s).", ERROR_MESSAGE);
    }
    mrb_file_free(mrb, ptr);
    DATA_PTR(self) = NULL;
    if (raised)
    {
      mrb_exc_raise(mrb, error);
    }
  }
  return self;
}

static mrb_value
mrb_file_closedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

static mrb_value
mrb_file_eofQ(mrb_state *mrb, mrb_value self)
{
  PHYSFS_File *file = get_file(mrb, self);
  return mrb_bool_value(PHYSFS_eof(file));
}

static mrb_value
mrb_file_close(mrb_state *mrb, mrb_value self)
{
  PHYSFS_File *file = get_file(mrb, self);
  mrb_file_free(mrb, file);
  DATA_PTR(self) = NULL;
  return self;
}

static mrb_value
mrb_s_file_open(mrb_state *mrb, mrb_value self)
{
  mrb_value block, *values;
  mrb_int size;
  mrb_get_args(mrb, "*&", &values, &size, &block);
  return mrb_funcall_with_block(mrb, self, mrb_intern_cstr(mrb, "new"), size, values, block);
}

static mrb_value
mrb_s_file_get_pwd(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, GetWorkingDirectory());
}

static mrb_value
mrb_s_file_get_base_dir(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, PHYSFS_getBaseDir());
}

static mrb_value
mrb_s_file_get_write_dir(mrb_state *mrb, mrb_value self)
{
  const char *dir = PHYSFS_getWriteDir();
  if (!dir)
  {
    return mrb_nil_value();
  }
  return mrb_str_new_cstr(mrb, dir);
}

static mrb_value
mrb_s_file_get_last_error(mrb_state *mrb, mrb_value self)
{
  PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
  return mrb_str_new_cstr(mrb, PHYSFS_getErrorByCode(code));
}

static mrb_value
mrb_s_file_preference_path_for(mrb_state *mrb, mrb_value self)
{
  const char *org, *product;
  mrb_get_args(mrb, "zz", &org, &product);
  return mrb_str_new_cstr(mrb, PHYSFS_getPrefDir(org, product));
}

static mrb_value
mrb_s_file_absolute_path(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
  return mrb_str_new_cstr(mrb, PHYSFS_getMountPoint(path));
}

static mrb_value
mrb_s_file_expand_path(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
  return mrb_str_new_cstr(mrb, PHYSFS_getRealDir(path));
}

static mrb_value
mrb_s_file_set_write_dir(mrb_state *mrb, mrb_value self)
{
  const char *dir;
  mrb_get_args(mrb, "z", &dir);
  if (!PHYSFS_setWriteDir(dir))
  {
    PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
    mrb_raisef(mrb, E_FILE_ERROR, "Failed to set '%s' as a write directory (%s).", dir, ERROR_MESSAGE);
  }
  return self;
}

static mrb_value
mrb_s_file_existsQ(mrb_state *mrb, mrb_value self)
{
  const char *path;
  mrb_get_args(mrb, "z", &path);
#ifdef __EMSCRIPTEN__
  mrb_carbuncle_fetch_file(mrb, path);
#endif   
  return mrb_bool_value(PHYSFS_exists(path));
}

static mrb_value
mrb_s_file_directoryQ(mrb_state *mrb, mrb_value self)
{
  PHYSFS_Stat stat;
  const char *path;
  mrb_get_args(mrb, "z", &path);
#ifdef __EMSCRIPTEN__
  mrb_carbuncle_fetch_file(mrb, path);
#endif
  PHYSFS_stat(path, &stat);
  return mrb_bool_value(stat.filetype == PHYSFS_FILETYPE_DIRECTORY);
}

static mrb_value
mrb_s_file_linkQ(mrb_state *mrb, mrb_value self)
{
  PHYSFS_Stat stat;
  const char *path;
  mrb_get_args(mrb, "z", &path);
#ifdef __EMSCRIPTEN__
  mrb_carbuncle_fetch_file(mrb, path);
#endif
  PHYSFS_stat(path, &stat);
  return mrb_bool_value(stat.filetype == PHYSFS_FILETYPE_SYMLINK);
}

static mrb_value
mrb_s_file_fileQ(mrb_state *mrb, mrb_value self)
{
  PHYSFS_Stat stat;
  const char *path;
  mrb_get_args(mrb, "z", &path);
#ifdef __EMSCRIPTEN__
  mrb_carbuncle_fetch_file(mrb, path);
#endif  
  PHYSFS_stat(path, &stat);
  return mrb_bool_value(stat.filetype == PHYSFS_FILETYPE_REGULAR);
}

static mrb_value
mrb_file_get_size(mrb_state *mrb, mrb_value self)
{
  PHYSFS_File *file = get_file(mrb, self);
  return mrb_int_value(mrb, PHYSFS_fileLength(file));
}

static mrb_value
mrb_file_seek(mrb_state *mrb, mrb_value self)
{
  mrb_int position;
  PHYSFS_File *file = get_file(mrb, self);
  mrb_get_args(mrb, "i", &position);
  if (!PHYSFS_seek(file, position))
  {
    raise_physfs_error(mrb, "seek", NULL);
  }
  return self;
}

static mrb_value
mrb_file_tell(mrb_state *mrb, mrb_value self)
{
  PHYSFS_File *file = get_file(mrb, self);
  mrb_int pos = PHYSFS_tell(file);
  if (pos == -1)
  {
    raise_physfs_error(mrb, "tell", NULL);
  }
  return mrb_int_value(mrb, pos);
}

static mrb_value
mrb_file_flush(mrb_state *mrb, mrb_value self)
{
  PHYSFS_File *file = get_file(mrb, self);
  if (!PHYSFS_flush(file))
  {
    raise_physfs_error(mrb, "flush", NULL);
  }
  return self;
}

static inline char *
append_to_string(mrb_state *mrb, char *str, size_t *size, size_t *capa, char *buffer, size_t buf_size)
{
  while ((*capa) < ((*size) + buf_size))
  {
    *capa = (*capa) * 2 + 1;
  }
  str = mrb_realloc(mrb, str, (*capa) * sizeof *str);
  memcpy(&(str[*size]), buffer, buf_size);
  *size += buf_size;
  return str;
}

static inline void
read_byte(mrb_state *mrb, PHYSFS_File *file, void *buffer, void *str)
{
  if (PHYSFS_readBytes(file, buffer, 1) == -1)
  {
    if (str)
    {
      mrb_free(mrb, str);
    }
    raise_physfs_error(mrb, "read", NULL);
  }
}

static inline mrb_value
read_line(mrb_state *mrb, PHYSFS_File *file, const char *sep)
{
  mrb_value result;
  char *str;
  mrb_bool loop;
  size_t sep_len, size = 0, capa = 7;
  if (PHYSFS_eof(file))
  {
    return mrb_nil_value();
  }
  sep_len = strlen(sep);
  str = mrb_malloc(mrb, capa * sizeof *str);
  loop = true;
  while (loop)
  {
    char buffer[sep_len];
    read_byte(mrb, file, buffer, str);
    if (buffer[0] == sep[0])
    {
      size_t i = 1;
      while (buffer[i - 1] == sep[i - 1] && i < sep_len)
      {
        read_byte(mrb, file, buffer, str);
        i++;
      }
      if (i == sep_len)
      {
        result = mrb_str_new(mrb, str, size);
        mrb_free(mrb, str);
        return result;
      }
      else
      {
        str = append_to_string(mrb, str, &size, &capa, buffer, i);
      }
    }
    else
    {
      str = append_to_string(mrb, str, &size, &capa, buffer, 1);
    }
    if (PHYSFS_eof(file))
    {
      loop = FALSE;
    }
  }
  result = mrb_str_new(mrb, str, size);
  mrb_free(mrb, str);
  return result;
}

static mrb_value
mrb_file_readline(mrb_state *mrb, mrb_value self)
{
  const char *sep;
  PHYSFS_File *file = get_file(mrb, self);
  sep = "\n";
  mrb_get_args(mrb, "|z", &sep);
  return read_line(mrb, file, sep);
}

static mrb_value
mrb_file_read(mrb_state *mrb, mrb_value self)
{
  mrb_int limit = -1;
  PHYSFS_File *file = get_file(mrb, self);
  PHYSFS_sint64 len, pos;
  mrb_get_args(mrb, "|i", &limit);
  if (limit <= 0)
  {
    len = PHYSFS_fileLength(file);
    if (len == -1)
    {
      raise_physfs_error(mrb, "read", NULL);
    }
    pos = PHYSFS_tell(file);
    if (pos == -1)
    {
      raise_physfs_error(mrb, "read", NULL);
    }
    limit = len - pos;
  }
  if (limit <= 0)
  {
    return mrb_nil_value();
  }
  {
    char *buffer[limit];
    len = PHYSFS_readBytes(file, buffer, limit);
    if (len < 0)
    {
      raise_physfs_error(mrb, "read", NULL);
    }
    return mrb_str_new(mrb, buffer, len);
  }
}

static mrb_value
mrb_file_write(mrb_state *mrb, mrb_value self)
{
  mrb_int size;
  const char *txt;
  PHYSFS_sint64 bytes;
  PHYSFS_File *file = get_file(mrb, self);
  mrb_get_args(mrb, "s", &txt, &size);
  bytes = PHYSFS_writeBytes(file, txt, size);
  if (bytes == -1)
  {
    raise_physfs_error(mrb, "write", NULL);
  }
  return mrb_int_value(mrb, bytes);
}

static mrb_value
mrb_s_file_mount(mrb_state *mrb, mrb_value self)
{
  const char *path, *sub;
  mrb_bool append;
  sub = NULL;
  append = FALSE;
  mrb_get_args(mrb, "z|zb", &path, &sub, &append);
  return mrb_bool_value(PHYSFS_mount(path, sub, append ? 1 : 0));
}

static mrb_value
mrb_s_file_delete(mrb_state *mrb, mrb_value self)
{
  const char *file;
  mrb_get_args(mrb, "z", &file);
  return mrb_bool_value(PHYSFS_delete(file));
}

static mrb_value
mrb_s_file_load(mrb_state *mrb, mrb_value self)
{
  PHYSFS_sint64 length;
  PHYSFS_File *file;
  mrb_value name, result, load_paths;
  mrb_get_args(mrb, "S", &name);
  load_paths = mrb_iv_get(mrb, self, LOAD_PATHS);
  for (mrb_int i = 0; i < RARRAY_LEN(load_paths); ++i)
  {
    mrb_value path = mrb_funcall(mrb, self, "join", 2, mrb_ary_entry(load_paths, i), name);
    mrb_value exists = mrb_funcall(mrb, self, "exists?", 1, path);
    if (mrb_test(exists))
    {
      const char *vname = mrb_str_to_cstr(mrb, name);
      const char *load_name = mrb_str_to_cstr(mrb, path);
      file = PHYSFS_openRead(load_name);
      result = self;
      if (!file)
      {
        PHYSFS_ErrorCode code = PHYSFS_getLastErrorCode();
        mrb_raisef(mrb, E_FILE_ERROR, "File '%s' could not be opened (%s)", vname, ERROR_MESSAGE);
      }
      length = PHYSFS_fileLength(file);
      if (length < 0)
      {
        PHYSFS_ErrorCode code;
        PHYSFS_close(file);
        code = PHYSFS_getLastErrorCode();
        mrb_raisef(mrb, E_FILE_ERROR, "File '%s' could not be read (%s)", vname, ERROR_MESSAGE);
      }
      if (length == 0)
      {
        PHYSFS_close(file);
        return mrb_false_value();
      }
      {
        mrbc_context *ctx;
        char str[length];
        if (PHYSFS_readBytes(file, str, length) == -1)
        {
          PHYSFS_ErrorCode code;
          PHYSFS_close(file);
          code = PHYSFS_getLastErrorCode();
          mrb_raisef(mrb, E_FILE_ERROR, "File '%s' could not be read (%s)", name, ERROR_MESSAGE);
        }
        PHYSFS_close(file);
        ctx = mrbc_context_new(mrb);
        ctx->filename = load_name;
        ctx->capture_errors = TRUE;
        result = mrb_load_nstring_cxt(mrb, str, length, ctx);
        if (mrb->exc)
        {
          mrb_exc_raise(mrb, mrb_obj_value(mrb->exc));
        }
        return result;
      }      
    }
  }
  {
    struct RClass *file_not_exists = mrb_carbuncle_class_get(mrb, "FileNotExists");
    mrb_raisef(mrb, file_not_exists, "File '%s' was not found.", mrb_str_to_cstr(mrb, name));
  }
  return result;
}

static mrb_value
mrb_s_file_get_load_paths(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, LOAD_PATHS);
}

static struct {
  mrb_state *mrb;
} carbuncle_fs_state;

static unsigned char *
raylib_load_file_data_cb(const char *filename, unsigned int *size)
{
  size_t bytes;
  char *result = mrb_carbuncle_load_file(carbuncle_fs_state.mrb, filename, &bytes);
  *size = (unsigned int)bytes;
  return (unsigned char *)result;
}

static bool
raylib_save_file_data_cb(const char *filename, void *data, unsigned int size)
{
  PHYSFS_File *file = PHYSFS_openWrite(filename);
  if (!file) return false;

  PHYSFS_sint64 bytes = PHYSFS_writeBytes(file, data, size);
  PHYSFS_close(file);

  return bytes != -1;
}

static char *
raylib_load_text_cb(const char *filename)
{
  return mrb_carbuncle_load_file_text(carbuncle_fs_state.mrb, filename);
}

static bool
raylib_save_text_cb(const char *filename, char *text)
{
  return raylib_save_file_data_cb(filename, text, strlen(text));
}

void mrb_init_carbuncle_filesystem(mrb_state *mrb)
{
  carbuncle_fs_state.mrb = mrb;

  SetLoadFileDataCallback(raylib_load_file_data_cb);
  SetSaveFileDataCallback(raylib_save_file_data_cb);
  SetLoadFileTextCallback(raylib_load_text_cb);
  SetSaveFileTextCallback(raylib_save_text_cb);

  struct RClass *carbuncle = mrb_carbuncle_get(mrb);

  struct RClass *file = mrb_define_class_under(mrb, carbuncle, "File", mrb->object_class);
  MRB_SET_INSTANCE_TT(file, MRB_TT_DATA);

  mrb_define_method(mrb, file, "initialize", mrb_file_initialize, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1) | MRB_ARGS_BLOCK());

  mrb_define_method(mrb, file, "closed?", mrb_file_closedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, file, "eof?", mrb_file_eofQ, MRB_ARGS_NONE());

  mrb_define_method(mrb, file, "size", mrb_file_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, file, "length", mrb_file_get_size, MRB_ARGS_NONE());
  mrb_define_method(mrb, file, "position", mrb_file_tell, MRB_ARGS_NONE());

  mrb_define_method(mrb, file, "close", mrb_file_close, MRB_ARGS_NONE());

  mrb_define_method(mrb, file, "seek", mrb_file_seek, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, file, "tell", mrb_file_tell, MRB_ARGS_NONE());
  mrb_define_method(mrb, file, "flush", mrb_file_flush, MRB_ARGS_NONE());

  mrb_define_method(mrb, file, "readline", mrb_file_readline, MRB_ARGS_OPT(1));

  mrb_define_method(mrb, file, "read", mrb_file_read, MRB_ARGS_OPT(1));
  mrb_define_method(mrb, file, "write", mrb_file_write, MRB_ARGS_REQ(1));

  mrb_define_class_method(mrb, file, "open", mrb_s_file_open, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1) | MRB_ARGS_BLOCK());

  mrb_define_class_method(mrb, file, "pwd", mrb_s_file_get_pwd, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, file, "base_dir", mrb_s_file_get_base_dir, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, file, "write_dir", mrb_s_file_get_write_dir, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, file, "last_error", mrb_s_file_get_last_error, MRB_ARGS_NONE());

  mrb_define_class_method(mrb, file, "write_dir=", mrb_s_file_set_write_dir, MRB_ARGS_REQ(1));

  mrb_define_class_method(mrb, file, "preference_path_for", mrb_s_file_preference_path_for, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, file, "save_path_for", mrb_s_file_preference_path_for, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, file, "preference_path_of", mrb_s_file_preference_path_for, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, file, "save_path_of", mrb_s_file_preference_path_for, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, file, "preference_path", mrb_s_file_preference_path_for, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, file, "save_path", mrb_s_file_preference_path_for, MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, file, "absolute_path_for", mrb_s_file_absolute_path, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "absolute_path_of", mrb_s_file_absolute_path, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "absolute_path", mrb_s_file_absolute_path, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "expand_path", mrb_s_file_expand_path, MRB_ARGS_REQ(1));

  mrb_define_class_method(mrb, file, "exists?", mrb_s_file_existsQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "exist?", mrb_s_file_existsQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "directory?", mrb_s_file_directoryQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "dir?", mrb_s_file_directoryQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "link?", mrb_s_file_linkQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "symbolic?", mrb_s_file_linkQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "symbolic_link?", mrb_s_file_linkQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "symlink?", mrb_s_file_linkQ, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, file, "file?", mrb_s_file_fileQ, MRB_ARGS_REQ(1));

  mrb_define_class_method(mrb, file, "mount", mrb_s_file_mount, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(2));

  mrb_define_class_method(mrb, file, "delete", mrb_s_file_delete, MRB_ARGS_REQ(1));

  mrb_define_class_method(mrb, file, "load", mrb_s_file_load, MRB_ARGS_REQ(1));

  mrb_define_const(mrb, file, "SEPARATOR", SEPARATOR);
  mrb_define_const(mrb, file, "PATH_SEPARATOR", SEPARATOR);

  {
    mrb_value load_path[] = {
      mrb_str_new_cstr(mrb, ""),
      mrb_str_new_cstr(mrb, "src"),
      mrb_str_new_cstr(mrb, "scripts"),
    };
    mrb_iv_set(mrb, mrb_obj_value(file), LOAD_PATHS, mrb_ary_new_from_values(mrb, 3, load_path));
    mrb_define_class_method(mrb, file, "load_paths", mrb_s_file_get_load_paths, MRB_ARGS_NONE());
  }
}

char *
mrb_carbuncle_load_file(mrb_state *mrb, const char *filename, size_t *size)
{
  char *result;
  PHYSFS_File *file;
#ifdef __EMSCRIPTEN__
  mrb_carbuncle_fetch_file(mrb, filename);
#endif  
  file = PHYSFS_openRead(filename);
  if (!file)
  {
    raise_physfs_error(mrb, "open", filename);
  }
  *size = PHYSFS_fileLength(file);
  if (*size == -1)
  {
    raise_physfs_error(mrb, "read", filename);
  }
  result = mrb_malloc(mrb, (*size) * sizeof *result);
  if (PHYSFS_readBytes(file, result, *size) == -1)
  {
    mrb_free(mrb, result);
    raise_physfs_error(mrb, "read", filename);
  }
  if (!PHYSFS_close(file))
  {
    mrb_free(mrb, result);
    raise_physfs_error(mrb, "close", filename);
  }
  return result;
}

static inline mrb_bool
is_image_extension(const char *filename)
{
  return IsFileExtension(filename, ".png") ||
         IsFileExtension(filename, ".bmp") ||
         IsFileExtension(filename, ".tga") ||
         IsFileExtension(filename, ".jpg") ||
         IsFileExtension(filename, ".gif") ||
         IsFileExtension(filename, ".pic") ||
         IsFileExtension(filename, ".psd");
}

char *
mrb_carbuncle_load_file_text(mrb_state *mrb, const char *filename)
{
  size_t byte_size;
  char *bytes;
#ifdef __EMSCRIPTEN__
  mrb_carbuncle_fetch_file(mrb, filename);
#endif
  bytes = mrb_carbuncle_load_file(mrb, filename, &byte_size);
  bytes = mrb_realloc(mrb, bytes, byte_size + 1);
  bytes[byte_size] = '\0';
  return bytes;
}

Image LoadCarbuncleImage(mrb_state *mrb, const char *filename)
{
  return LoadImage(filename);
}

Texture
LoadCarbuncleTexture(mrb_state *mrb, const char *filename)
{
  return LoadTexture(filename);
}

Music
LoadCarbuncleMusic(mrb_state *mrb, const char *filename)
{
  return LoadMusicStream(filename);
}

Sound
LoadCarbuncleSound(mrb_state *mrb, const char *filename)
{
  return LoadSound(filename);
}

void mrb_carbuncle_file_error(mrb_state *mrb, const char *action)
{
  raise_physfs_error(mrb, action, NULL);
}