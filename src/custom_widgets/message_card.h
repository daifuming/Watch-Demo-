

#ifndef TK_MESSAGE_CARD_H
#define TK_MESSAGE_CARD_H

#include "base/widget.h"

typedef struct _message_card_t {
  widget_t widget;

  char* message_icon;     // 图标图片名
  wstr_t* name;     // 应用的名称
  wstr_t* message;     // 需要展示的消息内容
  wstr_t* time;     // 消息产生的时间
  color_t icon_color;   // 图标的背景颜色
  float_t font_size;
} message_card_t;

/**
 * 创建message_card对象
 */
widget_t* message_card_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define WIDGET_TYPE_MESSAGE_CARD "message_card"
#define MESSAGE_CARD(widget) ((message_card_t*)(widget))

#define MESSAGE_CARD_PROP_ICON "message_icon"
#define MESSAGE_CARD_PROP_NAME "app_name"
#define MESSAGE_CARD_PROP_MESSAGE "message"
#define MESSAGE_CARD_PROP_TIME "time"
#define MESSAGE_CARD_PROP_ICON_COLOR "icon_color"

#endif