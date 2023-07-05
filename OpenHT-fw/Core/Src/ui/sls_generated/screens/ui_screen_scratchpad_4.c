// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: OpenHT_UI

#include "../ui.h"

void ui_screen_scratchpad_4_screen_init(void)
{
    ui_screen_scratchpad_4 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_screen_scratchpad_4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_callsign_change_panel = lv_obj_create(ui_screen_scratchpad_4);
    lv_obj_set_width(ui_callsign_change_panel, 449);
    lv_obj_set_height(ui_callsign_change_panel, 237);
    lv_obj_set_x(ui_callsign_change_panel, 0);
    lv_obj_set_y(ui_callsign_change_panel, 93);
    lv_obj_set_align(ui_callsign_change_panel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_callsign_change_panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_callsign_change_panel, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_callsign_change_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_callsign_change_panel, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_callsign_change_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_callsign_ok_btn = lv_btn_create(ui_callsign_change_panel);
    lv_obj_set_width(ui_callsign_ok_btn, 100);
    lv_obj_set_height(ui_callsign_ok_btn, 50);
    lv_obj_set_x(ui_callsign_ok_btn, 66);
    lv_obj_set_y(ui_callsign_ok_btn, -1);
    lv_obj_set_align(ui_callsign_ok_btn, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_callsign_ok_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_callsign_ok_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_callsign_ok_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_callsign_ok_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_callsign_ok_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_callsign_ok_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label19 = lv_label_create(ui_callsign_ok_btn);
    lv_obj_set_width(ui_Label19, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label19, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label19, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label19, "OK");
    lv_obj_set_style_text_font(ui_Label19, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_callsign_cancel_btn = lv_btn_create(ui_callsign_change_panel);
    lv_obj_set_width(ui_callsign_cancel_btn, 100);
    lv_obj_set_height(ui_callsign_cancel_btn, 50);
    lv_obj_set_x(ui_callsign_cancel_btn, -48);
    lv_obj_set_y(ui_callsign_cancel_btn, -1);
    lv_obj_set_align(ui_callsign_cancel_btn, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_callsign_cancel_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_callsign_cancel_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_callsign_cancel_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_callsign_cancel_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_callsign_cancel_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_callsign_cancel_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label20 = lv_label_create(ui_callsign_cancel_btn);
    lv_obj_set_width(ui_Label20, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label20, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label20, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label20, "Cancel");
    lv_obj_set_style_text_font(ui_Label20, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_text_area_callsign = lv_textarea_create(ui_callsign_change_panel);
    lv_obj_set_width(ui_text_area_callsign, 390);
    lv_obj_set_height(ui_text_area_callsign, LV_SIZE_CONTENT);    /// 70
    lv_obj_set_x(ui_text_area_callsign, 10);
    lv_obj_set_y(ui_text_area_callsign, 44);
    lv_textarea_set_max_length(ui_text_area_callsign, 9);
    lv_textarea_set_placeholder_text(ui_text_area_callsign, "CALLSIGN...");
    lv_textarea_set_one_line(ui_text_area_callsign, true);
    lv_obj_clear_flag(ui_text_area_callsign, LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_text_color(ui_text_area_callsign, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_text_area_callsign, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_text_area_callsign, &ui_font_RobotoBold, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_text_area_callsign, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_text_area_callsign, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_text_area_callsign, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(ui_text_area_callsign, &ui_font_RobotoBold, LV_PART_CURSOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_text_area_callsign, lv_color_hex(0x37B9F5), LV_PART_CURSOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

    ui_Label29 = lv_label_create(ui_callsign_change_panel);
    lv_obj_set_width(ui_Label29, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label29, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label29, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label29, "Change Callsign");
    lv_obj_set_style_text_color(ui_Label29, lv_color_hex(0xD2CDC8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label29, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label29, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label29, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_callsign_ok_btn, ui_event_callsign_ok_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_callsign_cancel_btn, ui_event_callsign_cancel_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_text_area_callsign, ui_event_text_area_callsign, LV_EVENT_ALL, NULL);

}
