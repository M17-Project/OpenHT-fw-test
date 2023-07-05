// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: OpenHT_UI

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// SCREEN: ui_main_screen
void ui_main_screen_screen_init(void);
lv_obj_t * ui_main_screen;
void ui_event_main_panel(lv_event_t * e);
lv_obj_t * ui_main_panel;
lv_obj_t * ui_header_panel;
void ui_event_title_image(lv_event_t * e);
lv_obj_t * ui_title_image;
lv_obj_t * ui_battery_image;
lv_obj_t * ui_signal_strenth_image;
void ui_event_header_callsign_button(lv_event_t * e);
lv_obj_t * ui_header_callsign_button;
lv_obj_t * ui_header_callsign_label;
void ui_event_header_mode_button(lv_event_t * e);
lv_obj_t * ui_header_mode_button;
lv_obj_t * ui_header_mode_label;
void ui_event_header_settings_button(lv_event_t * e);
lv_obj_t * ui_header_settings_button;
lv_obj_t * ui_Image1;
void ui_event_fpga_status_panel(lv_event_t * e);
lv_obj_t * ui_fpga_status_panel;
lv_obj_t * ui_vfo_label;
void ui_event_vfo_panel(lv_event_t * e);
lv_obj_t * ui_vfo_panel;
lv_obj_t * ui_rx_display_label;
lv_obj_t * ui_tx_display_label;
lv_obj_t * ui_rx_lbl;
lv_obj_t * ui_rx_lbl1;
void ui_event_ptt_btn(lv_event_t * e);
lv_obj_t * ui_ptt_btn;
lv_obj_t * ui_Label3;
lv_obj_t * ui_vol_label;
void ui_event_vol_slider(lv_event_t * e);
lv_obj_t * ui_vol_slider;

// SCREEN: ui_log_screen
void ui_log_screen_screen_init(void);
lv_obj_t * ui_log_screen;
void ui_event_Panel2(lv_event_t * e);
lv_obj_t * ui_Panel2;
lv_obj_t * ui_log_label;

// SCREEN: ui_screen_num_freq_pads
void ui_screen_num_freq_pads_screen_init(void);
void ui_event_screen_num_freq_pads(lv_event_t * e);
lv_obj_t * ui_screen_num_freq_pads;
lv_obj_t * ui_qwerty_key_panel;
lv_obj_t * ui_freq_key_panel;
lv_obj_t * ui_panel_freq_bump;
void ui_event_freq_button_down(lv_event_t * e);
lv_obj_t * ui_freq_button_down;
lv_obj_t * ui_Label4;
void ui_event_freq_button_up(lv_event_t * e);
lv_obj_t * ui_freq_button_up;
lv_obj_t * ui_Label1;
lv_obj_t * ui_freq_dropdown;

// SCREEN: ui_screen_scratchpad_1
void ui_screen_scratchpad_1_screen_init(void);
lv_obj_t * ui_screen_scratchpad_1;
lv_obj_t * ui_about_panel;
lv_obj_t * ui_about_tab_panel;
lv_obj_t * ui_about_text_area;
lv_obj_t * ui_about_hw_text_area;
lv_obj_t * ui_about_credits_text_area;
void ui_event_about_ok_btn(lv_event_t * e);
lv_obj_t * ui_about_ok_btn;
lv_obj_t * ui_Label6;
lv_obj_t * ui_about_logo;
lv_obj_t * ui_about_openht_logo;
lv_obj_t * ui_fw_txt_label;
lv_obj_t * ui_fw_ver_label;
lv_obj_t * ui_fpga_txt_label;
lv_obj_t * ui_fpga_rev_label;
lv_obj_t * ui_fw_build_date_txt_label;
lv_obj_t * ui_fw_build_date_label;

// SCREEN: ui_screen_scratchpad_2
void ui_screen_scratchpad_2_screen_init(void);
lv_obj_t * ui_screen_scratchpad_2;
lv_obj_t * ui_mode_change_panel;
lv_obj_t * ui_mode_roller;
void ui_event_mode_ok_button(lv_event_t * e);
lv_obj_t * ui_mode_ok_button;
lv_obj_t * ui_Label7;
void ui_event_mode_cancel_button(lv_event_t * e);
lv_obj_t * ui_mode_cancel_button;
lv_obj_t * ui_Label8;
lv_obj_t * ui_Label9;

// SCREEN: ui_screen_scratchpad_3
void ui_screen_scratchpad_3_screen_init(void);
lv_obj_t * ui_screen_scratchpad_3;
lv_obj_t * ui_settings_panel;
lv_obj_t * ui_settings_tabview_panel;
lv_obj_t * ui_settings_tab_panel;
void ui_event_settings_erase_user_btn(lv_event_t * e);
lv_obj_t * ui_settings_erase_user_btn;
lv_obj_t * ui_Label11;
void ui_event_settings_reboot_btn(lv_event_t * e);
lv_obj_t * ui_settings_reboot_btn;
lv_obj_t * ui_Label30;
void ui_event_settings_erase_radio_btn(lv_event_t * e);
lv_obj_t * ui_settings_erase_radio_btn;
lv_obj_t * ui_Label28;
lv_obj_t * ui_Label31;
void ui_event_settings_load_fpga_btn(lv_event_t * e);
lv_obj_t * ui_settings_load_fpga_btn;
lv_obj_t * ui_Label12;
void ui_event_settings_erase_fpga_btn(lv_event_t * e);
lv_obj_t * ui_settings_erase_fpga_btn;
lv_obj_t * ui_Label13;
void ui_event_use_soft_ptt_cb(lv_event_t * e);
lv_obj_t * ui_use_soft_ptt_cb;
void ui_event_use_freq_offset_cb(lv_event_t * e);
lv_obj_t * ui_use_freq_offset_cb;
lv_obj_t * ui_Label18;
void ui_event_display_brightness_slider(lv_event_t * e);
lv_obj_t * ui_display_brightness_slider;
lv_obj_t * ui_debug_tab_panel;
void ui_event_settings_a_btn(lv_event_t * e);
lv_obj_t * ui_settings_a_btn;
lv_obj_t * ui_Label14;
void ui_event_settings_b_btn(lv_event_t * e);
lv_obj_t * ui_settings_b_btn;
lv_obj_t * ui_Label16;
void ui_event_settings_c_btn(lv_event_t * e);
lv_obj_t * ui_settings_c_btn;
lv_obj_t * ui_Label15;
void ui_event_settings_d_btn(lv_event_t * e);
lv_obj_t * ui_settings_d_btn;
lv_obj_t * ui_Label17;
void ui_event_settings_e_btn(lv_event_t * e);
lv_obj_t * ui_settings_e_btn;
lv_obj_t * ui_Label2;
void ui_event_settings_f_btn(lv_event_t * e);
lv_obj_t * ui_settings_f_btn;
lv_obj_t * ui_Label24;
void ui_event_settings_ok_btn(lv_event_t * e);
lv_obj_t * ui_settings_ok_btn;
lv_obj_t * ui_Label10;

// SCREEN: ui_screen_scratchpad_4
void ui_screen_scratchpad_4_screen_init(void);
lv_obj_t * ui_screen_scratchpad_4;
lv_obj_t * ui_callsign_change_panel;
void ui_event_callsign_ok_btn(lv_event_t * e);
lv_obj_t * ui_callsign_ok_btn;
lv_obj_t * ui_Label19;
void ui_event_callsign_cancel_btn(lv_event_t * e);
lv_obj_t * ui_callsign_cancel_btn;
lv_obj_t * ui_Label20;
void ui_event_text_area_callsign(lv_event_t * e);
lv_obj_t * ui_text_area_callsign;
lv_obj_t * ui_Label29;

// SCREEN: ui_screen_scratchpad_5
void ui_screen_scratchpad_5_screen_init(void);
lv_obj_t * ui_screen_scratchpad_5;
void ui_event_freq_change_panel(lv_event_t * e);
lv_obj_t * ui_freq_change_panel;
void ui_event_freq_ok_btn(lv_event_t * e);
lv_obj_t * ui_freq_ok_btn;
lv_obj_t * ui_Label21;
void ui_event_freq_cancel_btn(lv_event_t * e);
lv_obj_t * ui_freq_cancel_btn;
lv_obj_t * ui_Label22;
lv_obj_t * ui_rx_freq_panel;
lv_obj_t * ui_label_rx;
void ui_event_rx_freq_ta(lv_event_t * e);
lv_obj_t * ui_rx_freq_ta;
lv_obj_t * ui_tx_freq_panel;
lv_obj_t * ui_label_tx;
void ui_event_tx_freq_ta(lv_event_t * e);
lv_obj_t * ui_tx_freq_ta;
lv_obj_t * ui_offset_tx_panel;
lv_obj_t * ui_label_offset;
lv_obj_t * ui_tx_offset_ta;
void ui_event_split_freq_cb(lv_event_t * e);
lv_obj_t * ui_split_freq_cb;
lv_obj_t * ui_fm_settings_panel;
lv_obj_t * ui_ctcss_tx_dropdown;
lv_obj_t * ui_Label23;
lv_obj_t * ui_ctcss_rx_dropdown;
lv_obj_t * ui_Label5;
lv_obj_t * ui_m17_settings_panel;
lv_obj_t * ui_can_dropdown;
lv_obj_t * ui_Label25;
void ui_event_encrypt_cb(lv_event_t * e);
lv_obj_t * ui_encrypt_cb;
void ui_event_send_geo_cb(lv_event_t * e);
lv_obj_t * ui_send_geo_cb;
void ui_event_encrypt_lfsr_cb(lv_event_t * e);
lv_obj_t * ui_encrypt_lfsr_cb;
void ui_event_encrypt_aes_cb(lv_event_t * e);
lv_obj_t * ui_encrypt_aes_cb;
lv_obj_t * ui_Label26;
lv_obj_t * ui_Label27;
lv_obj_t * ui_voice_rate_dropdown;
lv_obj_t * ui_dst_ta;
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_main_panel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_log_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0);
    }
}
void ui_event_title_image(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        on_about_clicked(e);
    }
}
void ui_event_header_callsign_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_callsign_clicked(e);
    }
}
void ui_event_header_mode_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_mode_clicked(e);
    }
}
void ui_event_header_settings_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_clicked(e);
    }
}
void ui_event_fpga_status_panel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_fpga_status_clicked(e);
    }
}
void ui_event_vfo_panel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_freq_click(e);
    }
}
void ui_event_ptt_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        on_xmit_button_press(e);
    }
    if(event_code == LV_EVENT_RELEASED) {
        on_xmit_button_release(e);
    }
}
void ui_event_vol_slider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        on_vol_changed(e);
    }
}
void ui_event_Panel2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_main_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0);
    }
}
void ui_event_screen_num_freq_pads(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(ui_main_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0);
    }
    if(event_code == LV_EVENT_PRESSED) {
        on_screen_pressed(e);
    }
}
void ui_event_freq_button_down(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_LONG_PRESSED_REPEAT) {
        on_freq_button_down_press(e);
    }
    if(event_code == LV_EVENT_PRESSED) {
        on_freq_button_down_press(e);
    }
}
void ui_event_freq_button_up(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        on_freq_button_up_press(e);
    }
    if(event_code == LV_EVENT_LONG_PRESSED_REPEAT) {
        on_freq_button_up_press(e);
    }
}
void ui_event_about_ok_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_about_ok_clicked(e);
    }
}
void ui_event_mode_ok_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_mode_ok_clicked(e);
    }
}
void ui_event_mode_cancel_button(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_mode_cancel_clicked(e);
    }
}
void ui_event_settings_erase_user_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_erase_usr_clicked(e);
    }
}
void ui_event_settings_reboot_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_reboot_clicked(e);
    }
}
void ui_event_settings_erase_radio_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_erase_radio_clicked(e);
    }
}
void ui_event_settings_load_fpga_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_load_fpga_clicked(e);
    }
}
void ui_event_settings_erase_fpga_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_erase_fpga_clicked(e);
    }
}
void ui_event_use_soft_ptt_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_use_soft_ptt_clicked(e);
    }
}
void ui_event_use_freq_offset_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_use_tx_offset_clicked(e);
    }
}
void ui_event_display_brightness_slider(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        on_disp_brightness_changed(e);
    }
}
void ui_event_settings_a_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_a_clicked(e);
    }
}
void ui_event_settings_b_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_b_clicked(e);
    }
}
void ui_event_settings_c_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_c_clicked(e);
    }
}
void ui_event_settings_d_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_d_clicked(e);
    }
}
void ui_event_settings_e_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_e_clicked(e);
    }
}
void ui_event_settings_f_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_f_clicked(e);
    }
}
void ui_event_settings_ok_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_settings_ok_clicked(e);
    }
}
void ui_event_callsign_ok_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_callsign_ok_clicked(e);
    }
}
void ui_event_callsign_cancel_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_callsign_cancel_clicked(e);
    }
}
void ui_event_text_area_callsign(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_callsign_ta_click(e);
    }
}
void ui_event_freq_change_panel(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_freq_change_panel_click(e);
    }
}
void ui_event_freq_ok_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_freq_ok_clicked(e);
    }
}
void ui_event_freq_cancel_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_freq_cancel_clicked(e);
    }
}
void ui_event_rx_freq_ta(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_rx_freq_ta_click(e);
    }
}
void ui_event_tx_freq_ta(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_tx_freq_ta_click(e);
    }
}
void ui_event_split_freq_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_split_txrx_clicked(e);
    }
}
void ui_event_encrypt_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_split_txrx_clicked(e);
    }
}
void ui_event_send_geo_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_split_txrx_clicked(e);
    }
}
void ui_event_encrypt_lfsr_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_split_txrx_clicked(e);
    }
}
void ui_event_encrypt_aes_cb(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        on_split_txrx_clicked(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_main_screen_screen_init();
    ui_log_screen_screen_init();
    ui_screen_num_freq_pads_screen_init();
    ui_screen_scratchpad_1_screen_init();
    ui_screen_scratchpad_2_screen_init();
    ui_screen_scratchpad_3_screen_init();
    ui_screen_scratchpad_4_screen_init();
    ui_screen_scratchpad_5_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_main_screen);
}
