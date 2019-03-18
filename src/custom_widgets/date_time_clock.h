/**
 * 
 * 
 * 
 */


#ifndef TK_DATE_TIME_CLOCK_H
#define TK_DATE_TIME_CLICK_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _date_time_clock_t {
  widget_t widget;

  color_t date_color;
  color_t time_color;
} date_time_clock_t;

widget_t* date_time_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* date_time_clock_cast(widget_t* widget);

#define DATE_TIME_PROP_DATE_COLOR "date_color"
#define DATE_TIME_PROP_TIME_COLOR "time_color"

#define WIDGET_TYPE_DATE_TIME_CLOCK "date_time_CLOCK"
#define DATE_TIME(widget) ((date_time_clock_t*)(widget))

END_C_DECLS

#endif