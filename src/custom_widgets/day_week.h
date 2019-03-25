/**
 * 
 * 
 * 
 */

#ifndef TK_DAY_WEEK_H
#define TK_DAY_WEEK_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _day_week_t{
  widget_t widget;

  color_t color;
  float_t size;
} day_week_t;

widget_t* day_week_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define WIDGET_TYPE_DAY_WEEK  "day_week"
#define DAY_WEEK(widget) ((day_week_t*)(widget))

#define DAY_WEEK_PROP_COLOR "color"
#define DAY_WEEK_PROP_SIZE "size"


END_C_DECLS

#endif