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

#include "ui/ui_about_panel.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <task_radio.h>
#include <ui/openht_ui.h>
#include <user_settings.h>
#include <radio_settings.h>

#include "ui/ui_fpga_status_panel.h"
#include "stm32469i_discovery_lcd.h"

static lv_obj_t *settings_tabview = NULL;

void init_settings_panel(void)
{
    // BEGIN SETTINGS TABVIEW UI INIT

	settings_tabview = lv_tabview_create(ui_settings_tabview_panel, LV_DIR_TOP, 40);
    lv_obj_set_style_bg_color(settings_tabview, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *tab_btns = lv_tabview_get_tab_btns(settings_tabview);
    lv_obj_set_style_text_font(tab_btns, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tab_btns, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tab_btns, lv_color_hex(0x37B9F5), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_border_side(tab_btns, LV_BORDER_SIDE_BOTTOM, LV_PART_ITEMS | LV_STATE_CHECKED);


    lv_obj_set_width(settings_tabview, lv_pct(100));
    lv_obj_set_height(settings_tabview, lv_pct(100));
    lv_obj_set_style_pad_left(settings_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(settings_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(settings_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(settings_tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_t *settings_tab = lv_tabview_add_tab(settings_tabview, "Settings");
    lv_obj_set_style_pad_left(settings_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(settings_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(settings_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(settings_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_parent(ui_settings_tab_panel, settings_tab);
	lv_obj_clear_flag(ui_settings_tab_panel, LV_OBJ_FLAG_HIDDEN);

	lv_obj_t *debug_tab = lv_tabview_add_tab(settings_tabview, "Debug");
    lv_obj_set_style_pad_left(debug_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(debug_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(debug_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(debug_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_parent(ui_debug_tab_panel, debug_tab);
	lv_obj_clear_flag(ui_debug_tab_panel, LV_OBJ_FLAG_HIDDEN);

	// END SETTINGS TABVIEW UI INIT
}

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

void on_settings_reboot_clicked(lv_event_t *e)
{
	NVIC_SystemReset();
}

void on_settings_load_fpga_clicked(lv_event_t *e)
{
	task_radio_event(DOWNLOAD_BITSTREAM);
}

void on_settings_erase_fpga_clicked(lv_event_t *e)
{
	task_radio_event(ERASE_BITSTREAM_STORAGE);
}

void on_settings_a_clicked(lv_event_t *e)
{
	task_radio_event(UPLOAD_BITSTREAM);
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
