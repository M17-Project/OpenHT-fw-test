// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.2.2
// LVGL version: 8.3.4
// Project name: OpenHT_UI

#ifndef _OPENHT_UI_UI_H
#define _OPENHT_UI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Drivers/lvgl/lvgl.h"

#include "ui_events.h"
void ui_event_Screen1(lv_event_t * e);
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Header;
extern lv_obj_t * ui_Header_Date;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_Header_Time;
extern lv_obj_t * ui_Label2;
extern lv_obj_t * ui_Arc_Group;
void ui_event_Arc1(lv_event_t * e);
extern lv_obj_t * ui_Arc1;
extern lv_obj_t * ui_Temp_Bg;
extern lv_obj_t * ui_Temp_Num_Bg;
extern lv_obj_t * ui_Label_Celsius;
extern lv_obj_t * ui_Label_Slider;
void ui_event_Fan_Speed_Control(lv_event_t * e);
extern lv_obj_t * ui_Fan_Speed_Control;
void ui_event_Button2(lv_event_t * e);
extern lv_obj_t * ui_Button2;
extern lv_obj_t * ui_Label3;
void ui_event_screen2(lv_event_t * e);
extern lv_obj_t * ui_screen2;
extern lv_obj_t * ui_panel_numpad;
void ui_event_text_area_rx_freq(lv_event_t * e);
extern lv_obj_t * ui_text_area_rx_freq;
extern lv_obj_t * ui_label_rx;
extern lv_obj_t * ui_label_tx;
void ui_event_text_area_tx_freq(lv_event_t * e);
extern lv_obj_t * ui_text_area_tx_freq;
extern lv_obj_t * ui_label_test_rx;
extern lv_obj_t * ui_label_test_tx;
extern lv_obj_t * ui____initial_actions0;


LV_IMG_DECLARE(ui_img_m17_logo_w_sm_png);    // assets/m17_logo_w_sm.png


LV_FONT_DECLARE(ui_font_Number60);
LV_FONT_DECLARE(ui_font_Number80);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
