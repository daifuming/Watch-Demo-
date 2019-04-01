#include "awtk.h"

ret_t on_close_window(void* ctx, event_t* e);

static char* location[16] = {
  "guangzhou","beijing","shanghai",NULL
};  

static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget != NULL && widget->name != NULL) {
    widget_t* win = widget_get_window(widget);
    if (strstr(widget->name, "weather:")) {
      char* name = widget->name+8;
      if (tk_str_eq(name, "return")) {
        widget_on(widget, EVT_CLICK, on_close_window, win);
      }
    }
  }
}

static void init_children_widget(widget_t *widget){
  widget_foreach(widget, init_widget, widget);
} 


#include "custom_widgets/hour_weather.h"
/**
 * 初始化weather界面
 */
void static init_weather(widget_t* win) {
  return_if_fail(win != NULL);

  widget_t* slide_view = widget_lookup(win, "weather:view", TRUE);
  if (slide_view) {
    for (int i=0; i<16; i++) {
      if (location[i] != NULL) {
        // widget_t* view = view_create(slide_view, 0, 0, 0, 0);
        // widget_set_self_layout(view, "default(x=0,y=0,w=100%,h=100%)");
        // widget_set_children_layout(view, "default(c=1,r=1,m=5)");
        // widget_t* hour_weather = hour_weather_create(view, 0, 0, 0, 0);
        widget_clone(slide_view, slide_view->parent);
        continue;
      }
      break;
    }
  }
}

/**
 * 点击天气图标打开天气应用
 */
ret_t on_open_weather(void* ctx, event_t* e) {
  widget_t* win = window_open("weather");
  if (win) {
    init_weather(win);
    init_children_widget(win);
    return RET_OK;
  }
  return RET_FAIL;
}