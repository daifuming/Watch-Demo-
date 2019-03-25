#include "awtk.h"
#include "base/assets_manager.h"
#ifndef WITH_FS_RES
#include "assets/inc/strings/en_US.data"
#include "assets/inc/strings/zh_CN.data"
#include "assets/inc/styles/button.data"
#include "assets/inc/styles/default.data"
#include "assets/inc/styles/keyboard.data"
#include "assets/inc/ui/application.data"
#include "assets/inc/ui/circle_button_test.data"
#include "assets/inc/ui/circle_button_with_progress_test.data"
#include "assets/inc/ui/day_week_test.data"
#include "assets/inc/ui/digit_dial.data"
#include "assets/inc/ui/health_circle_text.data"
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
#include "assets/inc/ui/system_bar.data"
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
#include "assets/inc/images/backspace.res"
#include "assets/inc/images/clock.res"
#include "assets/inc/images/clock_bg.res"
#include "assets/inc/images/clock_bg_red.res"
#include "assets/inc/images/clock_bg_white.res"
#include "assets/inc/images/clock_hour.res"
#include "assets/inc/images/clock_minute.res"
#include "assets/inc/images/clock_second.res"
#include "assets/inc/images/earth.res"
#include "assets/inc/images/grabient.res"
#include "assets/inc/images/huxi.res"
#include "assets/inc/images/message_card_bg.res"
#include "assets/inc/images/message_icon.res"
#include "assets/inc/images/msg_bg.res"
#include "assets/inc/images/music-1.res"
#include "assets/inc/images/music.res"
#include "assets/inc/images/music_icon.res"
#include "assets/inc/images/progress_circle.res"
#include "assets/inc/images/qingtian_48.res"
#include "assets/inc/images/return_pink.res"
#include "assets/inc/images/return_pink_p.res"
#include "assets/inc/images/run_icon.res"
#include "assets/inc/images/shift.res"
#include "assets/inc/images/shifton.res"
#include "assets/inc/images/weather_no_data.res"
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
  assets_manager_add(rm, image_backspace);
  assets_manager_add(rm, image_clock);
  assets_manager_add(rm, image_clock_bg);
  assets_manager_add(rm, image_clock_bg_red);
  assets_manager_add(rm, image_clock_bg_white);
  assets_manager_add(rm, image_clock_hour);
  assets_manager_add(rm, image_clock_minute);
  assets_manager_add(rm, image_clock_second);
  assets_manager_add(rm, image_earth);
  assets_manager_add(rm, image_grabient);
  assets_manager_add(rm, image_huxi);
  assets_manager_add(rm, image_message_card_bg);
  assets_manager_add(rm, image_message_icon);
  assets_manager_add(rm, image_msg_bg);
  assets_manager_add(rm, image_music-1);
  assets_manager_add(rm, image_music);
  assets_manager_add(rm, image_music_icon);
  assets_manager_add(rm, image_progress_circle);
  assets_manager_add(rm, image_qingtian_48);
  assets_manager_add(rm, image_return_pink);
  assets_manager_add(rm, image_return_pink_p);
  assets_manager_add(rm, image_run_icon);
  assets_manager_add(rm, image_shift);
  assets_manager_add(rm, image_shifton);
  assets_manager_add(rm, image_weather_no_data);
  assets_manager_add(rm, style_button);
  assets_manager_add(rm, style_default);
  assets_manager_add(rm, style_keyboard);
  assets_manager_add(rm, ui_application);
  assets_manager_add(rm, ui_circle_button_test);
  assets_manager_add(rm, ui_circle_button_with_progress_test);
  assets_manager_add(rm, ui_day_week_test);
  assets_manager_add(rm, ui_digit_dial);
  assets_manager_add(rm, ui_health_circle_text);
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
  assets_manager_add(rm, ui_system_bar);
  assets_manager_add(rm, strings_en_US);
  assets_manager_add(rm, strings_zh_CN);
#endif

  tk_init_assets();
  return RET_OK;
}
