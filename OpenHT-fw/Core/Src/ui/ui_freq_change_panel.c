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

#include "ui/ui_freq_change_panel.h"

#include "ui/openht_ui.h"
#include "ui/lvht_numpad.h"
#include "utils/str_formatting.h"
#include "../shell/inc/sys_command_line.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static char current_freq_str[] = EMPTY_FREQ;
static uint32_t *current_freq = NULL;
static lv_obj_t *current_freq_ta = NULL;

static uint32_t rx_freq = 0;
static uint32_t tx_freq = 0;
static bool split_mode = false;

static int32_t _move_cursor(int32_t curs_pos, int32_t movement);
static void _update_cursor_pos(lv_obj_t *freq_ta);
static void _update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq);
static void _end_input_freq_ta(bool finished_input);
static void _change_vfo_frequency(bool move_up);
static void _change_freq(int32_t freq_shift);
static void _update_freq_text(void);

void init_freq_change_panel()
{
	rx_freq = radio_settings_get_rx_freq();

	if (user_settings_get_split_mode()) {
		split_mode = true;
		tx_freq = radio_settings_get_tx_freq();
		lv_obj_add_state(ui_split_freq_cb, LV_STATE_CHECKED);
	} else {
		split_mode = false;
		tx_freq = radio_settings_get_rx_freq();
		lv_obj_clear_state(ui_split_freq_cb, LV_STATE_CHECKED);
	}

	update_split_mode();

	if (user_settings_get_use_freq_offset()) {
		lv_obj_clear_flag(ui_offset_tx_panel, LV_OBJ_FLAG_HIDDEN);
		lv_obj_add_flag(ui_tx_freq_panel, LV_OBJ_FLAG_HIDDEN);
	} else {
		lv_obj_add_flag(ui_offset_tx_panel, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(ui_tx_freq_panel, LV_OBJ_FLAG_HIDDEN);
	}


	char freq_str[] = EMPTY_FREQ;
	get_str_from_freq(rx_freq, freq_str, 1);
	lv_textarea_set_text(ui_rx_freq_ta, freq_str);

	get_str_from_freq(tx_freq, freq_str, 1);
	lv_textarea_set_text(ui_tx_freq_ta, freq_str);

	lv_obj_add_flag(ui_fm_settings_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_m17_settings_panel, LV_OBJ_FLAG_HIDDEN);

	openht_mode_t mode = radio_settings_get_mode();
	if (mode == OpMode_NFM ||
			mode == OpMode_WFM) {
		lv_obj_clear_flag(ui_fm_settings_panel, LV_OBJ_FLAG_HIDDEN);
	} else if (mode == OpMode_M17) {
		lv_obj_clear_flag(ui_m17_settings_panel, LV_OBJ_FLAG_HIDDEN);
	}

	// configure m17 panel


	// configure fm panel
	fmInfo_t fm_info = radio_settings_get_fm_settings();
	lv_dropdown_set_selected(ui_ctcss_tx_dropdown, fm_info.txTone);
}

void on_freq_click(lv_event_t *e)
{
	// init data
	init_freq_change_panel();

	// HACK WARNING: LVGL seems to render momentarily obscured widgets, so hide
	lv_obj_add_flag(ui_vfo_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);

	// End HACK

	// unhide the panel and make the top layer clickable
	lv_obj_clear_flag(ui_freq_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

// save all changes from the frequency change
void on_freq_ok_clicked(lv_event_t *e)
{
	// HACK WARNING: now unhide
	lv_obj_clear_flag(ui_vfo_panel, LV_OBJ_FLAG_HIDDEN);

	if (user_settings_get_use_soft_ptt()) {
		lv_obj_clear_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);
	}
	// End HACK

	_end_input_freq_ta(true);
	lv_obj_add_flag(ui_freq_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

	radio_settings_set_rx_freq(rx_freq);
	radio_settings_set_tx_freq(tx_freq);
	user_settings_set_split_mode(split_mode);


	// update fm settings...
	uint16_t tx_index = lv_dropdown_get_selected(ui_ctcss_tx_dropdown);
	fmInfo_t fm_info = radio_settings_get_fm_settings();

	if (tx_index == 0) { // disabled
		fm_info.txToneEn = false;
	} else {
		fm_info.txToneEn = true;
	}

	fm_info.txTone = tx_index;
	radio_settings_set_fm_settings(fm_info);

	// save to non-volatile storage...
	radio_settings_save();//&radio_settings);
	user_settings_save();//&user_settings);

	char rx_buffer[10];
	//get_str_from_freq(rx_freq, rx_buffer, -1);
	get_display_str_from_freq(rx_freq, rx_buffer);
//	lv_label_set_text(ui_rx_display_label, rx_buffer);
	lv_label_set_text_fmt(ui_rx_display_label, "%s", rx_buffer);

	char tx_buffer[10];
	//get_str_from_freq(tx_freq, tx_buffer, -1);
	get_display_str_from_freq(tx_freq, tx_buffer);
//	lv_label_set_text(ui_tx_display_label, tx_buffer);
	lv_label_set_text_fmt(ui_tx_display_label, "%s", tx_buffer);

}

// cancel all changes
void on_freq_cancel_clicked(lv_event_t *e)
{
	// HACK WARNING: now unhide
	lv_obj_clear_flag(ui_vfo_panel, LV_OBJ_FLAG_HIDDEN);

	if (user_settings_get_use_soft_ptt()) {
		lv_obj_clear_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);
	}

	// End HACK

	_end_input_freq_ta(true);
	lv_obj_add_flag(ui_freq_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

}

// when the user touches somewhere on the panel
void on_freq_change_panel_click(lv_event_t *e)
{
	// end the input
	_end_input_freq_ta(true);
}

// when the user touches the split check box
void on_split_txrx_clicked(lv_event_t *e)
{
	_end_input_freq_ta(true);
	split_mode = update_split_mode();

	if (!split_mode) {
		// set freq the same
		tx_freq = rx_freq;
		char freq_str[] = EMPTY_FREQ;
		get_str_from_freq(tx_freq, freq_str, 1);
		lv_textarea_set_text(ui_tx_freq_ta, freq_str);

	}
}

// when the user touches the freq down of the panel freq bump
void on_freq_button_down_press(lv_event_t * e)
{
	_change_vfo_frequency(false);
}

// when the user touches the freq up of the panel freq bump
void on_freq_button_up_press(lv_event_t * e)
{
	_change_vfo_frequency(true);
}

// when the user touches the rx frequency text area
void on_rx_freq_ta_click(lv_event_t *e)
{
	if (current_freq_ta == ui_rx_freq_ta) {
		_update_cursor_pos(ui_rx_freq_ta);
		return;
	}
	_update_active_freq_ta(ui_rx_freq_ta, &rx_freq);
}

// when the user touches the tx frequency text area
void on_tx_freq_ta_click(lv_event_t *e)
{
	if (current_freq_ta == ui_tx_freq_ta) {
		_update_cursor_pos(ui_tx_freq_ta);
		return;
	}
	_update_active_freq_ta(ui_tx_freq_ta, &tx_freq);
}

// this callback does the handling of user touches for the keypad
void numpad_btnmatrix_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *obj = lv_event_get_target(e);
	if (code == LV_EVENT_VALUE_CHANGED) {
		uint32_t id = lv_btnmatrix_get_selected_btn(obj);
		const char *txt = lv_btnmatrix_get_btn_text(obj, id);

		LOG(CLI_LOG_GUI, "%s was pressed\r\n", txt);

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
			curs_pos = _move_cursor(curs_pos, -1);
		}
		// move cursor right
		else if (strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
			curs_pos = _move_cursor(curs_pos, +1);
		}
		// end input
		else if (strcmp(txt, LV_SYMBOL_OK) == 0) {
			curs_pos = END_POS;
		} else {
			current_freq_str[curs_pos] = *txt;

			*current_freq = get_freq_from_str(current_freq_str);
			// update the freq text areas
			_update_freq_text();

			if (curs_pos == END_POS - 1) {
				curs_pos = END_POS;
			} else {
				// update cursor position...
				curs_pos = _move_cursor(curs_pos, +1);
			}
		}

		// update cursor position
		lv_textarea_set_cursor_pos(current_freq_ta, curs_pos);

		if (curs_pos == END_POS) {
			_end_input_freq_ta(true);
		}
	}
}

// based on the split mode the tx text area or tx offset text area are either active or inactive
bool update_split_mode()
{
	bool split = false;
	if (lv_obj_has_state(ui_split_freq_cb, LV_STATE_CHECKED)) {
		split = true;
		// ui_tx_freq_ta
		lv_obj_add_flag(ui_tx_freq_ta, LV_OBJ_FLAG_CLICKABLE);
	    lv_obj_set_style_text_color(ui_tx_freq_ta, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_text_color(ui_label_tx, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

	    // ui_offset_ta
		lv_obj_add_flag(ui_tx_offset_ta, LV_OBJ_FLAG_CLICKABLE);
	    lv_obj_set_style_text_color(ui_tx_offset_ta, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_text_color(ui_label_offset, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

	} else {
		// ui_tx_freq_ta
		lv_obj_clear_flag(ui_tx_freq_ta, LV_OBJ_FLAG_CLICKABLE);
	    lv_obj_set_style_text_color(ui_tx_freq_ta, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_text_color(ui_label_tx, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);

	    // ui_offset_ta
	    lv_obj_clear_flag(ui_tx_offset_ta, LV_OBJ_FLAG_CLICKABLE);
	    lv_obj_set_style_text_color(ui_tx_offset_ta, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
	    lv_obj_set_style_text_color(ui_label_offset, lv_color_hex(0x888888), LV_PART_MAIN | LV_STATE_DEFAULT);
	}

	return split;
}

// update the frequency text areas with the valid frequencies
static void _update_freq_text()
{
	if (current_freq_ta != NULL && current_freq != NULL) {
		get_str_from_freq(*current_freq, current_freq_str, 0);
		lv_textarea_set_text(current_freq_ta, current_freq_str);

		if (!split_mode) {
			char str[] = EMPTY_FREQ;
			get_str_from_freq(*current_freq, str, 1);
			lv_textarea_set_text(ui_tx_freq_ta, str);
		}
	}
}

// move the vfo up or down based on the selected freq shift
static void _change_vfo_frequency(bool move_up)
{
	uint16_t selected_item = lv_dropdown_get_selected(ui_freq_dropdown);

	// need signed value
	int32_t freq_shift = 0;

	switch (selected_item) {
		case 0:
			freq_shift = 1000;
			break;
		case 1:
			freq_shift = 6250;
			break;
		case 2:
			freq_shift = 12500;
			break;
		case 3:
			freq_shift = 25000;
			break;
		default:
			break;
	}

	_change_freq(move_up ? freq_shift : -freq_shift);
}

// change the frequency and update the text areas
static void _change_freq(int32_t freq_shift)
{
	if (current_freq_ta != NULL && current_freq != NULL) {
		uint32_t curs_pos = lv_textarea_get_cursor_pos(current_freq_ta);
		*current_freq += freq_shift;

		validate_freq(current_freq);

		_update_freq_text();

		lv_textarea_set_cursor_pos(current_freq_ta, curs_pos);
	}
}

static void _update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq)
{
	if (new_freq_ta == NULL)
		return;

	_end_input_freq_ta(false);
	// show the number keypad
	set_numpad_visibility(true);

	current_freq_ta = new_freq_ta;

	// store the ptr for the freq so val can be updated
	current_freq = freq;

	get_str_from_freq(*current_freq, current_freq_str, 0);
	uint32_t curs_pos = lv_textarea_get_cursor_pos(current_freq_ta);
	lv_textarea_set_text(current_freq_ta, current_freq_str);
	lv_textarea_set_cursor_pos(current_freq_ta, curs_pos);

	lv_obj_set_style_border_side(current_freq_ta, LV_BORDER_SIDE_FULL,
			LV_PART_MAIN | LV_STATE_DEFAULT);
	//lv_obj_set_style_anim_time(currentActiveTextAreaFreq, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

	_update_cursor_pos(current_freq_ta);
}

static void _end_input_freq_ta(bool finished_input)
{
	if (current_freq_ta == NULL) {
		return;
	}

	if (finished_input) {
		// hide the number keypad
		set_numpad_visibility(false);
	}

	lv_textarea_set_cursor_pos(current_freq_ta, END_POS);
	lv_obj_set_style_border_side(current_freq_ta, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN | LV_STATE_DEFAULT);

	//const char * txt = lv_textarea_get_text(currentActiveTextAreaFreq);
	*current_freq = get_freq_from_str(current_freq_str);

	// validate input to ensure within bands
	validate_freq(current_freq);

	get_str_from_freq(*current_freq, current_freq_str, 1);
	lv_textarea_set_text(current_freq_ta, current_freq_str);

	// if not in split tx/rx mode, set tx freq to same as rx
	if (!split_mode) {
		lv_textarea_set_text(ui_tx_freq_ta, current_freq_str);
		tx_freq = *current_freq;
	}

	current_freq = NULL;
	current_freq_ta = NULL;
}

static int32_t _move_cursor(int32_t curs_pos, int32_t movement)
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

static void _update_cursor_pos(lv_obj_t *textAreaFreq)
{
	uint32_t curs_pos = lv_textarea_get_cursor_pos((lv_obj_t*) textAreaFreq);
	if ((curs_pos == GIG_POS) || (curs_pos == MEG_POS) || (curs_pos == THOU_POS)) {
		curs_pos = curs_pos + 1;
	} else 	if (curs_pos == END_POS) {
		curs_pos = END_POS - 1;
	}
	lv_textarea_set_cursor_pos((lv_obj_t*) textAreaFreq, curs_pos);
}
