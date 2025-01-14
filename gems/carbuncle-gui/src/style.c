#include "carbuncle/core.h"
#include "carbuncle/font.h"
#include "carbuncle/rect.h"
#include "carbuncle/point.h"
#include "carbuncle/texture.h"

#include <carbuncle/nuklear_config.h>
#include <nuklear.h>

#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/error.h>
#include <mruby/array.h>

#include <raylib.h>

#define BUTTON_SYMBOL mrb_intern_cstr(mrb, "#button")
#define CHART_SYMBOL mrb_intern_cstr(mrb, "#chart")
#define CHECKBOX_SYMBOL mrb_intern_cstr(mrb, "#checkbox")
#define COMBO_SYMBOL mrb_intern_cstr(mrb, "#combo")
#define CONTEXTUAL_BUTTON_SYMBOL mrb_intern_cstr(mrb, "#contextual_button")
#define CURSOR_ACTIVE_SYMBOL mrb_intern_cstr(mrb, "#cursor_active")
#define CURSOR_LAST_SYMBOL mrb_intern_cstr(mrb, "#cursor_last")
#define CURSOR_VISIBLE_SYMBOL mrb_intern_cstr(mrb, "#cursor_visible")
#define CURSORS_SYMBOL mrb_intern_cstr(mrb, "#cursors")
#define EDIT_SYMBOL mrb_intern_cstr(mrb, "#edit")
#define MENU_BUTTON_SYMBOL mrb_intern_cstr(mrb, "#menu_button")
#define OPTION_SYMBOL mrb_intern_cstr(mrb, "#option")
#define PROGRESS_SYMBOL mrb_intern_cstr(mrb, "#progress")
#define PROPERTY_SYMBOL mrb_intern_cstr(mrb, "#property")
#define SCROLLH_SYMBOL mrb_intern_cstr(mrb, "#scrollh")
#define SCROLLV_SYMBOL mrb_intern_cstr(mrb, "#scrollv")
#define SELECTABLE_SYMBOL mrb_intern_cstr(mrb, "#selectable")
#define SLIDER_SYMBOL mrb_intern_cstr(mrb, "#slider")
#define TAB_SYMBOL mrb_intern_cstr(mrb, "#tab")
#define TEXT_SYMBOL mrb_intern_cstr(mrb, "#text")
#define WINDOW_SYMBOL mrb_intern_cstr(mrb, "#window")

#define HEADER mrb_intern_cstr(mrb, "#header")
#define MENU_BORDER_COLOR mrb_intern_cstr(mrb, "#menu_border_color")
#define MENU_PADDING mrb_intern_cstr(mrb, "#menu_padding")
#define MIN_SIZE mrb_intern_cstr(mrb, "#min_size")
#define PADDING mrb_intern_cstr(mrb, "#padding")
#define POPUP_BORDER_COLOR mrb_intern_cstr(mrb, "#popup_border_color")
#define POPUP_PADDING mrb_intern_cstr(mrb, "#popup_padding")
#define SCALER mrb_intern_cstr(mrb, "#scaler")
#define SCROLLBAR_SIZE mrb_intern_cstr(mrb, "#scrollbar_size")
#define SPACING mrb_intern_cstr(mrb, "#spacing")
#define TOOLTIP_BORDER_COLOR mrb_intern_cstr(mrb, "#tooltip_border_color")
#define TOOLTIP_PADDING mrb_intern_cstr(mrb, "#tooltip_padding")
#define ACTIVE mrb_intern_cstr(mrb, "#active")
#define CLOSE_BUTTON mrb_intern_cstr(mrb, "#close_button")
#define HOVER mrb_intern_cstr(mrb, "#hover")
#define LABEL_ACTIVE mrb_intern_cstr(mrb, "#active")
#define LABEL_HOVER mrb_intern_cstr(mrb, "#label_hover")
#define LABEL_NORMAL mrb_intern_cstr(mrb, "#label_normal")
#define LABEL_PADDING mrb_intern_cstr(mrb, "#label_padding")
#define MINIMIZE_BUTTON mrb_intern_cstr(mrb, "#minimize_button")
#define NORMAL mrb_intern_cstr(mrb, "#normal")
#define COLOR mrb_intern_cstr(mrb, "#color")
#define IMAGE mrb_intern_cstr(mrb, "#image")
#define REGION mrb_intern_cstr(mrb, "#region")
#define TEXTURE mrb_intern_cstr(mrb, "#texture")
#define OFFSET mrb_intern_cstr(mrb, "#offset")
#define SIZE mrb_intern_cstr(mrb, "#size")
#define BORDER_COLOR mrb_intern_cstr(mrb, "#border_color")
#define IMAGE_PADDING mrb_intern_cstr(mrb, "#image_padding")
#define TEXT_ACTIVE mrb_intern_cstr(mrb, "#text_active")
#define TEXT_BACKGROUND mrb_intern_cstr(mrb, "#text_background")
#define TEXT_HOVER mrb_intern_cstr(mrb, "#text_hover")
#define TEXT_NORMAL mrb_intern_cstr(mrb, "#text_normal")
#define TOUCH_PADDING mrb_intern_cstr(mrb, "#touch_padding")
#define BACKGROUND mrb_intern_cstr(mrb, "#background")
#define NODE_MAXIMIZE_BUTTON mrb_intern_cstr(mrb, "#node_maximize_button")
#define NODE_MINIMIZE_BUTTON mrb_intern_cstr(mrb, "#node_minimize_button")
#define TAB_MAXIMIZE_BUTTON mrb_intern_cstr(mrb, "#tab_maximize_button")
#define TAB_MINIMIZE_BUTTON mrb_intern_cstr(mrb, "#tab_minimize_button")
#define TEXT mrb_intern_cstr(mrb, "#text")
#define BUTTON mrb_intern_cstr(mrb, "#button")
#define CONTENT_PADDING mrb_intern_cstr(mrb, "#content_padding")
#define SYMBOL_ACTIVE mrb_intern_cstr(mrb, "#symbol_active")
#define SYMBOL_HOVER mrb_intern_cstr(mrb, "#symbol_hover")
#define SYMBOL_NORMAL mrb_intern_cstr(mrb, "#symbol_normal")
#define SELECTED_COLOR mrb_intern_cstr(mrb, "#selected_color")
#define DEC_BUTTON mrb_intern_cstr(mrb, "#dec_button")
#define EDIT mrb_intern_cstr(mrb, "#edit")
#define INC_BUTTON mrb_intern_cstr(mrb, "#inc_button")
#define CURSOR_HOVER mrb_intern_cstr(mrb, "#cursor_hover")
#define CURSOR_NORMAL mrb_intern_cstr(mrb, "#cursor_normal")
#define CURSOR_TEXT_HOVER mrb_intern_cstr(mrb, "#cursor_text_hover")
#define CURSOR_TEXT_NORMAL mrb_intern_cstr(mrb, "#cursor_text_normal")
#define SCROLLBAR mrb_intern_cstr(mrb, "#scrollbar")
#define SELECTED_HOVER mrb_intern_cstr(mrb, "#selected_hover")
#define SELECTED_NORMAL mrb_intern_cstr(mrb, "#selected_normal")
#define SELECTED_TEXT_HOVER mrb_intern_cstr(mrb, "#selected_text_hover")
#define SELECTED_TEXT_NORMAL mrb_intern_cstr(mrb, "#selected_text_normal")
#define HOVER_ACTIVE mrb_intern_cstr(mrb, "#hover_active")
#define NORMAL_ACTIVE mrb_intern_cstr(mrb, "#normal_active")
#define PRESSED mrb_intern_cstr(mrb, "#pressed")
#define PRESSED_ACTIVE mrb_intern_cstr(mrb, "#pressed_active")
#define TEXT_HOVER_ACTIVE mrb_intern_cstr(mrb, "#text_hover_active")
#define TEXT_NORMAL_ACTIVE mrb_intern_cstr(mrb, "#text_normal_active")
#define TEXT_PRESSED mrb_intern_cstr(mrb, "#text_pressed")
#define TEXT_PRESSED_ACTIVE mrb_intern_cstr(mrb, "#text_pressed_active")
#define CURSOR_ACTIVE mrb_intern_cstr(mrb, "#cursor_active")
#define CURSOR_BORDER_COLOR mrb_intern_cstr(mrb, "#cursor_border_color")
#define BAR_ACTIVE mrb_intern_cstr(mrb, "#bar_active")
#define BAR_FILLED mrb_intern_cstr(mrb, "#bar_filled")
#define BAR_HOVER mrb_intern_cstr(mrb, "#bar_hover")
#define BAR_NORMAL mrb_intern_cstr(mrb, "#bar_normal")
#define CURSOR_SIZE mrb_intern_cstr(mrb, "#cursor_size")

void
mrb_style_free(mrb_state *mrb, void *ptr) {}

static const struct mrb_data_type style_data_type = {
  "Carbuncle::GUI::Style", mrb_style_free
};

static struct nk_style *
get_style(mrb_state *mrb, mrb_value self)
{
  struct nk_style *data;
  Data_Get_Struct(mrb, self, &style_data_type, data);
  return data;
}

static struct RClass *
get_style_class(mrb_state *mrb, const char *name)
{
  struct RClass *gui = mrb_carbuncle_class_get(mrb, "GUI");
  struct RClass *style = mrb_class_get_under(mrb, gui, "Style");
  return mrb_class_get_under(mrb, style, name);
}

static mrb_value
new_style_object(mrb_state *mrb, const char *class_name, void *ptr, const struct mrb_data_type *data_type)
{
  struct RClass *style_class = get_style_class(mrb, class_name);
  mrb_value obj = mrb_obj_new(mrb, style_class, 0, NULL);
  DATA_TYPE(obj) = data_type;
  DATA_PTR(obj) = ptr;
  return obj;
}

static const struct mrb_data_type style_image_region_data_type = {
  "Carbuncle::GUI::Style::Image::Region", mrb_style_free
};

static unsigned short *
get_image_region(mrb_state *mrb, mrb_value self)
{
  unsigned short *ptr;
  Data_Get_Struct(mrb, self, &style_image_region_data_type, ptr);
  return ptr;
}

static mrb_value
new_image_region(mrb_state *mrb, unsigned short *ptr)
{
  struct RClass *style_class = get_style_class(mrb, "Image");
  struct RClass *region = mrb_class_get_under(mrb, style_class, "Region");
  mrb_value self = mrb_obj_new(mrb, region, 0, NULL);
  DATA_TYPE(self) = &style_image_region_data_type;
  DATA_PTR(self) = ptr;
  return self;
}

static const struct mrb_data_type style_image_data_type = {
  "Carbuncle::GUI::Style::Image", mrb_style_free
};

static struct nk_image *
get_image(mrb_state *mrb, mrb_value self)
{
  struct nk_image *ptr;
  Data_Get_Struct(mrb, self, &style_image_data_type, ptr);
  return ptr;
}

static mrb_value
new_image(mrb_state *mrb, struct nk_image *ptr)
{
  mrb_value self = new_style_object(mrb, "Image", ptr, &style_image_data_type);
  mrb_iv_set(mrb, self, REGION, new_image_region(mrb, ptr->region));
  return self;
}

static const struct mrb_data_type style_color_data_type = {
  "Carbuncle::GUI::Style::Color", mrb_style_free
};

static struct nk_color *
get_color(mrb_state *mrb, mrb_value self)
{
  struct nk_color *ptr;
  Data_Get_Struct(mrb, self, &style_color_data_type, ptr);
  return ptr;
}

static mrb_value
new_color(mrb_state *mrb, struct nk_color *ptr)
{
  return new_style_object(mrb, "Color", ptr, &style_color_data_type);
}

static const struct mrb_data_type style_item_data_type = {
  "Carbuncle::GUI::Style::Item", mrb_style_free
};

static struct nk_style_item *
get_item(mrb_state *mrb, mrb_value self)
{
  struct nk_style_item *ptr;
  Data_Get_Struct(mrb, self, &style_item_data_type, ptr);
  return ptr;
}

static mrb_value
new_item(mrb_state *mrb, struct nk_style_item *ptr)
{
  mrb_value self = new_style_object(mrb, "Item", ptr, &style_item_data_type);
  mrb_iv_set(mrb, self, COLOR, new_color(mrb, &ptr->data.color));
  mrb_iv_set(mrb, self, IMAGE, new_image(mrb, &ptr->data.image));
  return self;
}

static const struct mrb_data_type style_vec2_data_type = {
  "Carbuncle::GUI::Style::Vec2", mrb_style_free
};

static struct nk_vec2 *
get_vec2(mrb_state *mrb, mrb_value self)
{
  struct nk_vec2 *ptr;
  Data_Get_Struct(mrb, self, &style_vec2_data_type, ptr);
  return ptr;
}

static mrb_value
new_vec2(mrb_state *mrb, struct nk_vec2 *ptr)
{
  return new_style_object(mrb, "Vec2", ptr, &style_vec2_data_type);
}

static const struct mrb_data_type style_cursor_data_type = {
  "Carbuncle::GUI::Style::Cursor", mrb_style_free
};

static struct nk_cursor *
get_cursor(mrb_state *mrb, mrb_value self)
{
  struct nk_cursor *ptr;
  Data_Get_Struct(mrb, self, &style_cursor_data_type, ptr);
  return ptr;
}

static mrb_value
new_cursor(mrb_state *mrb, const struct nk_cursor *ptr, mrb_bool frozen)
{
  mrb_value self = new_style_object(mrb, "Cursor", ptr, &style_cursor_data_type);
  if (frozen)
  {
    return mrb_obj_freeze(mrb, self);
  }
  mrb_iv_set(mrb, self, IMAGE, new_image(mrb, &(ptr->img)));
  mrb_iv_set(mrb, self, OFFSET, new_vec2(mrb, &(ptr->offset)));
  mrb_iv_set(mrb, self, SIZE, new_vec2(mrb, &(ptr->size)));
  return self;
}

static const struct mrb_data_type style_text_data_type = {
  "Carbuncle::GUI::Style::Text", mrb_style_free
};

static struct nk_style_text *
get_text(mrb_state *mrb, mrb_value self)
{
  struct nk_style_text *ptr;
  Data_Get_Struct(mrb, self, &style_text_data_type, ptr);
  return ptr;
}

static mrb_value
new_text(mrb_state *mrb, struct nk_style_text *ptr)
{
  mrb_value self =  new_style_object(mrb, "Text", ptr, &style_text_data_type);
  mrb_iv_set(mrb, self, COLOR, new_color(mrb, &(ptr->color)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  return self;
}

static const struct mrb_data_type style_button_data_type = {
  "Carbuncle::GUI::Style::Button", mrb_style_free
};

static struct nk_style_button *
get_button(mrb_state *mrb, mrb_value self)
{
  struct nk_style_button *ptr;
  Data_Get_Struct(mrb, self, &style_button_data_type, ptr);
  return ptr;
}

static mrb_value
new_button(mrb_state *mrb, struct nk_style_button *ptr)
{
  mrb_value self = new_style_object(mrb, "Button", ptr, &style_button_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, IMAGE_PADDING, new_vec2(mrb, &(ptr->image_padding)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, TEXT_ACTIVE, new_color(mrb, &(ptr->text_active)));
  mrb_iv_set(mrb, self, TEXT_BACKGROUND, new_color(mrb, &(ptr->text_background)));
  mrb_iv_set(mrb, self, TEXT_HOVER, new_color(mrb, &(ptr->text_hover)));
  mrb_iv_set(mrb, self, TEXT_NORMAL, new_color(mrb, &(ptr->text_normal)));
  mrb_iv_set(mrb, self, TOUCH_PADDING, new_vec2(mrb, &(ptr->touch_padding)));
  return self;
}

static const struct mrb_data_type style_toggle_data_type = {
  "Carbuncle::GUI::Style::Toggle", mrb_style_free
};

static struct nk_style_toggle *
get_toggle(mrb_state *mrb, mrb_value self)
{
  struct nk_style_toggle *ptr;
  Data_Get_Struct(mrb, self, &style_toggle_data_type, ptr);
  return ptr;
}

static mrb_value
new_toggle(mrb_state *mrb, struct nk_style_toggle *ptr)
{
  mrb_value self = new_style_object(mrb, "Toggle", ptr, &style_toggle_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, CURSOR_HOVER, new_item(mrb, &(ptr->cursor_hover)));
  mrb_iv_set(mrb, self, CURSOR_NORMAL, new_item(mrb, &(ptr->cursor_normal)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, TEXT_ACTIVE, new_color(mrb, &(ptr->text_active)));
  mrb_iv_set(mrb, self, TEXT_BACKGROUND, new_color(mrb, &(ptr->text_background)));
  mrb_iv_set(mrb, self, TEXT_HOVER, new_color(mrb, &(ptr->text_hover)));
  mrb_iv_set(mrb, self, TEXT_NORMAL, new_color(mrb, &(ptr->text_normal)));
  mrb_iv_set(mrb, self, TOUCH_PADDING, new_vec2(mrb, &(ptr->touch_padding)));
  return self;
}

static const struct mrb_data_type style_selectable_data_type = {
  "Carbuncle::GUI::Style::Selectable", mrb_style_free
};

static struct nk_style_selectable *
get_selectable(mrb_state *mrb, mrb_value self)
{
  struct nk_style_selectable *ptr;
  Data_Get_Struct(mrb, self, &style_selectable_data_type, ptr);
  return ptr;
}

static mrb_value
new_selectable(mrb_state *mrb, struct nk_style_selectable *ptr)
{
  mrb_value self = new_style_object(mrb, "Selectable", ptr, &style_selectable_data_type);
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, HOVER_ACTIVE, new_item(mrb, &(ptr->hover_active)));
  mrb_iv_set(mrb, self, IMAGE_PADDING, new_vec2(mrb, &(ptr->image_padding)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, NORMAL_ACTIVE, new_item(mrb, &(ptr->normal_active)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, PRESSED, new_item(mrb, &(ptr->pressed)));
  mrb_iv_set(mrb, self, PRESSED_ACTIVE, new_item(mrb, &(ptr->pressed_active)));
  mrb_iv_set(mrb, self, TEXT_BACKGROUND, new_color(mrb, &(ptr->text_background)));
  mrb_iv_set(mrb, self, TEXT_HOVER, new_color(mrb, &(ptr->text_hover)));
  mrb_iv_set(mrb, self, TEXT_HOVER_ACTIVE, new_color(mrb, &(ptr->text_hover_active)));
  mrb_iv_set(mrb, self, TEXT_NORMAL, new_color(mrb, &(ptr->text_normal)));
  mrb_iv_set(mrb, self, TEXT_NORMAL_ACTIVE, new_color(mrb, &(ptr->text_normal_active)));
  mrb_iv_set(mrb, self, TEXT_PRESSED, new_color(mrb, &(ptr->text_pressed)));
  mrb_iv_set(mrb, self, TEXT_PRESSED_ACTIVE, new_color(mrb, &(ptr->text_pressed_active)));
  mrb_iv_set(mrb, self, TOUCH_PADDING, new_vec2(mrb, &(ptr->touch_padding)));
  return self;
}

static const struct mrb_data_type style_slider_data_type = {
  "Carbuncle::GUI::Style::Slider", mrb_style_free
};

static struct nk_style_slider *
get_slider(mrb_state *mrb, mrb_value self)
{
  struct nk_style_slider *ptr;
  Data_Get_Struct(mrb, self, &style_slider_data_type, ptr);
  return ptr;
}

static mrb_value
new_slider(mrb_state *mrb, struct nk_style_slider *ptr)
{
  mrb_value self = new_style_object(mrb, "Slider", ptr, &style_slider_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BAR_ACTIVE, new_color(mrb, &(ptr->bar_active)));
  mrb_iv_set(mrb, self, BAR_FILLED, new_color(mrb, &(ptr->bar_filled)));
  mrb_iv_set(mrb, self, BAR_HOVER, new_color(mrb, &(ptr->bar_hover)));
  mrb_iv_set(mrb, self, BAR_NORMAL, new_color(mrb, &(ptr->bar_normal)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, CURSOR_ACTIVE, new_item(mrb, &(ptr->cursor_active)));
  mrb_iv_set(mrb, self, CURSOR_HOVER, new_item(mrb, &(ptr->cursor_hover)));
  mrb_iv_set(mrb, self, CURSOR_NORMAL, new_item(mrb, &(ptr->cursor_normal)));
  mrb_iv_set(mrb, self, CURSOR_SIZE, new_vec2(mrb, &(ptr->cursor_size)));
  mrb_iv_set(mrb, self, DEC_BUTTON, new_button(mrb, &(ptr->dec_button)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, INC_BUTTON, new_button(mrb, &(ptr->inc_button)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, SPACING, new_vec2(mrb, &(ptr->spacing)));
  return self;
}

static const struct mrb_data_type style_progress_data_type = {
  "Carbuncle::GUI::Style::Progress", mrb_style_free
};

static struct nk_style_progress *
get_progress(mrb_state *mrb, mrb_value self)
{
  struct nk_style_progress *ptr;
  Data_Get_Struct(mrb, self, &style_progress_data_type, ptr);
  return ptr;
}

static mrb_value
new_progress(mrb_state *mrb, struct nk_style_progress *ptr)
{
  mrb_value self = new_style_object(mrb, "Progress", ptr, &style_progress_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, CURSOR_ACTIVE, new_item(mrb, &(ptr->cursor_active)));
  mrb_iv_set(mrb, self, CURSOR_BORDER_COLOR, new_color(mrb, &(ptr->cursor_border_color)));
  mrb_iv_set(mrb, self, CURSOR_HOVER, new_item(mrb, &(ptr->cursor_hover)));
  mrb_iv_set(mrb, self, CURSOR_NORMAL, new_item(mrb, &(ptr->cursor_normal)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  return self;
}

static const struct mrb_data_type style_scrollbar_data_type = {
  "Carbuncle::GUI::Style::Scrollbar", mrb_style_free
};

static struct nk_style_scrollbar *
get_scrollbar(mrb_state *mrb, mrb_value self)
{
  struct nk_style_scrollbar *ptr;
  Data_Get_Struct(mrb, self, &style_scrollbar_data_type, ptr);
  return ptr;
}

static mrb_value
new_scrollbar(mrb_state *mrb, struct nk_style_scrollbar *ptr)
{
  mrb_value self = new_style_object(mrb, "Scrollbar", ptr, &style_scrollbar_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, CURSOR_ACTIVE, new_item(mrb, &(ptr->cursor_active)));
  mrb_iv_set(mrb, self, CURSOR_BORDER_COLOR, new_color(mrb, &(ptr->cursor_border_color)));
  mrb_iv_set(mrb, self, CURSOR_HOVER, new_item(mrb, &(ptr->cursor_hover)));
  mrb_iv_set(mrb, self, CURSOR_NORMAL, new_item(mrb, &(ptr->cursor_normal)));
  mrb_iv_set(mrb, self, DEC_BUTTON, new_button(mrb, &(ptr->dec_button)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, INC_BUTTON, new_button(mrb, &(ptr->inc_button)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  return self;
}

static const struct mrb_data_type style_edit_data_type = {
  "Carbuncle::GUI::Style::Edit", mrb_style_free
};

static struct nk_style_edit *
get_edit(mrb_state *mrb, mrb_value self)
{
  struct nk_style_edit *ptr;
  Data_Get_Struct(mrb, self, &style_edit_data_type, ptr);
  return ptr;
}

static mrb_value
new_edit(mrb_state *mrb, struct nk_style_edit *ptr)
{
  mrb_value self = new_style_object(mrb, "Edit", ptr, &style_edit_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, CURSOR_HOVER, new_color(mrb, &(ptr->cursor_hover)));
  mrb_iv_set(mrb, self, CURSOR_NORMAL, new_color(mrb, &(ptr->cursor_normal)));
  mrb_iv_set(mrb, self, CURSOR_TEXT_HOVER, new_color(mrb, &(ptr->cursor_text_hover)));
  mrb_iv_set(mrb, self, CURSOR_TEXT_NORMAL, new_color(mrb, &(ptr->cursor_text_normal)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, SCROLLBAR, new_scrollbar(mrb, &(ptr->scrollbar)));
  mrb_iv_set(mrb, self, SCROLLBAR_SIZE, new_vec2(mrb, &(ptr->scrollbar_size)));
  mrb_iv_set(mrb, self, SELECTED_HOVER, new_color(mrb, &(ptr->selected_hover)));
  mrb_iv_set(mrb, self, SELECTED_NORMAL, new_color(mrb, &(ptr->selected_normal)));
  mrb_iv_set(mrb, self, SELECTED_TEXT_HOVER, new_color(mrb, &(ptr->selected_text_hover)));
  mrb_iv_set(mrb, self, SELECTED_TEXT_NORMAL, new_color(mrb, &(ptr->selected_text_normal)));
  mrb_iv_set(mrb, self, TEXT_ACTIVE, new_color(mrb, &(ptr->text_active)));
  mrb_iv_set(mrb, self, TEXT_HOVER, new_color(mrb, &(ptr->text_hover)));
  mrb_iv_set(mrb, self, TEXT_NORMAL, new_color(mrb, &(ptr->text_normal)));
  return self;
}

static const struct mrb_data_type style_property_data_type = {
  "Carbuncle::GUI::Style::Property", mrb_style_free
};

static struct nk_style_property *
get_property(mrb_state *mrb, mrb_value self)
{
  struct nk_style_property *ptr;
  Data_Get_Struct(mrb, self, &style_property_data_type, ptr);
  return ptr;
}

static mrb_value
new_property(mrb_state *mrb, struct nk_style_property *ptr)
{
  mrb_value self = new_style_object(mrb, "Property", ptr, &style_property_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, DEC_BUTTON, new_button(mrb, &(ptr->dec_button)));
  mrb_iv_set(mrb, self, EDIT, new_edit(mrb, &(ptr->edit)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, INC_BUTTON, new_button(mrb, &(ptr->inc_button)));
  mrb_iv_set(mrb, self, LABEL_ACTIVE, new_color(mrb, &(ptr->label_active)));
  mrb_iv_set(mrb, self, LABEL_HOVER, new_color(mrb, &(ptr->label_hover)));
  mrb_iv_set(mrb, self, LABEL_NORMAL, new_color(mrb, &(ptr->label_normal)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  return self;
}

static const struct mrb_data_type style_chart_data_type = {
  "Carbuncle::GUI::Style::Chart", mrb_style_free
};

static struct nk_style_chart *
get_chart(mrb_state *mrb, mrb_value self)
{
  struct nk_style_chart *ptr;
  Data_Get_Struct(mrb, self, &style_chart_data_type, ptr);
  return ptr;
}

static mrb_value
new_chart(mrb_state *mrb, struct nk_style_chart *ptr)
{
  mrb_value self = new_style_object(mrb, "Chart", ptr, &style_chart_data_type);
  mrb_iv_set(mrb, self, BACKGROUND, new_item(mrb, &(ptr->background)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, COLOR, new_color(mrb, &(ptr->color)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, SELECTED_COLOR, new_color(mrb, &(ptr->selected_color)));
  return self;
}

static const struct mrb_data_type style_combo_data_type = {
  "Carbuncle::GUI::Style::Combo", mrb_style_free
};

static struct nk_style_combo *
get_combo(mrb_state *mrb, mrb_value self)
{
  struct nk_style_combo *ptr;
  Data_Get_Struct(mrb, self, &style_combo_data_type, ptr);
  return ptr;
}

static mrb_value
new_combo(mrb_state *mrb, struct nk_style_combo *ptr)
{
  mrb_value self = new_style_object(mrb, "Combo", ptr, &style_combo_data_type);
  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, BUTTON, new_button(mrb, &(ptr->button)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->button_padding)));
  mrb_iv_set(mrb, self, CONTENT_PADDING, new_vec2(mrb, &(ptr->content_padding)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, LABEL_ACTIVE, new_color(mrb, &(ptr->label_active)));
  mrb_iv_set(mrb, self, LABEL_HOVER, new_color(mrb, &(ptr->label_hover)));
  mrb_iv_set(mrb, self, LABEL_NORMAL, new_color(mrb, &(ptr->label_normal)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, SPACING, new_vec2(mrb, &(ptr->spacing)));
  mrb_iv_set(mrb, self, SYMBOL_ACTIVE, new_color(mrb, &(ptr->symbol_active)));
  mrb_iv_set(mrb, self, SYMBOL_HOVER, new_color(mrb, &(ptr->symbol_hover)));
  mrb_iv_set(mrb, self, SYMBOL_NORMAL, new_color(mrb, &(ptr->symbol_normal)));
  return self;
}

static const struct mrb_data_type style_tab_data_type = {
  "Carbuncle::GUI::Style::Tab", mrb_style_free
};

static struct nk_style_tab *
get_tab(mrb_state *mrb, mrb_value self)
{
  struct nk_style_tab *ptr;
  Data_Get_Struct(mrb, self, &style_tab_data_type, ptr);
  return ptr;
}

static mrb_value
new_tab(mrb_state *mrb, struct nk_style_tab *ptr)
{
  mrb_value self = new_style_object(mrb, "Tab", ptr, &style_tab_data_type);
  mrb_iv_set(mrb, self, BACKGROUND, new_item(mrb, &(ptr->background)));
  mrb_iv_set(mrb, self, BORDER_COLOR, new_color(mrb, &(ptr->border_color)));
  mrb_iv_set(mrb, self, NODE_MAXIMIZE_BUTTON, new_button(mrb, &(ptr->node_maximize_button)));
  mrb_iv_set(mrb, self, NODE_MINIMIZE_BUTTON, new_button(mrb, &(ptr->node_minimize_button)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, SPACING, new_vec2(mrb, &(ptr->spacing)));
  mrb_iv_set(mrb, self, TAB_MAXIMIZE_BUTTON, new_button(mrb, &(ptr->tab_maximize_button)));
  mrb_iv_set(mrb, self, TAB_MINIMIZE_BUTTON, new_button(mrb, &(ptr->tab_minimize_button)));
  mrb_iv_set(mrb, self, TEXT, new_color(mrb, &(ptr->text)));
  return self;
}

static const struct mrb_data_type style_window_header_data_type = {
  "Carbuncle::GUI::Style::Window::Header", mrb_style_free
};

static struct nk_style_window_header *
get_window_header(mrb_state *mrb, mrb_value self)
{
  struct nk_style_window_header *ptr;
  Data_Get_Struct(mrb, self, &style_window_header_data_type, ptr);
  return ptr;
}

static mrb_value
new_window_header(mrb_state *mrb, struct nk_style_window_header *ptr)
{
  struct RClass *style_class = get_style_class(mrb, "Window");
  struct RClass *header = mrb_class_get_under(mrb, style_class, "Header");
  mrb_value self = mrb_obj_new(mrb, header, 0, NULL);
  DATA_TYPE(self) = &style_window_header_data_type;
  DATA_PTR(self) = ptr;

  mrb_iv_set(mrb, self, ACTIVE, new_item(mrb, &(ptr->active)));
  mrb_iv_set(mrb, self, CLOSE_BUTTON, new_button(mrb, &(ptr->close_button)));
  mrb_iv_set(mrb, self, HOVER, new_item(mrb, &(ptr->hover)));
  mrb_iv_set(mrb, self, LABEL_ACTIVE, new_color(mrb, &(ptr->label_active)));
  mrb_iv_set(mrb, self, LABEL_HOVER, new_color(mrb, &(ptr->label_hover)));
  mrb_iv_set(mrb, self, LABEL_NORMAL, new_color(mrb, &(ptr->label_normal)));
  mrb_iv_set(mrb, self, LABEL_PADDING, new_vec2(mrb, &(ptr->label_padding)));
  mrb_iv_set(mrb, self, MINIMIZE_BUTTON, new_button(mrb, &(ptr->minimize_button)));
  mrb_iv_set(mrb, self, NORMAL, new_item(mrb, &(ptr->normal)));
  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, SPACING, new_vec2(mrb, &(ptr->spacing)));

  return self;
}

static const struct mrb_data_type style_window_data_type = {
  "Carbuncle::GUI::Style::Window", mrb_style_free
};

static struct nk_style_window *
get_window(mrb_state *mrb, mrb_value self)
{
  struct nk_style_window *ptr;
  Data_Get_Struct(mrb, self, &style_window_data_type, ptr);
  return ptr;
}

static mrb_value
new_window(mrb_state *mrb, struct nk_style_window *ptr)
{
  mrb_value self = new_style_object(mrb, "Window", ptr, &style_window_data_type);

  mrb_iv_set(mrb, self, HEADER, new_window_header(mrb, &(ptr->header)));
  mrb_iv_set(mrb, self, MENU_BORDER_COLOR, new_color(mrb, &(ptr->menu_border_color)));
  mrb_iv_set(mrb, self, MENU_PADDING, new_vec2(mrb, &(ptr->menu_padding)));
  mrb_iv_set(mrb, self, MIN_SIZE, new_vec2(mrb, &(ptr->min_size)));

  mrb_iv_set(mrb, self, PADDING, new_vec2(mrb, &(ptr->padding)));
  mrb_iv_set(mrb, self, POPUP_BORDER_COLOR, new_color(mrb, &(ptr->popup_border_color)));
  mrb_iv_set(mrb, self, POPUP_PADDING, new_vec2(mrb, &(ptr->popup_padding)));
  mrb_iv_set(mrb, self, SCALER, new_item(mrb, &(ptr->scaler)));
  mrb_iv_set(mrb, self, SCROLLBAR_SIZE, new_vec2(mrb, &(ptr->scrollbar_size)));
  mrb_iv_set(mrb, self, SPACING, new_vec2(mrb, &(ptr->spacing)));
  mrb_iv_set(mrb, self, TOOLTIP_BORDER_COLOR, new_vec2(mrb, &(ptr->tooltip_border_color)));
  mrb_iv_set(mrb, self, TOOLTIP_PADDING, new_vec2(mrb, &(ptr->tooltip_padding)));

  return self;
}

mrb_value
mrb_style_initialize(mrb_state *mrb, mrb_value self)
{
  mrb_value gui;
  struct mrb_GuiContext *ctx;
  struct nk_style *style;
  mrb_get_args(mrb, "o", &gui);
  ctx = mrb_carbuncle_gui_get_context(mrb, gui);
  style = &(ctx->nk.style);
  DATA_TYPE(self) = &style_data_type;
  DATA_PTR(self) = style;
  mrb_iv_set(mrb, self, mrb_intern_cstr(mrb, "#gui"), gui);

  mrb_iv_set(mrb, self, BUTTON_SYMBOL, new_button(mrb, &(style->button)));  
  mrb_iv_set(mrb, self, CHART_SYMBOL, new_chart(mrb, &(style->chart)));
  mrb_iv_set(mrb, self, CHECKBOX_SYMBOL, new_toggle(mrb, &(style->checkbox)));
  mrb_iv_set(mrb, self, COMBO_SYMBOL, new_combo(mrb, &(style->combo)));
  mrb_iv_set(mrb, self, CONTEXTUAL_BUTTON_SYMBOL, new_button(mrb, &(style->contextual_button)));
  mrb_iv_set(mrb, self, CURSOR_ACTIVE_SYMBOL, new_cursor(mrb, style->cursor_active, TRUE));
  mrb_iv_set(mrb, self, CURSOR_LAST_SYMBOL, new_cursor(mrb, style->cursor_last, FALSE));
  {
    mrb_value cursor_list[] = {
      new_cursor(mrb, style->cursors[0], TRUE),
      new_cursor(mrb, style->cursors[1], TRUE),
      new_cursor(mrb, style->cursors[2], TRUE),
      new_cursor(mrb, style->cursors[3], TRUE),
      new_cursor(mrb, style->cursors[4], TRUE),
      new_cursor(mrb, style->cursors[5], TRUE),
      new_cursor(mrb, style->cursors[6], TRUE)
    };
    mrb_iv_set(mrb, self, CURSORS_SYMBOL, mrb_ary_new_from_values(mrb, 7, cursor_list));
  }
  mrb_iv_set(mrb, self, EDIT_SYMBOL, new_edit(mrb, &(style->edit)));
  mrb_iv_set(mrb, self, MENU_BUTTON_SYMBOL, new_button(mrb, &(style->menu_button)));
  mrb_iv_set(mrb, self, PROGRESS_SYMBOL, new_toggle(mrb, &(style->option)));
  mrb_iv_set(mrb, self, PROGRESS_SYMBOL, new_progress(mrb, &(style->progress)));
  mrb_iv_set(mrb, self, PROPERTY_SYMBOL, new_property(mrb, &(style->property)));
  mrb_iv_set(mrb, self, SCROLLH_SYMBOL, new_scrollbar(mrb, &(style->scrollh)));
  mrb_iv_set(mrb, self, SCROLLV_SYMBOL, new_scrollbar(mrb, &(style->scrollv)));
  mrb_iv_set(mrb, self, SELECTABLE_SYMBOL, new_selectable(mrb, &(style->selectable)));
  mrb_iv_set(mrb, self, SLIDER_SYMBOL, new_slider(mrb, &(style->slider)));
  mrb_iv_set(mrb, self, TAB_SYMBOL, new_tab(mrb, &(style->tab)));
  mrb_iv_set(mrb, self, TEXT_SYMBOL, new_text(mrb, &(style->text)));
  mrb_iv_set(mrb, self, WINDOW_SYMBOL, new_window(mrb, &(style->window)));
  return self;
}

mrb_value
mrb_style_get_button(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, BUTTON_SYMBOL);
}

mrb_value
mrb_style_get_chart(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CHART_SYMBOL);
}

mrb_value
mrb_style_get_checkbox(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CHECKBOX_SYMBOL);
}

mrb_value
mrb_style_get_combo(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, COMBO_SYMBOL);
}

mrb_value
mrb_style_get_contextual_button(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CONTEXTUAL_BUTTON_SYMBOL);
}

mrb_value
mrb_style_get_cursor_active(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSOR_ACTIVE_SYMBOL);
}

mrb_value
mrb_style_get_cursor_last(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSOR_LAST_SYMBOL);
}

mrb_value
mrb_style_get_cursors(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, CURSORS_SYMBOL);
}

mrb_value
mrb_style_get_edit(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, EDIT_SYMBOL);
}

mrb_value
mrb_style_get_menu_button(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, MENU_BUTTON_SYMBOL);
}

mrb_value
mrb_style_get_option(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, OPTION_SYMBOL);
}

mrb_value
mrb_style_get_progress(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROGRESS_SYMBOL);
}

mrb_value
mrb_style_get_property(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PROPERTY_SYMBOL);
}

mrb_value
mrb_style_get_scrollh(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SCROLLH_SYMBOL);
}

mrb_value
mrb_style_get_scrollv(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SCROLLV_SYMBOL);
}

mrb_value
mrb_style_get_selectable(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SELECTABLE_SYMBOL);
}

mrb_value
mrb_style_get_slider(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SLIDER_SYMBOL);
}

mrb_value
mrb_style_get_tab(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TAB_SYMBOL);
}

mrb_value
mrb_style_get_text(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TEXT_SYMBOL);
}

static mrb_value
mrb_style_get_window(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, WINDOW_SYMBOL);
}

static mrb_value
mrb_style_get_cursor_visible(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(get_style(mrb, self)->cursor_visible);
}

static mrb_value
mrb_style_set_cursor_visible(mrb_state *mrb, mrb_value self)
{
  mrb_bool value;
  mrb_get_args(mrb, "b", &value);
  get_style(mrb, self)->cursor_visible = value;
  return mrb_bool_value(value);
}

static mrb_value
color_get_r(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_color(mrb, self)->r);
}

static mrb_value
color_get_g(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_color(mrb, self)->g);
}

static mrb_value
color_get_b(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_color(mrb, self)->b);
}

static mrb_value
color_get_a(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_color(mrb, self)->a);
}

static mrb_value
color_set_r(mrb_state *mrb, mrb_value self)
{
  mrb_int value;
  mrb_get_args(mrb, "i", &value);
  if (value < 0) { value = 0; }
  if (value > 255) { value = 255; }
  get_color(mrb, self)->r = value;
  return mrb_fixnum_value(value);
}

static mrb_value
color_set_g(mrb_state *mrb, mrb_value self)
{
  mrb_int value;
  mrb_get_args(mrb, "i", &value);
  if (value < 0) { value = 0; }
  if (value > 255) { value = 255; }
  get_color(mrb, self)->g = value;
  return mrb_fixnum_value(value);
}

static mrb_value
color_set_b(mrb_state *mrb, mrb_value self)
{
  mrb_int value;
  mrb_get_args(mrb, "i", &value);
  if (value < 0) { value = 0; }
  if (value > 255) { value = 255; }
  get_color(mrb, self)->b = value;
  return mrb_fixnum_value(value);
}

static mrb_value
color_set_a(mrb_state *mrb, mrb_value self)
{
  mrb_int value;
  mrb_get_args(mrb, "i", &value);
  if (value < 0) { value = 0; }
  if (value > 255) { value = 255; }
  get_color(mrb, self)->a = value;
  return mrb_fixnum_value(value);
}

static mrb_value
item_colorQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(get_item(mrb, self)->type == NK_STYLE_ITEM_COLOR);
}

static mrb_value
item_imageQ(mrb_state *mrb, mrb_value self)
{
  return mrb_bool_value(get_item(mrb, self)->type == NK_STYLE_ITEM_IMAGE);
}

static mrb_value
item_type(mrb_state *mrb, mrb_value self)
{
  switch (get_item(mrb, self)->type)
  {
    case NK_STYLE_ITEM_COLOR: return mrb_str_new_cstr(mrb, "color");
    case NK_STYLE_ITEM_IMAGE: return mrb_str_new_cstr(mrb, "image");
  }
  return mrb_nil_value();
}

static mrb_value
item_image(mrb_state *mrb, mrb_value self)
{
  if (get_item(mrb, self)->type != NK_STYLE_ITEM_IMAGE)
  {
    mrb_raise(mrb, E_TYPE_ERROR, "Item is not an Image");
  }
  return mrb_iv_get(mrb, self, IMAGE);
}

static mrb_value
item_color(mrb_state *mrb, mrb_value self)
{
  if (get_item(mrb, self)->type != NK_STYLE_ITEM_COLOR)
  {
    mrb_raise(mrb, E_TYPE_ERROR, "Item is not a Color");
  }
  return mrb_iv_get(mrb, self, COLOR);
}

static mrb_value
item_set_image(mrb_state *mrb, mrb_value self)
{
  mrb_value value;
  mrb_get_args(mrb, "o", &value);
  mrb_funcall(mrb, mrb_iv_get(mrb, self, IMAGE), "set", 1, value);
  get_item(mrb, self)->type = NK_STYLE_ITEM_IMAGE;
  return value;
}

static mrb_value
item_set_color(mrb_state *mrb, mrb_value self)
{
  mrb_value value;
  mrb_get_args(mrb, "o", &value);
  mrb_funcall(mrb, mrb_iv_get(mrb, self, COLOR), "set", 1, value);
  get_item(mrb, self)->type = NK_STYLE_ITEM_COLOR;
  return value;
}

static mrb_value
image_set(mrb_state *mrb, mrb_value self)
{
  mrb_value obj;
  Texture2D *texture;
  struct nk_image *img = get_image(mrb, self);
  mrb_get_args(mrb, "o", &obj);
  texture = mrb_carbuncle_get_texture(mrb, obj);
  img->handle.ptr = texture;
  img->w = texture->width;
  img->h = texture->height;
  img->region[0] = 0;
  img->region[1] = 0;
  img->region[2] = img->w;
  img->region[3] = img->h;
  mrb_iv_set(mrb, self, TEXTURE, obj);
  return self;
}

static mrb_value
image_get_region(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, REGION);
}

static mrb_value
image_region_get(mrb_state *mrb, mrb_value self)
{
  mrb_int i;
  mrb_get_args(mrb, "i", &i);
  if (i > 3 || i < 0) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid region index");
  }
  return mrb_fixnum_value(get_image_region(mrb, self)[i]);
}

static mrb_value
image_region_set(mrb_state *mrb, mrb_value self)
{
  mrb_int i, value;
  mrb_get_args(mrb, "ii", &i, &value);
  if (i > 3 || i < 0) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "Invalid region index");
  }
  get_image_region(mrb, self)[i] = value;
  return mrb_fixnum_value(value);
}

static mrb_value
vec2_get_x(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vec2(mrb, self)->x);
}

static mrb_value
vec2_get_y(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_vec2(mrb, self)->y);
}

static mrb_value
vec2_set_x(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  get_vec2(mrb, self)->x = value;
  return mrb_float_value(mrb, value);
}

static mrb_value
vec2_set_y(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  get_vec2(mrb, self)->y = value;
  return mrb_float_value(mrb, value);
}

static mrb_value
cursor_get_image(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, IMAGE);
}

static mrb_value
cursor_get_offset(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, OFFSET);
}

static mrb_value
cursor_get_size(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, SIZE);
}

static mrb_value
button_active(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, ACTIVE);
}

static mrb_value
button_border_color(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, BORDER_COLOR);
}

static mrb_value
button_hover(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, HOVER);
}

static mrb_value
button_image_padding(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, IMAGE_PADDING);
}

static mrb_value
button_normal(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, NORMAL);
}

static mrb_value
button_padding(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, PADDING);
}

static mrb_value
button_text_active(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TEXT_ACTIVE);
}

static mrb_value
button_text_background(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TEXT_BACKGROUND);
}

static mrb_value
button_text_hover(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TEXT_HOVER);
}

static mrb_value
button_text_normal(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TEXT_NORMAL);
}

static mrb_value
button_touch_padding(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, TOUCH_PADDING);
}

static mrb_value
button_get_border(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_button(mrb, self)->border);
}

static mrb_value
button_get_rounding(mrb_state *mrb, mrb_value self)
{
  return mrb_float_value(mrb, get_button(mrb, self)->rounding);
}

static mrb_value
button_get_text_alignment(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(get_button(mrb, self)->text_alignment);
}

static mrb_value
button_set_border(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  get_button(mrb, self)->border = value;
  return mrb_float_value(mrb, value);
}

static mrb_value
button_set_rounding(mrb_state *mrb, mrb_value self)
{
  mrb_float value;
  mrb_get_args(mrb, "f", &value);
  get_button(mrb, self)->rounding = value;
  return mrb_float_value(mrb, value);
}

static mrb_value
button_set_text_alignment(mrb_state *mrb, mrb_value self)
{
  mrb_int value;
  mrb_get_args(mrb, "i", &value);
  get_button(mrb, self)->text_alignment = value;
  return mrb_fixnum_value(value);
}

void
mrb_init_carbuncle_gui_style(mrb_state *mrb, struct RClass *gui)
{
  struct RClass *style = mrb_define_class_under(mrb, gui, "Style", mrb->object_class);
  MRB_SET_INSTANCE_TT(style, MRB_TT_DATA);

  mrb_define_method(mrb, style, "initialize", mrb_style_initialize, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, style, "button", mrb_style_get_button, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "chart", mrb_style_get_chart, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "checkbox", mrb_style_get_checkbox, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "combo", mrb_style_get_combo, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "contextual_button", mrb_style_get_contextual_button, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_active", mrb_style_get_cursor_active, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_last", mrb_style_get_cursor_last, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursors", mrb_style_get_cursors, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "edit", mrb_style_get_edit, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "menu_button", mrb_style_get_menu_button, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "option", mrb_style_get_option, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "progress", mrb_style_get_progress, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "property", mrb_style_get_property, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "scrollh", mrb_style_get_scrollh, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "scrollv", mrb_style_get_scrollv, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "selectable", mrb_style_get_selectable, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "slider", mrb_style_get_slider, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "tab", mrb_style_get_tab, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "text", mrb_style_get_text, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "window", mrb_style_get_window, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_visible", mrb_style_get_cursor_visible, MRB_ARGS_NONE());
  mrb_define_method(mrb, style, "cursor_visible=", mrb_style_set_cursor_visible, MRB_ARGS_REQ(1));

  struct RClass *color = mrb_define_class_under(mrb, style, "Color", mrb->object_class);
  MRB_SET_INSTANCE_TT(color, MRB_TT_DATA);

  mrb_define_method(mrb, color, "r", color_get_r, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "red", color_get_r, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "g", color_get_g, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "green", color_get_g, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "b", color_get_b, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "blue", color_get_b, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "a", color_get_a, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "alpha", color_get_a, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "r=", color_set_r, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "red=", color_set_r, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "g=", color_set_g, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "green=", color_set_g, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "b=", color_set_b, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "blue=", color_set_b, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "a=", color_set_a, MRB_ARGS_NONE());
  mrb_define_method(mrb, color, "alpha=", color_set_a, MRB_ARGS_NONE());

  struct RClass *image = mrb_define_class_under(mrb, style, "Image", mrb->object_class);
  MRB_SET_INSTANCE_TT(image, MRB_TT_DATA);

  mrb_define_method(mrb, image, "set", image_set, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, image, "region", image_get_region, MRB_ARGS_NONE());

  struct RClass *image_region = mrb_define_class_under(mrb, image, "Region", mrb->object_class);
  MRB_SET_INSTANCE_TT(image_region, MRB_TT_DATA);

  mrb_define_method(mrb, image_region, "[]", image_region_get, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, image_region, "[]=", image_region_set, MRB_ARGS_REQ(2));

  struct RClass *vec2 = mrb_define_class_under(mrb, style, "Vec2", mrb->object_class);
  MRB_SET_INSTANCE_TT(vec2, MRB_TT_DATA);

  mrb_define_method(mrb, vec2, "x", vec2_get_x, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2, "y", vec2_get_y, MRB_ARGS_NONE());
  mrb_define_method(mrb, vec2, "x=", vec2_set_x, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, vec2, "x=", vec2_set_y, MRB_ARGS_REQ(1));

  struct RClass *item = mrb_define_class_under(mrb, style, "Item", mrb->object_class);
  MRB_SET_INSTANCE_TT(item, MRB_TT_DATA);

  mrb_define_method(mrb, item, "color?", item_colorQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, item, "image?", item_imageQ, MRB_ARGS_NONE());
  mrb_define_method(mrb, item, "type", item_type, MRB_ARGS_NONE());
  mrb_define_method(mrb, item, "color", item_color, MRB_ARGS_NONE());
  mrb_define_method(mrb, item, "image", item_image, MRB_ARGS_NONE());
  mrb_define_method(mrb, item, "color=", item_set_color, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, item, "image=", item_set_image, MRB_ARGS_REQ(1));

  struct RClass *cursor = mrb_define_class_under(mrb, style, "Cursor", mrb->object_class);
  MRB_SET_INSTANCE_TT(cursor, MRB_TT_DATA);

  mrb_define_method(mrb, cursor, "image", cursor_get_image, MRB_ARGS_NONE());
  mrb_define_method(mrb, cursor, "offset", cursor_get_offset, MRB_ARGS_NONE());
  mrb_define_method(mrb, cursor, "size", cursor_get_size, MRB_ARGS_NONE());

  struct RClass *button = mrb_define_class_under(mrb, style, "Button", mrb->object_class);
  MRB_SET_INSTANCE_TT(button, MRB_TT_DATA);

  mrb_define_method(mrb, button, "active", button_active, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "border_color", button_border_color, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "hover", button_hover, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "image_padding", button_image_padding, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "normal", button_normal, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "padding", button_padding, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "text_active", button_text_active, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "text_background", button_text_background, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "text_hover", button_text_hover, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "text_normal", button_text_normal, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "touch_padding", button_touch_padding, MRB_ARGS_NONE());

  mrb_define_method(mrb, button, "border", button_get_border, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "border=", button_set_border, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, button, "rounding", button_get_rounding, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "rounding=", button_set_rounding, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, button, "text_alignment", button_get_text_alignment, MRB_ARGS_NONE());
  mrb_define_method(mrb, button, "text_alignment=", button_set_text_alignment, MRB_ARGS_REQ(1));

  struct RClass *toggle = mrb_define_class_under(mrb, style, "Toggle", mrb->object_class);
  MRB_SET_INSTANCE_TT(toggle, MRB_TT_DATA);

  struct RClass *selectable = mrb_define_class_under(mrb, style, "Selectable", mrb->object_class);
  MRB_SET_INSTANCE_TT(selectable, MRB_TT_DATA);

  struct RClass *slider = mrb_define_class_under(mrb, style, "Slider", mrb->object_class);
  MRB_SET_INSTANCE_TT(slider, MRB_TT_DATA);

  struct RClass *progress = mrb_define_class_under(mrb, style, "Progress", mrb->object_class);
  MRB_SET_INSTANCE_TT(progress, MRB_TT_DATA);

  struct RClass *scrollbar = mrb_define_class_under(mrb, style, "Scrollbar", mrb->object_class);
  MRB_SET_INSTANCE_TT(scrollbar, MRB_TT_DATA);

  struct RClass *edit = mrb_define_class_under(mrb, style, "Edit", mrb->object_class);
  MRB_SET_INSTANCE_TT(edit, MRB_TT_DATA);

  struct RClass *property = mrb_define_class_under(mrb, style, "Property", mrb->object_class);
  MRB_SET_INSTANCE_TT(property, MRB_TT_DATA);

  struct RClass *chart = mrb_define_class_under(mrb, style, "Chart", mrb->object_class);
  MRB_SET_INSTANCE_TT(chart, MRB_TT_DATA);

  struct RClass *combo = mrb_define_class_under(mrb, style, "Combo", mrb->object_class);
  MRB_SET_INSTANCE_TT(combo, MRB_TT_DATA);

  struct RClass *tab = mrb_define_class_under(mrb, style, "Tab", mrb->object_class);
  MRB_SET_INSTANCE_TT(tab, MRB_TT_DATA);

  struct RClass *text = mrb_define_class_under(mrb, style, "Text", mrb->object_class);
  MRB_SET_INSTANCE_TT(text, MRB_TT_DATA);

  struct RClass *window = mrb_define_class_under(mrb, style, "Window", mrb->object_class);
  MRB_SET_INSTANCE_TT(window, MRB_TT_DATA);

  struct RClass *window_header = mrb_define_class_under(mrb, window, "Header", mrb->object_class);
  MRB_SET_INSTANCE_TT(window_header, MRB_TT_DATA);

  struct RClass *symbols = mrb_define_module_under(mrb, style, "Symbols");

  mrb_define_const(mrb, symbols, "NONE", mrb_fixnum_value(NK_SYMBOL_NONE));
  mrb_define_const(mrb, symbols, "X", mrb_fixnum_value(NK_SYMBOL_X));
  mrb_define_const(mrb, symbols, "UNDERSCORE", mrb_fixnum_value(NK_SYMBOL_UNDERSCORE));
  mrb_define_const(mrb, symbols, "SOLID", mrb_fixnum_value(NK_SYMBOL_CIRCLE_SOLID));
  mrb_define_const(mrb, symbols, "OUTLINE", mrb_fixnum_value(NK_SYMBOL_CIRCLE_OUTLINE));
  mrb_define_const(mrb, symbols, "RECT_SOLID", mrb_fixnum_value(NK_SYMBOL_RECT_SOLID));
  mrb_define_const(mrb, symbols, "RECT_OUTLINE", mrb_fixnum_value(NK_SYMBOL_RECT_OUTLINE));
  mrb_define_const(mrb, symbols, "TRIANGLE_UP", mrb_fixnum_value(NK_SYMBOL_TRIANGLE_UP));
  mrb_define_const(mrb, symbols, "TRIANGLE_DOWN", mrb_fixnum_value(NK_SYMBOL_TRIANGLE_DOWN));
  mrb_define_const(mrb, symbols, "TRIANGLE_LEFT", mrb_fixnum_value(NK_SYMBOL_TRIANGLE_LEFT));
  mrb_define_const(mrb, symbols, "TRIANGLE_RIGHT", mrb_fixnum_value(NK_SYMBOL_TRIANGLE_RIGHT));
  mrb_define_const(mrb, symbols, "PLUS", mrb_fixnum_value(NK_SYMBOL_PLUS));
  mrb_define_const(mrb, symbols, "MINUS", mrb_fixnum_value(NK_SYMBOL_MINUS));
  mrb_define_const(mrb, symbols, "MAX", mrb_fixnum_value(NK_SYMBOL_MAX));

  struct RClass *align = mrb_define_module_under(mrb, style, "Align");

  mrb_define_const(mrb, align, "LEFT", mrb_fixnum_value(NK_TEXT_ALIGN_LEFT));
  mrb_define_const(mrb, align, "CENTERED", mrb_fixnum_value(NK_TEXT_ALIGN_CENTERED));
  mrb_define_const(mrb, align, "RIGHT", mrb_fixnum_value(NK_TEXT_ALIGN_RIGHT));
  mrb_define_const(mrb, align, "TOP", mrb_fixnum_value(NK_TEXT_ALIGN_TOP));
  mrb_define_const(mrb, align, "MIDDLE", mrb_fixnum_value(NK_TEXT_ALIGN_MIDDLE));
  mrb_define_const(mrb, align, "BOTTOM", mrb_fixnum_value(NK_TEXT_ALIGN_BOTTOM));
}
