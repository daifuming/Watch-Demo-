/**
 * 
 * 
 */

#ifndef TK_CIRCLE_BUTTON_H
#define TK_CURCLE_BUTTON_H

#include "base/widget.h"
#include "base/image_base.h"

typedef struct _circle_button_t {
  widget_t widget;

  char* image;
  color_t bg_color;
} circle_button_t;

widget_t* circle_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);


#define CIRCLE_BUTTON(widget) ((circle_button_t*)(widget))
#define WIDGET_TYPE_CIRCLE_BUTTON "circle_button"

#define CIRCLE_BUTTON_PROP_IMAGE "image"
#define CIRCLE_BUTTON_PROP_BG_COLOR "bg_color"


#endif