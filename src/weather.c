#include "awtk.h"

static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget != NULL && widget->name != NULL) {
    widget_t* win = widget_get_window(widget);
    if (strstr(widget->name, "weather:")) {
      
    }
  }
}

static void init_children_widget(widget_t *widget){
  widget_foreach(widget, init_widget, widget);
} 

/**
 * 点击天气图标打开天气应用
 */
ret_t on_open_weather(void* ctx, event_t* e) {
  widget_t* win = window_open("weather");
  if (win) {
    init_children_widget(win);
    return RET_OK;
  }
  return RET_FAIL;
}