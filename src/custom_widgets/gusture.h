
#ifndef TK_CUSTOM_GUSTURE_H
#define TK_CUSTOM_GUSTURE_H

#include "base/widget.h"

BEGIN_C_DECLS

#define EVT_SLIDE_DOWN EVT_USER_START+1
#define EVT_SLIDE_UP EVT_USER_START+2
#define EVT_SLIDE_LEFT EVT_USER_START+3
#define EVT_SLIDE_RIGHT EVT_USER_START+4

typedef struct _gusture_t{
  widget_t widget;

  /**
   * 按下时的点击点
   */
  point_t press_point;

  /**
   * 是否被按下
   */
  bool_t pressed;

  /**
   * 长按计时器id
   */
  int32_t timer_id;
} gusture_t;

widget_t* gusture_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define WIDGET_TYPE_GUSTURE "gusture"
#define GUSTURE(widget) ((gusture_t*)(widget))

END_C_DECLS

#endif
