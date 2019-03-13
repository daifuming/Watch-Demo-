#include "awtk.h"

static ret_t on_close(void* ctx, event_t* e){
    widget_t* win = (widget_t*)ctx;
    (void)e;

    return window_close(win);
}

static ret_t init_widget(void* ctx, const void* iter){
    widget_t* widget = WIDGET(iter);

    (void)ctx;
    if (widget->name != NULL){
        const char* name = widget->name;
        if (strstr(name, "close") != NULL){
            widget_t* win = widget_get_window(widget);
            widget_on(widget, EVT_CLICK, on_close, win);
        }
    }
}

static void init_children_widget(widget_t* widget){
    widget_foreach(widget, init_widget, widget);
}


ret_t open_application_window(void){
    widget_t* win = window_open("application");

    if (win){
        init_children_widget(win);
        return RET_OK;
    }

    return RET_FAIL;
}