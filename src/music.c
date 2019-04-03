#include "awtk.h"

static struct _music_state {
  uint8_t play_state;     // 0:暂停播放，1:正在播放
  uint8_t play_mode;      // 0:playlist 1:loop 2:rand
  uint8_t play_vol;
} music_state = {0,0,5};



ret_t on_close_window(void* ctx, event_t* e);

/**
 * 点击上一首按钮
 */
static ret_t on_music_pre(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  log_debug("add pre sing code here!\n");
  return RET_OK;
}

/**
 * 点击下一首按钮
 */
static ret_t on_music_next(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  log_debug("add next sing code here!\n");
  return RET_OK;
}

/**
 * 点击播放、暂停按钮
 */
static ret_t on_music_play_pause(void* ctx, event_t* e) {
  widget_t* button = e->target;
  
  if (music_state.play_state == 0) {
    widget_use_style(button, "music_stop");
    music_state.play_state = 1;
  } else {
    widget_use_style(button, "music_play");
    music_state.play_state = 0;
  }

  return RET_OK; 
}

static char* play_mode[3] = {"music_playlist","music_loop",  "music_rand"};
static ret_t on_music_mode(void* ctx, event_t* e) {
  widget_t* button = e->target;
  if (++music_state.play_mode==3) {
    music_state.play_mode = 0;
  }
  widget_use_style(button, play_mode[music_state.play_mode]);
  
  return RET_OK;
}

static ret_t on_music_vol(void* ctx, event_t* e) {
  
}

static ret_t on_music_list(void* ctx, event_t* e) {
  
}

static ret_t init_widget(void* ctx, const void* iter) {
  widget_t* widget = WIDGET(iter);

  if (widget != NULL && widget->name != NULL) {
    widget_t* win = widget_get_window(widget);
    if (strstr(widget->name, "music:")) {
      char* name = widget->name+6;
      if (tk_str_eq(name, "return")) {
        widget_on(widget, EVT_CLICK, on_close_window, win);
      } else if (tk_str_eq(name, "pre")) {
        widget_on(widget, EVT_CLICK, on_music_pre, win);
      } else if (tk_str_eq(name, "pause_play")) {
        widget_on(widget, EVT_CLICK, on_music_play_pause, win);
      } else if (tk_str_eq(name, "next")) {
        widget_on(widget, EVT_CLICK, on_music_next, win);
      } else if (tk_str_eq(name, "mode")) {
        widget_on(widget, EVT_CLICK, on_music_mode, win);
      } else if (tk_str_eq(name, "vol")) {
        widget_on(widget, EVT_CLICK, on_music_vol, win);
      } else if (tk_str_eq(name, "list")) {
        widget_on(widget, EVT_CLICK, on_music_list, win);
      }
    }
  }

  return RET_OK;
}

static void init_children_widget(widget_t *widget){
  widget_foreach(widget, init_widget, widget);
} 

/**
 * 打开窗口时初始化播放状态，如播放模式、音量等
 */
static void init_music_state(widget_t* win) {
  return_if_fail(win != NULL);

  widget_t* widget = widget_lookup(win, "music:pause_play", TRUE);
  if (widget) {
    if (music_state.play_state == 0) {
      widget_use_style(widget, "music_play");
    } else {
      widget_use_style(widget, "music_stop");
    } 
  }

  widget = widget_lookup(win, "music:mode", TRUE);
  if (widget) {
    switch (music_state.play_mode)
    {
      case 0: widget_use_style(widget, "music_playlist"); break;
      case 1: widget_use_style(widget, "music_loop"); break;
      case 2: widget_use_style(widget, "music_rand"); break;
      default: break;
    }
  }

  widget = widget_lookup(win, "music:vol", TRUE);

}

/**
 * 打开音乐窗口
 */
ret_t on_open_music(void* ctx, event_t* e) {
  widget_t* win = window_open("music");
  if (win) {
    init_music_state(win);
    init_children_widget(win);
    return RET_OK;
  }

  return RET_FAIL;
}