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

static ret_t init_widget(void* ctx, const void* iter);

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

  return open_application_window();
}

/**
 * 点击weather按钮，创建weather窗口
 * */
static ret_t on_weather(void* ctx, event_t* e){
  return RET_FAIL;
}

/**
 * 点击health按钮，创建health窗口
 * */
static ret_t on_health(void* ctx, event_t* e){
  return RET_FAIL;
}


/**
 * 为一个控件添加点击事件
 * */
static ret_t init_widget(void* ctx, const void* iter)
{
  widget_t* widget = WIDGET(iter);

  if (widget->name != NULL){
    const char* name = widget->name;
    if (strstr(name, "application") != NULL){
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_application, win);
    }else if(strstr(name, "weather") != NULL){
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_weather, win);
    }else if (strstr(name, "health") != NULL){
      widget_t* win = widget_get_window(widget);
      widget_on(widget, EVT_CLICK, on_health, win);
    }
  }

  return RET_OK;
}


/**
 * 初始化
 */
void application_init() {
  widget_t* win = window_open("main");
  if (win){   

    init_children_widget(win);
  }
}
