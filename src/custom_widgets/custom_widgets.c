/**
 * File:   custom_widgets.c
 * Author: AWTK Develop Team
 * Brief:  custom widgets
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
 * 2018-12-11 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#include "custom_widgets.h"
#include "circle_button_with_progress.h"
#include "health_circle.h"
#include "hour_weather.h"

ret_t custom_widgets_init() {
  widget_factory_register(widget_factory(), WIDGET_TYPE_CIRCLE_BUTTON_WITH_PROGRESS, circle_button_with_progress_create);
  widget_factory_register(widget_factory(), WIDGET_TYPE_HEALTH_CIRCLE, health_circle_create);
  widget_factory_register(widget_factory(), WIDGET_TYPE_HOUR_WEATHER, hour_weather_create);
  return RET_OK;
}
