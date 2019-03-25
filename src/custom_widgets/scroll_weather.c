/**
 * 
 * 
 * 
 * 
 */


#include "scroll_weather.h"

ret_t scroll_weather_on_paint_self(widget_t *widget, canvas_t *c) {
  rect_t r = {100, 100, 200, 200};
  rect_t src = rect_init(10, 10, 20, 20);
  rect_t dst = rect_init(30, 30, 20, 20);
  canvas_set_stroke_color_str(c, "0xffffff");
  // canvas_get_clip_rect(c, &r);
  bitmap_t image;
  widget_load_image(widget, "1", &image);
  canvas_draw_image(c, &image, &src, &dst);
  return RET_OK;
}

ret_t scroll_weather_on_paint_background(widget_t *widget, canvas_t *c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  scroll_weather_t* scroll_weather = SCROLL_WEATHER(widget);
  vgcanvas_translate(vg, c->ox, c->oy);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  vgcanvas_set_fill_color(vg, color_init(0x00, 0x00, 0xff, 0xff));
  vgcanvas_begin_path(vg);
  vgcanvas_ellipse(vg, 0, 0, 100, 100);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t scroll_weather_on_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return RET_OK;
}

static const widget_vtable_t s_scroll_weather_vtable = {.size = sizeof(scroll_weather_t),
                                                        .type = WIDGET_TYPE_SCROLL_WEATHER,
                                                        .create = scroll_weather_create,
                                                        .on_paint_self = scroll_weather_on_paint_self,
                                                        .on_paint_background = scroll_weather_on_paint_background,
                                                        .set_prop = scroll_weather_on_set_prop};


widget_t* scroll_weather_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  scroll_weather_t* scroll_weather = TKMEM_ZALLOC(scroll_weather_t);
  widget_t* widget = WIDGET(scroll_weather);
  return_value_if_fail(widget != NULL, NULL);

  widget_init(widget, parent, &s_scroll_weather_vtable, x, y, w, h);

  scroll_weather->yoffset = 0;

  return widget;
}