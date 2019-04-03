/**
 * File:    circle_button_with_progress.h
 * 
 * 
 * 
 * 
 * 
 */


#ifndef TK_CIRCLE_BUTTON_WITH_PROGRESS_H
#define TK_CIRCLE_BUTTON_WITH_PROGRESS_H

// #include "ext_widgets/progress_circle.h"
#include "base/widget.h"

BEGIN_C_DECLS


typedef struct _circle_button_with_progress_t{
	widget_t widget;

	/**
	 * @property {bool_t} center_is_image
	 * 控件为图片/文字（缺省为TRUE，图片）
	 */
	bool_t center_is_image;

	/**
	 * @property {char*} text
	 * 为文字时的文本(缺省为"text")
	 */
	char* text;

	/**
	 * @property {color_t} text_color
	 * 文本的颜色(缺省为0x000000)
	 */
	color_t text_color;

	/**
	 * @property {color_t} text_bg_color
	 * 不显示图片时，显示的颜色（缺省为0xffffff）
	 */
	color_t text_bg_color;
	
	/**
	 * @property {int8_t} text_size
	 * 文本的字体大小(缺省值为16)
	 */
	int8_t text_size;

	/**
	 * @property {char*} image
	 * @annotation ["set_prop","get_prop","readable"]
	 * 图片名称(缺省值为NULL)
	 */
	char* image;

	/**
	 * @property {bool_t} zoom
	 * 是否对图片进行缩放（缺省值为FALSE）
	 */
	bool_t zoom;

	/**
	 * @property {float_t} value
	 * 进度值（缺省值为30）
	 */
	float_t value;

	/**
	 * @property {float_t} max
	 * 最大值（缺省为100）
	 */
	float_t max;

	/**
	 * @property {uint_32_t} start_angle
	 * 起始角度（缺省为-90）
	 */
	uint32_t start_angle; 

	/**
	 * @property {color_t} progress_angle
	 * 进度条颜色(缺省值为0xff0000)
	 */
	color_t progress_color;

	/**
	 * @property {uint8_t} width
	 * 进度条的宽度(缺省值为6)
	 */
	uint8_t width;

	/**
	 * @property {bool_t} counter_clock_wise
	 * 进度条方向为顺时针或逆时针（缺省值为TRUE顺时针）
	 */
	bool_t counter_clock_wise;

	/**
	 * 控件是否被按下
	 */
	bool_t pressed;
} circle_button_with_progress_t;


/**
 * @method circle_button_with_progress_create
 * @annotation ["constructor", "scriptable"]
 * 创建circle_button_with_progress对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* circle_button_with_progress_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_CENTER_IS_IMAGE "center_is_image"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_COLOR "text_color"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_BG_COLOR "text_bg_color"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_TEXT_SIZE "text_size"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_IMAGE "image"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_ZOOM "zoom"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_START_ANGLE "start_angle"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_PROGRESS_COLOR "progress_color"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_WIDTH "width"
#define CIRCLE_BUTTON_WITH_PROGRESS_PROP_COUNTER_CLOCK_WISE "counter_clock_wise"

#define WIDGET_TYPE_CIRCLE_BUTTON_WITH_PROGRESS "circle_button_with_progress"
#define CIRCLE_BUTTON_WITH_PROGRESS(widget) ((circle_button_with_progress_t*)(widget))

END_C_DECLS

#endif /* TK_CIRCLE_BUTTON_WITH_PROGRESS */