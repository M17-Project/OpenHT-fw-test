// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: OpenHT_UI

#include "../ui.h"

void ui_screen_num_freq_pads_screen_init(void)
{
    ui_screen_num_freq_pads = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_screen_num_freq_pads, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_qwerty_key_panel = lv_obj_create(ui_screen_num_freq_pads);
    lv_obj_set_height(ui_qwerty_key_panel, 470);
    lv_obj_set_width(ui_qwerty_key_panel, lv_pct(100));
    lv_obj_set_x(ui_qwerty_key_panel, 0);
    lv_obj_set_y(ui_qwerty_key_panel, 330);
    lv_obj_clear_flag(ui_qwerty_key_panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_qwerty_key_panel, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_qwerty_key_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_qwerty_key_panel, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_qwerty_key_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_qwerty_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_qwerty_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_qwerty_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_qwerty_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_qwerty_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_qwerty_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_freq_key_panel = lv_obj_create(ui_screen_num_freq_pads);
    lv_obj_set_height(ui_freq_key_panel, 495);
    lv_obj_set_width(ui_freq_key_panel, lv_pct(100));
    lv_obj_set_x(ui_freq_key_panel, 0);
    lv_obj_set_y(ui_freq_key_panel, 305);
    lv_obj_clear_flag(ui_freq_key_panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_freq_key_panel, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_freq_key_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_freq_key_panel, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_freq_key_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_freq_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_freq_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_freq_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_freq_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_freq_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_freq_key_panel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_panel_freq_bump = lv_obj_create(ui_freq_key_panel);
    lv_obj_set_height(ui_panel_freq_bump, 60);
    lv_obj_set_width(ui_panel_freq_bump, lv_pct(90));
    lv_obj_set_x(ui_panel_freq_bump, 0);
    lv_obj_set_y(ui_panel_freq_bump, 5);
    lv_obj_set_align(ui_panel_freq_bump, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_panel_freq_bump, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_panel_freq_bump, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_panel_freq_bump, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_panel_freq_bump, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_panel_freq_bump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_freq_button_down = lv_btn_create(ui_panel_freq_bump);
    lv_obj_set_width(ui_freq_button_down, 130);
    lv_obj_set_height(ui_freq_button_down, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_freq_button_down, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_freq_button_down, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_freq_button_down, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_freq_button_down, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_freq_button_down, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_freq_button_down, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_freq_button_down, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label4 = lv_label_create(ui_freq_button_down);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "-");
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_freq_button_up = lv_btn_create(ui_panel_freq_bump);
    lv_obj_set_width(ui_freq_button_up, 130);
    lv_obj_set_height(ui_freq_button_up, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_freq_button_up, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_freq_button_up, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_freq_button_up, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_freq_button_up, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_freq_button_up, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_freq_button_up, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_freq_button_up, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label1 = lv_label_create(ui_freq_button_up);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "+");
    lv_obj_set_style_text_font(ui_Label1, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_freq_dropdown = lv_dropdown_create(ui_panel_freq_bump);
    lv_dropdown_set_options(ui_freq_dropdown, "1.00 kHz\n6.25 kHz\n12.5 kHz\n25 kHz");
    lv_obj_set_width(ui_freq_dropdown, 150);
    lv_obj_set_height(ui_freq_dropdown, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_freq_dropdown, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_freq_dropdown, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_text_color(ui_freq_dropdown, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_freq_dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_freq_dropdown, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_freq_dropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_freq_dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_freq_dropdown, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_freq_dropdown, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_color(lv_dropdown_get_list(ui_freq_dropdown), lv_color_hex(0xFFFFFF),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(lv_dropdown_get_list(ui_freq_dropdown), 255,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lv_dropdown_get_list(ui_freq_dropdown), &lv_font_montserrat_20,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_dropdown_get_list(ui_freq_dropdown), lv_color_hex(0x464B50),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lv_dropdown_get_list(ui_freq_dropdown), 255,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(lv_dropdown_get_list(ui_freq_dropdown), lv_color_hex(0x000000),
                                  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(lv_dropdown_get_list(ui_freq_dropdown), 255,  LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_freq_button_down, ui_event_freq_button_down, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_freq_button_up, ui_event_freq_button_up, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_screen_num_freq_pads, ui_event_screen_num_freq_pads, LV_EVENT_ALL, NULL);

}
