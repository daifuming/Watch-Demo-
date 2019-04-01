/**
 * 
 * 
 */

#ifndef TK_HEALTH_CIRCLE_H
#define TK_HEALTH_CIRCLE_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _health_circle_t {
  widget_t widget;

  float_t value_b;
  float_t value_m;
  float_t value_s;

  float_t max_b;
  float_t max_m;
  float_t max_s;

  color_t color_b;
  color_t color_m;
  color_t color_s;

  float_t width;
  bool_t only_big;
} health_circle_t;

widget_t* health_circle_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* health_circle_cast(widget_t* widget);
ret_t health_circle_set_value_b(widget_t* widget, float_t value);
ret_t health_circle_set_value_m(widget_t* widget, float_t value);
ret_t health_circle_set_vlaue_s(widget_t* widget, float_t value);
ret_t health_circle_set_value_max_b(widget_t* widget, float_t value);
ret_t health_circle_set_value_max_m(widget_t* widget, float_t value);
ret_t health_circle_set_value_max_s(widget_t* widget, float_t value);

#define HEALTH_CIRCLE_PROP_VALUE_B "value_b"
#define HEALTH_CIRCLE_PROP_VALUE_M "value_m"
#define HEALTH_CIRCLE_PROP_VALUE_S "value_s"
#define HEALTH_CIRCLE_PROP_MAX_B "max_b"
#define HEALTH_CIRCLE_PROP_MAX_M "max_m"
#define HEALTH_CIRCLE_PROP_MAX_S "max_s"
#define HEALTH_CIRCLE_PROP_COLOR_B "color_b"
#define HEALTH_CIRCLE_PROP_COLOR_M "color_m"
#define HEALTH_CIRCLE_PROP_COLOR_S "color_s"
#define HEALTH_CIRCLE_PROP_WIDTH "width"
#define HEALTH_CIRCLE_PROP_ONLY_BIG "only_big"

#define WIDGET_TYPE_HEALTH_CIRCLE "health_circle"
#define HEALTH_CIRCLE(widget) ((health_circle_t*)(widget))

END_C_DECLS

#endif