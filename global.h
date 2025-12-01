// global.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

//NTP时间数据变量
extern lv_obj_t *TIME;
extern int hh, mm, ss;

// 样式
extern lv_style_t style_main;
extern lv_style_t style_knob;

// 控件
extern lv_obj_t *slider;

// 全局图片资源（在 ratboss.c 里定义的那个）
extern const lv_img_dsc_t ratboss;

#ifdef __cplusplus
}
#endif

#endif // GLOBALS_H
