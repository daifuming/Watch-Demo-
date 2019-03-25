/**
 * 
 * 
 * 
 */

#include "day_week.h"
#include "tkc/date_time.h"

static ret_t day_week_on_paint_self(widget_t* widget, canvas_t* c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  day_week_t* day_week = DAY_WEEK(widget);

  // 计算文本
  char text[16] = {0};
  date_time_t date;
  date_time_init(&date);
  if (1 == date.month || 2 == date.month) {
    date.month += 12;
    date.year --;
  }
  int32_t week = (date.day + 1 + 2 * date.month + 3 * (date.month + 1) / 5 + date.year + date.year / 4 - date.year / 100 + date.year / 400) % 7;
  switch (week){
    case 0: tk_snprintf(text, 16, "%2d 周日", date.day); break;
    case 1: tk_snprintf(text, 16, "%2d 周一", date.day); break;
    case 2: tk_snprintf(text, 16, "%2d 周二", date.day); break;
    case 3: tk_snprintf(text, 16, "%2d 周三", date.day); break;
    case 4: tk_snprintf(text, 16, "%2d 周四", date.day); break;
    case 5: tk_snprintf(text, 16, "%2d 周五", date.day); break;
    case 6: tk_snprintf(text, 16, "%2d 周六", date.day); break;
    default:
      break;
  }
  log_debug("str is \"%s\"\n", text);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_set_font(vg, "Deng");
  vgcanvas_set_font_size(vg, day_week->size);
  vgcanvas_set_fill_color(vg, day_week->color);
  float_t text_length = vgcanvas_measure_text(vg, text);
  vgcanvas_fill_text(vg, text, widget->w/2-text_length/2, widget->h/2-day_week->size, widget->w);

  vgcanvas_restore(vg);

  return RET_OK;
}

ret_t day_week_set_color(widget_t* widget, const char* color) {
  day_week_t* day_week = DAY_WEEK(widget);
  return_value_if_fail(day_week != NULL, RET_BAD_PARAMS);

  color_from_str(&day_week->color, color);

  return widget_invalidate(widget, NULL);
}

ret_t day_week_set_size(widget_t* widget, float_t size) {
  day_week_t* day_week = DAY_WEEK(widget);
  return_value_if_fail(day_week != NULL, RET_BAD_PARAMS);

  day_week->size = size;

  return widget_invalidate(widget, NULL);
}

static ret_t day_week_set_prop(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, DAY_WEEK_PROP_COLOR)) {
    return day_week_set_color(widget, value_str(v));
  } else if (tk_str_eq(name, DAY_WEEK_PROP_SIZE)) {
    return day_week_set_size(widget, value_float(v));
  }

  return RET_NOT_FOUND;
}


static const char* s_day_week_clone_properties[] = {DAY_WEEK_PROP_COLOR,
                                                    NULL};

static const widget_vtable_t s_day_week_vtable = {.size = sizeof(day_week_t),
                                                  .type = WIDGET_TYPE_DAY_WEEK,
                                                  .clone_properties = s_day_week_clone_properties,
                                                  .create = day_week_create,
                                                  .on_paint_self = day_week_on_paint_self,
                                                  .set_prop = day_week_set_prop};

widget_t* day_week_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  day_week_t* day_week = TKMEM_ZALLOC(day_week_t);
  widget_t* widget = WIDGET(day_week);
  return_value_if_fail(widget != NULL, NULL);

  widget_init(widget, parent, &s_day_week_vtable, x, y, w, h);

  day_week->color = color_init(0xff, 0x00, 0x00, 0xff);
  day_week->size = 22;

  return widget;
}