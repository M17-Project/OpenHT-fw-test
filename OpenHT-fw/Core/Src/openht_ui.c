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
#include "openht_ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <lvgl.h>

#include "lvgl_ui/ui.h"

#include "disk_mgr.h"
#include "fatfs.h"
#include "bmp_utils.h"
#include "user_settings.h"
#include "openht_hwconfig.h"
#include "openht_types.h"
#include <../../Drivers/BSP/Components/nt35510/nt35510.h>


#define EMPTY_FREQ "_.___.___.___"
#define END_POS 13
#define THOU_POS 9
#define MEG_POS 5
#define GIG_POS 1

static char callsign_str[10] = ""; // 9 digits for callsign

static char current_freq_str[] = EMPTY_FREQ;
static lv_obj_t *current_freq_ta = NULL;
static uint32_t *current_freq = NULL;

static settings_t user_settings;

static void change_vfo_frequency(bool move_up);
static bool validate_freq(uint32_t *freq);
static int32_t move_cursor(int32_t curs_pos, int32_t movement);
static void update_cursor_pos(lv_obj_t *textAreaFreq);
static void end_input_callsign_ta();
static void update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq);
static void end_input_freq_ta(bool finished_input);

static void screen_capture(void);

static lv_obj_t *about_tabview = NULL;

void custom_ui_init(void)
{
	// test inputs...
	//uint32_t test = get_freq_from_str("_39.123.___");
	//get_str_from_freq(test, rxfreqstr);
	//get_str_from_freq(0, rxfreqstr);

	// squareline designer clean up code...
	lv_obj_set_parent(ui_about_panel, lv_layer_top());
	lv_obj_add_flag(ui_about_panel, LV_OBJ_FLAG_HIDDEN);
//    lv_obj_set_y(ui_about_panel, 800);
	//lv_obj_move_foreground(ui_about_panel);

	lv_obj_set_parent(ui_mode_change_panel, lv_layer_top());
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);


    // BEGIN ABOUT TABVIEW UI INIT

	about_tabview = lv_tabview_create(ui_about_tab_panel, LV_DIR_TOP, 40);
    lv_obj_set_style_bg_color(about_tabview, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * tab_btns = lv_tabview_get_tab_btns(about_tabview);
//    lv_obj_set_style_bg_color(tab_btns, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
//    lv_obj_set_style_text_color(tab_btns, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_text_font(tab_btns, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tab_btns, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0x37B9F5), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_BOTTOM, LV_PART_ITEMS | LV_STATE_CHECKED);


    lv_obj_set_width(about_tabview, lv_pct(100));
    lv_obj_set_height(about_tabview, lv_pct(100));
    lv_obj_set_style_pad_left(about_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(about_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(about_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(about_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_t *about_tab = lv_tabview_add_tab(about_tabview, "About");
    lv_obj_set_style_pad_left(about_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(about_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(about_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(about_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_parent(ui_about_text_area, about_tab);
	lv_obj_clear_flag(ui_about_text_area, LV_OBJ_FLAG_HIDDEN);

	lv_obj_t *capes_tab = lv_tabview_add_tab(about_tabview, "HW Info");
    lv_obj_set_style_pad_left(capes_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(capes_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(capes_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(capes_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_parent(ui_about_hw_text_area, capes_tab);
	lv_obj_clear_flag(ui_about_hw_text_area, LV_OBJ_FLAG_HIDDEN);

	// END ABOUT TABVIEW UI INIT

	// BEGIN MODE CHANGE UI INIT

	char labels[128];
	char *label = labels;

	for (int i = 0; i < openht_mode_count; i++) {
		if (i > 0) {
			*label++ = '\n';
		}
		int mode_str_len = strlen(openht_mode_data[i].mode_name);
		strlcpy(label, openht_mode_data[i].mode_name, sizeof(labels) - (label - labels));

		label += mode_str_len;
	}

    lv_roller_set_options(ui_mode_roller, labels, LV_ROLLER_MODE_NORMAL);


	// END MODE CHANGE UI INIT


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
	lv_obj_t * num_pad = create_number_pad(ui_panel_num_pad);

	// callback handler
	lv_obj_add_event_cb(num_pad, numpad_btnmatrix_event_cb, LV_EVENT_ALL, NULL);
	lv_obj_add_event_cb(qwerty_pad, qwertypad_btnmatrix_event_cb, LV_EVENT_ALL, NULL);


	// GET STORED SETTINGS AND UPDATE UI
	get_settings(&user_settings);

	char buffer[15];
	snprintf(buffer, 15, "%u", user_settings.rx_freq);
	lv_label_set_text(ui_label_test_rx, buffer);
	get_str_from_freq(user_settings.rx_freq, current_freq_str, true);
	lv_textarea_set_text(ui_text_area_rx_freq, current_freq_str);

	snprintf(buffer, 15, "%u", user_settings.tx_freq);
	lv_label_set_text(ui_label_test_tx, buffer);
	get_str_from_freq(user_settings.tx_freq, current_freq_str, true);
	lv_textarea_set_text(ui_text_area_tx_freq, current_freq_str);

	strcpy(callsign_str, user_settings.callsign);
	lv_textarea_set_text(ui_text_area_callsign, callsign_str);
	lv_label_set_text_fmt(ui_header_callsign_label, "Call: %s", callsign_str);

	lv_roller_set_selected(ui_mode_roller, user_settings.mode, LV_ANIM_OFF);

	lv_label_set_text_fmt(ui_header_mode_label, "Mode: %s", openht_get_mode_str(user_settings.mode));
}

void on_about_clicked(lv_event_t *e)
{
	if (about_tabview != NULL) {
    //lv_obj_set_y(ui_about_panel, 0);
	lv_tabview_set_act(about_tabview, 0, LV_ANIM_OFF);
	lv_obj_clear_flag(ui_about_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
	}
}

void on_about_ok_clicked(lv_event_t *e)
{
	lv_obj_add_flag(ui_about_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

void on_callsign_clicked(lv_event_t *e)
{
}

void on_mode_clicked(lv_event_t *e)
{
	lv_obj_clear_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

}

void on_mode_cancel_clicked(lv_event_t *e)
{
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

void on_mode_ok_clicked(lv_event_t *e)
{
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

	char roller_str[15];

	lv_roller_get_selected_str(ui_mode_roller, roller_str, sizeof(roller_str));
	lv_label_set_text_fmt(ui_header_mode_label, "Mode: %s", roller_str);

	user_settings.mode = lv_roller_get_selected(ui_mode_roller);
	save_settings(&user_settings);
}

void on_screen_pressed(lv_event_t *e)
{
	end_input_callsign_ta();
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

void on_callsign_ta_click(lv_event_t *e)
{
	end_input_freq_ta(true);

	lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_FULL,
			LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 150, LV_PART_CURSOR | LV_STATE_DEFAULT);

	set_qwertypad_visibility(true);
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
}

void on_xmit_button_release(lv_event_t *e)
{
	BSP_LED_Off(LED_RED);
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

void qwertypad_btnmatrix_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED) {
		uint32_t id = lv_btnmatrix_get_selected_btn(obj);
		const char *txt = lv_btnmatrix_get_btn_text(obj, id);

		LV_LOG_USER("%s was pressed\n", txt);

		// get current cursor position
		uint32_t curs_pos = lv_textarea_get_cursor_pos(ui_text_area_callsign);

		if (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) {
			if (curs_pos > 0) {
				curs_pos--;
				memmove(&callsign_str[curs_pos], &callsign_str[curs_pos + 1], strlen(callsign_str) - curs_pos);
			}
		} else if (curs_pos < sizeof(callsign_str) - 1){
			memmove(&callsign_str[curs_pos + 1], &callsign_str[curs_pos], sizeof(callsign_str) - 1 - curs_pos);
			callsign_str[curs_pos] = *txt;
			curs_pos++;
		}

		// update the text
		lv_textarea_set_text(ui_text_area_callsign, callsign_str);

		// update cursor position
		lv_textarea_set_cursor_pos(ui_text_area_callsign, curs_pos);

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

static void end_input_callsign_ta()
{
	set_qwertypad_visibility(false);

	lv_textarea_set_cursor_pos(ui_text_area_callsign, LV_TEXTAREA_CURSOR_LAST);
	lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

	lv_label_set_text_fmt(ui_header_callsign_label, "Call: %s", callsign_str);

    strcpy(user_settings.callsign, callsign_str);
	save_settings(&user_settings);
}

static void update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq)
{
	if (new_freq_ta == NULL)
		return;

	end_input_freq_ta(false);
	end_input_callsign_ta();
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
	snprintf(buffer, 15, "%u", user_settings.rx_freq);
	lv_label_set_text(ui_label_test_rx, buffer);

//	user_settings.tx_freq = get_freq_from_str(txfreqstr);
	snprintf(buffer, 15, "%u", user_settings.tx_freq);
	lv_label_set_text(ui_label_test_tx, buffer);

	save_settings(&user_settings);
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
				!= OPENHT_OK) {
			// error
			BSP_LED_On(LED_ORANGE);
		} else {
			BSP_LED_On(LED_GREEN);
		}
	}
}

