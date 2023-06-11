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

#include "ui/lvht_qwertypad.h"

#include "sls_generated/ui.h"

#include <lvgl.h>

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

// array matrix of buttons
static const char *btnm_map[] = { "1", "2", "3", "4", "5","6", "7", "8", "9","0", "\n",
								  "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "\n",
								  "A", "S", "D", "F", "G", "H", "J", "K", "L", LV_SYMBOL_BACKSPACE, "\n",
								  "Z", "X", "C", "V", "B", "N", "M", "-", ".", "/", ""
//								  "Z", "X", "C", "V", "B", "N", "M", LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
								};
static lv_obj_t *panel = NULL;
//static lv_obj_t *this_obj = NULL;

// top y of the panel
static lv_coord_t panel_visible_y = 0;
static lv_coord_t panel_invsible_y = 800;

lv_obj_t * create_qwerty_pad(lv_obj_t *lv_obj)
{
	panel_invsible_y = LV_VER_RES;

	panel = lv_obj;

    lv_obj_set_y(panel, panel_invsible_y);
	lv_obj_move_foreground(panel);

	// create button matrix and assign as parent: lv_obj
	lv_obj_t *this_obj = lv_btnmatrix_create(panel);
	lv_btnmatrix_set_map(this_obj, btnm_map);

	// get the style width/height of the parent.
	// Using this because actual width and height are not defined yet since not visible

	lv_coord_t w = lv_obj_get_style_width(lv_obj, LV_PART_MAIN);
	lv_coord_t h = lv_obj_get_style_height(lv_obj, LV_PART_MAIN);
	panel_visible_y = panel_invsible_y - h;

	lv_obj_align(this_obj, LV_ALIGN_DEFAULT, 0, 0);
	lv_obj_set_width(this_obj, w);
	lv_obj_set_height(this_obj, h);

	// area between buttons
	lv_obj_set_style_border_side(this_obj, LV_BORDER_SIDE_NONE, LV_PART_MAIN);
	lv_obj_set_style_bg_color(this_obj, lv_color_hex(0x000000),
			LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(this_obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// the buttons in the matrix
	lv_obj_set_style_bg_color(this_obj, lv_color_hex(0x191C26),
			LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(this_obj, lv_color_hex(0x37B9F5),
			LV_PART_ITEMS | LV_STATE_PRESSED);

	lv_obj_set_style_text_font(this_obj, &lv_font_montserrat_20,
			LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(this_obj, lv_color_hex(0xFFFFFF),
			LV_PART_ITEMS | LV_STATE_DEFAULT);

	return this_obj;
}

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

void set_qwertypad_visibility(bool visible)
{
	int32_t start = panel_invsible_y;
	int32_t end = panel_visible_y;

	if (!visible) {
		start = panel_visible_y;
		end = panel_invsible_y;
	}

	// get current y to determine visibility status
	int32_t current = lv_obj_get_y(panel);

	if (current != end) {
		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_var(&a, panel);
		lv_anim_set_values(&a, start, end);
		lv_anim_set_time(&a, 500);
		lv_anim_set_exec_cb(&a, anim_x_cb);
		lv_anim_set_path_cb(&a, lv_anim_path_linear);
		lv_anim_start(&a);
	}
}

