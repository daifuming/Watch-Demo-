




#include "circle_button_with_progress.h"

static ret_t circle_button_with_progress_on_event(widget_t* widget, event_t* e) {
  return RET_OK;
}


ret_t circle_button_with_progress_set_center_is_image(widget_t* widget, bool_t value) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  if (cbp->center_is_image != value) {
    if (!value) {
      cbp->image = NULL;
      cbp->zoom = FALSE;
    }
    cbp->center_is_image = value;
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t circle_button_with_progress_set_text(widget_t* widget, const char* text) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  // log_debug("str len = %d\n", strlen(text));
  // cbp->text = tk_str_copy(cbp->text, text);

  return widget_invalidate(widget, NULL); 
}

ret_t circle_button_with_progress_set_text_color(widget_t* widget, const char* color) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  color_from_str(&cbp->text_color, color);

  return widget_invalidate(widget, NULL);
}


ret_t circle_button_with_progress_set_text_bg_color(widget_t* widget, const char* color) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  color_from_str(&cbp->text_bg_color, color);

  return widget_invalidate(widget, NULL);
}

ret_t circle_button_with_progress_set_text_size(widget_t* widget, int8_t size) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  cbp->text_size = size;

  return widget_invalidate(widget, NULL);
}


ret_t circle_button_with_progress_set_image(widget_t* widget, const char* image) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);


  // log_debug("set image--input image name:%s\n", image);
  cbp->image = (char*)malloc(strlen(image)+1);
  memset(cbp->image, 0, strlen(image)+1);
  strcpy(cbp->image, image);
  cbp->center_is_image = TRUE;
  // cbp->image = tk_str_copy(cbp->image, image);

  return widget_invalidate(widget, NULL);
}

ret_t circle_button_with_progress_set_zoom(widget_t* widget, bool_t zoom) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  cbp->zoom = zoom;

  return widget_invalidate(widget, NULL);
}


ret_t circle_button_with_progress_set_value(widget_t* widget, float_t value) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  if (cbp->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    cbp->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t circle_button_with_progress_set_max(widget_t* widget, float_t max) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  cbp->max = max;

  return widget_invalidate(widget, NULL);
}


ret_t circle_button_with_progress_set_start_angle(widget_t* widget, int32_t angle) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  cbp->start_angle = angle;

  return widget_invalidate(widget, NULL);
}

ret_t circle_button_with_progress_set_progress_color(widget_t* widget, const char* color) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  color_from_str(&cbp->progress_color, color);

  return widget_invalidate(widget, NULL);
}

ret_t circle_button_with_progress_set_width(widget_t* widget, int8_t width) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  cbp->width = width;

  return widget_invalidate(widget, NULL);
}

ret_t circle_button_with_progress_set_counter_clock_wise(widget_t* widget, bool_t yes) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(cbp != NULL, RET_BAD_PARAMS);

  cbp->counter_clock_wise = yes;

  return widget_invalidate(widget, NULL);
}


static ret_t circle_button_with_progress_on_paint_self(widget_t* widget, canvas_t* c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  circle_button_with_progress_t* circle_button_with_progress = CIRCLE_BUTTON_WITH_PROGRESS(widget);

  // 当前值超过最大值时设置为最大值
  if (circle_button_with_progress->value > circle_button_with_progress->max) {
    circle_button_with_progress->value = circle_button_with_progress->max;
  }

  // 有图片/透明度的时候的画法
  vgcanvas_save(vg);
  if (circle_button_with_progress->center_is_image) {
    xy_t center_x = vg->w/2;
    wh_t center_y = vg->h/2;

    float_t dr =  (widget->w - 2*circle_button_with_progress->width) / 2;  // 中心圆部分的半径
    float_t dw = dr*2;    // 中心矩形的长宽

    // draw center circle
    vgcanvas_translate(vg, c->ox, c->oy);
    vgcanvas_save(vg);
    if (circle_button_with_progress->image != NULL) {
      bitmap_t image;
      widget_load_image(widget, circle_button_with_progress->image, &image);

      vgcanvas_translate(vg, widget->w/2, widget->h/2);
      vgcanvas_begin_path(vg);
      vgcanvas_ellipse(vg, 0, 0, dr+1, dr+1);
      vgcanvas_translate(vg, -image.w/2, -image.h/2);
      vgcanvas_draw_image(vg, &image, 0, 0, image.w, image.h, 0, 0, image.w, image.h);
    } else {
      vgcanvas_translate(vg, widget->w/2, widget->h/2);
      vgcanvas_begin_path(vg);

      // draw text
      float text_width = dw;
      float text_hight = circle_button_with_progress->text_size;
      vgcanvas_set_font_size(vg, circle_button_with_progress->text_size);
      vgcanvas_set_font(vg, TK_DEFAULT_FONT);
      text_width = vgcanvas_measure_text(vg, circle_button_with_progress->text);
      vgcanvas_translate(vg, -text_width/2, -text_hight/2);
      vgcanvas_fill_text(vg, circle_button_with_progress->text, 0, 0, dw);
    }
    vgcanvas_restore(vg);

    // calculate angle
    circle_button_with_progress->value = circle_button_with_progress->value > circle_button_with_progress->max ?
                                         circle_button_with_progress->max : circle_button_with_progress->value;
    float_t start_angle = -0.5*M_PI;
    float_t end_angle = (circle_button_with_progress->value/circle_button_with_progress->max) * 2*M_PI - 0.5*M_PI;

    // draw progress bar
    vgcanvas_save(vg);
    vgcanvas_translate(vg, widget->w/2, widget->h/2);
    color_t prog_bg_color = color_init(color_r(&circle_button_with_progress->progress_color)/4,
                                       color_g(&circle_button_with_progress->progress_color)/4,
                                       color_b(&circle_button_with_progress->progress_color)/4,
                                       color_a(&circle_button_with_progress->progress_color)
                                      );
    vgcanvas_set_line_width(vg, circle_button_with_progress->width);
    vgcanvas_set_line_cap(vg, "round");
    vgcanvas_set_stroke_color(vg, prog_bg_color);
    vgcanvas_begin_path(vg);
    vgcanvas_arc(vg, 0, 0, dr+circle_button_with_progress->width/2.0, start_angle, 2*3.14, circle_button_with_progress->counter_clock_wise);
    vgcanvas_stroke(vg);
    
    vgcanvas_set_stroke_color(vg, circle_button_with_progress->progress_color);
    vgcanvas_begin_path(vg);
    vgcanvas_arc(vg, 0, 0, dr+circle_button_with_progress->width/2.0, start_angle, end_angle, circle_button_with_progress->counter_clock_wise);
    vgcanvas_stroke(vg);
    vgcanvas_restore(vg);
  }
  vgcanvas_restore(vg);

  return RET_OK;
}


/**
 * 可以获得的属性：
 * - 是否为图片center_is_image
 * - 文本text
 * - 文本大小text_size
 * - 进度条数值value
 * - 进度条最大值max
 */
static ret_t circle_button_with_progress_get_prop(widget_t* widget, const char* name, value_t* v) {
  circle_button_with_progress_t* cbp = CIRCLE_BUTTON_WITH_PROGRESS(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_CENTER_IS_IMAGE)) {
    value_set_bool(v, cbp->center_is_image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    value_set_str(v, cbp->text);
    return RET_OK;
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_SIZE)) {
    value_set_int8(v, cbp->text_size);
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_float(v, cbp->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_float(v, cbp->max);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t circle_button_with_progress_set_prop(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_CENTER_IS_IMAGE)) {
    return circle_button_with_progress_set_center_is_image(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    return circle_button_with_progress_set_text(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_COLOR)) {
    return circle_button_with_progress_set_text_color(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_BG_COLOR)) {
    return circle_button_with_progress_set_text_bg_color(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_SIZE)) {
    return circle_button_with_progress_set_text_size(widget, value_int8(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_IMAGE)) {
    return circle_button_with_progress_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_ZOOM)) {
    return circle_button_with_progress_set_zoom(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return circle_button_with_progress_set_value(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return circle_button_with_progress_set_max(widget, value_float(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_START_ANGLE)) {
    return circle_button_with_progress_set_start_angle(widget, value_int32(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_PROGRESS_COLOR)) {
    return circle_button_with_progress_set_progress_color(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_WIDTH)) {
    return circle_button_with_progress_set_width(widget, value_int8(v));
  } else if (tk_str_eq(name, CIRCLE_BUTTON_WITH_PROGRESS_PROP_COUNTER_CLOCK_WISE)) {
    return circle_button_with_progress_set_counter_clock_wise(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}


static const char* s_circle_button_with_progress_clone_properties[] = {
                              WIDGET_PROP_VALUE,
                              WIDGET_PROP_MAX,
                              CIRCLE_BUTTON_WITH_PROGRESS_PROP_CENTER_IS_IMAGE,
                              CIRCLE_BUTTON_WITH_PROGRESS_PROP_START_ANGLE,
                              CIRCLE_BUTTON_WITH_PROGRESS_PROP_WIDTH,
                              NULL};

static const widget_vtable_t s_circle_button_with_progress_vtable = {
                              .size = sizeof(circle_button_with_progress_t),
                              .type = WIDGET_TYPE_CIRCLE_BUTTON_WITH_PROGRESS,
                              .clone_properties = s_circle_button_with_progress_clone_properties,
                              .create = circle_button_with_progress_create,
                              .on_paint_self = circle_button_with_progress_on_paint_self,
                              .on_event = circle_button_with_progress_on_event,
                              .get_prop = circle_button_with_progress_get_prop,
                              .set_prop = circle_button_with_progress_set_prop};


widget_t* circle_button_with_progress_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  // 申请内存构建widget
  circle_button_with_progress_t* circle_button_with_progress = TKMEM_ZALLOC(circle_button_with_progress_t);
  widget_t* widget = WIDGET(circle_button_with_progress);
  return_value_if_fail(circle_button_with_progress != NULL, NULL);

  // 申请到内存后初始化widget
  widget_init(widget, parent, &s_circle_button_with_progress_vtable, x, y, w, h);

  // 设置默认值
  circle_button_with_progress->center_is_image = TRUE;
  circle_button_with_progress->text = NULL;
  circle_button_with_progress->text_color = color_init(0x00, 0x00, 0x00, 0xff);
  circle_button_with_progress->text_bg_color = color_init(0xff, 0xff, 0xff, 0xff);
  circle_button_with_progress->text_size = 16;
  circle_button_with_progress->image = NULL;
  circle_button_with_progress->zoom = TRUE;
  circle_button_with_progress->value = 30;
  circle_button_with_progress->max = 100;
  circle_button_with_progress->start_angle = -90;
  circle_button_with_progress->progress_color = color_init(0xff, 0x00, 0x00, 0xff);
  circle_button_with_progress->width = 6;
  circle_button_with_progress->counter_clock_wise = FALSE;

  return widget;
}