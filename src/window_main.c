/**
 * File:   edit.c
 * Author: AWTK Develop Team
 * Brief:  window main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-11-20 TangJunJie created
 *
 */

#include "awtk.h"
#include "tkc/date_time.h"

static ret_t init_widget(void* ctx, const void* iter);
static ret_t on_dial_long_pressed(void* ctx, event_t* e);
ret_t on_open_calendar(void* ctx, event_t* e);
ret_t on_open_alarm(void* ctx, event_t* e);

/**
 * 为widget所有控件添加点击事件
 * */
static void init_children_widget(widget_t *widget){
  widget_foreach(widget, init_widget, widget);
}  

/**
 * 点击application按钮，创建application窗口
 * */
static ret_t on_application(void* ctx, event_t* e){
  (void)e;
  (void)ctx;

  log_debug("click application button\n");
  return open_application_window();
}

/**
 * 点击weather按钮，创建weather窗口
 * */
static ret_t on_weather(void* ctx, event_t* e){

  log_debug("click weather button\n");
  return RET_FAIL;
}

/**
 * 点击health按钮，创建health窗口
 * */
static ret_t on_health(void* ctx, event_t* e){

  log_debug("click health button\n");
  return RET_FAIL;
}


static ret_t on_close_window(void* ctx, event_t* e){
  widget_t* win = WIDGET(ctx);
  window_close(win);

  return RET_OK;
}

static void init_digit_dial(widget_t* widget) {
  date_time_t date;
  date_time_init(&date);
  int32_t week = (date.day + 1 + 2 * date.month + 3 * (date.month + 1) / 5 + date.year + date.year / 4 - date.year / 100 + date.year / 400) % 7;

  char tmp_8[12] = {0};
  switch (week)
  {
    case 0: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周日"); break;
    case 1: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周一"); break;
    case 2: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周二"); break;
    case 3: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周三"); break;
    case 4: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周四"); break;
    case 5: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周五"); break;
    case 6: tk_snprintf(tmp_8, 12, "%2d %s", date.day, "周六"); break;
    default:
      break;
  }

  widget_t* label = widget_lookup(widget, "digit_dial:date", TRUE);
  widget_set_text_utf8(label, tmp_8);
  
  memset(tmp_8, 0, sizeof(tmp_8));
  tk_snprintf(tmp_8, 12, "%02d:%02d", date.hour, date.minute);
  label = widget_lookup(widget, "digit_dial:time", TRUE);
  widget_set_text_utf8(label, tmp_8);
  
  widget_t* digit_dial = widget_lookup(widget, "digit_dial", TRUE);
  widget_on(digit_dial, EVT_LONG_PRESS, on_dial_long_pressed, widget );
}

/**
 * 更换表盘
 */
static ret_t on_dial_select(void* ctx, event_t* e) {
  widget_t* parent = WIDGET(ctx);
  widget_t* win = window_open_and_close("digit_dial", parent);
  init_digit_dial(win);

  return RET_OK;
}

/**
 * 长按更换表盘
 */
static ret_t on_dial_long_pressed(void* ctx, event_t* e) {
  log_debug("on_dial_long_pressed!!\n");
  widget_t* parent = WIDGET(ctx);
  widget_t* win = window_open("select_dial");
  if (win) {
    init_children_widget(win);
    widget_t* select = widget_lookup(win, "button:select_dial", TRUE);
    widget_on(select, EVT_CLICK, on_dial_select, parent);
  }
  return RET_OK;
}

/**
 * 消息界面上划收起
 */
static ret_t on_message_slide_top(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  window_close(win);

  return RET_OK;
}

/**
 * 从表盘界面下滑打开消息界面
 */
static ret_t open_message() {
  widget_t* win = window_open("message");
  widget_t* message = widget_lookup(win, "message_window", TRUE);
  widget_on(message, EVT_SLIDE_UP, on_message_slide_top, win);
  return RET_OK;
}

/**
 * 表盘右滑事件
 */
static ret_t on_dial_slide_bottom(void* ctx, event_t* e) {
  log_debug("on_dial_slide_button!\n");
  // window_open("message");
  open_message();
  return RET_OK;
}

static void open_application() {
  widget_t* win = window_open("application");
  if (win) {
    // init  children
    init_children_widget(win);
  }
}

/**
 * 表盘右滑事件
 */
static ret_t on_dial_slide_right(void* ctx, event_t* e) {
  log_debug("on_dial_slide_right!\n");
  // window_open("application");
  open_application();
  return RET_OK;
}

/**
 * 为一个控件添加点击事件
 * */
static ret_t init_widget(void* ctx, const void* iter)
{
  widget_t* widget = WIDGET(iter);

  if (widget->name != NULL){
    const char* name = widget->name;
    if (tk_str_eq(name, "clock_bg")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_LONG_PRESS, on_dial_long_pressed, win);
      widget_on(widget, EVT_SLIDE_DOWN, on_dial_slide_bottom, win);
      widget_on(widget, EVT_SLIDE_RIGHT, on_dial_slide_right, win);
    } else if (strstr(name, "return")) {
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_close_window, win);
    } else if (strstr(name, "open:")) {
      name += 5;
      if (tk_str_eq(name, "calendar")) {
        widget_t* win = widget_get_window(widget);
        widget_on(widget, EVT_CLICK, on_open_calendar, win);
      } else if (tk_str_eq(name, "alarm")) {
        widget_t* win = widget_get_window(widget);
        widget_on(widget, EVT_CLICK, on_open_alarm, win);
      }
    }
  }

  return RET_OK;
}

/**
 * 初始化
 */
void application_init() {
  widget_t* win = window_open("main");
  // calendar_cal(win);
  if (win){
    init_children_widget(win);
  }
}
