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

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <task_radio.h>
#include <ui/openht_ui.h>
#include <user_settings.h>
#include <radio_settings.h>

#include "ui/ui_fpga_status_panel.h"
#include "stm32469i_discovery_lcd.h"

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

void on_settings_erase_radio_clicked(lv_event_t *e)
{
	radio_settings_reset();
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
	set_fpga_status(FPGA_Offline);
}

void on_settings_d_clicked(lv_event_t *e)
{
	// TODO: add button d
	set_fpga_status(FPGA_Error);
}

void on_settings_e_clicked(lv_event_t *e)
{
	// TODO: add button c
	set_fpga_status(FPGA_Loading);
}

void on_settings_f_clicked(lv_event_t *e)
{
	// TODO: add button d
	set_fpga_status(FPGA_Running);
}

void on_use_soft_ptt_clicked(lv_event_t *e)
{
	if (lv_obj_has_state(ui_use_soft_ptt_cb, LV_STATE_CHECKED)) {
		user_settings.use_soft_ptt = true;
		lv_obj_clear_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);
	} else {
		user_settings.use_soft_ptt = false;
		lv_obj_add_flag(ui_ptt_btn, LV_OBJ_FLAG_HIDDEN);
	}

	user_settings_save(&user_settings);
}

void on_use_tx_offset_clicked(lv_event_t *e)
{
	if (lv_obj_has_state(ui_use_freq_offset_cb, LV_STATE_CHECKED)) {
		user_settings.use_freq_offset = true;
	} else {
		user_settings.use_freq_offset = false;
	}

	user_settings_save(&user_settings);
}

void on_disp_brightness_changed(lv_event_t *e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    uint8_t val = (uint8_t)lv_slider_get_value(slider);

    // make sure that the display doesn't go off completely
    if (val < 25) val = 25;

    NT35510_Set_Backlight(val);
}
