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

#include <ui/ui_about_panel.h>
#include <ui/openht_ui.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <lvgl.h>

#include "lvgl_ui/ui.h"

static lv_obj_t *about_tabview = NULL;

void init_about_panel(void)
{
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

