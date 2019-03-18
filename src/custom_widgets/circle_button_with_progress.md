## 属性

- widget

- center_is_image+

- text+
- text_color
- text_bg_color
- text_size+
- image
- zoom

- value+
- max+
- start_angle
- progress_color
- width
- counter_clock_wise(顺逆时针)
  


```c

// 绘制流程
if (has_img) {
  load_image();
  draw_image();

  if (!zoom) {
    draw_no_zoom_image();
  } else {
    draw_zoom_image();
  }
} else {
  fill_bg_color();
  draw_text();
}

cal_angle();
draw_progress();

```