#include "awtk.h"
#include "custom_widgets/health_circle.h"

/**
 * 打开每周摘要窗口
 */


static ret_t init_widget(void* ctx, const void* iter);
/**
 * 打开更改活动目标窗口
 * ctx为设置的目标，win为health_circle所在窗口
 */
static ret_t open_set_aims(void* ctx, widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  int index = ((int)ctx)-2;
  log_debug("index is %d\n", index);
  widget_t* win = window_open("set_aims");
  if (win) {
    widget_foreach(win, init_widget, (void*)index);

    widget_t* number = widget_lookup(win, "setaims:value", TRUE);
    value_t v;
    if (index==0) {
      widget_get_prop(widget, HEALTH_CIRCLE_PROP_MAX_B, &v);
    } else if (index==1) {
      widget_get_prop(widget, HEALTH_CIRCLE_PROP_MAX_M, &v);
    } else if (index==2) {
      widget_get_prop(widget, HEALTH_CIRCLE_PROP_MAX_S, &v);
    }
    int value = value_int(&v);
    char value_str[5] = {0};
    tk_snprintf(value_str, sizeof(value_str), "%d", value);
    widget_set_text_utf8(number, value_str);
  }
  return RET_OK;
}

/**
 * 点击对话框按钮
 */
static ret_t on_dialog_btn_click(void* ctx, event_t* e) {
  widget_t* win = widget_get_window(WIDGET(e->target));
  int code = (char*)ctx - (char*)NULL;

  dialog_quit(win, code);
  
  return RET_OK;
}

/**
 * ctx为长按的index
 * 点击按钮打开对话框
 */
static ret_t on_open_dialog(void* ctx, event_t* e) {
  widget_t* dialog = dialog_open("health_dialog");
  if (dialog) {
    widget_t* record = widget_lookup(dialog, "record", TRUE);
    widget_on(record, EVT_CLICK, on_dialog_btn_click, (char*)NULL+1);
    widget_t* set_aims = widget_lookup(dialog, "set_aims", TRUE);
    widget_on(set_aims, EVT_CLICK, on_dialog_btn_click, (char*)NULL+2);
    widget_t* bg = widget_lookup(dialog, "dialog_bg", TRUE);
    widget_on(bg, EVT_CLICK, on_dialog_btn_click, (char*)NULL+3);

    static int code = 0;
    code = dialog_modal(dialog);
    if (code==1) {
      log_debug("add open record code here!\n");
    } else if (code==2) {
      log_debug("add open set_aims code here!\n");
      widget_t* win = widget_get_window(e->target);
      widget_t* health_circle = widget_lookup(win, "health:all", TRUE);
      if (health_circle) {
        open_set_aims(ctx, health_circle);
      }
    }
    return RET_OK;
  } 

  return RET_FAIL;
}

static ret_t on_close_window(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  window_close(win);

  return RET_OK;
}


/**
 * 健康圆环数据发生改变时（如更改了活动目标/数值），在此函数更新其他三个view中圆环的数据
 */
static ret_t on_update_others(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  for (int index=2; index<=4; index++) {
    char name[16] = {0};tk_snprintf(name, 16, "health:view%dbtn", index);
    widget_t* btn = widget_lookup(win, name, TRUE);
    if (btn) {
      widget_t* circle = widget_lookup(btn, "circle", TRUE);
      widget_t* value = widget_lookup(btn, "value", TRUE);
      widget_t* max = widget_lookup(btn, "max", TRUE);
      value_t v;
      char value_str[4] = {0};
      char max_str[20] = {0};
      switch (index)
      {
        case 2: 
          widget_get_prop(e->target, HEALTH_CIRCLE_PROP_MAX_B, &v);
          widget_set_prop(circle, HEALTH_CIRCLE_PROP_MAX_B, &v);
          tk_snprintf(max_str, sizeof(max_str), "%d分钟", value_int(&v));
          widget_get_prop(e->target, HEALTH_CIRCLE_PROP_VALUE_B, &v);
          widget_set_prop(circle, HEALTH_CIRCLE_PROP_VALUE_B, &v);
          tk_snprintf(value_str, sizeof(value_str), "%d", value_int(&v));
          widget_set_text_utf8(value, value_str);
          widget_set_text_utf8(max, max_str);
          break;
        case 3:
          widget_get_prop(e->target, HEALTH_CIRCLE_PROP_MAX_M, &v);
          widget_set_prop(circle, HEALTH_CIRCLE_PROP_MAX_B, &v);
          tk_snprintf(max_str, sizeof(max_str), "%d分钟", value_int(&v));
          widget_get_prop(e->target, HEALTH_CIRCLE_PROP_VALUE_M, &v);
          widget_set_prop(circle, HEALTH_CIRCLE_PROP_VALUE_B, &v);
          tk_snprintf(value_str, sizeof(value_str), "%d", value_int(&v));
          widget_set_text_utf8(value, value_str);
          widget_set_text_utf8(max, max_str);
          break;
        case 4:
          widget_get_prop(e->target, HEALTH_CIRCLE_PROP_MAX_S, &v);
          widget_set_prop(circle, HEALTH_CIRCLE_PROP_MAX_B, &v);
          tk_snprintf(max_str, sizeof(max_str), "%d分钟", value_int(&v));
          widget_get_prop(e->target, HEALTH_CIRCLE_PROP_VALUE_S, &v);
          widget_set_prop(circle, HEALTH_CIRCLE_PROP_VALUE_B, &v);
          tk_snprintf(value_str, sizeof(value_str), "%d", value_int(&v));
          widget_set_text_utf8(value, value_str);
          widget_set_text_utf8(max, max_str);
          break;
        default:
          break;
      }
    }
  }

  return RET_OK;
}

/**
 * 初始化窗口控件
 */
static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget != NULL && widget->name != NULL) {
    widget_t* win = widget_get_window(widget);
    if (tk_str_start_with(widget->name, "health:")) {
      char* name = widget->name+7;
      if (tk_str_eq(name, "return")) {
        widget_on(widget, EVT_CLICK, on_close_window, win);
      } else if (tk_str_eq(name, "view2btn")) {
        widget_on(widget, EVT_CLICK, on_open_dialog, (void*)2);
      } else if (tk_str_eq(name, "view3btn")) {
        widget_on(widget, EVT_CLICK, on_open_dialog, (void*)3);
      } else if (tk_str_eq(name, "view4btn")) {
        widget_on(widget, EVT_CLICK, on_open_dialog, (void*)4);
      } else if (tk_str_eq(name, "all")) {
        widget_on(widget, EVT_VALUE_CHANGED, on_update_others, win);
      }
    } else if (tk_str_start_with(widget->name, "setaims:")) {
      int index = (int)ctx;
      char *name = widget->name+8;
      char* button_label_style[3] = {"pink", "green", "blue"};
      char* inc[3] = {"pink_inc", "green_inc", "blue_inc"};
      char* dec[3] = {"pink_dec", "green_dec", "blue_dec"};
      char* ok[3] = {"pink_ok", "green_ok", "blue_ok"};
      char* title[3] = {"更改每日活动目标", "更改每日锻炼目标", "更改每日站立目标"};
      if (tk_str_eq(name, "return")) {
        widget_use_style(widget, button_label_style[index]);
        widget_on(widget, EVT_CLICK, on_close_window, win);
      } else if (tk_str_eq(name, "title")) {
        widget_set_text_utf8(widget, title[index]);
      } else if (tk_str_eq(name, "dec")) {
        widget_use_style(widget, dec[index]);
      } else if (tk_str_eq(name, "inc")) {
        widget_use_style(widget, inc[index]);
      } else if (tk_str_eq(name, "cal")) {
        widget_use_style(widget, button_label_style[index]);
      } else if (tk_str_eq(name, "ok")) {
        widget_use_style(widget, ok[index]);
      }
    }
  }

  return RET_OK;
}

static void init_children_widget(widget_t *widget){
  widget_foreach(widget, init_widget, widget);
}  

ret_t on_opne_health(void* ctx, event_t* e) {
  widget_t* win = window_open("health");
  if (win) {
    init_children_widget(win);
    return RET_OK;
  }
  return RET_FAIL;
}