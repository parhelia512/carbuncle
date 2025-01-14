#ifndef CARBUNCLE_CORE_H
#define CARBUNCLE_CORE_H

#define SUPPORT_FILEFORMAT_TTF 1

#include <mruby.h>
#include <mruby/data.h>

#include "raylib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DATA_GET_DISPOSABLE_PTR(mrb, obj, type, cast) (cast *)mrb_check_disposed(mrb, obj, type);

#ifdef CARBUNCLE_DEBUG
extern mrb_bool mrb_carbuncle_debug_drawing;
extern mrb_int mrb_carbuncle_debug_color_index;
#endif

void
mrb_carbuncle_raise(mrb_state *mrb, const char *name, const char *msg);

void
mrb_carbuncle_check_frozen(mrb_state *mrb, mrb_value obj);

void
mrb_carbuncle_arg_error(mrb_state *mrb, const char *options, mrb_int argc);

void *
mrb_check_disposed(mrb_state *mrb, mrb_value obj, const struct mrb_data_type *type);

void
mrb_carbuncle_check_file(mrb_state *mrb, const char *filename);

#ifdef __EMSCRIPTEN__
void
mrb_carbuncle_fetch_file(mrb_state *mrb, const char *filename);
#endif

struct RClass *
mrb_carbuncle_class_get(mrb_state *mrb, const char *name);

struct RClass *
mrb_carbuncle_module_get(mrb_state *mrb, const char *name);

struct RClass *
mrb_carbuncle_get(mrb_state *mrb);

#ifdef CARBUNCLE_DEBUG
void
mrb_carbuncle_draw_debug_rect(Rectangle rect, Vector2 origin, float rotation);
#endif

static inline int
carbuncle_utf8_encode(char *out, uint32_t utf)
{
  if (utf <= 0x7F) {
    // Plain ASCII
    out[0] = (char) utf;
    out[1] = 0;
    return 1;
  }
  else if (utf <= 0x07FF) {
    // 2-byte unicode
    out[0] = (char) (((utf >> 6) & 0x1F) | 0xC0);
    out[1] = (char) (((utf >> 0) & 0x3F) | 0x80);
    out[2] = 0;
    return 2;
  }
  else if (utf <= 0xFFFF) {
    // 3-byte unicode
    out[0] = (char) (((utf >> 12) & 0x0F) | 0xE0);
    out[1] = (char) (((utf >>  6) & 0x3F) | 0x80);
    out[2] = (char) (((utf >>  0) & 0x3F) | 0x80);
    out[3] = 0;
    return 3;
  }
  else if (utf <= 0x10FFFF) {
    // 4-byte unicode
    out[0] = (char) (((utf >> 18) & 0x07) | 0xF0);
    out[1] = (char) (((utf >> 12) & 0x3F) | 0x80);
    out[2] = (char) (((utf >>  6) & 0x3F) | 0x80);
    out[3] = (char) (((utf >>  0) & 0x3F) | 0x80);
    out[4] = 0;
    return 4;
  }
  else { 
    // error - use replacement character
    out[0] = (char) 0xEF;  
    out[1] = (char) 0xBF;
    out[2] = (char) 0xBD;
    out[3] = 0;
    return 0;
  }
}

mrb_value
mrb_carbuncle_call_block(mrb_state *mrb, mrb_value block);

char *
mrb_carbuncle_load_file(mrb_state *mrb, const char *filename, size_t *size);

void
mrb_carbuncle_file_error(mrb_state *mrb, const char *action);

static inline mrb_bool
mrb_carbuncle_test(mrb_value obj)
{
  return !(mrb_false_p(obj) || mrb_undef_p(obj) || mrb_nil_p(obj));
}

void
mrb_carbuncle_add_buffer(void *key, void *value);

void *
mrb_carbuncle_get_buffer(void *key);

void
mrb_carbuncle_remove_buffer(void *key);

#ifdef __cplusplus
}
#endif

#endif
