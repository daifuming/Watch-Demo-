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
#include "custom_widgets/gusture.h"

static ret_t init_widget(void* ctx, const void* iter);
static ret_t on_dial_long_pressed(void* ctx, event_t* e);
ret_t on_open_calendar(void* ctx, event_t* e);
ret_t on_open_alarm(void* ctx, event_t* e);
ret_t on_open_weather(void* ctx, event_t* e);
ret_t on_open_health(void* ctx, event_t* e);
ret_t on_open_music(void* ctx, event_t* e);

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


ret_t on_close_window(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  window_close(win);

  return RET_OK;
}

/**
 * 用于循环切换天气
 */
static char* weather_icon[] = {
  "qingtian_48", "taiyang_48", "yintian_48", "xiaoyu_48"
};

static ret_t _on_timer_switch_weather(const timer_info_t* timer) {
  widget_t* image = timer->ctx;
  static i = 1;
  if (++i == 4) {
    i = 0;
  }
  widget_set_prop_str(image, "image", weather_icon[i]);
  widget_set_animation(image, "opacity(from=0,to=255,duration=500)");
  return RET_OK;
}

/**
 * 循环切换天气的图标定时器
 */
static ret_t on_timer_switch_weather(const timer_info_t* timer) {
  
  widget_t* image = timer->ctx;
  widget_set_animation(image, "opacity(to=0,duration=500)");
  timer_add(_on_timer_switch_weather, image, 500);

  return RET_REPEAT;
}

/**
 * 更新日期
 */
static ret_t on_timer_update_date(const timer_info_t* timer) {
  widget_t* widget = timer->ctx;
  char tmp_8[12] = {0};
  date_time_t date;
  date_time_init(&date);
  int32_t week = (date.day + 1 + 2 * date.month + 3 * (date.month + 1) / 5 + date.year + date.year / 4 - date.year / 100 + date.year / 400) % 7;

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
  widget_set_text_utf8(widget, tmp_8);

  return RET_REPEAT;
}

/**
 * 更新时间
 */
static ret_t on_timer_update_clock(const timer_info_t* timer) {
  widget_t* widget = timer->ctx;
  date_time_t date;
  date_time_init(&date);
  char tmp_8[12] = {0};

  tk_snprintf(tmp_8, 12, "%02d:%02d", date.hour, date.minute);
  widget_set_text_utf8(widget, tmp_8);

  return RET_REPEAT;
}

#include "custom_widgets/circle_progress.h"
#include "custom_widgets/health_circle.h"
#include "custom_widgets/circle_button_with_progress.h"
/**
 * 为表盘添加控件
 */
static void dial_add_widget(widget_t* widget, uint8_t type) {
  return_if_fail(widget != NULL && type >=0);

  switch (type) {
    case 0: {
      widget_use_style(widget, "time_dial");

      widget_t* image_sport_bg = image_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(image_sport_bg, "default(x=c,y=240,w=80,h=80)");
      widget_set_prop_str(image_sport_bg, "image", "weather_icon_bg");
      widget_t* image_sport = image_create(image_sport_bg, 0, 0, 0, 0);
      widget_set_self_layout(image_sport, "default(x=c,y=m,w=48,h=48)");
      widget_set_prop_str(image_sport, "image", "run_icon");
      widget_set_name(image_sport, "open:sport");

      widget_t* circle_prog_AQI = circle_progress_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(circle_prog_AQI, "default(x=c,y=80,w=80,h=80)");
      widget_set_prop_str(circle_prog_AQI, "mark", "AQI");
      widget_set_animation(circle_prog_AQI, "value(from=25, to=75, duration=3000, yoyo_times=0, easing=sin_inout)");

      widget_t* health_circle = health_circle_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(health_circle, "default(x=80,y=m,w=80,h=80)");
      widget_set_prop_int(health_circle, "width", 10);
      widget_set_animation(health_circle, 
              "value_b(from=0, to=100, duration=10000, repeat_times=0, easing=sin_inout);value_m(from=0, to=80, duration=10000, repeat_times=0, easing=sin_inout);value_s(from=0, to=50, duration=10000, repeat_times=0, easing=sin_inout)");
      
      widget_t* cbwp = circle_button_with_progress_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(cbwp, "default(x=240,y=m,w=80,h=80)");
      widget_set_prop_str(cbwp, "image", "music_icon");
      widget_set_animation(cbwp, "value(from=0, to=100, duration=10000, repeat_times=0, easing=linear)");
      
      break;
    }
    case 1: {
      widget_use_style(widget, "none");
      widget_t* label_date = label_create(widget, 0, 0, 0, 0);
      widget_set_name(label_date, "digit_dial:date");
      widget_set_self_layout(label_date, "default(x=right:33,y=0,w=100,h=50)");
      widget_use_style(label_date, "day_week");
      widget_add_timer(label_date, on_timer_update_date, 1000);

      widget_t* image_weather_bg = image_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(image_weather_bg, "default(x=33,y=50,w=80,h=80)");
      widget_set_prop_str(image_weather_bg, "image", "weather_icon_bg");
      widget_t* image_weather = image_create(image_weather_bg, 0, 0, 0, 0);
      widget_set_self_layout(image_weather, "default(x=c,y=m,w=48,h=48)");
      widget_set_name(image_weather, "digit_dial:weather");
      widget_set_prop_str(image_weather, "image", "taiyang_48");
      widget_add_timer(image_weather, on_timer_switch_weather, 5000);

      widget_t* label_time = label_create(widget, 0, 0, 0, 0);
      widget_set_name(label_time, "digit_dial:time");
      widget_set_self_layout(label_time, "default(x=right:33,y=50,w=245,h=80)");
      widget_use_style(label_time, "digit_clock");
      widget_add_timer(label_time, on_timer_update_clock, 1000);

      widget_t* image_hx_bg = image_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(image_hx_bg, "default(x=33,y=287,w=80,h=80)");
      widget_set_prop_str(image_hx_bg, "image", "hx_icon");

      widget_t* circle_prog = circle_progress_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(circle_prog, "default(x=c,y=287,w=80,h=80)");
      widget_set_prop_str(circle_prog, "mark", "AQI");
      widget_set_animation(circle_prog, "value(from=25, to=75, duration=3000, yoyo_times=0, easing=sin_inout)");
      
      widget_t* image_sport = image_create(widget, 0, 0, 0, 0);
      widget_set_self_layout(image_sport, "default(x=right:33,y=287,w=80,h=80)");
      widget_set_prop_str(image_sport, "image", "weather_icon_bg");

      break;
    }
    default: break;
  }
}


/**
 * ctx为主窗口
 * 更换表盘
 */
static ret_t on_dial_select(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* select_win = widget_get_window(e->target);
  return_value_if_fail(win != NULL && select_win != NULL, RET_BAD_PARAMS);

  static uint8_t dial_now = 0;

  // 获取用户的选择
  slide_view_t* select = SLIDE_VIEW(widget_lookup(select_win, "select:slideview", TRUE));
  log_debug("select:%d\n", select->active);
  if (select->active != dial_now) {
    // 更换表盘样式
    widget_t* button = widget_lookup(win, "clock_bg", TRUE);
    widget_destroy_children(button);
    dial_add_widget(button, select->active);
    dial_now = select->active;
  }

  return RET_OK;
}

/**
 * ctx:主界面窗口指针
 * 长按更换表盘
 */
static ret_t on_dial_long_pressed(void* ctx, event_t* e) {
  log_debug("on_dial_long_pressed!!\n");
  widget_t* win = window_open("select_dial");
  widget_t* window_mn = window_manager();
  if (win) {
    init_children_widget(win);
    widget_t* select = widget_lookup(win, "button:select_dial", TRUE);
    widget_on(select, EVT_CLICK, on_dial_select, ctx);
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
 * 表盘下滑事件
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
 * 表盘左滑事件
 */
static ret_t on_dial_slide_left(void* ctx, event_t* e) {
  log_debug("on_dial_slide_right!\n");
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
      widget_on(widget, EVT_SLIDE_LEFT, on_dial_slide_left, win);
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
      } else if (tk_str_eq(name, "weather")) {
        widget_t* win = widget_get_window(widget);
        widget_on(widget, EVT_CLICK, on_open_weather, win);
      } else if (tk_str_eq(name, "sport")) {
        widget_t* win = widget_get_window(widget);
        widget_on(widget, EVT_CLICK, on_open_health, win);
      } else if (tk_str_eq(name, "music")) {
        widget_t* win = widget_get_window(widget);
        widget_on(widget, EVT_CLICK, on_open_music, win);
      } else if (tk_str_eq(name, "health")) {
        widget_t* win = widget_get_window(widget); 
        widget_on(widget, EVT_CLICK, on_open_health, win); 
      } else if (tk_str_eq(name, "AQI")) {
        widget_t* win = widget_get_window(widget); 
        widget_on(widget, EVT_CLICK, on_open_weather, win); 
      } else if (tk_str_eq(name, "music")) {
        widget_t* win = widget_get_window(widget);
        widget_on(widget, EVT_CLICK, on_open_music, win);
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
