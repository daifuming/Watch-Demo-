/**
 * 
 * 
 * 
 */



#include "message_card.h"
#include "tkc/date_time.h"


static ret_t message_card_on_paint_self(widget_t* widget, canvas_t* c) {
  message_card_t* message_card = MESSAGE_CARD(widget);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  // draw bg message_card_bg
  bitmap_t message_card_bg;
  if (widget_load_image(widget, "message_card_bg", &message_card_bg) == RET_OK) {
    rect_t src = rect_init(0, 0, message_card_bg.w, message_card_bg.h);
    rect_t dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image(c, &message_card_bg, &src, &dst);
  }

  // draw icon bg
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);
  vgcanvas_ellipse(vg, 62.0/336*widget->w, 35.0/220*widget->h, 27.0/220*widget->h, 27.0/220*widget->h);
  vgcanvas_set_fill_color(vg, message_card->icon_color);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);

  // draw icon
  if (message_card->message_icon != NULL) {
    bitmap_t icon;
    if (widget_load_image(widget, message_card->message_icon, &icon) == RET_OK) {
      rect_t src = rect_init(0, 0, icon.w, icon.h);
      rect_t dst = rect_init(62.0/336*widget->w-icon.w/2, 35.0/220*widget->h-icon.h/2, icon.w, icon.h);
      canvas_draw_image(c, &icon, &src, &dst);
    }
  }

  // draw name
  if (message_card->name->size != 0) {
    canvas_set_font(c, "Dengb", message_card->font_size);
    canvas_set_text_color(c, color_init(0xff, 0xff, 0xff, 0xff));
    canvas_set_text_align(c, ALIGN_H_RIGHT, ALIGN_H_CENTER);
    rect_t dst = rect_init(0, 35.0/220*widget->h, widget->w-10, 35.0/220*widget->h);
    canvas_draw_text_in_rect(c, message_card->name->str, message_card->name->size, &dst);
  }

  // draw time
  if (message_card->time->size != 0) {
    canvas_set_font(c, "Dengb", message_card->font_size);
    canvas_set_text_color(c, color_init(0xff, 0xff, 0xff, 0xff));
    canvas_set_text_align(c, ALIGN_H_LEFT, ALIGN_H_CENTER);
    rect_t dst = rect_init(10, 177.0/220*widget->h, widget->w, 35.0/220*widget->h);
    canvas_draw_text_in_rect(c, message_card->time->str, message_card->time->size, &dst);
  }

  // draw message
  if (message_card->message->size != 0) {
    canvas_set_font(c, "Deng", message_card->font_size);
    canvas_set_text_color(c, color_init(0xff, 0xff, 0xff, 0xff));
    canvas_set_text_align(c, ALIGN_H_LEFT, ALIGN_H_CENTER);
    rect_t dst;    // {{10, 70, 316, 36}, {10, 106, 316, 36}, {10, 142, 316, 36}};
    wchar_t* p = message_card->message->str;
    for (int i=0; i<3; i++) {
      dst = rect_init(10, 70+36*i, 316, 36);
      int len = 0;
      wchar_t* q = p;
      while(*p != '\0'){
        if (300 < canvas_measure_text(c, q, len)) {
          if (i==2) {
            *(p-1) = '.';
            *(p-2) = '.';
            *(p-3) = '.';
            *p = '\0';
          }
          canvas_draw_text_in_rect(c, q, len, &dst);
          break;
        } else {
          len ++;
          p++;
        }
      }

      if (*p == '\0') {
        canvas_draw_text_in_rect(c, q, len, &dst);
          break;
      }
    }
  }
  return RET_OK;
}

static ret_t message_card_set_name(widget_t* widget, const char* name) {
  message_card_t* message_card = MESSAGE_CARD(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  wchar_t* _name = TKMEM_ALLOC(sizeof(wchar_t)*(strlen(name)+1));
  utf8_to_utf16(name, _name, strlen(name)+1);
  wstr_clear(message_card->name);
  wstr_append(message_card->name, _name);
  TKMEM_FREE(_name);

  return RET_OK;
}

static ret_t message_card_set_time(widget_t* widget, const char* time) {
  message_card_t* message_card = MESSAGE_CARD(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  wchar_t* _time = TKMEM_ALLOC(sizeof(wchar_t)*(strlen(time)+1));
  utf8_to_utf16(time, _time, strlen(time)+1);
  wstr_clear(message_card->time);
  wstr_append(message_card->time, _time);
  TKMEM_FREE(_time);

  return RET_OK;
}

static ret_t message_card_set_message(widget_t* widget, const char* message) {
  message_card_t* message_card = MESSAGE_CARD(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  wchar_t* _message = TKMEM_ALLOC(sizeof(wchar_t)*(strlen(message)+1));
  utf8_to_utf16(message, _message, strlen(message)+1);
  wstr_clear(message_card->message);
  wstr_append(message_card->message, _message);
  TKMEM_FREE(_message);

  return RET_OK;
}

static ret_t message_card_set_message_icon(widget_t* widget, const char* icon) {
  message_card_t* message_card = MESSAGE_CARD(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  message_card->message_icon = TKMEM_REALLOC(message_card->message_icon, strlen(icon)+1);
  
  tk_snprintf(message_card->message_icon, strlen(icon)+1, "%s", icon);

  return RET_OK;
}

static ret_t message_card_set_icon_color(widget_t* widget, const char* color) {
  message_card_t* message_card = MESSAGE_CARD(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  color_from_str(&message_card->icon_color, color);

  return RET_OK;
}

static ret_t message_card_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, MESSAGE_CARD_PROP_NAME)) {
    return message_card_set_name(widget, value_str(v));
  } else if (tk_str_eq(name, MESSAGE_CARD_PROP_TIME)) {
    return message_card_set_time(widget, value_str(v));
  } else if (tk_str_eq(name, MESSAGE_CARD_PROP_MESSAGE)) {
    return message_card_set_message(widget, value_str(v));
  } else if (tk_str_eq(name, MESSAGE_CARD_PROP_ICON)) {
    return message_card_set_message_icon(widget, value_str(v));
  } else if (tk_str_eq(name, MESSAGE_CARD_PROP_ICON_COLOR)) {
    return message_card_set_icon_color(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}


static ret_t message_card_on_destroy(widget_t* widget) {
  message_card_t* message_card = MESSAGE_CARD(widget);

  wstr_reset(message_card->name);
  TKMEM_FREE(message_card->name);
  wstr_reset(message_card->message);
  TKMEM_FREE(message_card->message);
  wstr_reset(message_card->time);
  TKMEM_FREE(message_card->time);
  TKMEM_FREE(message_card->message_icon);

  return RET_OK;
}

static const widget_vtable_t s_message_card_vtable = {.size = sizeof(message_card_t),
                                                      .type = WIDGET_TYPE_MESSAGE_CARD,
                                                      .create = message_card_create,
                                                      .on_paint_self = message_card_on_paint_self,
                                                      .set_prop = message_card_set_prop,
                                                      .on_destroy = message_card_on_destroy};

widget_t* message_card_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  message_card_t* message_card = TKMEM_ZALLOC(message_card_t);
  widget_t* widget = WIDGET(message_card);
  return_value_if_fail(widget != NULL, NULL);

  widget_init(widget, parent, &s_message_card_vtable, x, y, w, h);

  // set default value
  message_card->message_icon = NULL;

  message_card->name = TKMEM_ALLOC(sizeof(wstr_t));
  message_card->name = wstr_init(message_card->name, 0);

  message_card->message = TKMEM_ALLOC(sizeof(wstr_t));
  message_card->message = wstr_init(message_card->message, 0);

  message_card->time = TKMEM_ALLOC(sizeof(wstr_t));
  message_card->time = wstr_init(message_card->time, 0);
  date_time_t time;
  date_time_init(&time);
  char time_8[9] = {0};
  uint32_t hour = time.hour==12 ? time.hour : time.hour%12;
  tk_snprintf(time_8, 9, "%02d:%02d %s", hour, time.minute, time.hour>=12?"PM":"AM");
  wchar_t time_16[9] = {0};
  utf8_to_utf16(time_8, time_16, 9);
  wstr_append(message_card->time, time_16);

  message_card->icon_color = color_init(0xe6, 0xe6, 0x1a, 0xff);    // #E6E61A
  message_card->font_size = 26;

  return widget;
}