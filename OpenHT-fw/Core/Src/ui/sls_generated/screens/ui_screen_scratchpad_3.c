// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: OpenHT_UI

#include "../ui.h"

void ui_screen_scratchpad_3_screen_init(void)
{
    ui_screen_scratchpad_3 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_screen_scratchpad_3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_settings_panel = lv_obj_create(ui_screen_scratchpad_3);
    lv_obj_set_width(ui_settings_panel, 440);
    lv_obj_set_height(ui_settings_panel, 660);
    lv_obj_set_x(ui_settings_panel, 0);
    lv_obj_set_y(ui_settings_panel, 50);
    lv_obj_set_align(ui_settings_panel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_settings_panel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_panel, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_settings_panel, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_settings_panel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_erase_user_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_erase_user_btn, 261);
    lv_obj_set_height(ui_settings_erase_user_btn, 50);
    lv_obj_set_x(ui_settings_erase_user_btn, 0);
    lv_obj_set_y(ui_settings_erase_user_btn, -5);
    lv_obj_add_flag(ui_settings_erase_user_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_erase_user_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_erase_user_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_erase_user_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_erase_user_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_erase_user_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label11 = lv_label_create(ui_settings_erase_user_btn);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "Erase User Settings");
    lv_obj_set_style_text_font(ui_Label11, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_reboot_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_reboot_btn, 120);
    lv_obj_set_height(ui_settings_reboot_btn, 100);
    lv_obj_set_x(ui_settings_reboot_btn, 280);
    lv_obj_set_y(ui_settings_reboot_btn, -5);
    lv_obj_add_flag(ui_settings_reboot_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_reboot_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_reboot_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_reboot_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_reboot_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_reboot_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label30 = lv_label_create(ui_settings_reboot_btn);
    lv_obj_set_width(ui_Label30, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label30, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label30, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label30, "Reboot\nRadio");
    lv_obj_set_style_text_align(ui_Label30, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label30, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label31 = lv_label_create(ui_settings_panel);
    lv_obj_set_width(ui_Label31, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label31, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label31, 0);
    lv_obj_set_y(ui_Label31, 122);
    lv_label_set_text(ui_Label31, "FPGA:");
    lv_obj_set_style_text_color(ui_Label31, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label31, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label31, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label31, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_load_fpga_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_load_fpga_btn, 156);
    lv_obj_set_height(ui_settings_load_fpga_btn, 50);
    lv_obj_set_x(ui_settings_load_fpga_btn, 70);
    lv_obj_set_y(ui_settings_load_fpga_btn, 110);
    lv_obj_add_flag(ui_settings_load_fpga_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_load_fpga_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_load_fpga_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_load_fpga_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_load_fpga_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_load_fpga_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label12 = lv_label_create(ui_settings_load_fpga_btn);
    lv_obj_set_width(ui_Label12, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label12, "SD to NOR");
    lv_obj_set_style_text_font(ui_Label12, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_erase_fpga_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_erase_fpga_btn, 156);
    lv_obj_set_height(ui_settings_erase_fpga_btn, 50);
    lv_obj_set_x(ui_settings_erase_fpga_btn, 240);
    lv_obj_set_y(ui_settings_erase_fpga_btn, 110);
    lv_obj_add_flag(ui_settings_erase_fpga_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_erase_fpga_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_erase_fpga_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_erase_fpga_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_erase_fpga_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_erase_fpga_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label13 = lv_label_create(ui_settings_erase_fpga_btn);
    lv_obj_set_width(ui_Label13, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label13, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label13, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label13, "Erase NOR");
    lv_obj_set_style_text_font(ui_Label13, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_a_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_a_btn, 156);
    lv_obj_set_height(ui_settings_a_btn, 50);
    lv_obj_set_x(ui_settings_a_btn, 0);
    lv_obj_set_y(ui_settings_a_btn, 170);
    lv_obj_add_flag(ui_settings_a_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_a_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_a_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_a_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_a_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_a_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label14 = lv_label_create(ui_settings_a_btn);
    lv_obj_set_width(ui_Label14, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label14, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label14, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label14, "Upload BIN");
    lv_obj_set_style_text_font(ui_Label14, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_b_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_b_btn, 156);
    lv_obj_set_height(ui_settings_b_btn, 50);
    lv_obj_set_x(ui_settings_b_btn, 180);
    lv_obj_set_y(ui_settings_b_btn, 170);
    lv_obj_add_flag(ui_settings_b_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_b_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_b_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_b_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_b_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_b_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label16 = lv_label_create(ui_settings_b_btn);
    lv_obj_set_width(ui_Label16, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label16, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label16, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label16, "Button B");
    lv_obj_set_style_text_font(ui_Label16, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_c_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_c_btn, 156);
    lv_obj_set_height(ui_settings_c_btn, 50);
    lv_obj_set_x(ui_settings_c_btn, 0);
    lv_obj_set_y(ui_settings_c_btn, 230);
    lv_obj_add_flag(ui_settings_c_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_c_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_c_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_c_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_c_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_c_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label15 = lv_label_create(ui_settings_c_btn);
    lv_obj_set_width(ui_Label15, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label15, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label15, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label15, "Button C");
    lv_obj_set_style_text_font(ui_Label15, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_d_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_d_btn, 156);
    lv_obj_set_height(ui_settings_d_btn, 50);
    lv_obj_set_x(ui_settings_d_btn, 180);
    lv_obj_set_y(ui_settings_d_btn, 230);
    lv_obj_add_flag(ui_settings_d_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_d_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_d_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_d_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_d_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_d_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label17 = lv_label_create(ui_settings_d_btn);
    lv_obj_set_width(ui_Label17, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label17, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label17, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label17, "Button D");
    lv_obj_set_style_text_font(ui_Label17, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label18 = lv_label_create(ui_settings_panel);
    lv_obj_set_width(ui_Label18, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label18, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label18, -80);
    lv_obj_set_y(ui_Label18, 452);
    lv_obj_set_align(ui_Label18, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_Label18, "Display Brightness:");
    lv_obj_set_style_text_color(ui_Label18, lv_color_hex(0xD2CDC8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label18, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label18, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label18, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_use_freq_offset_cb = lv_checkbox_create(ui_settings_panel);
    lv_checkbox_set_text(ui_use_freq_offset_cb, "Use TX Frequency offset");
    lv_obj_set_width(ui_use_freq_offset_cb, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_use_freq_offset_cb, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_use_freq_offset_cb, 0);
    lv_obj_set_y(ui_use_freq_offset_cb, 400);
    lv_obj_add_flag(ui_use_freq_offset_cb, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_text_color(ui_use_freq_offset_cb, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_use_freq_offset_cb, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_use_freq_offset_cb, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_use_freq_offset_cb, lv_color_hex(0x4E4E4E), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_use_freq_offset_cb, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_display_brightness_slider = lv_slider_create(ui_settings_panel);
    lv_slider_set_range(ui_display_brightness_slider, 25, 255);
    lv_slider_set_value(ui_display_brightness_slider, 127, LV_ANIM_OFF);
    if(lv_slider_get_mode(ui_display_brightness_slider) == LV_SLIDER_MODE_RANGE) lv_slider_set_left_value(
            ui_display_brightness_slider, 0, LV_ANIM_OFF);
    lv_obj_set_width(ui_display_brightness_slider, 350);
    lv_obj_set_height(ui_display_brightness_slider, 20);
    lv_obj_set_x(ui_display_brightness_slider, -1);
    lv_obj_set_y(ui_display_brightness_slider, -105);
    lv_obj_set_align(ui_display_brightness_slider, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(ui_display_brightness_slider, lv_color_hex(0x0F1215), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_display_brightness_slider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_display_brightness_slider, lv_color_hex(0x1E242D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_display_brightness_slider, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_display_brightness_slider, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_display_brightness_slider, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_display_brightness_slider, lv_color_hex(0x37B9F5), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_display_brightness_slider, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_display_brightness_slider, lv_color_hex(0x1464F0),
                                   LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_display_brightness_slider, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_display_brightness_slider, lv_color_hex(0x5F6464), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_display_brightness_slider, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_display_brightness_slider, lv_color_hex(0x3C424B), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_display_brightness_slider, LV_GRAD_DIR_VER, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_display_brightness_slider, lv_color_hex(0x373C46), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_display_brightness_slider, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_display_brightness_slider, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_display_brightness_slider, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_display_brightness_slider, 10, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_display_brightness_slider, 10, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_display_brightness_slider, 10, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_display_brightness_slider, 10, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_display_brightness_slider, lv_color_hex(0x0096FF), LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_display_brightness_slider, 255, LV_PART_KNOB | LV_STATE_PRESSED);
    lv_obj_set_style_border_width(ui_display_brightness_slider, 4, LV_PART_KNOB | LV_STATE_PRESSED);

    ui_settings_ok_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_ok_btn, 100);
    lv_obj_set_height(ui_settings_ok_btn, 50);
    lv_obj_set_align(ui_settings_ok_btn, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_settings_ok_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_ok_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_ok_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_ok_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_ok_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_ok_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label10 = lv_label_create(ui_settings_ok_btn);
    lv_obj_set_width(ui_Label10, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label10, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label10, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label10, "OK");
    lv_obj_set_style_text_font(ui_Label10, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_e_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_e_btn, 156);
    lv_obj_set_height(ui_settings_e_btn, 50);
    lv_obj_set_x(ui_settings_e_btn, 0);
    lv_obj_set_y(ui_settings_e_btn, 290);
    lv_obj_add_flag(ui_settings_e_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_e_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_e_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_e_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_e_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_e_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label2 = lv_label_create(ui_settings_e_btn);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "Button E");
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_settings_f_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_f_btn, 156);
    lv_obj_set_height(ui_settings_f_btn, 50);
    lv_obj_set_x(ui_settings_f_btn, 180);
    lv_obj_set_y(ui_settings_f_btn, 290);
    lv_obj_add_flag(ui_settings_f_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_f_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_f_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_f_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_f_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_f_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label24 = lv_label_create(ui_settings_f_btn);
    lv_obj_set_width(ui_Label24, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label24, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label24, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label24, "Button F");
    lv_obj_set_style_text_font(ui_Label24, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_use_soft_ptt_cb = lv_checkbox_create(ui_settings_panel);
    lv_checkbox_set_text(ui_use_soft_ptt_cb, "Use Soft PTT Button");
    lv_obj_set_width(ui_use_soft_ptt_cb, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_use_soft_ptt_cb, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_use_soft_ptt_cb, 0);
    lv_obj_set_y(ui_use_soft_ptt_cb, 360);
    lv_obj_add_flag(ui_use_soft_ptt_cb, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_set_style_text_color(ui_use_soft_ptt_cb, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_use_soft_ptt_cb, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_use_soft_ptt_cb, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_use_soft_ptt_cb, lv_color_hex(0x4E4E4E), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_use_soft_ptt_cb, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    ui_settings_erase_radio_btn = lv_btn_create(ui_settings_panel);
    lv_obj_set_width(ui_settings_erase_radio_btn, 261);
    lv_obj_set_height(ui_settings_erase_radio_btn, 50);
    lv_obj_set_x(ui_settings_erase_radio_btn, 0);
    lv_obj_set_y(ui_settings_erase_radio_btn, 53);
    lv_obj_add_flag(ui_settings_erase_radio_btn, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_settings_erase_radio_btn, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_settings_erase_radio_btn, lv_color_hex(0x191C26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_settings_erase_radio_btn, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_settings_erase_radio_btn, lv_color_hex(0x37B9F5), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_settings_erase_radio_btn, 255, LV_PART_MAIN | LV_STATE_PRESSED);

    ui_Label28 = lv_label_create(ui_settings_erase_radio_btn);
    lv_obj_set_width(ui_Label28, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label28, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label28, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label28, "Erase Radio Settings");
    lv_obj_set_style_text_font(ui_Label28, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_settings_erase_user_btn, ui_event_settings_erase_user_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_reboot_btn, ui_event_settings_reboot_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_load_fpga_btn, ui_event_settings_load_fpga_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_erase_fpga_btn, ui_event_settings_erase_fpga_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_a_btn, ui_event_settings_a_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_b_btn, ui_event_settings_b_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_c_btn, ui_event_settings_c_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_d_btn, ui_event_settings_d_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_use_freq_offset_cb, ui_event_use_freq_offset_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_display_brightness_slider, ui_event_display_brightness_slider, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_ok_btn, ui_event_settings_ok_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_e_btn, ui_event_settings_e_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_f_btn, ui_event_settings_f_btn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_use_soft_ptt_cb, ui_event_use_soft_ptt_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_settings_erase_radio_btn, ui_event_settings_erase_radio_btn, LV_EVENT_ALL, NULL);

}
