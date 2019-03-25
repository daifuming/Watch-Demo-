/**
 * 
 * 
 */


#include "date_time_clock.h"

static ret_t date_time_clock_on_paint_self(widget_t* widget, canvas_t* c) {
  return RET_OK;
}

static const char* s_date_time_clock_clone_properties[] = {DATE_TIME_PROP_DATE_COLOR,
                                                     DATE_TIME_PROP_TIME_COLOR,
                                                     NULL};
static const widget_vtable_t s_date_time_clock_vtable = {.size = sizeof(date_time_clock_t),
                                                   .type = WIDGET_TYPE_DATE_TIME_CLOCK,
                                                   .clone_properties = s_date_time_clock_clone_properties,
                                                   .create = date_time_clock_create,
                                                   .on_paint_self = date_time_clock_on_paint_self};

widget_t* date_time_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  date_time_clock_t* dtc = TKMEM_ZALLOC(date_time_clock_t);
  widget_t* widget = WIDGET(dtc);
  return_value_if_fail(dtc != NULL, NULL);

  widget_init(widget, parent, &s_date_time_clock_vtable, x, y, w, h);

  // set default value
  dtc->date_color = color_init(0xff, 0x00, 0x00, 0xff);  // #ff0000
  dtc->time_color = color_init(0xff, 0xff, 0xff, 0xff);  // #ffffff

  return widget;   
}

widget_t* date_time_clock_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_date_time_clock_vtable, NULL);

  return widget;
}