#include <mruby.h>
#include <mruby/string.h>
#include <mruby/hash.h>
#include <mruby/value.h>
#include <mruby/variable.h>

#include <stdlib.h>
#include <string.h>

#include "raylib.h"

#include "carbuncle/core.h"
#include "carbuncle/loader.h"

#define PRELOADED_SYMBOL mrb_intern_cstr(mrb, "#preloaded")

static mrb_int total_loading_files;
static mrb_int loading_files;
static mrb_int errored_files;

static mrb_value
get_loader(mrb_state *mrb)
{
  return mrb_obj_value(mrb_carbuncle_module_get(mrb, "Loader"));
}

static mrb_value
get_preloaded(mrb_state *mrb, mrb_value self)
{
  mrb_value result = mrb_iv_get(mrb, self, PRELOADED_SYMBOL);
  if (!mrb_test(result))
  {
    result = mrb_hash_new(mrb);
     mrb_iv_set(mrb, self, PRELOADED_SYMBOL, result);
  }
  return result;
}

#ifdef __EMSCRIPTEN__

#include <emscripten/emscripten.h>

static void
on_plugin_ok(const char *filename)
{
  loading_files--;
}

static void
on_plugin_error(const char *filename)
{
  loading_files--;
  errored_files++;
}

static void
on_file_loaded(unsigned int _, void *p, const char *filename)
{
  emscripten_run_preload_plugins(filename, on_plugin_ok, on_plugin_error);
  mrb_state *mrb = (mrb_state *)p;
}

static void
on_file_error(unsigned int _, void *p, int code)
{
  mrb_state *mrb = (mrb_state *)p;
  loading_files--;
  errored_files++;
}

static void
on_file_progress(unsigned int _, void *p, int progress)
{
}

#endif

mrb_value
mrb_s_loader_prepare(mrb_state *mrb, mrb_value self)
{
  mrb_value file, preloaded;
  const char *filename;
  mrb_get_args(mrb, "S", &file);
  preloaded = get_preloaded(mrb, self);
  if (mrb_hash_key_p(mrb, preloaded, file))
  {
    return mrb_false_value();
  }
#ifdef __EMSCRIPTEN__
  filename = RSTRING_CSTR(mrb, file);
  if (strstr(filename, "https://") == filename)
  {
    total_loading_files++;
    loading_files++;
    emscripten_async_wget2(
      filename, filename, "GET", "",
      mrb, on_file_loaded, on_file_error, on_file_progress
    );    
    mrb_hash_set(mrb, preloaded, file, mrb_true_value());
    return mrb_true_value();
  }
  char *host = emscripten_run_script_string("/.*(?=\\/\\w*)/.exec(window.location.href).join() + '/'");
  char *endpoint = mrb_alloca(mrb, strlen(host) + strlen(filename) + 1);
  strcpy(endpoint, host);
  strcat(endpoint, filename);
  total_loading_files++;
  loading_files++;
  emscripten_async_wget2(
    endpoint, filename, "GET", "",
    mrb, on_file_loaded, on_file_error, on_file_progress
  );
#endif
  mrb_hash_set(mrb, preloaded, file, mrb_true_value());
  return mrb_true_value();
}

mrb_value
mrb_s_loader_clear(mrb_state *mrb, mrb_value self)
{
  total_loading_files = 0;
  return self;
}

mrb_value
mrb_s_loader_rate(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, mrb_carbuncle_loader_rate(mrb));
}

mrb_value
mrb_s_loader_loadingQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(mrb_carbuncle_loader_loading_p(mrb));
}

mrb_value
mrb_s_loader_readyQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!mrb_carbuncle_loader_loading_p(mrb));
}

mrb_value
mrb_s_loader_errorQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(mrb_carbuncle_loader_error_p(mrb));
}

void
mrb_init_carbuncle_loader(mrb_state *mrb)
{
  total_loading_files = 0;
  loading_files = 0;
  errored_files = 0;
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);

  struct RClass *loader = mrb_define_module_under(mrb, carbuncle, "Loader");

  mrb_define_module_function(mrb, loader, "clear", mrb_s_loader_clear, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, loader, "rate", mrb_s_loader_rate, MRB_ARGS_NONE());

  mrb_define_module_function(mrb, loader, "prepare", mrb_s_loader_prepare, MRB_ARGS_REQ(1));
  mrb_define_module_function(mrb, loader, "loading?", mrb_s_loader_loadingQ, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, loader, "ready?", mrb_s_loader_readyQ, MRB_ARGS_NONE());
  mrb_define_module_function(mrb, loader, "error?", mrb_s_loader_errorQ, MRB_ARGS_NONE());
}

mrb_bool
mrb_carbuncle_loader_loading_p(mrb_state *mrb)
{
  return loading_files > 0;
}

mrb_bool
mrb_carbuncle_loader_error_p(mrb_state *mrb)
{
  return errored_files > 0;
}

mrb_float
mrb_carbuncle_loader_rate(mrb_state *mrb)
{
  if (total_loading_files <= 0) return 1.0;

  mrb_float total = total_loading_files;
  mrb_float left = loading_files;
  return 1.0 - left / total;
}