/**
 * 
 * 
 * 
 */

/**
 * 弧度转坐标
 * x = r*cos(c)
 * y = r*sin(c)
 * c为弧度制，r为半径
 */
#include <math.h>

#include "hour_weather.h"

// [-5/6pai, -2/3pai, -1/2pai, -1/3pai, -1/6pai, 0, 1/6pai, 1/3pai, 1/2pai, 2/3pai, 5/6pai]
static float_t x_arr[] = {-0.866025404, -0.5, 0, 0.5, 0.866025404, 1, 0.866025404, 0.5, 0, -0.5, -0.866025404};
static float_t y_arr[] = {-0.5, -0.866025404, -1, -0.866025404, -0.5, 0, 0.5, 0.866025404, 1, 0.866025404, 0.5};

static void draw_clock_number(widget_t* widget, vgcanvas_t* vg, int32_t start, float_t r) {
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  return_if_fail(hour_weather != NULL && vg != NULL && r > 0);

  char str[3] = {0};      // 保存小时的字符串
  float_t text_width = 0; // 字符串的宽度

  vgcanvas_save(vg);
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  for (int i=0; i<11; i++) {
    vgcanvas_save(vg);
    vgcanvas_translate(vg, r*x_arr[i], r*y_arr[i]);

    // 生成字符串
    memset(str, 0, 3);
    sprintf(str, "%d", (hour_weather->time+i)%12);
    if (hour_weather->time == 12) {
      memset(str, 0, 3);
      sprintf(str, "%d", 12);
    }
    // log_debug("cal num str is %s\n", str);

    // 填写数字
    vgcanvas_set_font_size(vg, hour_weather->width-3);
    vgcanvas_set_font(vg, "defualt.mini");
    vgcanvas_set_fill_color(vg, color_init(0xff, 0xff, 0xff, 0xff));
    text_width = vgcanvas_measure_text(vg, str);
    // log_debug("text_width is %f\n", text_width);
    vgcanvas_translate(vg, -text_width/2, -(hour_weather->width-3)/2);
    vgcanvas_fill_text(vg, str, 0, 0, 100);

    vgcanvas_restore(vg);
  }
  vgcanvas_restore(vg);
}

static void draw_weather_icon(widget_t* widget, vgcanvas_t* vg, int32_t start, float_t r) {
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  return_if_fail(hour_weather != NULL && vg != NULL && r > 0);

  char image_name[32] = {0};

  vgcanvas_save(vg);
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  for (int i=0; i<11; i++) {
    bitmap_t image;
    memset(image_name, 0, 32);
    sprintf(image_name, "%d", hour_weather->weather[i]);
    widget_load_image(widget, image_name, &image);
    // log_debug("load image: %s\n", image_name);

    vgcanvas_save(vg);
    vgcanvas_translate(vg, r*x_arr[i], r*y_arr[i]);
    vgcanvas_translate(vg, -image.w/2, -image.h/2);
    vgcanvas_draw_image(vg, &image, 0, 0, image.w, image.h, 0, 0, image.w, image.w);

    vgcanvas_restore(vg);
  }
  vgcanvas_restore(vg);
}

static void draw_temp(widget_t* widget, vgcanvas_t* vg, int32_t start, float_t r) {
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  return_if_fail(hour_weather != NULL && vg != NULL && r > 0);

  char tmp[8] = {0};
  sprintf(tmp, "%d°", hour_weather->temp[hour_weather->selected]);
  // log_debug("select time temp is %s\n", tmp);

  // 填写数字
  vgcanvas_save(vg);
  vgcanvas_set_font_size(vg, widget->w/4);
  vgcanvas_set_font(vg, "Deng");
  vgcanvas_set_fill_color(vg, color_init(0xff, 0xff, 0xff, 0xff));
  float_t text_width = vgcanvas_measure_text(vg, tmp);
  // log_debug("text_width is %f\n", text_width);
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  vgcanvas_translate(vg, -text_width/2, -widget->w/8);
  vgcanvas_fill_text(vg, tmp, 0, 0, 100);
  vgcanvas_restore(vg);
}

static ret_t hour_weather_on_paint_self(widget_t* widget, canvas_t* c) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  float_t rr = widget->w/2 > widget->h/2 ? widget->h/2 : widget->w/2;
  float_t bar_r = rr/2;

  /* 绘制弧形背景 */
  vgcanvas_save(vg);
  vgcanvas_set_line_width(vg, hour_weather->width);  
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  vgcanvas_set_line_cap(vg, "round");
  vgcanvas_set_stroke_color(vg, hour_weather->bar_color);
  vgcanvas_begin_path(vg);
  vgcanvas_arc(vg, 0, 0, bar_r, -5/6.0*M_PI, 5/6.0*M_PI, FALSE);
  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);

  // 填充数字（时间）
  draw_clock_number(widget, vg, hour_weather->time, bar_r);

  // 填充圆点
  // log_debug("time is %d\n", hour_weather->time);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, widget->w/2, widget->h/2);
  vgcanvas_translate(vg, bar_r*cos(hour_weather->point_rad), 
                     bar_r*sin(hour_weather->point_rad));
  vgcanvas_begin_path(vg);
  vgcanvas_ellipse(vg, 0, 0, hour_weather->width/2-1, hour_weather->width/2-1);
  vgcanvas_set_fill_color(vg, hour_weather->point_color);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);

  // 绘制周围天气图标
  draw_weather_icon(widget, vg, hour_weather->time, 0.35*widget->w);

  // 绘制中心温度文字
  draw_temp(widget, vg, hour_weather->time, 0.35*widget->w);

  return RET_OK;
}

/**
 * 更新天气函数、、未写
 */


static uint8_t hour_health_get_click_select(widget_t* widget, xy_t x_f, xy_t y_f) {
  float_t result = 0;   // 记录弧度
  float_t rad = atan(fabs(y_f) / fabs(x_f));
  if (x_f > 0 && y_f > 0) {
    result = rad;
  }
  if (x_f < 0 && y_f > 0) {
    result = M_PI - rad;
  }
  if (x_f < 0 && y_f < 0) {
    result = M_PI + rad;
  }
  if (x_f > 0 && y_f < 0) {
    result = 2 * M_PI - rad;
  }
  // log_debug("rad is %f\n", result);

  result *= 12;
  // log_debug("rad is %f\n", result);
  uint8_t select = 255;
  if (result>M_PI && result<3*M_PI) { select = 6; }
  else if (result>3*M_PI && result<5*M_PI) { select = 7; }
  else if (result>5*M_PI && result<7*M_PI) { select = 8; }
  else if (result>7*M_PI && result<9*M_PI) { select = 9; }
  else if (result>9*M_PI && result<11*M_PI) { select = 10; }
  else if (result>13*M_PI && result<15*M_PI) { select = 0; }
  else if (result>15*M_PI && result<17*M_PI) { select = 1; }
  else if (result>17*M_PI && result<19*M_PI) { select = 2; }
  else if (result>19*M_PI && result<21*M_PI) { select = 3; }
  else if (result>21*M_PI && result<23*M_PI) { select = 4; }
  else if (result>23*M_PI || result<M_PI) { select = 5; }
  else { select = 0; }

  log_debug("select is %d\n", select);
  return select;
}

static void on_change_select(widget_t* widget) {
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  return_if_fail(widget != NULL);
  // 销毁动画
  widget_destroy_animator(widget, NULL);
  // 计算弧度值
  float_t change_rad = (hour_weather->selected-5)*M_PI/6;
  char param[128] = {0};
  tk_snprintf(param, 128, "point_rad(from=%f, to=%f, duration=300, easing=linear)", 
              hour_weather->point_rad, change_rad);
  log_debug("from %f, to %f\n",hour_weather->point_rad, change_rad );
  

  // 更新widget的弧度值
  hour_weather->point_rad = change_rad;
  widget_create_animator(widget, param);
}

static ret_t hour_weather_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  switch (type)
  {
    case EVT_POINTER_DOWN: {
      hour_weather->pressed = TRUE;
      break;
    }
    case EVT_POINTER_UP: {
      // 获取点击点
      pointer_event_t* pointer_event = (pointer_event_t*)e;
      point_t point = {pointer_event->x, pointer_event->y};
      widget_to_local(widget, &point);
      log_debug("(%d, %d) clicked!\n", (int32_t)point.x, (int32_t)point.y);

      // 获取点击的点代表的区域
      xy_t x_c = point.x - widget->w/2;
      xy_t y_c = point.y - widget->h/2;
      hour_weather->selected = hour_health_get_click_select(widget, x_c, y_c);

      on_change_select(widget);
      hour_weather->pressed = FALSE;

      break;
    }
    default:
      break;
  }

  return RET_OK;
}

ret_t hour_weather_set_point_rad(widget_t* widget, float_t rad) {
  hour_weather_t* hour_weather = HOUR_WEATHER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  hour_weather->point_rad = rad;

  return widget_invalidate(widget, NULL);
}


static ret_t hour_weather_on_set_prop(widget_t* widget, const char* name, const value_t* v) {
  if (tk_str_eq(name, "point_rad")) {
    return hour_weather_set_point_rad(widget, value_float(v));
  }
  return RET_NOT_FOUND;
}

static const widget_vtable_t s_hour_weather_vtable = {.size = sizeof(hour_weather_t),
                                                      .type = WIDGET_TYPE_HOUR_WEATHER,
                                                      .create = hour_weather_create,
                                                      .on_paint_self = hour_weather_on_paint_self,
                                                      .on_event = hour_weather_on_event,
                                                      .set_prop = hour_weather_on_set_prop};

widget_t* hour_weather_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  hour_weather_t* hour_weather = TKMEM_ZALLOC(hour_weather_t);
  widget_t* widget = WIDGET(hour_weather);
  return_value_if_fail(hour_weather != NULL, NULL);

  widget_init(widget, parent, &s_hour_weather_vtable, x, y, w, h);

  hour_weather->weather_info = NULL;
  hour_weather->width = 30;
  hour_weather->bar_color = color_init(0x24, 0x20, 0x21, 0xff);     // #242021
  hour_weather->point_color = color_init(0x33, 0xcc, 0xcc, 0xff);   // #33CCCC
  hour_weather->selected = 0;
  hour_weather->point_rad = -5.0/6*M_PI;

  // set time
  date_time_t dt;
  date_time_init(&dt);
  hour_weather->time = dt.hour%12;
  if (dt.hour == 12) {
    hour_weather->time = 12;
  }

  for (int i=0; i<11; i++) {
    hour_weather->temp[i] = i-4;
    hour_weather->weather[i] = i;
  }

  return widget;
}