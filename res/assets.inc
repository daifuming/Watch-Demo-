#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "assets/inc/strings/en_US.data"
#include "assets/inc/strings/zh_CN.data"
#include "assets/inc/styles/button.data"
#include "assets/inc/styles/default.data"
#include "assets/inc/styles/keyboard.data"
#include "assets/inc/ui/alarm_clock.data"
#include "assets/inc/ui/alarm_list.data"
#include "assets/inc/ui/application.data"
#include "assets/inc/ui/calendar.data"
#include "assets/inc/ui/circle_button_test.data"
#include "assets/inc/ui/circle_button_with_progress_test.data"
#include "assets/inc/ui/day_week_test.data"
#include "assets/inc/ui/digit_dial.data"
#include "assets/inc/ui/health_circle_text.data"
#include "assets/inc/ui/heart_rate.data"
#include "assets/inc/ui/kb_ascii.data"
#include "assets/inc/ui/kb_default.data"
#include "assets/inc/ui/kb_float.data"
#include "assets/inc/ui/kb_hex.data"
#include "assets/inc/ui/kb_int.data"
#include "assets/inc/ui/kb_phone.data"
#include "assets/inc/ui/kb_ufloat.data"
#include "assets/inc/ui/kb_uint.data"
#include "assets/inc/ui/main.data"
#include "assets/inc/ui/message.data"
#include "assets/inc/ui/music_play.data"
#include "assets/inc/ui/select_dial.data"
#include "assets/inc/ui/system_bar.data"
#include "assets/inc/ui/weather.data"
#ifdef WITH_STB_IMAGE
#include "assets/inc/images/0.res"
#include "assets/inc/images/1.res"
#include "assets/inc/images/10.res"
#include "assets/inc/images/11.res"
#include "assets/inc/images/2.res"
#include "assets/inc/images/3.res"
#include "assets/inc/images/4.res"
#include "assets/inc/images/5.res"
#include "assets/inc/images/6.res"
#include "assets/inc/images/7.res"
#include "assets/inc/images/8.res"
#include "assets/inc/images/9.res"
#include "assets/inc/images/active_dot.res"
#include "assets/inc/images/alarmitem_bg.res"
#include "assets/inc/images/alarm_am_no_select.res"
#include "assets/inc/images/alarm_am_select.res"
#include "assets/inc/images/alarm_cancel.res"
#include "assets/inc/images/alarm_clock_bg.res"
#include "assets/inc/images/alarm_no_select.res"
#include "assets/inc/images/alarm_pm_no_select.res"
#include "assets/inc/images/alarm_pm_select.res"
#include "assets/inc/images/alarm_point.res"
#include "assets/inc/images/alarm_selected.res"
#include "assets/inc/images/alarm_set.res"
#include "assets/inc/images/backspace.res"
#include "assets/inc/images/button_gray.res"
#include "assets/inc/images/button_green.res"
#include "assets/inc/images/calander_down.res"
#include "assets/inc/images/calendar_current.res"
#include "assets/inc/images/calendar_down.res"
#include "assets/inc/images/calendar_up.res"
#include "assets/inc/images/calender_down.res"
#include "assets/inc/images/clock.res"
#include "assets/inc/images/clock_bg.res"
#include "assets/inc/images/clock_bg_red.res"
#include "assets/inc/images/clock_bg_white.res"
#include "assets/inc/images/clock_hour.res"
#include "assets/inc/images/clock_minute.res"
#include "assets/inc/images/clock_second.res"
#include "assets/inc/images/dial_style_digit.res"
#include "assets/inc/images/dial_style_time.res"
#include "assets/inc/images/dot.res"
#include "assets/inc/images/earth.res"
#include "assets/inc/images/grabient.res"
#include "assets/inc/images/huxi.res"
#include "assets/inc/images/message_card_bg.res"
#include "assets/inc/images/message_icon.res"
#include "assets/inc/images/msg_bg.res"
#include "assets/inc/images/music-1.res"
#include "assets/inc/images/music.res"
#include "assets/inc/images/music_icon.res"
#include "assets/inc/images/music_next.res"
#include "assets/inc/images/music_player_bar.res"
#include "assets/inc/images/music_player_bg.res"
#include "assets/inc/images/music_player_point.res"
#include "assets/inc/images/music_pre.res"
#include "assets/inc/images/music_stop.res"
#include "assets/inc/images/progress_circle.res"
#include "assets/inc/images/qingtian_48.res"
#include "assets/inc/images/return_pink.res"
#include "assets/inc/images/return_pink_p.res"
#include "assets/inc/images/run_icon.res"
#include "assets/inc/images/shift.res"
#include "assets/inc/images/shifton.res"
#include "assets/inc/images/switch.res"
#include "assets/inc/images/switch_fg.res"
#include "assets/inc/images/switch_off.res"
#include "assets/inc/images/switch_on.res"
#include "assets/inc/images/taiyang_48.res"
#include "assets/inc/images/weather_icon_bg.res"
#include "assets/inc/images/weather_no_data.res"
#include "assets/inc/images/xiaoyu_48.res"
#include "assets/inc/images/yintian_48.res"
#include "assets/inc/images/zhoujielun.res"
#else
#endif/*WITH_STB_IMAGE*/
#ifdef WITH_STB_FONT
#ifdef WITH_MINI_FONT
#include "assets/inc/fonts/default.mini.res"
#else/*WITH_MINI_FONT*/
#include "assets/inc/fonts/default.res"
#endif/*WITH_MINI_FONT*/
#else/*WITH_STB_FONT*/
#endif/*WITH_STB_FONT*/
#endif/*WITH_FS_RES*/

ret_t assets_init(void) {
  assets_manager_t* rm = assets_manager();

#ifdef WITH_FS_RES
#ifdef WITH_MINI_FONT
  asset_info_t* info = assets_manager_load(rm, ASSET_TYPE_FONT, "default.mini");
  if (info) {
    strcpy(info->name, "default");
  }
#else 
  assets_manager_load(rm, ASSET_TYPE_FONT, "default");
#endif
  assets_manager_load(rm, ASSET_TYPE_STYLE, "default");
#else
#ifdef WITH_STB_FONT
  assets_manager_add(rm, font_default);
#endif
  assets_manager_add(rm, image_0);
  assets_manager_add(rm, image_1);
  assets_manager_add(rm, image_10);
  assets_manager_add(rm, image_11);
  assets_manager_add(rm, image_2);
  assets_manager_add(rm, image_3);
  assets_manager_add(rm, image_4);
  assets_manager_add(rm, image_5);
  assets_manager_add(rm, image_6);
  assets_manager_add(rm, image_7);
  assets_manager_add(rm, image_8);
  assets_manager_add(rm, image_9);
  assets_manager_add(rm, image_active_dot);
  assets_manager_add(rm, image_alarmitem_bg);
  assets_manager_add(rm, image_alarm_am_no_select);
  assets_manager_add(rm, image_alarm_am_select);
  assets_manager_add(rm, image_alarm_cancel);
  assets_manager_add(rm, image_alarm_clock_bg);
  assets_manager_add(rm, image_alarm_no_select);
  assets_manager_add(rm, image_alarm_pm_no_select);
  assets_manager_add(rm, image_alarm_pm_select);
  assets_manager_add(rm, image_alarm_point);
  assets_manager_add(rm, image_alarm_selected);
  assets_manager_add(rm, image_alarm_set);
  assets_manager_add(rm, image_backspace);
  assets_manager_add(rm, image_button_gray);
  assets_manager_add(rm, image_button_green);
  assets_manager_add(rm, image_calander_down);
  assets_manager_add(rm, image_calendar_current);
  assets_manager_add(rm, image_calendar_down);
  assets_manager_add(rm, image_calendar_up);
  assets_manager_add(rm, image_calender_down);
  assets_manager_add(rm, image_clock);
  assets_manager_add(rm, image_clock_bg);
  assets_manager_add(rm, image_clock_bg_red);
  assets_manager_add(rm, image_clock_bg_white);
  assets_manager_add(rm, image_clock_hour);
  assets_manager_add(rm, image_clock_minute);
  assets_manager_add(rm, image_clock_second);
  assets_manager_add(rm, image_dial_style_digit);
  assets_manager_add(rm, image_dial_style_time);
  assets_manager_add(rm, image_dot);
  assets_manager_add(rm, image_earth);
  assets_manager_add(rm, image_grabient);
  assets_manager_add(rm, image_huxi);
  assets_manager_add(rm, image_message_card_bg);
  assets_manager_add(rm, image_message_icon);
  assets_manager_add(rm, image_msg_bg);
  assets_manager_add(rm, image_music-1);
  assets_manager_add(rm, image_music);
  assets_manager_add(rm, image_music_icon);
  assets_manager_add(rm, image_music_next);
  assets_manager_add(rm, image_music_player_bar);
  assets_manager_add(rm, image_music_player_bg);
  assets_manager_add(rm, image_music_player_point);
  assets_manager_add(rm, image_music_pre);
  assets_manager_add(rm, image_music_stop);
  assets_manager_add(rm, image_progress_circle);
  assets_manager_add(rm, image_qingtian_48);
  assets_manager_add(rm, image_return_pink);
  assets_manager_add(rm, image_return_pink_p);
  assets_manager_add(rm, image_run_icon);
  assets_manager_add(rm, image_shift);
  assets_manager_add(rm, image_shifton);
  assets_manager_add(rm, image_switch);
  assets_manager_add(rm, image_switch_fg);
  assets_manager_add(rm, image_switch_off);
  assets_manager_add(rm, image_switch_on);
  assets_manager_add(rm, image_taiyang_48);
  assets_manager_add(rm, image_weather_icon_bg);
  assets_manager_add(rm, image_weather_no_data);
  assets_manager_add(rm, image_xiaoyu_48);
  assets_manager_add(rm, image_yintian_48);
  assets_manager_add(rm, image_zhoujielun);
  assets_manager_add(rm, style_button);
  assets_manager_add(rm, style_default);
  assets_manager_add(rm, style_keyboard);
  assets_manager_add(rm, ui_alarm_clock);
  assets_manager_add(rm, ui_alarm_list);
  assets_manager_add(rm, ui_application);
  assets_manager_add(rm, ui_calendar);
  assets_manager_add(rm, ui_circle_button_test);
  assets_manager_add(rm, ui_circle_button_with_progress_test);
  assets_manager_add(rm, ui_day_week_test);
  assets_manager_add(rm, ui_digit_dial);
  assets_manager_add(rm, ui_health_circle_text);
  assets_manager_add(rm, ui_heart_rate);
  assets_manager_add(rm, ui_kb_ascii);
  assets_manager_add(rm, ui_kb_default);
  assets_manager_add(rm, ui_kb_float);
  assets_manager_add(rm, ui_kb_hex);
  assets_manager_add(rm, ui_kb_int);
  assets_manager_add(rm, ui_kb_phone);
  assets_manager_add(rm, ui_kb_ufloat);
  assets_manager_add(rm, ui_kb_uint);
  assets_manager_add(rm, ui_main);
  assets_manager_add(rm, ui_message);
  assets_manager_add(rm, ui_music_play);
  assets_manager_add(rm, ui_select_dial);
  assets_manager_add(rm, ui_system_bar);
  assets_manager_add(rm, ui_weather);
  assets_manager_add(rm, strings_en_US);
  assets_manager_add(rm, strings_zh_CN);
#endif

  tk_init_assets();
  return RET_OK;
}
