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

// LVGL version: 8.3.4
// Project name: OpenHT_UI

#include <ui/openht_ui.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <lvgl.h>

#include "lvgl_ui/ui.h"
#include <ui/ui_about_panel.h>
#include <ui/ui_mode_change_panel.h>
#include "ui/ui_callsign_change_panel.h"

#include "disk_mgr.h"
#include "fatfs.h"
#include "bmp_utils.h"
#include "openht_hwconfig.h"
#include "openht_types.h"
#include <../../Drivers/BSP/Components/nt35510/nt35510.h>
#include <ui/lvht_numpad.h>
#include <ui/lvht_qwertypad.h>

#include "task_microphone.h"
#include "task_fpga.h"

#define EMPTY_FREQ "_.___.___.___"
#define END_POS 13
#define THOU_POS 9
#define MEG_POS 5
#define GIG_POS 1

char callsign_str[10] = ""; // 9 digits for callsign

static char current_freq_str[] = EMPTY_FREQ;
static lv_obj_t *current_freq_ta = NULL;
static uint32_t *current_freq = NULL;

settings_t user_settings;

static void change_vfo_frequency(bool move_up);
static bool validate_freq(uint32_t *freq);
static int32_t move_cursor(int32_t curs_pos, int32_t movement);
static void update_cursor_pos(lv_obj_t *textAreaFreq);

static void update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq);
static void end_input_freq_ta(bool finished_input);

static void screen_capture(void);

void custom_ui_init(void)
{
	// test inputs...
	//uint32_t test = get_freq_from_str("_39.123.___");
	//get_str_from_freq(test, rxfreqstr);
	//get_str_from_freq(0, rxfreqstr);

	// squareline designer clean up code...
	lv_obj_set_parent(ui_about_panel, lv_layer_top());
	lv_obj_add_flag(ui_about_panel, LV_OBJ_FLAG_HIDDEN);
	init_about_panel();

//    lv_obj_set_y(ui_about_panel, 800);
	//lv_obj_move_foreground(ui_about_panel);

	lv_obj_set_parent(ui_mode_change_panel, lv_layer_top());
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	init_mode_change_panel();

	lv_obj_set_parent(ui_settings_panel, lv_layer_top());
	lv_obj_add_flag(ui_settings_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_parent(ui_callsign_change_panel, lv_layer_top());
	lv_obj_add_flag(ui_callsign_change_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_set_parent(ui_panel_qwerty_pad, lv_layer_top());
	lv_obj_add_flag(ui_panel_qwerty_pad, LV_OBJ_FLAG_HIDDEN);


	// remove the border for the UI placeholder
	// Hint: using border in SquareLine Studio allows visibility while moving
	// things around, then set border to none at runtime...
	lv_obj_set_style_border_side(ui_panel_qwerty_pad, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_obj_set_style_border_side(ui_panel_num_pad, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_obj_set_style_border_side(ui_panel_freq_bump, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_dropdown_set_selected(ui_freq_dropdown, 1);


	lv_obj_t * qwerty_pad = create_qwerty_pad(ui_panel_qwerty_pad);
	init_callsign_change_panel(qwerty_pad);

	lv_obj_t * num_pad = create_number_pad(ui_panel_num_pad);

	// callback handler
	lv_obj_add_event_cb(num_pad, numpad_btnmatrix_event_cb, LV_EVENT_ALL, NULL);


	// GET STORED SETTINGS AND UPDATE UI
	user_settings_get(&user_settings);

	char buffer[15];
	snprintf(buffer, 15, "%lu", user_settings.rx_freq);
	lv_label_set_text(ui_label_test_rx, buffer);
	get_str_from_freq(user_settings.rx_freq, current_freq_str, true);
	lv_textarea_set_text(ui_text_area_rx_freq, current_freq_str);

	snprintf(buffer, 15, "%lu", user_settings.tx_freq);
	lv_label_set_text(ui_label_test_tx, buffer);
	get_str_from_freq(user_settings.tx_freq, current_freq_str, true);
	lv_textarea_set_text(ui_text_area_tx_freq, current_freq_str);

	strcpy(callsign_str, user_settings.callsign);
	lv_textarea_set_text(ui_text_area_callsign, callsign_str);
	lv_label_set_text_fmt(ui_header_callsign_label, "Call: %s", callsign_str);


	lv_label_set_text_fmt(ui_header_mode_label, "Mode: %s", openht_get_mode_str(user_settings.mode));
}


void on_screen_pressed(lv_event_t *e)
{
	//end_input_callsign_ta();
	end_input_freq_ta(true);
}

void on_freq_button_down_press(lv_event_t * e)
{
	change_vfo_frequency(false);
}

void on_freq_button_up_press(lv_event_t * e)
{
	change_vfo_frequency(true);
}



void on_rx_freq_ta_click(lv_event_t *e)
{
	if (current_freq_ta == ui_text_area_rx_freq) {
		update_cursor_pos(ui_text_area_rx_freq);
		return;
	}
	update_active_freq_ta(ui_text_area_rx_freq, &user_settings.rx_freq);
}

void on_tx_freq_ta_click(lv_event_t *e)
{
	if (current_freq_ta == ui_text_area_tx_freq) {
		update_cursor_pos(ui_text_area_tx_freq);
		return;
	}
	update_active_freq_ta(ui_text_area_tx_freq, &user_settings.tx_freq);
}

void on_xmit_button_press(lv_event_t *e)
{
	BSP_LED_On(LED_RED);
	start_microphone_acquisition();
}

void on_xmit_button_release(lv_event_t *e)
{
	BSP_LED_Off(LED_RED);
	stop_microphone_acquisition();
}

void on_vol_changed(lv_event_t *e)
{
}

void on_disp_brightness_changed(lv_event_t *e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    NT35510_Set_Backlight((uint8_t)lv_slider_get_value(slider));
}


bool display_toggle = false;
void on_userbutton_press()
{
	BSP_LED_On(LED_BLUE);

	// reset other LEDs
	BSP_LED_Off(LED_GREEN);
	BSP_LED_Off(LED_ORANGE);
	BSP_LED_Off(LED_RED);

	screen_capture();

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

void numpad_btnmatrix_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED) {
		uint32_t id = lv_btnmatrix_get_selected_btn(obj);
		const char *txt = lv_btnmatrix_get_btn_text(obj, id);

		LV_LOG_USER("%s was pressed\n", txt);

		if (current_freq_ta == NULL)
			return;

		// for future use...(or not!)
//		if ((strcmp(txt, "*") == 0) || (strcmp(txt, ".") == 0)
//				|| (strcmp(txt, "#") == 0))
//			return;

		// get current cursor position
		uint32_t curs_pos = lv_textarea_get_cursor_pos(
				current_freq_ta);

		// move cursor left
		if (strcmp(txt, LV_SYMBOL_LEFT) == 0) {
			curs_pos = move_cursor(curs_pos, -1);
		}
		// move cursor right
		else if (strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
			curs_pos = move_cursor(curs_pos, +1);
		}
		// end input
		else if (strcmp(txt, LV_SYMBOL_OK) == 0) {
			curs_pos = END_POS;
		} else {
			current_freq_str[curs_pos] = *txt;

			// update the text
			lv_textarea_set_text(current_freq_ta, current_freq_str);

			if (curs_pos == END_POS - 1) {
				curs_pos = END_POS;
			} else {
				// update cursor position...
				curs_pos = move_cursor(curs_pos, +1);
			}
		}

		// update cursor position
		lv_textarea_set_cursor_pos(current_freq_ta, curs_pos);

		if (curs_pos == END_POS) {
			end_input_freq_ta(true);
		}
	}
}


static bool validate_freq(uint32_t *freq)
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

	//config.set_frequency_cb(freq);

	return found_band;
}

void update_callsign()
{
	lv_label_set_text_fmt(ui_header_callsign_label, "Call: %s", callsign_str);

    strcpy(user_settings.callsign, callsign_str);
	user_settings_save(&user_settings);
}

static void change_freq(int32_t freq_shift)
{
	if (current_freq_ta != NULL && current_freq != NULL) {
		uint32_t curs_pos = lv_textarea_get_cursor_pos(current_freq_ta);
		*current_freq += freq_shift;

		validate_freq(current_freq);

		get_str_from_freq(*current_freq, current_freq_str, false);
		lv_textarea_set_text(current_freq_ta, current_freq_str);
		lv_textarea_set_cursor_pos(current_freq_ta, curs_pos);
	}
}

static void change_vfo_frequency(bool move_up)
{
	uint16_t selected_item = lv_dropdown_get_selected(ui_freq_dropdown);

	// need signed value
	int32_t freq_shift = 0;

	switch (selected_item) {
		case 0:
			freq_shift = 6250;
			break;
		case 1:
			freq_shift = 12500;
			break;
		case 2:
			freq_shift = 25000;
			break;
		default:
			break;
	}

	change_freq(move_up ? freq_shift : -freq_shift);
}



static void update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq)
{
	if (new_freq_ta == NULL)
		return;

	end_input_freq_ta(false);
//	end_input_callsign_ta();
	set_numpad_visibility(true);

	current_freq_ta = new_freq_ta;

	// store the ptr for the freq so val can be updated
	current_freq = freq;

	get_str_from_freq(*current_freq, current_freq_str, false);
	uint32_t curs_pos = lv_textarea_get_cursor_pos(current_freq_ta);
	lv_textarea_set_text(current_freq_ta, current_freq_str);
	lv_textarea_set_cursor_pos(current_freq_ta, curs_pos);

	lv_obj_set_style_border_side(current_freq_ta, LV_BORDER_SIDE_FULL,
			LV_PART_MAIN | LV_STATE_DEFAULT);
	//lv_obj_set_style_anim_time(currentActiveTextAreaFreq, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

	update_cursor_pos(current_freq_ta);
}

static void end_input_freq_ta(bool finished_input)
{
	if (current_freq_ta == NULL) {
		return;
	}

	if (finished_input) {
		set_numpad_visibility(false);
	}

	lv_textarea_set_cursor_pos(current_freq_ta, END_POS);
	lv_obj_set_style_border_side(current_freq_ta, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN | LV_STATE_DEFAULT);

	//const char * txt = lv_textarea_get_text(currentActiveTextAreaFreq);
	*current_freq = get_freq_from_str(current_freq_str);

	// validate input to ensure within bands
	validate_freq(current_freq);

	get_str_from_freq(*current_freq, current_freq_str, true);
	lv_textarea_set_text(current_freq_ta, current_freq_str);
	current_freq = NULL;
	current_freq_ta = NULL;

	/* log output... */
	char buffer[15];
//	user_settings.rx_freq = get_freq_from_str(rxfreqstr);
	snprintf(buffer, 15, "%lu", user_settings.rx_freq);
	lv_label_set_text(ui_label_test_rx, buffer);

//	user_settings.tx_freq = get_freq_from_str(txfreqstr);
	snprintf(buffer, 15, "%lu", user_settings.tx_freq);
	lv_label_set_text(ui_label_test_tx, buffer);

	user_settings_save(&user_settings);
}

static int32_t move_cursor(int32_t curs_pos, int32_t movement)
{
	curs_pos = curs_pos + movement;

	if (curs_pos == -1)
		curs_pos += END_POS;
	else if ((curs_pos == GIG_POS) || (curs_pos == MEG_POS) || (curs_pos == THOU_POS)) {
		curs_pos += movement;
	} else if (curs_pos == END_POS) {
		curs_pos = 0;
	}

	return curs_pos;
}

static void update_cursor_pos(lv_obj_t *textAreaFreq)
{
	uint32_t curs_pos = lv_textarea_get_cursor_pos((lv_obj_t*) textAreaFreq);
	if ((curs_pos == GIG_POS) || (curs_pos == MEG_POS) || (curs_pos == THOU_POS)) {
		curs_pos = curs_pos + 1;
	} else 	if (curs_pos == END_POS) {
		curs_pos = END_POS - 1;
	}
	lv_textarea_set_cursor_pos((lv_obj_t*) textAreaFreq, curs_pos);
}

uint32_t get_freq_from_str(const char *str)
{
	uint32_t num = 0;  // uint32 max: 4294967295

	// converting string to number
	for (int i = 0; str[i] != '\0'; i++) {
		uint32_t digit = str[i] - 48;

		// if it is a "_" treat as a 0
		if (digit == 47) {
			digit = 0;
		}

		// if it is a digit 0-9
		if (digit >= 0 && digit <= 9) {

			// check for overflow and handle it
		    if (num > UINT_MAX / 10) {
		    	return UINT_MAX;
		    }
		    num *= 10;

		    // check for overflow and handle it
		    if (num > UINT_MAX - digit) {
		    	return UINT_MAX;
		    }
			num += digit;
		}
	}

	return num;
}

void get_str_from_freq(uint32_t i, char b[], bool prepend_blank)
{
	char const digit[] = "0123456789";
	char *p = b;

	if (i == 0) {
		strcpy(p, EMPTY_FREQ);
		return;
	}

	//Move to end of str
	int str_i;
	for (str_i = 0; b[str_i] != '\0'; ++str_i)
		;
	p = p + str_i;

	int thousands = 0;
	do { // add digits backwards, inserting thousands separator
		if (thousands == 3) {
			if (i == 0 && prepend_blank) {
				*--p = ' ';
			} else {
				*--p = '.';
			}
			thousands = 0;
		} else {
			if (i) {
				*--p = digit[i % 10];
				i = i / 10;
			} else if (prepend_blank){
				*--p = ' ';
			} else { // prepend underscores
				*--p = '_';
			}
			thousands++;
		}
	} while (p > b);
}

// assign buffer to SDRAM since we are limited on SRAM
__attribute__((section(".sdram")))  static uint8_t img_buffer[480 * 800 * 3]; // 480px * 800px * 3 bytes per pixel

static void screen_capture(void)
{
	lv_obj_t *scr = lv_scr_act();
	lv_img_dsc_t snapshot_img;
	lv_res_t snap = lv_snapshot_take_to_buf(scr, LV_IMG_CF_TRUE_COLOR_ALPHA,
			&snapshot_img, &img_buffer, sizeof(img_buffer));

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

		if (save_image(header, sizeof(header), img_buffer, sizeof(img_buffer))
				!= FR_OK) {
			// error
			BSP_LED_On(LED_ORANGE);
		} else {
			BSP_LED_On(LED_GREEN);
		}
	}
}

