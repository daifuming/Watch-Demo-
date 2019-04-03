/**
 * 
 * 
 * 
 * 
 */

#include <math.h>
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "circle_progress.h"

static ret_t circle_progress_on_paint_self(widget_t* widget, canvas_t* c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);

  float_t r = widget->w/2-circle_progress->line_width/2;

  // 角度转弧度
  float_t start_rad = TK_D2R(circle_progress->start_angle);
  float_t end_rad = TK_D2R(circle_progress->end_angle);

  // 绘制弧形
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_set_line_width(vg, circle_progress->line_width);
  vgcanvas_set_line_cap(vg, "round");
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, widget->w/2, widget->h/2, r, start_rad, end_rad, FALSE);
  bitmap_t image;
  if (circle_progress->mark_local) {
    widget_load_image(widget, "grabient", &image);
  } else {
    widget_load_image(widget, "progress_circle", &image);
  }
  if (vgcanvas_paint(vg, TRUE, &image) != RET_OK) {
    vgcanvas_set_stroke_color(vg, color_init(0xff, 0xff, 0x00, 0xff));
    vgcanvas_stroke(vg);
  }
  vgcanvas_restore(vg);

  //// 绘制圆圈
  // 计算value弧度
  if (end_rad < start_rad) {
    end_rad += 2*M_PI;
  }
  float_t value_rad = (start_rad+(circle_progress->value/circle_progress->max)*(end_rad-start_rad));
  end_rad -= 2*M_PI;
  // 弧度转坐标
  float_t value_x = widget->w/2 + r*cos(value_rad);
  float_t value_y = widget->h/2 + r*sin(value_rad);
  // 绘制环
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, value_x, value_y, circle_progress->line_width/2+2, 0, 6.3, FALSE);
  vgcanvas_set_stroke_color(vg, color_init(0x00, 0x00, 0x00, 0xff));
  vgcanvas_set_line_width(vg, circle_progress->line_width/2);
  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);
  
  // 绘制中间数值图案
  if (circle_progress->mark_local == FALSE) {   // 绘制位置为中间/上方
    vgcanvas_save(vg);
    vgcanvas_translate(vg, c->ox, c->oy);
    vgcanvas_set_font(vg, "Dengb");
    vgcanvas_set_font_size(vg, widget->h/3);
    vgcanvas_set_fill_color(vg, circle_progress->value_color);
    char value_str[8] = {0};
    tk_snprintf(value_str, 8, "%d", (int)circle_progress->value);
    float_t text_length = vgcanvas_measure_text(vg, value_str);
    vgcanvas_fill_text(vg, value_str, widget->w/2-text_length/2, widget->h/3, widget->w);
    vgcanvas_restore(vg);
  }
  
  // 绘制标志文本，如空气指数"AQI"
  if (circle_progress->mark != NULL) {    // 有标志文本提供
    if (circle_progress->mark_local == FALSE) {   // 绘制位置为下方中央
      vgcanvas_save(vg);
      vgcanvas_translate(vg, c->ox, c->oy);
      vgcanvas_set_font(vg, "Deng");
      vgcanvas_set_font_size(vg, circle_progress->line_width*2);
      vgcanvas_set_fill_color(vg, circle_progress->mark_color);

      // 计算标志文本的y值
      float_t mark_y = widget->h/2 + r*sin(start_rad) - circle_progress->line_width*0.6;
      float_t mark_x = widget->w/2  - vgcanvas_measure_text(vg, circle_progress->mark)/2;

      vgcanvas_fill_text(vg, circle_progress->mark, mark_x, mark_y, widget->w/2);
      vgcanvas_restore(vg);
    }
  }

  return RET_OK;
}


ret_t circle_progress_set_value(widget_t* widget, float_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->value = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_max(widget_t*widget, uint32_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->max = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_start_angle(widget_t*widget, int32_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->start_angle = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_end_angle(widget_t*widget, int32_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->end_angle = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_line_width(widget_t*widget, uint32_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->line_width = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_show_text(widget_t*widget, bool_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->show_text = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_mark_local(widget_t*widget, bool_t v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->mark_local = v;

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_value_color(widget_t*widget, const char* v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  color_from_str(&circle_progress->value_color, v);

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_mark_color(widget_t*widget, const char* v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  color_from_str(&circle_progress->mark_color, v);

  return widget_invalidate(widget, NULL);
}

ret_t circle_progress_set_mark(widget_t*widget, const char* str) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  circle_progress->mark = tk_str_copy(circle_progress->mark, str);

  return widget_invalidate(widget, NULL);
}

static ret_t circle_progress_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  circle_progress_t* cirle_progress = CIRCLE_PROGRESS(widget);
  switch (type)
  {
    case EVT_POINTER_DOWN: {
      cirle_progress->pressed = TRUE;
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (cirle_progress->pressed == TRUE && widget_is_point_in(widget, evt.x, evt.y, FALSE)) {
        evt.e = event_init(EVT_CLICK, widget->parent);
        widget_dispatch(widget, (event_t*)&evt);
        cirle_progress->pressed = FALSE;
      }
      break;
    }
    default:
      break;
  }
}

static ret_t circle_progress_on_set_prop(widget_t* widget, const char* name, const value_t* v) {
  if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_VALUE)) {
    return circle_progress_set_value(widget, value_float(v));
  } else if(tk_str_eq(name, CIRCLE_PROGRESS_PROP_MAX)) {
    return circle_progress_set_max(widget, value_uint32(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_START_ANGLE)) {
    return circle_progress_set_start_angle(widget, value_int32(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_END_ANGLE)) {
    return circle_progress_set_end_angle(widget, value_int32(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_LINE_WIDTH)) {
    return circle_progress_set_line_width(widget, value_uint32(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_SHOW_TEXT)) {
    return circle_progress_set_show_text(widget, value_bool(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_MARK_LOCAL)) {
    return circle_progress_set_mark_local(widget, value_bool(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_VALUE_COLOR)) {
    return circle_progress_set_value_color(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_MARK_COLOR)) {
    return circle_progress_set_mark_color(widget, value_str(v));
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_MARK)) {
    return circle_progress_set_mark(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t circle_progress_on_get_prop(widget_t* widget, const char* name, value_t* v) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_VALUE)) {
    value_set_float(v, circle_progress->value);
    return RET_OK;
  } else if (tk_str_eq(name, CIRCLE_PROGRESS_PROP_MAX)) {
    value_set_uint32(v, circle_progress->max);
    return RET_OK;
  } 

  return RET_NOT_FOUND;
}


static ret_t circle_progress_on_destroy(widget_t* widget) {
  circle_progress_t* circle_progress = CIRCLE_PROGRESS(widget);

  TKMEM_FREE(circle_progress->mark);

  return RET_OK;

}


static const widget_vtable_t s_circle_progress_vtable = {.size = sizeof(circle_progress_t),
                                                         .type = WIDGET_TYPE_CIRCLE_PROGRESS,
                                                         .create = circle_progress_create,
                                                         .on_paint_self = circle_progress_on_paint_self,
                                                         .on_event = circle_progress_on_event,
                                                         .set_prop = circle_progress_on_set_prop,
                                                         .get_prop = circle_progress_on_get_prop,
                                                         .on_destroy = circle_progress_on_destroy};

widget_t* circle_progress_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  circle_progress_t* circle_progress = TKMEM_ZALLOC(circle_progress_t);
  widget_t* widget = WIDGET(circle_progress);
  return_value_if_fail(circle_progress != NULL, NULL);

  widget_init(widget, parent, &s_circle_progress_vtable, x, y, w, h);

  circle_progress->value = 45;
  circle_progress->max = 100;
  circle_progress->start_angle = 135;
  circle_progress->end_angle = 45;
  circle_progress->line_width = 6;
  circle_progress->show_text = TRUE;
  circle_progress->mark_local = FALSE;
  circle_progress->value_color = color_init(0xff, 0xff, 0xff, 0xff);  // #ffffff
  circle_progress->mark_color = color_init(0x08, 0xe7, 0x08, 0xff);   // #08e708
  circle_progress->mark = NULL;
  circle_progress->pressed = FALSE;

  return widget;
}