/*
 * Copyright (C) 2023 M17 Project and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ui/openht_ui.h"

#include "task_microphone.h"
#include "task_radio.h"
#include "cps.h"
#include "disk_mgr.h"
#include "bmp_utils.h"
#include "openht_hwconfig.h"
#include "openht_types.h"
#include "ui/ui_about_panel.h"
#include "ui/ui_mode_change_panel.h"
#include "ui/ui_callsign_change_panel.h"
#include "ui/ui_freq_change_panel.h"
#include "ui/ui_fpga_status_panel.h"
#include "ui/lvht_numpad.h"
#include "ui/lvht_qwertypad.h"
#include "utils/str_builder.h"
#include "utils/str_formatting.h"

#include "fatfs.h"
#include <lvgl.h>
#include <../../Drivers/BSP/Components/nt35510/nt35510.h>

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

char callsign_str[10] = ""; // 9 digits for callsign
user_settings_t user_settings;

char * callsign_prefix = NULL;
char * mode_prefix = NULL;
char * ctcss_options_str;

static int32_t _screen_capture(lv_obj_t *scr, const char * filename);

void custom_ui_init(void)
{
	// SquareLine designer add widgets to screens, however, we want to use
	// these as top layer widgets. So we can still use the designer
	// but then need to set the parent to the top layer for each widget

	// top layer is scrollable by default, so this will keep our hidden keyboards hidden!
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_SCROLLABLE);

	lv_obj_set_parent(ui_about_panel, lv_layer_top());
	lv_obj_add_flag(ui_about_panel, LV_OBJ_FLAG_HIDDEN);
	init_about_panel();

	lv_obj_set_parent(ui_mode_change_panel, lv_layer_top());
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	init_mode_change_panel();

	lv_obj_set_parent(ui_settings_panel, lv_layer_top());
	lv_obj_add_flag(ui_settings_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_parent(ui_callsign_change_panel, lv_layer_top());
	lv_obj_add_flag(ui_callsign_change_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_parent(ui_freq_change_panel, lv_layer_top());
	lv_obj_add_flag(ui_freq_change_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_parent(ui_qwerty_key_panel, lv_layer_top());
	lv_obj_add_flag(ui_qwerty_key_panel, LV_OBJ_FLAG_HIDDEN);

	// no need to hide this panel since it sits below the screen
	// when it is time to be visible it "slides" up from the bottom.
	lv_obj_set_parent(ui_freq_key_panel, lv_layer_top());

	// initialize the color status for fpga
	init_fpga_status_panel();

	// remove the border for the UI placeholder
	// Hint: using border in SquareLine Studio allows visibility while moving
	// things around, then set border to none at runtime...
	lv_obj_set_style_border_side(ui_qwerty_key_panel, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_obj_set_style_border_side(ui_freq_key_panel, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_obj_set_style_border_side(ui_panel_freq_bump, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_obj_set_style_border_side(ui_fpga_status_panel, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_dropdown_set_selected(ui_freq_dropdown, 1);


	lv_obj_t * qwerty_key = create_qwerty_pad(ui_qwerty_key_panel);
	init_callsign_change_panel(qwerty_key);

	lv_obj_t * num_pad = create_number_pad(ui_freq_key_panel);
	//init_num_change_panel(qwerty_pad);

	// callback handler
	lv_obj_add_event_cb(num_pad, numpad_btnmatrix_event_cb, LV_EVENT_ALL, NULL);

	//ctcss_tone

    str_builder_t * sb = str_builder_create();

	str_builder_add_str(sb, "None\n", 0);

    for (int index = 0; index < MAX_TONE_INDEX; index++) {
		str_builder_add_mag_val_decimal(sb, ctcss_tone[index], 10);
		str_builder_add_char(sb, '\n');
    }

    ctcss_options_str = str_builder_dump(sb, NULL);
    str_builder_destroy(sb);

    lv_dropdown_set_options_static(ui_ctcss_tx_dropdown, ctcss_options_str);
    lv_dropdown_set_options_static(ui_ctcss_rx_dropdown, ctcss_options_str);


	// GET STORED SETTINGS AND UPDATE UI
	radio_settings_init(); //get(&radio_settings);
	user_settings_get(&user_settings);

	char rx_buffer[10];
	//get_str_from_freq(user_settings.rx_freq, rx_buffer, -1);
	get_display_str_from_freq(radio_settings_get_rx_freq(), rx_buffer);
	lv_label_set_text_fmt(ui_rx_display_label, "%s", rx_buffer);

	char tx_buffer[10];
	//get_str_from_freq(user_settings.tx_freq, tx_buffer, -1);
	get_display_str_from_freq(radio_settings_get_tx_freq(), tx_buffer);
	lv_label_set_text_fmt(ui_tx_display_label, "%s", tx_buffer);

	strcpy(callsign_str, user_settings.callsign);
	lv_textarea_set_text(ui_text_area_callsign, callsign_str);

	if (user_settings.use_freq_offset) {
		lv_obj_add_state(ui_use_freq_offset_cb, LV_STATE_CHECKED);
	} else {
		lv_obj_clear_state(ui_use_freq_offset_cb, LV_STATE_CHECKED);
	}

	if (user_settings.use_soft_ptt) {
		lv_obj_clear_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_state(ui_use_soft_ptt_cb, LV_STATE_CHECKED);
	} else {
		lv_obj_add_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_state(ui_use_soft_ptt_cb, LV_STATE_CHECKED);
	}

	char * label_str = lv_label_get_text(ui_header_callsign_label);

	callsign_prefix = malloc(strlen(label_str) + 1);
	strcpy(callsign_prefix, label_str);

	lv_label_set_text_fmt(ui_header_callsign_label, "%s%s", callsign_prefix, callsign_str);

	label_str = lv_label_get_text(ui_header_mode_label);
	mode_prefix = malloc(strlen(label_str) + 1);
	strcpy(mode_prefix, label_str);

	lv_label_set_text_fmt(ui_header_mode_label, "%s%s", mode_prefix, openht_get_mode_str(radio_settings_get_mode()));
}

void on_screen_pressed(lv_event_t *e)
{
	// TBD
}


void on_xmit_button_press(lv_event_t *e)
{
	radio_soft_ptt(true);

	start_microphone_acquisition();
}

void on_xmit_button_release(lv_event_t *e)
{
	radio_soft_ptt(false);

	stop_microphone_acquisition();
}

void on_vol_changed(lv_event_t *e)
{
}



bool display_toggle = false;
void on_userbutton_press()
{
	BSP_LED_On(LED_BLUE);

	// reset other LEDs
	BSP_LED_Off(LED_GREEN);
	BSP_LED_Off(LED_ORANGE);
	BSP_LED_Off(LED_RED);

	// a bit convoluted in it's current form... To take a screenshot
	// of the display, we must capture both the active screen and the top
	// layer. For now we just create two bitmap images and save them
	// to the microsd card... use whatever image editor you want
	// to combine them!

	int32_t image_num = _screen_capture(lv_scr_act(), "");

	if (image_num != -1) {
		char filename[13];
		sprintf(filename, "OVERLY%02ld", image_num);
		_screen_capture(lv_layer_top(), filename);
	}

//	if (display_toggle) {
//		NT35510_BacklightOn();
//		//BSP_LCD_DisplayOn();
//		display_toggle = false;
//	} else {
//		NT35510_BacklightOff();
//		//BSP_LCD_DisplayOff();
//		display_toggle = true;
//	}

}

void on_userbutton_release()
{
	BSP_LED_Off(LED_BLUE);
}

bool validate_freq(uint32_t *freq)
{
	openht_hwconfig_t config = get_openht_hwconfig();

	bool found_band = false;
	uint32_t test_freq = UINT_MAX;
	uint32_t band_edge = 0;

	for (int i = 0; i < config.num_bands; i++) {
		if(*freq >= config.bands[i].start_freq && *freq <= config.bands[i].end_freq) {
			found_band = true;
			break;
		} else {
			int32_t test = abs(*freq - config.bands[i].start_freq);
			if (test < test_freq) {
				test_freq = test;
				band_edge = config.bands[i].start_freq;
			}
			test = abs(*freq - config.bands[i].end_freq);
			if (test < test_freq) {
				test_freq = test;
				band_edge = config.bands[i].end_freq;
			}
		}
	}

	// snap to closest band edge
	if (!found_band) {
		*freq = band_edge;
	}

	return found_band;
}

void update_callsign()
{
	lv_label_set_text_fmt(ui_header_callsign_label, "%s%s", callsign_prefix, callsign_str);

    strcpy(user_settings.callsign, callsign_str);
	user_settings_save(&user_settings);
}

// assign buffer to SDRAM since we are limited on SRAM
__attribute__((section(".sdram")))  static uint8_t img_buffer[480 * 800 * 3]; // 480px * 800px * 3 bytes per pixel

static int32_t _screen_capture(lv_obj_t *scr, const char * filename)
{
//	lv_obj_t *scr = lv_scr_act();
	lv_img_dsc_t snapshot_img;
	lv_res_t snap = lv_snapshot_take_to_buf(scr, LV_IMG_CF_TRUE_COLOR_ALPHA,
			&snapshot_img, &img_buffer, sizeof(img_buffer));
	int32_t ret = -1;

	if (snap == LV_RES_INV) {
		//Snapshot failed
		BSP_LED_Off(LED_ORANGE);
	} else {
		//Snapshot Success

		lv_coord_t width = lv_obj_get_width(scr);
		lv_coord_t height = lv_obj_get_height(scr);

		uint8_t header[sizeof(struct bmp_header_t) + 2];

		get_bitmap_header(width, height, LV_COLOR_DEPTH, header,
				sizeof(header));

		ret = save_image(filename, header, sizeof(header), img_buffer, sizeof(img_buffer));

		if (ret == -1) {
			// error
			BSP_LED_On(LED_ORANGE);
		} else {
			BSP_LED_On(LED_GREEN);
		}
	}

	return ret;
}

