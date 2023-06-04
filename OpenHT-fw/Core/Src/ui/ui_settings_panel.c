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


void on_settings_clicked(lv_event_t *e)
{
	lv_obj_clear_flag(ui_settings_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

void on_settings_ok_clicked(lv_event_t *e)
{
	lv_obj_add_flag(ui_settings_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

void on_settings_erase_usr_clicked(lv_event_t *e)
{
	user_settings_reset();
}

void on_settings_load_fpga_clicked(lv_event_t *e)
{
	download_fpga_binary_file();
}

void on_settings_erase_fpga_clicked(lv_event_t *e)
{
	erase_fpga_storage();
}

void on_settings_a_clicked(lv_event_t *e)
{
	// TODO: add button a
	upload_fpga_binary();

}

void on_settings_b_clicked(lv_event_t *e)
{
	// TODO: add button b
}

void on_settings_c_clicked(lv_event_t *e)
{
	// TODO: add button c
}

void on_settings_d_clicked(lv_event_t *e)
{
	// TODO: add button d
}
