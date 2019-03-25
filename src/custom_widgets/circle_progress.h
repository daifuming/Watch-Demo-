/**
 * 
 * 
 * 
 */


#ifndef TK_CIRCLE_PROGRESS_H
#define TK_CIRCLE_PROGRESS_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _circle_progress_t{
  widget_t widget;

  float_t value;
  uint32_t max;
  int32_t start_angle;
  int32_t end_angle;
  uint32_t line_width;
  bool_t show_text;       // 是否显示文本
  bool_t mark_local;       // 标识文本的位置 false:中间下部 true 进度条左方

  color_t value_color;
  color_t mark_color;

  char* mark;
} circle_progress_t;

widget_t* circle_progress_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define WIDGET_TYPE_CIRCLE_PROGRESS "circle_progress"
#define CIRCLE_PROGRESS(widget) ((circle_progress_t*)(widget))

#define CIRCLE_PROGRESS_PROP_VALUE "value"
#define CIRCLE_PROGRESS_PROP_MAX "max"
#define CIRCLE_PROGRESS_PROP_START_ANGLE "start_angle"
#define CIRCLE_PROGRESS_PROP_END_ANGLE "end_angle"
#define CIRCLE_PROGRESS_PROP_LINE_WIDTH "line_width"
#define CIRCLE_PROGRESS_PROP_SHOW_TEXT "show_text"
#define CIRCLE_PROGRESS_PROP_MARK_LOCAL "mark_local"
#define CIRCLE_PROGRESS_PROP_VALUE_COLOR "value_color"
#define CIRCLE_PROGRESS_PROP_MARK_COLOR "mark_color"
#define CIRCLE_PROGRESS_PROP_MARK "mark"

END_C_DECLS

#endif