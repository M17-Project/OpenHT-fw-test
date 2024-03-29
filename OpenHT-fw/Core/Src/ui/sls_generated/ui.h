// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: OpenHT_UI

#ifndef _OPENHT_UI_UI_H
#define _OPENHT_UI_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../Drivers/lvgl/lvgl.h"

#include "ui_helpers.h"
#include "ui_events.h"
// SCREEN: ui_main_screen
void ui_main_screen_screen_init(void);
extern lv_obj_t * ui_main_screen;
void ui_event_main_panel(lv_event_t * e);
extern lv_obj_t * ui_main_panel;
extern lv_obj_t * ui_header_panel;
void ui_event_title_image(lv_event_t * e);
extern lv_obj_t * ui_title_image;
extern lv_obj_t * ui_battery_image;
extern lv_obj_t * ui_signal_strenth_image;
void ui_event_header_callsign_button(lv_event_t * e);
extern lv_obj_t * ui_header_callsign_button;
extern lv_obj_t * ui_header_callsign_label;
void ui_event_header_mode_button(lv_event_t * e);
extern lv_obj_t * ui_header_mode_button;
extern lv_obj_t * ui_header_mode_label;
void ui_event_header_settings_button(lv_event_t * e);
extern lv_obj_t * ui_header_settings_button;
extern lv_obj_t * ui_Image1;
void ui_event_fpga_status_panel(lv_event_t * e);
extern lv_obj_t * ui_fpga_status_panel;
extern lv_obj_t * ui_vfo_label;
void ui_event_vfo_panel(lv_event_t * e);
extern lv_obj_t * ui_vfo_panel;
extern lv_obj_t * ui_rx_display_label;
extern lv_obj_t * ui_tx_display_label;
extern lv_obj_t * ui_rx_lbl;
extern lv_obj_t * ui_rx_lbl1;
void ui_event_ptt_btn(lv_event_t * e);
extern lv_obj_t * ui_ptt_btn;
extern lv_obj_t * ui_Label3;
extern lv_obj_t * ui_vol_label;
void ui_event_vol_slider(lv_event_t * e);
extern lv_obj_t * ui_vol_slider;
extern lv_obj_t * ui_mic_label;
void ui_event_mic_slider(lv_event_t * e);
extern lv_obj_t * ui_mic_slider;
// SCREEN: ui_log_screen
void ui_log_screen_screen_init(void);
extern lv_obj_t * ui_log_screen;
void ui_event_Panel2(lv_event_t * e);
extern lv_obj_t * ui_Panel2;
extern lv_obj_t * ui_log_label;
// SCREEN: ui_screen_num_freq_pads
void ui_screen_num_freq_pads_screen_init(void);
void ui_event_screen_num_freq_pads(lv_event_t * e);
extern lv_obj_t * ui_screen_num_freq_pads;
extern lv_obj_t * ui_qwerty_key_panel;
extern lv_obj_t * ui_freq_key_panel;
extern lv_obj_t * ui_panel_freq_bump;
void ui_event_freq_button_down(lv_event_t * e);
extern lv_obj_t * ui_freq_button_down;
extern lv_obj_t * ui_Label4;
void ui_event_freq_button_up(lv_event_t * e);
extern lv_obj_t * ui_freq_button_up;
extern lv_obj_t * ui_Label1;
extern lv_obj_t * ui_freq_dropdown;
// SCREEN: ui_screen_scratchpad_1
void ui_screen_scratchpad_1_screen_init(void);
extern lv_obj_t * ui_screen_scratchpad_1;
extern lv_obj_t * ui_about_panel;
extern lv_obj_t * ui_about_tab_panel;
extern lv_obj_t * ui_about_text_area;
extern lv_obj_t * ui_about_hw_text_area;
extern lv_obj_t * ui_about_credits_text_area;
void ui_event_about_ok_btn(lv_event_t * e);
extern lv_obj_t * ui_about_ok_btn;
extern lv_obj_t * ui_Label6;
extern lv_obj_t * ui_about_logo;
extern lv_obj_t * ui_about_openht_logo;
extern lv_obj_t * ui_fw_txt_label;
extern lv_obj_t * ui_fw_ver_label;
extern lv_obj_t * ui_fpga_txt_label;
extern lv_obj_t * ui_fpga_rev_label;
extern lv_obj_t * ui_fw_build_date_txt_label;
extern lv_obj_t * ui_fw_build_date_label;
// SCREEN: ui_screen_scratchpad_2
void ui_screen_scratchpad_2_screen_init(void);
extern lv_obj_t * ui_screen_scratchpad_2;
extern lv_obj_t * ui_mode_change_panel;
extern lv_obj_t * ui_mode_roller;
void ui_event_mode_ok_button(lv_event_t * e);
extern lv_obj_t * ui_mode_ok_button;
extern lv_obj_t * ui_Label7;
void ui_event_mode_cancel_button(lv_event_t * e);
extern lv_obj_t * ui_mode_cancel_button;
extern lv_obj_t * ui_Label8;
extern lv_obj_t * ui_Label9;
// SCREEN: ui_screen_scratchpad_3
void ui_screen_scratchpad_3_screen_init(void);
extern lv_obj_t * ui_screen_scratchpad_3;
extern lv_obj_t * ui_settings_panel;
extern lv_obj_t * ui_settings_tabview_panel;
extern lv_obj_t * ui_settings_tab_panel;
void ui_event_settings_erase_user_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_erase_user_btn;
extern lv_obj_t * ui_Label11;
void ui_event_settings_reboot_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_reboot_btn;
extern lv_obj_t * ui_Label30;
void ui_event_settings_erase_radio_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_erase_radio_btn;
extern lv_obj_t * ui_Label28;
extern lv_obj_t * ui_Label31;
void ui_event_settings_load_fpga_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_load_fpga_btn;
extern lv_obj_t * ui_Label12;
void ui_event_settings_erase_fpga_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_erase_fpga_btn;
extern lv_obj_t * ui_Label13;
void ui_event_show_callsign_boot_cb(lv_event_t * e);
extern lv_obj_t * ui_show_callsign_boot_cb;
void ui_event_use_soft_ptt_cb(lv_event_t * e);
extern lv_obj_t * ui_use_soft_ptt_cb;
void ui_event_use_freq_offset_cb(lv_event_t * e);
extern lv_obj_t * ui_use_freq_offset_cb;
extern lv_obj_t * ui_Label18;
void ui_event_display_brightness_slider(lv_event_t * e);
extern lv_obj_t * ui_display_brightness_slider;
extern lv_obj_t * ui_debug_tab_panel;
void ui_event_settings_a_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_a_btn;
extern lv_obj_t * ui_Label14;
void ui_event_settings_b_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_b_btn;
extern lv_obj_t * ui_Label16;
void ui_event_settings_c_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_c_btn;
extern lv_obj_t * ui_Label15;
void ui_event_settings_d_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_d_btn;
extern lv_obj_t * ui_Label17;
void ui_event_settings_e_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_e_btn;
extern lv_obj_t * ui_Label2;
void ui_event_settings_f_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_f_btn;
extern lv_obj_t * ui_Label24;
extern lv_obj_t * ui_xcvr_tab_panel;
extern lv_obj_t * ui_Label42;
extern lv_obj_t * ui_ppm_spinbox_panel;
extern lv_obj_t * ui_Label43;
extern lv_obj_t * ui_dpd1_spinbox_panel;
extern lv_obj_t * ui_Label44;
extern lv_obj_t * ui_dpd2_spinbox_panel;
extern lv_obj_t * ui_Label45;
extern lv_obj_t * ui_dpd3_spinbox_panel;
extern lv_obj_t * ui_offset_i_spinbox_panel;
extern lv_obj_t * ui_offset_q_spinbox_panel;
extern lv_obj_t * ui_balance_i_spinbox_panel;
extern lv_obj_t * ui_balance_q_spinbox_panel;
extern lv_obj_t * ui_Label32;
extern lv_obj_t * ui_Label33;
extern lv_obj_t * ui_Label34;
extern lv_obj_t * ui_Label35;
extern lv_obj_t * ui_tx_pwr_spinbox_panel;
extern lv_obj_t * ui_Label36;
void ui_event_xcvr_reset_btn(lv_event_t * e);
extern lv_obj_t * ui_xcvr_reset_btn;
extern lv_obj_t * ui_Label37;
void ui_event_settings_ok_btn(lv_event_t * e);
extern lv_obj_t * ui_settings_ok_btn;
extern lv_obj_t * ui_Label10;
// SCREEN: ui_screen_scratchpad_4
void ui_screen_scratchpad_4_screen_init(void);
extern lv_obj_t * ui_screen_scratchpad_4;
extern lv_obj_t * ui_callsign_change_panel;
void ui_event_callsign_ok_btn(lv_event_t * e);
extern lv_obj_t * ui_callsign_ok_btn;
extern lv_obj_t * ui_Label19;
void ui_event_callsign_cancel_btn(lv_event_t * e);
extern lv_obj_t * ui_callsign_cancel_btn;
extern lv_obj_t * ui_Label20;
void ui_event_text_area_callsign(lv_event_t * e);
extern lv_obj_t * ui_text_area_callsign;
extern lv_obj_t * ui_Label29;
// SCREEN: ui_screen_scratchpad_5
void ui_screen_scratchpad_5_screen_init(void);
extern lv_obj_t * ui_screen_scratchpad_5;
void ui_event_freq_change_panel(lv_event_t * e);
extern lv_obj_t * ui_freq_change_panel;
void ui_event_freq_ok_btn(lv_event_t * e);
extern lv_obj_t * ui_freq_ok_btn;
extern lv_obj_t * ui_Label21;
void ui_event_freq_cancel_btn(lv_event_t * e);
extern lv_obj_t * ui_freq_cancel_btn;
extern lv_obj_t * ui_Label22;
extern lv_obj_t * ui_rx_freq_panel;
extern lv_obj_t * ui_label_rx;
void ui_event_rx_freq_ta(lv_event_t * e);
extern lv_obj_t * ui_rx_freq_ta;
extern lv_obj_t * ui_tx_freq_panel;
extern lv_obj_t * ui_label_tx;
void ui_event_tx_freq_ta(lv_event_t * e);
extern lv_obj_t * ui_tx_freq_ta;
extern lv_obj_t * ui_offset_tx_panel;
extern lv_obj_t * ui_label_offset;
extern lv_obj_t * ui_tx_offset_ta;
void ui_event_split_freq_cb(lv_event_t * e);
extern lv_obj_t * ui_split_freq_cb;
extern lv_obj_t * ui_fm_settings_panel;
extern lv_obj_t * ui_ctcss_tx_dropdown;
extern lv_obj_t * ui_Label23;
extern lv_obj_t * ui_ctcss_rx_dropdown;
extern lv_obj_t * ui_Label5;
extern lv_obj_t * ui_m17_settings_panel;
extern lv_obj_t * ui_can_dropdown;
extern lv_obj_t * ui_Label25;
void ui_event_encrypt_cb(lv_event_t * e);
extern lv_obj_t * ui_encrypt_cb;
void ui_event_send_geo_cb(lv_event_t * e);
extern lv_obj_t * ui_send_geo_cb;
void ui_event_encrypt_lfsr_cb(lv_event_t * e);
extern lv_obj_t * ui_encrypt_lfsr_cb;
void ui_event_encrypt_aes_cb(lv_event_t * e);
extern lv_obj_t * ui_encrypt_aes_cb;
extern lv_obj_t * ui_Label26;
extern lv_obj_t * ui_Label27;
extern lv_obj_t * ui_voice_rate_dropdown;
extern lv_obj_t * ui_dst_ta;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_openht_logo_w_png);    // assets/openht_logo_w.png
LV_IMG_DECLARE(ui_img_152149816);    // assets/battery-charging_w.png
LV_IMG_DECLARE(ui_img_492966190);    // assets/reception-3_w.png
LV_IMG_DECLARE(ui_img_gear_w_png);    // assets/gear_w.png
LV_IMG_DECLARE(ui_img_m17_logo_sm_png);    // assets/m17_logo_sm.png
LV_IMG_DECLARE(ui_img_openht_logo_png);    // assets/openht_logo.png

LV_FONT_DECLARE(ui_font_RobotoBold_48);
LV_FONT_DECLARE(ui_font_RobotoMonoRegular_24);
LV_FONT_DECLARE(ui_font_number_50);
LV_FONT_DECLARE(ui_font_number_60);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
