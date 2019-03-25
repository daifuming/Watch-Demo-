/**
 * 
 * 
 * 
 */

#ifndef TK_SCROLL_WEATHER_H
#define TK_SCROLL_WEATHER_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _scroll_weather_t {
  widget_t widget;

  int32_t yoffset;    // Y轴偏移量
} scroll_weather_t;

widget_t* scroll_weather_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define SCROLL_WEATHER(widget) ((scroll_weather_t*)(widget))
#define WIDGET_TYPE_SCROLL_WEATHER "scroll_weather"

#define SCROLL_WEATHER_PROP_YOFFSET "yoffset"

END_C_DECLS

#endif