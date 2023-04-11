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

#include "lvht_numpad.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <lvgl.h>

#include "lvgl_ui/ui.h"

// array matrix of buttons
static const char *btnm_map[] = { "7", "8", "9", "\n",
								  "4", "5", "6", "\n",
								  "1", "2", "3", "\n",
								//"*", "0", ".", "#", "\n",
								  "0", LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
								};

// top y of the panel
#define PANEL_VISIBLE 400
#define PANEL_INVISIBLE 800

lv_obj_t * create_number_pad(lv_obj_t *lv_obj)
{
	// remove the border for the UI placeholder
	// Hint: using border in SquareLine Studio allows visibility while moving
	// things around, then set border to none at runtime...
    lv_obj_set_y(ui_panel_numpad, PANEL_INVISIBLE);
	lv_obj_set_style_border_side(ui_panel_numpad, LV_BORDER_SIDE_NONE,
			LV_PART_MAIN);
	lv_obj_move_foreground(ui_panel_numpad);

	// create button matrix and assign as parent: lv_obj
	lv_obj_t *btnm1 = lv_btnmatrix_create(lv_obj);
	lv_btnmatrix_set_map(btnm1, btnm_map);
	lv_btnmatrix_set_btn_width(btnm1, 9, 7);
	lv_btnmatrix_set_btn_width(btnm1, 10, 4);
	lv_btnmatrix_set_btn_width(btnm1, 11, 4);
	lv_btnmatrix_set_btn_width(btnm1, 12, 5);

	// get the style width/height of the parent.
	// Using this because actual width and height are not defined yet since not visible
	lv_coord_t w = lv_obj_get_style_width(lv_obj, LV_PART_MAIN);
	lv_coord_t h = lv_obj_get_style_height(lv_obj, LV_PART_MAIN);
	lv_obj_align(btnm1, LV_ALIGN_DEFAULT, 0, 0);
	lv_obj_set_width(btnm1, w);
	lv_obj_set_height(btnm1, h);

	// area between buttons
	lv_obj_set_style_border_side(btnm1, LV_BORDER_SIDE_NONE, LV_PART_MAIN);
	lv_obj_set_style_bg_color(btnm1, lv_color_hex(0x000000),
			LV_PART_MAIN | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(btnm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	// the buttons in the matrix
	lv_obj_set_style_bg_color(btnm1, lv_color_hex(0x191C26),
			LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(btnm1, lv_color_hex(0x37B9F5),
			LV_PART_ITEMS | LV_STATE_PRESSED);

	lv_obj_set_style_text_font(btnm1, &lv_font_montserrat_42,
			LV_PART_ITEMS | LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(btnm1, lv_color_hex(0xFFFFFF),
			LV_PART_ITEMS | LV_STATE_DEFAULT);

	return btnm1;
}

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

void set_numpad_visibility(bool visible)
{
	int32_t start = PANEL_INVISIBLE;
	int32_t end = PANEL_VISIBLE;

	if (!visible) {
		start = PANEL_VISIBLE;
		end = PANEL_INVISIBLE;
	}

	// get current y to determine visibility status
	int32_t current = lv_obj_get_y(ui_panel_numpad);

	if (current != end) {
		lv_anim_t a;
		lv_anim_init(&a);
		lv_anim_set_var(&a, ui_panel_numpad);
		lv_anim_set_values(&a, start, end);
		lv_anim_set_time(&a, 500);
		lv_anim_set_exec_cb(&a, anim_x_cb);
		lv_anim_set_path_cb(&a, lv_anim_path_linear);
		lv_anim_start(&a);
	}
}

