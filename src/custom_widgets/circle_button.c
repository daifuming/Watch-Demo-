/**
 * 
 * 
 */


#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "circle_button.h"

static ret_t circle_button_on_paint_self(widget_t *widget,canvas_t *c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  circle_button_t* button = CIRCLE_BUTTON(widget);

  float_t r = widget->w < widget->h ? widget->w/2 : widget->h/2;

  // 绘制背景圆形
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);
  vgcanvas_ellipse(vg, widget->w/2, widget->h/2, r, r);
  vgcanvas_set_fill_color(vg, button->bg_color);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);

  // 绘制图片
  if (button->image != NULL) {
    vgcanvas_save(vg);
    vgcanvas_translate(vg, c->ox, c->oy);
    bitmap_t image;
    if (widget_load_image(widget, button->image, &image) == RET_OK) {
      vgcanvas_translate(vg, widget->w/2, widget->h/2);
      vgcanvas_translate(vg, -image.w/2, -image.h/2);
      vgcanvas_begin_path(vg);
      vgcanvas_draw_image(vg, &image, 0, 0, image.w, image.h, 0, 0, image.w, image.h);
    }
    vgcanvas_restore(vg);
  }

  return RET_OK;
}

ret_t circle_button_set_image(widget_t* widget, const char* image) {
  circle_button_t* button = CIRCLE_BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  // 淡出动画
  if (button->image != NULL) {
    widget_destroy_animator(widget, NULL);
    widget_create_animator(widget, "opacity(from=255, to=0, duration=1000)"); 
  }
  button->image = tk_str_copy(button->image, image);
  // 淡入动画
  widget_create_animator(widget, "opacity(from=0, to=255, duration=3000)"); 

  return widget_invalidate(widget, NULL);
}

ret_t circle_button_set_bg_color(widget_t* widget, const char* color) {
  circle_button_t* button = CIRCLE_BUTTON(widget);
  return_value_if_fail(button != NULL, RET_BAD_PARAMS);

  color_from_str(&button->bg_color, color);

  return widget_invalidate(widget, NULL);
}

static ret_t circle_button_on_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CIRCLE_BUTTON_PROP_IMAGE)) {
    return circle_button_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_PROP_BG_COLOR)) {
    return circle_button_set_bg_color(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t circle_button_on_destroy(widget_t* widget) {
  circle_button_t* button = CIRCLE_BUTTON(widget);

  TKMEM_FREE(button->image);

  return RET_OK;
}

static const widget_vtable_t s_circle_button_vtable = {.size = sizeof(circle_button_t),
                                                       .create = circle_button_create,
                                                       .on_destroy = circle_button_on_destroy,
                                                       .on_paint_self = circle_button_on_paint_self,
                                                       .set_prop = circle_button_on_set_prop};

widget_t* circle_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  circle_button_t* button = TKMEM_ZALLOC(circle_button_t);
  widget_t* widget = WIDGET(button);
  return_value_if_fail(widget != NULL, NULL);

  widget_init(widget, parent, &s_circle_button_vtable, x, y, w, h);

  button->image = NULL;
  button->bg_color = color_init(0x23, 0x43, 0x00, 0xff);

  return widget;
}