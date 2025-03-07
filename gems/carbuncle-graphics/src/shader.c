#include "carbuncle/core.h"
#include "carbuncle/point.h"
#include "carbuncle/vector3.h"
#include "carbuncle/vector4.h"
#include "carbuncle/rect.h"
#include "carbuncle/color.h"
#include "carbuncle/matrix.h"
#include "carbuncle/texture.h"
#include "carbuncle/shader.h"
#include "carbuncle/filesystem.h"

#include <stdlib.h>

#include "raylib.h"
#include "rlgl.h"
#include "physfs.h"

#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/error.h>
#include <mruby/hash.h>
#include <mruby/variable.h>

#define UNIFORM_VALUES_SYMBOL    mrb_intern_cstr(mrb, "@uniform_values")
#define UNIFORM_TYPES_SYMBOL     mrb_intern_cstr(mrb, "@uniform_types")
#define UNIFORM_LOCATIONS_SYMBOL mrb_intern_cstr(mrb, "@uniform_locations")

struct mrb_Shader
{
  mrb_bool default_shader;
  Shader shader;
};

static void
mrb_shader_free(mrb_state *mrb, void *ptr)
{
  if (ptr)
  {
    struct mrb_Shader *shader = ptr;
    if (!shader->default_shader)
    {
      UnloadShader(shader->shader);
    }
    mrb_free(mrb, shader);
  }
}

static const struct mrb_data_type shader_data_type = {
  "Carbuncle::Shader", mrb_shader_free
};

static inline struct mrb_Shader *
get_shader(mrb_state *mrb, mrb_value obj)
{
  return DATA_GET_DISPOSABLE_PTR(mrb, obj, &shader_data_type, struct mrb_Shader);
}

static void
load_shader(mrb_state *mrb, struct mrb_Shader *shader, const char *vertex, const char *fragment)
{
  if ((vertex && PHYSFS_exists(vertex)) || (fragment && PHYSFS_exists(fragment)))
  {
    shader->shader = LoadShader(vertex, fragment);
  }
  else
  {
    shader->shader = LoadShaderFromMemory(vertex, fragment);
  }
}

static void
send_integer(struct mrb_Shader *shader, mrb_int location, int value)
{
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_INT);
}

static void
send_float(struct mrb_Shader *shader, mrb_int location, float value)
{
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_FLOAT);
}

static void
send_point(struct mrb_Shader *shader, mrb_int location, Vector2 *point)
{
  float value[2] = { point->x, point->y };
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_VEC2);
}

static void
send_vector3(struct mrb_Shader *shader, mrb_int location, Vector3 *vector)
{
  float value[3] = { vector->x, vector->y, vector->z };
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_VEC3);
}

static void
send_vector4(struct mrb_Shader *shader, mrb_int location, Vector4 *vector)
{
  float value[4] = { vector->x, vector->y, vector->z, vector->w };
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_VEC4);
}

static void
send_rect(struct mrb_Shader *shader, mrb_int location, Rectangle *rect)
{
  float value[4] = { rect->x, rect->y, rect->width, rect->height };
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_VEC4);
}

static void
send_color(struct mrb_Shader *shader, mrb_int location, Color *color)
{
  float value[4] = {
    (float)color->r / 255.0f,
    (float)color->g / 255.0f,
    (float)color->b / 255.0f,
    (float)color->a / 255.0f
  };
  SetShaderValue(shader->shader, location, &value, SHADER_UNIFORM_VEC4);
}

static void
send_matrix(struct mrb_Shader *shader, mrb_int location, Matrix *matrix)
{
  SetShaderValueMatrix(shader->shader, location, *matrix);
}

static void
send_texture(struct mrb_Shader *shader, mrb_int location, Texture *texture)
{
  SetShaderValueTexture(shader->shader, location, *texture);
}

mrb_value
mrb_shader_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  const char *first, *second;
  struct mrb_Shader *shader = mrb_malloc(mrb, sizeof *shader);
  shader->default_shader = FALSE;
  argc = mrb_get_args(mrb, "|zz", &first, &second);
  DATA_PTR(self) = shader;
  DATA_TYPE(self) = &shader_data_type;
  mrb_iv_set(mrb, self, UNIFORM_LOCATIONS_SYMBOL, mrb_hash_new(mrb));
  mrb_iv_set(mrb, self, UNIFORM_TYPES_SYMBOL, mrb_hash_new(mrb));
  mrb_iv_set(mrb, self, UNIFORM_VALUES_SYMBOL, mrb_hash_new(mrb));
  switch (argc)
  {
    case 0:
    {
      shader->default_shader = TRUE;
      break;
    }
    case 1:
    {
      if (FileExists(first) && (IsFileExtension(first, ".vert") || IsFileExtension(first, ".vs")))
      {
        load_shader(mrb, shader, first, NULL);
      }
      else
      {
        load_shader(mrb, shader, NULL, first);
      }
      break;
    }
    case 2:
    {
      load_shader(mrb, shader, first, second);
      break;
    }
  }
  return self;
}

mrb_value
mrb_s_shader_get_default(mrb_state *mrb, mrb_value self)
{
  return mrb_obj_new(mrb, mrb_carbuncle_class_get(mrb, "Shader"), 0, NULL);
}

mrb_value
mrb_s_shader_compile(mrb_state *mrb, mrb_value self)
{
  mrb_value *argv;
  mrb_int argc;
  struct RClass *shader = mrb_class_ptr(self);
  mrb_get_args(mrb, "*", &argv, &argc);
  return mrb_obj_new(mrb, shader, argc, argv);
}

mrb_value
mrb_shader_disposedQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(!DATA_PTR(self));
}

mrb_value
mrb_shader_dispose(mrb_state *mrb, mrb_value self)
{
  struct mrb_Shader *shader = get_shader(mrb, self);
  mrb_shader_free(mrb, shader);
  DATA_PTR(self) = NULL;
  return self;
}

mrb_value
mrb_shader_find_uniform_location(mrb_state *mrb, mrb_value self)
{
  const char *name;
  struct mrb_Shader *shader = get_shader(mrb, self);
  mrb_get_args(mrb, "z", &name);
  return mrb_int_value(mrb, GetShaderLocation(shader->shader, name));
}

mrb_value
mrb_shader_send_uniform_value(mrb_state *mrb, mrb_value self)
{
  mrb_int location;
  mrb_value obj;
  struct mrb_Shader *shader = get_shader(mrb, self);
  mrb_get_args(mrb, "io", &location, &obj);
  if (mrb_fixnum_p(obj))
  {
    send_integer(shader, location, mrb_fixnum(obj));
  }
  else if (mrb_float_p(obj))
  {
    send_float(shader, location, mrb_float(obj));
  }
  else if (mrb_carbuncle_point_p(obj))
  {
    send_point(shader, location, mrb_carbuncle_get_point(mrb, obj));
  }
  else if (mrb_carbuncle_vector3_p(obj))
  {
    send_vector3(shader, location, mrb_carbuncle_get_vector3(mrb, obj));
  }
  else if (mrb_carbuncle_vector4_p(obj))
  {
    send_vector4(shader, location, mrb_carbuncle_get_vector4(mrb, obj));
  }
  else if (mrb_carbuncle_rect_p(obj))
  {
    send_rect(shader, location, mrb_carbuncle_get_rect(mrb, obj));
  }
  else if (mrb_carbuncle_color_p(obj))
  {
    send_color(shader, location, mrb_carbuncle_get_color(mrb, obj));
  }
  else if (mrb_carbuncle_matrix_p(obj))
  {
    send_matrix(shader, location, mrb_carbuncle_get_matrix(mrb, obj));
  }
  else if (mrb_carbuncle_texture_p(obj))
  {
    send_texture(shader, location, mrb_carbuncle_get_texture(mrb, obj));
  }
  return self;
}

mrb_value
mrb_shader_begin_render(mrb_state *mrb, mrb_value self)
{
  struct mrb_Shader *data = get_shader(mrb, self);
  mrb_value block = mrb_nil_value();
  mrb_get_args(mrb, "|&", &block);
  if (!data->default_shader) BeginShaderMode(data->shader);
  if (!mrb_nil_p(block))
  {
    mrb_bool raised = FALSE;
    mrb_value result = mrb_protect(mrb, mrb_carbuncle_call_block, block, &raised);

    if (!data->default_shader) EndShaderMode();
    if (raised) { mrb_exc_raise(mrb, result); }
  }
  return self;
}

mrb_value
mrb_shader_end_render(mrb_state *mrb, mrb_value self)
{
  struct mrb_Shader *data = get_shader(mrb, self);
  if (!data->default_shader) EndShaderMode();
  return self;
}

void
mrb_init_carbuncle_shader(mrb_state *mrb)
{
  struct RClass *carbuncle = mrb_carbuncle_get(mrb);
  struct RClass *shader = mrb_define_class_under(mrb, carbuncle, "Shader", mrb->object_class);
  MRB_SET_INSTANCE_TT(shader, MRB_TT_DATA);

  mrb_define_method(mrb, shader, "initialize", mrb_shader_initialize, MRB_ARGS_OPT(2));

  mrb_define_class_method(mrb, shader, "default", mrb_s_shader_get_default, MRB_ARGS_NONE());

  mrb_define_class_method(mrb, shader, "compile", mrb_s_shader_compile, MRB_ARGS_OPT(2));
  mrb_define_class_method(mrb, shader, "load", mrb_s_shader_compile, MRB_ARGS_OPT(2));

  mrb_define_method(mrb, shader, "disposed?", mrb_shader_disposedQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader, "dispose", mrb_shader_dispose, MRB_ARGS_NONE());

  mrb_define_method(mrb, shader, "find_uniform_location", mrb_shader_find_uniform_location, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, shader, "send_uniform_value", mrb_shader_send_uniform_value, MRB_ARGS_REQ(2));

  mrb_define_method(mrb, shader, "render", mrb_shader_begin_render, MRB_ARGS_BLOCK());
  mrb_define_method(mrb, shader, "begin_render", mrb_shader_begin_render, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader, "end_render", mrb_shader_end_render, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader, "draw", mrb_shader_begin_render, MRB_ARGS_BLOCK());
  mrb_define_method(mrb, shader, "begin_draw", mrb_shader_begin_render, MRB_ARGS_NONE());
  mrb_define_method(mrb, shader, "end_draw", mrb_shader_end_render, MRB_ARGS_NONE());  
}
