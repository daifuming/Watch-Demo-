



#include "gusture.h"
#include "base/timer.h"

static ret_t gusture_remove_timer(widget_t* widget) {
  gusture_t* gusture = GUSTURE(widget);
  if (gusture->timer_id != TK_INVALID_ID) {
    timer_remove(gusture->timer_id);
    gusture->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

static ret_t gusture_pointer_up_cleanup(widget_t* widget) {
  gusture_t* gusture = GUSTURE(widget);

  gusture->pressed = FALSE;
  gusture_remove_timer(widget);
  widget_ungrab(widget->parent, widget);
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return RET_OK;
}

static ret_t gusture_on_long_press(const timer_info_t* info) {
  pointer_event_t evt;
  widget_t* widget = WIDGET(info->ctx);

  evt.x = 0;
  evt.y = 0;
  evt.e = event_init(EVT_LONG_PRESS, widget);

  gusture_pointer_up_cleanup(widget);
  widget_dispatch(widget, (event_t*)&evt);

  return RET_REMOVE;
}

static ret_t gusture_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  gusture_t* gusture = GUSTURE(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      gusture->pressed = TRUE;
      pointer_event_t* pointer_event = (pointer_event_t*)e;
      gusture->press_point.x = pointer_event->x;
      gusture->press_point.y = pointer_event->y;
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      gusture_remove_timer(widget);
      gusture->timer_id = timer_add(gusture_on_long_press, widget, TK_LONG_PRESS_TIME);
      widget_grab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      gusture_pointer_up_cleanup(widget);
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      if (gusture->pressed && widget_is_point_in(widget, evt.x, evt.y, FALSE)) {
        // 滑动事件
        if (evt.x != gusture->press_point.x || evt.y != gusture->press_point.y) {
          float_t distance_x = gusture->press_point.x - evt.x;
          float_t distance_y = gusture->press_point.y - evt.y;
          distance_x = distance_x>0 ? distance_x:-distance_x;
          distance_y = distance_y>0 ? distance_y:-distance_y;
          if (distance_x > distance_y) {
            if (distance_x < 80 && distance_x < widget->w*0.2f) {
              gusture_pointer_up_cleanup(widget);
              break;
            }
            if (evt.x > gusture->press_point.x) {
              evt.e = event_init(EVT_SLIDE_RIGHT, widget);
              widget_dispatch(widget, (event_t*)&evt);
            } else {
              evt.e = event_init(EVT_SLIDE_LEFT, widget);
              widget_dispatch(widget, (event_t*)&evt);
            }
          } else {
            if (distance_y < 80 && distance_y < widget->h*0.2f) {
              gusture_pointer_up_cleanup(widget);
              break;
            }
            if (evt.y > gusture->press_point.y) {
              evt.e = event_init(EVT_SLIDE_DOWN, widget);
              widget_dispatch(widget, (event_t*)&evt);
            } else {
              evt.e = event_init(EVT_SLIDE_UP, widget);
              widget_dispatch(widget, (event_t*)&evt);
            }
          }
        }
      }
      gusture_pointer_up_cleanup(widget);
      break;
    }
    default:
      break;
  }
}


static const widget_vtable_t s_gusture_vtable = {.size = sizeof(gusture_t),
                                                 .type = WIDGET_TYPE_GUSTURE,
                                                 .create = gusture_create,
                                                 .on_event = gusture_on_event};

widget_t* gusture_create(widget_t* parent,xy_t x,xy_t y,wh_t w,wh_t h){
  gusture_t* gusture = TKMEM_ZALLOC(gusture_t);
  widget_t* widget = WIDGET(gusture);
  return_value_if_fail(gusture != NULL, NULL);

  widget_init(widget, parent, &s_gusture_vtable, x, y, w, h);

  gusture->pressed = FALSE;

  return widget;
}