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
#include "ui/lvht_qwertypad.h"
#include "ui/openht_ui.h"

static char work_str[10] = ""; // 9 digits for callsign

void init_callsign_change_panel(lv_obj_t * obj)
{
	lv_obj_add_event_cb(obj, qwertypad_btnmatrix_event_cb, LV_EVENT_ALL, NULL);
}

void on_callsign_clicked(lv_event_t *e)
{
	// update text area with current callsign...
	strcpy(work_str, callsign_str);
	lv_textarea_set_text(ui_text_area_callsign, callsign_str);

	lv_obj_clear_flag(ui_callsign_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(ui_qwerty_key_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

	// make border selected
	lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_FULL,
			LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 150, LV_PART_CURSOR | LV_STATE_DEFAULT);

    // show keyboard
	set_qwertypad_visibility(true);
}

void on_callsign_ok_clicked(lv_event_t *e)
{
	set_qwertypad_visibility(false);

	lv_textarea_set_cursor_pos(ui_text_area_callsign, LV_TEXTAREA_CURSOR_LAST);
	lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

	lv_obj_add_flag(ui_callsign_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_qwerty_key_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

	// save callsign
	strcpy(callsign_str, work_str);
	update_callsign();
}

void on_callsign_cancel_clicked(lv_event_t *e)
{
	set_qwertypad_visibility(false);

	lv_textarea_set_cursor_pos(ui_text_area_callsign, LV_TEXTAREA_CURSOR_LAST);
	lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

	lv_obj_add_flag(ui_callsign_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(ui_qwerty_key_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

void on_callsign_ta_click(lv_event_t *e)
{
	/*
	end_input_freq_ta(true);

	lv_obj_set_style_border_side(ui_text_area_callsign, LV_BORDER_SIDE_FULL,
			LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_text_area_callsign, 150, LV_PART_CURSOR | LV_STATE_DEFAULT);

	set_qwertypad_visibility(true);
	*/
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
				memmove(&work_str[curs_pos], &work_str[curs_pos + 1], strlen(work_str) - curs_pos);
			}
		} else if (curs_pos < sizeof(work_str) - 1){
			memmove(&work_str[curs_pos + 1], &work_str[curs_pos], sizeof(work_str) - 1 - curs_pos);
			work_str[curs_pos] = *txt;
			curs_pos++;
		}

		// update the text
		lv_textarea_set_text(ui_text_area_callsign, work_str);

		// update cursor position
		lv_textarea_set_cursor_pos(ui_text_area_callsign, curs_pos);

	}
}

