/**
 * 闹钟使用uint16_t数据表示。
 * 0-7位：分钟数
 * 8-11位：小时数
 * 12位：0为上午，1为下午
 * 13为：0为关闭，1为开启
 * 剩余位保留
 */


#include <math.h>
#include "awtk.h"
#include "custom_widgets/gusture.h"

#define ALARM_GET_MINUTE(alarm) ((alarm)&0x00ff)
#define ALARM_GET_HOUR(alarm) (((alarm)&0x0f00)>>8)
#define ALARM_GET_AMPM(alarm) ((alarm)&0x1000)
#define ALARM_SET_MINUTE(alarm,minute) (((alarm)&0xff00)|minute)
#define ALARM_SET_HOUR(alarm,hour) (((alarm)&0xf0ff)|(hour)<<8)
#define ALARM_SET_AMPM(alarm,ampm) (((alarm)&0xEFFF)|ampm)
#define ALARM_OPEN(alarm) ((alarm)|0x2000)
#define ALARM_CLOSE(alarm) ((alarm)&0xDFFF)
#define ALARM_IS_OPEN(alarm) ((alarm)&0x2000)
#define ALARM_IS_PM(alarm) ((alarm)&0x1000)
#define ALARM_AM 0
#define ALARM_PM 0x1000

static struct _alarm {
  uint16_t alarm_alarm[16];    // 保存闹钟
  uint16_t alarm_flag;               // 闹钟设置标志
  uint8_t alarm_number;              // 闹钟设置数量
} alarm;

static void init_children_widget(widget_t *widget);

//========================================================================================
//                                     alarm list
//========================================================================================

/**
 * ctx为传入的闹钟编号
 * 点击开关设置开启/关闭对应闹钟
 */
static ret_t on_alarmitem_switch(void* ctx, event_t* e) {
  int index = (int)ctx;
  widget_t* image = e->target;
  return_value_if_fail(index >= 0, RET_BAD_PARAMS);

  if (ALARM_IS_OPEN(alarm.alarm_alarm[index])) {
    alarm.alarm_alarm[index] = ALARM_CLOSE(alarm.alarm_alarm[index]);
    widget_set_prop_str(image, "image", "switch_off");
  } else {
    alarm.alarm_alarm[index] = ALARM_OPEN(alarm.alarm_alarm[index]);
    widget_set_prop_str(image, "image", "switch_on");
  }
  log_debug("change NO%d:%#X", index, alarm.alarm_alarm[index]);
  return RET_OK;
}

/**
 * ctx为销毁的item下标
 * 左滑item销毁闹钟
 */
static ret_t on_alarmitem_destroy(void* ctx, event_t* e) {
  // int index = (int)ctx;
  widget_t* button = e->target;
  int index = (int)ctx;
  
  return_value_if_fail(index >= 0, RET_BAD_PARAMS);

  alarm.alarm_alarm[index] = 0x0000;
  alarm.alarm_flag &= ~(1<<index);
  alarm.alarm_number --;

  log_debug("change NO%d:%#X", index, alarm.alarm_alarm[index]);
  widget_destroy(button->parent);

  return RET_OK;

}

/**
 * parent: 父控件
 * alarm_item: 闹钟数据
 * index: 闹钟对应编号
 * 创建一个alarm list item及其空间
 */
static ret_t create_alarmlist_item(widget_t* parent, uint16_t alarm_item, int index) {
  return_value_if_fail(parent != NULL && alarm_item != 0, RET_BAD_PARAMS);

  widget_t* list_item = list_item_create(parent, 0, 0, 0, 0);
  widget_use_style(list_item, "odd");
  widget_set_children_layout(list_item, "default(rows=1,cols=0)");

  widget_t* button = button_create(list_item, 0, 0, 0, 0);
  widget_set_self_layout(button, "default(x=c,y=m,w=100%)");
  widget_use_style(button, "alarmitem_bg");

  widget_t* label = label_create(button, 0, 0, 0, 0);
  widget_set_self_layout(label, "default(x=15,y=m,w=-70,h=50)");
  widget_use_style(label, "alarmitem_time");

  widget_t* image = image_create(button, 0, 0, 0, 0);
  widget_set_self_layout(image, "default(x=r:15,y=m,w=70,h=50)");
  widget_set_prop_bool(image, "clickable", TRUE);

  // init data
  char time[12] = {0};
  tk_snprintf(time, sizeof(time), "%s %02d:%02d",
              ALARM_IS_PM(alarm_item) ? "PM" : "AM",
              ALARM_GET_HOUR(alarm_item),
              ALARM_GET_MINUTE(alarm_item));
  widget_set_text_utf8(label, time);
  widget_set_prop_str(image, "image",
              ALARM_IS_OPEN(alarm_item) ? "switch_on" : "switch_off");

  //add event
  widget_on(image, EVT_CLICK, on_alarmitem_switch, (void*)index);
  widget_on(button, EVT_SLIDE_LEFT, on_alarmitem_destroy, (void*)index);

  return RET_OK;
}

/**
 * 初始化list item
 */
static void init_alarm_list(widget_t* win) {
  widget_t* view = widget_lookup(win, "alarmlist:view", TRUE);
  return_if_fail(win != NULL && view != NULL && alarm.alarm_number >= 0);

  for (int i=0; i<16; i++) {
    if (alarm.alarm_alarm[i] != 0) {
      create_alarmlist_item(view, alarm.alarm_alarm[i],i);
    }
  }
}


//========================================================================================
//                                     alarm clock
//========================================================================================

/**
 * 窗口下滑进入列表
 */
static ret_t on_alarm_slide_down(void* ctx, event_t* e) {
  log_debug("add open alarm list code here!\n");
  widget_t* win = window_open("alarm_list");
  if (win) {
    init_children_widget(win);
    init_alarm_list(win);       // 根据设置的闹钟在list中添加item
    return RET_OK;
  }
  return RET_FAIL;
}

/**
 * 点击cancel按钮返回上一窗口
 */
static ret_t on_return(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  window_close(win);

  return RET_OK;
}

/**
 * 点击set按钮设置闹钟
 */
static ret_t on_alarm_set(void *ctx, event_t* e) {

  if (alarm.alarm_number < 16) {
    widget_t* win = WIDGET(ctx);
    widget_t* alarm_hour = widget_lookup(win, "alarm:hour", TRUE);
    widget_t* alarm_minute = widget_lookup(win, "alarm:minute", TRUE);
    widget_t* alarm_am = widget_lookup(win, "alarm:am", TRUE);
    if (alarm_hour && alarm_minute && alarm_am) {
      // 根据界面获取到点击时设置的时间
      uint16_t hour = alarm_text_selector_get_value(alarm_hour);
      uint16_t minute = alarm_text_selector_get_value(alarm_minute);
      uint16_t ampm = tk_str_eq(widget_get_prop_str(alarm_am, "image", NULL), "alarm_am_select") ?
                          ALARM_AM : ALARM_PM;
      log_debug("set alarm: %d:%d, ampm: %d\n", hour, minute, ampm);

      // 根据时间转换为alarm
      for (int i=0; i<16; i++) {
        if (((1<<i)&alarm.alarm_flag) == 0) {   // 查找可用位置
          alarm.alarm_alarm[i] = ALARM_SET_HOUR(alarm.alarm_alarm[i], hour); 
          alarm.alarm_alarm[i] = ALARM_SET_MINUTE(alarm.alarm_alarm[i], minute); 
          alarm.alarm_alarm[i] = ALARM_SET_AMPM(alarm.alarm_alarm[i], ampm); 
          alarm.alarm_alarm[i] = ALARM_OPEN(alarm.alarm_alarm[i]);
          alarm.alarm_flag = alarm.alarm_flag|(1<<i);
          alarm.alarm_number ++;
          log_debug("set alarm is %#X, flag is %#X\n", alarm.alarm_alarm[i], alarm.alarm_flag);
          break;
        }
      }
    } else {
      return RET_NOT_FOUND;
    }
  } else {
    log_debug("too many alarm !\n");
    return RET_FAIL;
  }

  return RET_OK;
}

/**
 * 点击AM按钮设置为上午
 * */
static ret_t on_alarm_am(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* pm_image = widget_lookup(win, "alarm:pm", TRUE);
  if (pm_image) {
    widget_set_prop_str(pm_image, "image", "alarm_pm_no_select");
    widget_set_prop_str(e->target, "image", "alarm_am_select");
  }

  return RET_OK;
}

/**
 * 点击PM按钮设置为下午
 * */
static ret_t on_alarm_pm(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* am_image = widget_lookup(win, "alarm:am", TRUE);
  if (am_image) {
    widget_set_prop_str(am_image, "image", "alarm_am_no_select");
    widget_set_prop_str(e->target, "image", "alarm_pm_select");
  }

  return RET_OK;
}

/**
 * 分钟数改变时更改小圆点的位置
 */
static ret_t on_alarm_minute_value_changed(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* alarm_point = widget_lookup(win, "alarm:point", TRUE);
  widget_t* alarm_bg = widget_lookup(win, "alarm:bg", TRUE);
  if (alarm_point && alarm_bg) {
    int32_t value = alarm_text_selector_get_value(e->target);

    // 计算圆点位置并赋值
    float_t r = 125;  // 圆点所在的圆根据时钟背景图来设置，本例中半径为125处
    int32_t x = (int32_t)(r*cos(TK_D2R(6*value-90)) + r);
    int32_t y = (int32_t)(r*sin(TK_D2R(6*value-90)) + r);

    widget_set_prop_int(alarm_point, "x", x-3); // 圆点图片大小为6x6,半径为3
    widget_set_prop_int(alarm_point, "y", y-3);
  }
  return RET_OK;
}

/**
 * 小时数改变时更改小圆点的位置
 */
static ret_t on_alarm_hour_value_changed(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* alarm_point = widget_lookup(win, "alarm:point", TRUE);
  widget_t* alarm_bg = widget_lookup(win, "alarm:bg", TRUE);
  if (alarm_point && alarm_bg) {
    int32_t value = alarm_text_selector_get_value(e->target);

    // 计算圆点位置并赋值
    float_t r = 125;  // 圆点所在的圆根据时钟背景图来设置，本例中半径为125处
    int32_t x = (int32_t)(r*cos(TK_D2R(30*value-90)) + r);
    int32_t y = (int32_t)(r*sin(TK_D2R(30*value-90)) + r);

    widget_set_prop_int(alarm_point, "x", x-3); // 圆点图片大小为6x6,半径为3
    widget_set_prop_int(alarm_point, "y", y-3);
  }
  return RET_OK;
}


static ret_t on_alarm_minute_pointer_down(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* alarm_hour_bg = widget_lookup(win, "alarm:hour_bg", TRUE);
  widget_t* alarm_minute_bg = widget_lookup(win, "alarm:minute_bg", TRUE);
  widget_t* alarm_minute = widget_lookup(win, "alarm:minute", TRUE);

  if (alarm_hour_bg && alarm_minute_bg) {
    widget_set_prop_str(alarm_hour_bg, "image", "alarm_no_select");
    widget_set_prop_str(alarm_minute_bg, "image", "alarm_selected");
    if (alarm_minute) {
      event_t new_e = event_init(EVT_VALUE_CHANGED, alarm_minute);
      widget_dispatch(alarm_minute, &new_e);
    }
  }
  return RET_OK;
}

static ret_t on_alarm_hour_pointer_down(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  widget_t* alarm_hour_bg = widget_lookup(win, "alarm:hour_bg", TRUE);
  widget_t* alarm_minute_bg = widget_lookup(win, "alarm:minute_bg", TRUE);
  widget_t* alarm_hour = widget_lookup(win, "alarm:hour", TRUE);

  if (alarm_hour_bg && alarm_minute_bg ) {
    widget_set_prop_str(alarm_hour_bg, "image", "alarm_selected");
    widget_set_prop_str(alarm_minute_bg, "image", "alarm_no_select");
    if (alarm_hour) {
      event_t new_e = event_init(EVT_VALUE_CHANGED, alarm_hour);
      widget_dispatch(alarm_hour, &new_e);
    }
  }
  return RET_OK;
}

static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget != NULL && widget->name != NULL) {
    widget_t* win = widget_get_window(widget);
    if (strstr(widget->name, "alarm:")) {
      if (tk_str_eq(widget->name, "alarm:bg_button")) {
        widget_on(widget, EVT_SLIDE_DOWN, on_alarm_slide_down, win);
        widget_on(widget, EVT_SLIDE_RIGHT, on_return, win);
      } else if (tk_str_eq(widget->name, "alarm:cancel")) {
        widget_on(widget, EVT_CLICK, on_return, win);
      } else if (tk_str_eq(widget->name, "alarm:set")) {
        widget_on(widget, EVT_CLICK, on_alarm_set, win);
      } else if (tk_str_eq(widget->name, "alarm:am")) {
        widget_on(widget, EVT_CLICK, on_alarm_am, win);
      } else if (tk_str_eq(widget->name, "alarm:pm")) {
        widget_on(widget, EVT_CLICK, on_alarm_pm, win);
      } else if (tk_str_eq(widget->name, "alarm:minute")) {
        widget_on(widget, EVT_VALUE_CHANGED, on_alarm_minute_value_changed, win);
        widget_on(widget, EVT_POINTER_DOWN, on_alarm_minute_pointer_down, win);
      } else if (tk_str_eq(widget->name, "alarm:hour")) {
        widget_on(widget, EVT_VALUE_CHANGED, on_alarm_hour_value_changed, win);
        widget_on(widget, EVT_POINTER_DOWN, on_alarm_hour_pointer_down, win);
      }
    } else if (strstr(widget->name, "alarmlist:")) {
      if (tk_str_eq(widget->name, "alarmlist:bg_button")) {
        widget_on(widget, EVT_SLIDE_UP, on_return, win);
      }
    }
  }

  return RET_OK;
}

static void init_children_widget(widget_t *widget){
  widget_foreach(widget, init_widget, widget);
}  

ret_t on_open_alarm(void* ctx, event_t* e) {
  widget_t* win = window_open("alarm_clock");
  if (win) {
    init_children_widget(win);
    return RET_OK;
  }
  return RET_FAIL;
}