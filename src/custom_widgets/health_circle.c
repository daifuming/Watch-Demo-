/**
 * 
 * 
 */

#include "health_circle.h"

ret_t health_circle_set_value_b(widget_t* widget, float_t value) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  if (health_circle->value_b != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    health_circle->value_b = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t health_circle_set_value_m(widget_t* widget, float_t value) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  if (health_circle->value_m != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    health_circle->value_m = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t health_circle_set_vlaue_s(widget_t* widget, float_t value) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  if (health_circle->value_s != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    health_circle->value_s = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t health_circle_set_value_max_b(widget_t* widget, float_t value) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  health_circle->max_b = value;

  return widget_invalidate(widget, NULL);
}

ret_t health_circle_set_value_max_m(widget_t* widget, float_t value) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  health_circle->max_m = value;

  return widget_invalidate(widget, NULL);
}

ret_t health_circle_set_value_max_s(widget_t* widget, float_t value) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  health_circle->max_s = value;

  return widget_invalidate(widget, NULL);
}

ret_t health_circle_set_color_b(widget_t* widget, const char* color) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  color_from_str(&health_circle->color_b, color);

  return widget_invalidate(widget, NULL);
}

ret_t health_circle_set_color_m(widget_t* widget, const char* color) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  color_from_str(&health_circle->color_m, color);

  return widget_invalidate(widget, NULL);
}

ret_t health_circle_set_color_s(widget_t* widget, const char* color) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  color_from_str(&health_circle->color_s, color);

  return widget_invalidate(widget, NULL);
}

ret_t health_circle_set_width(widget_t* widget, float_t width) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(health_circle != NULL, RET_BAD_PARAMS);

  health_circle->width = width;

  return widget_invalidate(widget, NULL);
}

static ret_t health_circle_on_paint_self(widget_t* widget, canvas_t* c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  float_t rr = widget->w/2 > widget->h/2 ? widget->h/2 : widget->w/2;

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_set_line_width(vg, health_circle->width);
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  vgcanvas_set_line_cap(vg, "round");
  // vgcanvas_set_antialias(vg, TRUE);
  // draw big circle
  vgcanvas_save(vg);
  color_t bg_color_b = color_init(color_r(&health_circle->color_b)/4,
                                  color_g(&health_circle->color_b)/4,
                                  color_b(&health_circle->color_b)/4,
                                  color_a(&health_circle->color_b));
  vgcanvas_set_stroke_color(vg, bg_color_b);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, rr-health_circle->width/2, 0, 6.3, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, health_circle->color_b);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, rr-health_circle->width/2, -0.5*M_PI,
               (health_circle->value_b/health_circle->max_b) * 2*M_PI - 0.5*M_PI,
               FALSE);
  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);

  // draw middle circle
  vgcanvas_save(vg);
  color_t bg_color_m = color_init(color_r(&health_circle->color_m)/4,
                                  color_g(&health_circle->color_m)/4,
                                  color_b(&health_circle->color_m)/4,
                                  color_a(&health_circle->color_m));
  vgcanvas_set_stroke_color(vg, bg_color_m);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, rr-health_circle->width*1.5-2, 0, 6.3, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, health_circle->color_m);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, rr-health_circle->width*1.5-2, -0.5*M_PI,
               (health_circle->value_m/health_circle->max_m) * 2*M_PI - 0.5*M_PI,
               FALSE);
  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);

  // draw small circle
  vgcanvas_save(vg);
  color_t bg_color_s = color_init(color_r(&health_circle->color_s)/4,
                                  color_g(&health_circle->color_s)/4,
                                  color_b(&health_circle->color_s)/4,
                                  color_a(&health_circle->color_s));
  vgcanvas_set_stroke_color(vg, bg_color_s);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, rr-health_circle->width*2.5-4, 0, 6.3, FALSE);
  vgcanvas_stroke(vg);

  vgcanvas_set_stroke_color(vg, health_circle->color_s);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, rr-health_circle->width*2.5-4, -0.5*M_PI,
               (health_circle->value_s/health_circle->max_s) * 2*M_PI - 0.5*M_PI,
               FALSE);
  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t health_circle_on_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, HEALTH_CIRCLE_PROP_VALUE_B)) {
    return health_circle_set_value_b(widget, value_float(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_VALUE_M)) {
    return health_circle_set_value_m(widget, value_float(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_VALUE_S)) {
    return health_circle_set_vlaue_s(widget, value_float(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_MAX_B)) {
    return health_circle_set_value_max_b(widget, value_float(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_MAX_M)) {
    return health_circle_set_value_max_m(widget, value_float(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_MAX_S)) {
    return health_circle_set_value_max_s(widget, value_float(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_COLOR_B)) {
    return health_circle_set_color_b(widget, value_str(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_COLOR_M)) {
    return health_circle_set_color_m(widget, value_str(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_COLOR_S)) {
    return health_circle_set_color_s(widget, value_str(v));
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_WIDTH)) {
    return health_circle_set_width(widget, value_float(v));
  }

  return RET_NOT_FOUND;
}

static ret_t health_circle_on_get_prop(widget_t* widget, const char* name, value_t* v) {
  health_circle_t* health_circle = HEALTH_CIRCLE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, HEALTH_CIRCLE_PROP_MAX_B)) {
    value_set_float(v, health_circle->max_b);
    return RET_OK;
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_MAX_M)) {
    value_set_float(v, health_circle->max_m);
    return RET_OK;
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_MAX_S)) {
    value_set_float(v, health_circle->max_s);
    return RET_OK;
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_VALUE_B)) {
    value_set_float(v, health_circle->value_b);
    return RET_OK;
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_VALUE_M)) {
    value_set_float(v, health_circle->value_m);
    return RET_OK;
  } else if (tk_str_eq(name, HEALTH_CIRCLE_PROP_VALUE_S)) {
    value_set_float(v, health_circle->value_s);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* s_health_circle_clone_properties[] = {HEALTH_CIRCLE_PROP_VALUE_B,
                                                   HEALTH_CIRCLE_PROP_VALUE_M,
                                                   HEALTH_CIRCLE_PROP_VALUE_S,
                                                   HEALTH_CIRCLE_PROP_MAX_B,
                                                   HEALTH_CIRCLE_PROP_MAX_M,
                                                   HEALTH_CIRCLE_PROP_MAX_S,
                                                   HEALTH_CIRCLE_PROP_COLOR_B,
                                                   HEALTH_CIRCLE_PROP_COLOR_M,
                                                   HEALTH_CIRCLE_PROP_COLOR_S,
                                                   NULL};
static const widget_vtable_t s_health_circle_vtable = {.size = sizeof(health_circle_t),
                                                       .type = WIDGET_TYPE_HEALTH_CIRCLE,
                                                       .clone_properties = s_health_circle_clone_properties,
                                                       .create = health_circle_create,
                                                       .on_paint_self = health_circle_on_paint_self,
                                                       .get_prop = health_circle_on_get_prop,
                                                       .set_prop = health_circle_on_set_prop};

widget_t* health_circle_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  health_circle_t* health_circle = TKMEM_ZALLOC(health_circle_t);
  widget_t* widget = WIDGET(health_circle);
  return_value_if_fail(health_circle != NULL, NULL);

  widget_init(widget, parent, &s_health_circle_vtable, x, y, w, h);

  // set default value
  health_circle->value_s = 20;
  health_circle->value_m = 40;
  health_circle->value_b = 60;
  health_circle->max_s = 100;
  health_circle->max_m = 100;
  health_circle->max_b = 100;
  health_circle->color_b = color_init(0xEE, 0x11, 0xC2, 0xFF); // "#EE11C2"
  health_circle->color_m = color_init(0xC2, 0xEE, 0x11, 0xFF); // "#C2EE11"
  health_circle->color_s = color_init(0x09, 0xC7, 0xF7, 0xFF); // "#09C7F7"
  health_circle->width = 40;
  
  return widget;
}

widget_t* health_circle_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_health_circle_vtable, NULL);

  return widget;
}