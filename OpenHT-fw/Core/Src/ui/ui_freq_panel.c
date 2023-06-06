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
#include "task_fpga.h"


static char current_freq_str[] = EMPTY_FREQ;
static uint32_t *current_freq = NULL;
static lv_obj_t *current_freq_ta = NULL;

static int32_t move_cursor(int32_t curs_pos, int32_t movement);
static void update_cursor_pos(lv_obj_t *textAreaFreq);

static void update_active_freq_ta(lv_obj_t *new_freq_ta, uint32_t *freq);
static void end_input_freq_ta(bool finished_input);
static void change_vfo_frequency(bool move_up);
static void change_freq(int32_t freq_shift);

void on_freq_click(lv_event_t *e)
{
	update_active_freq_ta(ui_text_area_rx_freq, &user_settings.rx_freq);

	lv_obj_clear_flag(ui_freq_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
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
		lv_obj_add_flag(ui_freq_panel, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
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
