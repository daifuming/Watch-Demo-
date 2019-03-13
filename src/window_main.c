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


/**
 * 点击application按钮，创建application窗口
 * */
static ret_t on_open_application(void* ctx, event_t* e){
  printf("click application\n");
  (void)e;
  (void)ctx;
  widget_t* win = window_open("application");
  if (win){
    // 初始化界面控件
    return RET_OK;
  }

  return RET_FAIL;
}

/**
 * 点击weather按钮，创建weather窗口
 * */
static ret_t on_open_weather(void* ctx, event_t* e){
  return RET_FAIL;
}

/**
 * 点击health按钮，创建health窗口
 * */
static ret_t on_open_health(void* ctx, event_t* e){
  return RET_FAIL;
}

/**
 * 为一个控件添加点击事件
 * */
static ret_t install_one(void* ctx, const void* iter)
{
  widget_t* widget = WIDGET(iter);

  if (widget->name != NULL){
    const char* name = widget->name;
    if (strstr(name, "application") != NULL){
      widget_on(widget, EVT_CLICK, on_open_application, widget);
    }else if(strstr(name, "weather") != NULL){
      widget_on(widget, EVT_CLICK, on_open_weather, widget);
    }else if (strstr(name, "health") != NULL){
      widget_on(widget, EVT_CLICK, on_open_health, widget);
    }
  }

  return RET_OK;
}


/**
 * 为widget所有控件添加点击事件
 * */
static void install_click_hander(widget_t *widget)
{
  widget_foreach(widget, install_one, widget);
}  


/**
 * 初始化
 */
void application_init() {
  widget_t* win = window_open("main");
  if (win){
    widget_layout(win);

    // 添加点击事件
    install_click_hander(win);
  }
}
