/**
 * 
 * 
 * 
 */



#include "message_card.h"


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
  if (message_card->icon != NULL) {
    bitmap_t icon;
    if (widget_load_image(widget, message_card->icon, &icon) == RET_OK) {
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
    canvas_draw_text_in_rect(c, message_card->name->str, 4, &dst);
  }

  // draw time
  if (message_card->time != NULL) {
    canvas_set_font(c, "Dengb", message_card->font_size);
    canvas_set_text_color(c, color_init(0xff, 0xff, 0xff, 0xff));
    canvas_set_text_align(c, ALIGN_H_LEFT, ALIGN_H_CENTER);
    rect_t dst = rect_init(10, 177.0/220*widget->h, widget->w, 35.0/220*widget->h);
    canvas_draw_text_in_rect(c, L"10:20 AM", 8, &dst);
  }

  // draw message
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

static ret_t message_card_set_prop(widget_t* widget, const char* name, const value_t* v) {
}


static ret_t message_card_on_destroy(widget_t* widget) {
  message_card_t* message_card = MESSAGE_CARD(widget);

  wstr_reset(message_card->name);
  TKMEM_FREE(message_card->name);
  wstr_reset(message_card->message);
  TKMEM_FREE(message_card->message);
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
  message_card->icon = "message_icon";

  message_card->name = TKMEM_ALLOC(sizeof(wstr_t));
  message_card->name = wstr_init(message_card->name, 0);
  wchar_t *name = TKMEM_ALLOC(sizeof(wchar_t)*5);
  utf8_to_utf16("1234", name, 5);
  wstr_append(message_card->name, name);
  TKMEM_FREE(name);

  message_card->message = TKMEM_ALLOC(sizeof(wstr_t));
  message_card->message = wstr_init(message_card->message, 0);
  char str[] = "消息萨法对啊chengjian床前明月光了";
  wchar_t *message = TKMEM_ALLOC(sizeof(wchar_t)*(strlen(str)+1));
  utf8_to_utf16(str, message, strlen(str)+1);
  wstr_append(message_card->message, message);
  TKMEM_FREE(message);

  message_card->time = NULL;
  message_card->icon_color = color_init(0xe6, 0xe6, 0x1a, 0xff);    // #E6E61A
  message_card->font_size = 26;

  return widget;
}