/**
 * 
 * 
 * 
 */

#include "awtk.h"
#include "tkc/date_time.h"

static date_time_t date;          // 当前时间
static date_time_t date_current;  // 展示时间
static day_of_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};   // 每个月的天数

/**
 * 计算日历
 */
static void calendar_cal(widget_t* widget, date_time_t* date_toshow) {

  // 更新年份/月份标签
  widget_t* label = widget_lookup(widget, "calendar_date", TRUE);
  if (label) {
    char tmp[12] = {0};
    tk_snprintf(tmp, sizeof(tmp), "%4d/%02d", date_toshow->year, date_toshow->month);
    widget_set_text_utf8(label, tmp);
  }

  // 计算平闰年
  if ( ((date_toshow->year%4 == 0) && date_toshow->year%100!=0) || date_toshow->year%400==0 ) {
    day_of_month[1] = 29;
  } else {
    day_of_month[1] = 28;
  }

  // 计算当月1日为星期几
  int32_t offset = (1 + 1 + 2 * date_toshow->month + 3 * (date_toshow->month + 1) / 5 + date_toshow->year + date_toshow->year / 4 - date_toshow->year / 100 + date_toshow->year / 400) % 7;
  if (date_toshow->month == 1 || date_toshow->month == 2) {
    date_toshow->month += 12;
    date_toshow->year --; 
    offset = (1 + 1 + 2 * date_toshow->month + 3 * (date_toshow->month + 1) / 5 + date_toshow->year + date_toshow->year / 4 - date_toshow->year / 100 + date_toshow->year / 400) % 7;
    date_toshow->month -= 12;
    date_toshow->year ++;
  }
  
  // 初始化日历标签
  widget_t* labels = widget_lookup(widget, "calendar_labels", TRUE);
  if (labels) {
    WIDGET_FOR_EACH_CHILD_BEGIN(labels, iter, i)
    if (iter->name != NULL) {
      widget_set_text_utf8(iter, "");
    }
    widget_use_style(iter, "calendar");
    WIDGET_FOR_EACH_CHILD_END();
  }

  // 更改日历标签显示的日期
  char widget_name[16] = {0};
  char tmp_8[3] = {0};
  label = NULL;
  for (int i=0; i < day_of_month[date_toshow->month-1]; i++) {
    memset(widget_name, 0, 16);
    tk_snprintf(widget_name, 16, "calendar_day%02d", i+offset);
    label = widget_lookup(widget, widget_name, TRUE);
    if (label != NULL) {
      tk_snprintf(tmp_8, 3, "%d", i+1);
      widget_set_text_utf8(label, tmp_8);
      widget_use_style(label, "calendar");
      if (i+1 == date_toshow->day && date_toshow->month == date.month && date_toshow->year == date.year) {
        widget_use_style(label, "calendar_today");
      }
    }
  }
}

/**
 * 点击返回按钮关闭窗口
 */
static ret_t on_return(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  window_close(win);

  return RET_OK;
}

/**
 * 点击上翻按钮更新为上个月日历
 */
static ret_t on_calendar_up(void* ctx, event_t* e) {
  if (date_current.month == 1) {
    date_current.month = 12;
    date_current.year --;
  } else {
    date_current.month -= 1;
  }
  
  calendar_cal(ctx, &date_current);
  return RET_OK;
}

/**
 * 点击下翻按钮更新为下个月日历
 */
static ret_t on_calendar_down(void* ctx, event_t* e) {
  if (date_current.month == 12) {
    date_current.month = 1;
    date_current.year ++;
  } else {
    date_current.month += 1;
  }
  calendar_cal(ctx, &date_current);
  return RET_OK;
}

/**
 * 点击当前按钮更新日历为当前月日历
 */
static ret_t on_calendar_current(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);

  date_time_init(&date);
  date_time_init(&date_current);
  
  calendar_cal(ctx, &date);
  return RET_OK;
}

/**
 * 打开日历窗口
 */
ret_t on_open_calendar(void* ctx, event_t* e) {
  widget_t* win = window_open("calendar");
  if (win) {
    on_calendar_current(win, NULL);
    // init button
    widget_t* buttons = widget_lookup(win, "calendar_ctrl", TRUE);
    if (buttons) {
      WIDGET_FOR_EACH_CHILD_BEGIN(buttons, iter, i)
      if (tk_str_eq(iter->name, "calendar:return")) {
        widget_on(iter, EVT_CLICK, on_return, win);
      } else if (tk_str_eq(iter->name, "calendar:up")) {
        widget_on(iter, EVT_CLICK, on_calendar_up, win);
      } else if (tk_str_eq(iter->name, "calendar:current")) {
        widget_on(iter, EVT_CLICK, on_calendar_current, win);
      } else if (tk_str_eq(iter->name, "calendar:down")) {
        widget_on(iter, EVT_CLICK, on_calendar_down, win);
      }
      WIDGET_FOR_EACH_CHILD_END();
    }

    return RET_OK;
  }

  return RET_FAIL;
}