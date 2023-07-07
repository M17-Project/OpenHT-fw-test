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

static bool _init_done = false;
static lv_obj_t *settings_tabview = NULL;
static lv_obj_t * ppm_spinbox = NULL;
static lv_obj_t * dpd1_spinbox = NULL;
static lv_obj_t * dpd2_spinbox = NULL;
static lv_obj_t * dpd3_spinbox = NULL;
static lv_obj_t * offset_i_spinbox = NULL;
static lv_obj_t * offset_q_spinbox = NULL;
static lv_obj_t * balance_i_spinbox = NULL;
static lv_obj_t * balance_q_spinbox = NULL;
static lv_obj_t * tx_pwr_spinbox = NULL;

static void _ppm_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(ppm_spinbox);
    }
}

static void _ppm_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(ppm_spinbox);
    }
}

static void _dpd1_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(dpd1_spinbox);
    }
}

static void lv_dpd1_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(dpd1_spinbox);
    }
}

static void _dpd2_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(dpd2_spinbox);
    }
}

static void _dpd2_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(dpd2_spinbox);
    }
}

static void _dpd3_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(dpd3_spinbox);
    }
}

static void _dpd3_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(dpd3_spinbox);
    }
}

static void _offset_i_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(offset_i_spinbox);
    }
}

static void _offset_i_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(offset_i_spinbox);
    }
}

static void _offset_q_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(offset_q_spinbox);
    }
}

static void _offset_q_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(offset_q_spinbox);
    }
}

static void _balance_i_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(balance_i_spinbox);
    }
}

static void _balance_i_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(balance_i_spinbox);
    }
}

static void _balance_q_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(balance_q_spinbox);
    }
}

static void _balance_q_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(balance_q_spinbox);
    }
}

static void _tx_pwr_spinbox_inc_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code  == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(tx_pwr_spinbox);
    }
}

static void _tx_pwr_spinbox_dec_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(tx_pwr_spinbox);
    }
}

static void _get_xcvr_settings(bool default_xcvr_settings)
{
	xcvr_settings_t xcvr_settings;

	// if resetting to default xcvr settings
	if (default_xcvr_settings) {
		// get the default settings
		xcvr_settings = radio_settings_get_default_xcvr_settings();
	} else {
		// get the stored settings
		xcvr_settings = radio_settings_get_xcvr_settings();
	}

    lv_spinbox_set_value(ppm_spinbox, xcvr_settings.ppm);
    lv_spinbox_set_cursor_pos(ppm_spinbox, 0);

    lv_spinbox_set_value(dpd1_spinbox, xcvr_settings.dpd1);
    lv_spinbox_set_cursor_pos(dpd1_spinbox, 0);

    lv_spinbox_set_value(dpd2_spinbox, xcvr_settings.dpd2);
    lv_spinbox_set_cursor_pos(dpd2_spinbox, 0);

    lv_spinbox_set_value(dpd3_spinbox, xcvr_settings.dpd3);
    lv_spinbox_set_cursor_pos(dpd3_spinbox, 0);

    lv_spinbox_set_value(offset_i_spinbox, xcvr_settings.offset_i);
    lv_spinbox_set_cursor_pos(offset_i_spinbox, 0);

    lv_spinbox_set_value(offset_q_spinbox, xcvr_settings.offset_q);
    lv_spinbox_set_cursor_pos(offset_q_spinbox, 0);

    lv_spinbox_set_value(balance_i_spinbox, xcvr_settings.balance_i);
    lv_spinbox_set_cursor_pos(balance_i_spinbox, 0);

    lv_spinbox_set_value(balance_q_spinbox, xcvr_settings.balance_q);
    lv_spinbox_set_cursor_pos(balance_q_spinbox, 0);

    lv_spinbox_set_value(tx_pwr_spinbox, xcvr_settings.tx_pwr);
    lv_spinbox_set_cursor_pos(tx_pwr_spinbox, 0);

    if (xcvr_settings.phase_dither) {
    	lv_obj_add_state(ui_xcvr_dither_cb, LV_STATE_CHECKED);
    } else {
    	lv_obj_clear_state(ui_xcvr_dither_cb, LV_STATE_CHECKED);
    }

}

static void _save_xcvr_settings()
{
	xcvr_settings_t xcvr_settings = radio_settings_get_xcvr_settings();

	xcvr_settings.ppm = (int16_t)lv_spinbox_get_value(ppm_spinbox);
	xcvr_settings.dpd1 = (int16_t)lv_spinbox_get_value(dpd1_spinbox);

	xcvr_settings.dpd2 = (int16_t)lv_spinbox_get_value(dpd2_spinbox);
	xcvr_settings.dpd3 = (int16_t)lv_spinbox_get_value(dpd3_spinbox);

	xcvr_settings.offset_i = (int16_t)lv_spinbox_get_value(offset_i_spinbox);
	xcvr_settings.offset_q = (int16_t)lv_spinbox_get_value(offset_q_spinbox);

	xcvr_settings.balance_i = (int16_t)lv_spinbox_get_value(balance_i_spinbox);
	xcvr_settings.balance_q = (int16_t)lv_spinbox_get_value(balance_q_spinbox);

	xcvr_settings.tx_pwr = (int16_t)lv_spinbox_get_value(tx_pwr_spinbox);

	if (lv_obj_has_state(ui_xcvr_dither_cb, LV_STATE_CHECKED)) {
		xcvr_settings.phase_dither = true;
	} else {
		xcvr_settings.phase_dither = false;
	}

	// save the xcvr settings into radio_settings
	radio_settings_set_xcvr_settings(xcvr_settings);
}

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

	lv_obj_t *xcvr_tab = lv_tabview_add_tab(settings_tabview, "XCVR");
    lv_obj_set_style_pad_left(xcvr_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(xcvr_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(xcvr_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(xcvr_tab, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

	lv_obj_set_parent(ui_xcvr_tab_panel, xcvr_tab);
	lv_obj_clear_flag(ui_xcvr_tab_panel, LV_OBJ_FLAG_HIDDEN);

	// END SETTINGS TABVIEW UI INIT

	// START PPM Spinbox
    ppm_spinbox = lv_spinbox_create(ui_ppm_spinbox_panel);
    lv_obj_set_style_text_color(ppm_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ppm_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ppm_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ppm_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ppm_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ppm_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(ppm_spinbox, -100, 100);
    lv_spinbox_set_digit_format(ppm_spinbox, 3, 2);
    lv_spinbox_step_prev(ppm_spinbox);
    lv_obj_set_width(ppm_spinbox, 100);
    lv_obj_center(ppm_spinbox);

    lv_coord_t h = lv_obj_get_height(ppm_spinbox);

	lv_obj_t * btn = lv_btn_create(ui_ppm_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, ppm_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _ppm_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_ppm_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, ppm_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _ppm_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END PPM Spinbox

	// START DPD1 Spinbox
    dpd1_spinbox = lv_spinbox_create(ui_dpd1_spinbox_panel);
    lv_obj_set_style_text_color(dpd1_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(dpd1_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dpd1_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dpd1_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dpd1_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dpd1_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(dpd1_spinbox, 500, 1500);
    lv_spinbox_set_digit_format(dpd1_spinbox, 4, 1);
    lv_spinbox_step_prev(dpd1_spinbox);
    lv_obj_set_width(dpd1_spinbox, 100);
    lv_obj_center(dpd1_spinbox);

    h = lv_obj_get_height(dpd1_spinbox);

	btn = lv_btn_create(ui_dpd1_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, dpd1_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _dpd1_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_dpd1_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, dpd1_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, lv_dpd1_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END DPD1 Spinbox

	// START DPD2 Spinbox
    dpd2_spinbox = lv_spinbox_create(ui_dpd2_spinbox_panel);
    lv_obj_set_style_text_color(dpd2_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(dpd2_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dpd2_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dpd2_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dpd2_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dpd2_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(dpd2_spinbox, -500, 500);
    lv_spinbox_set_digit_format(dpd2_spinbox, 4, 1);
    lv_spinbox_step_prev(dpd2_spinbox);
    lv_obj_set_width(dpd2_spinbox, 100);
    lv_obj_center(dpd2_spinbox);

    h = lv_obj_get_height(dpd2_spinbox);

	btn = lv_btn_create(ui_dpd2_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, dpd2_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _dpd2_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_dpd2_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, dpd2_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _dpd2_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END DPD2 Spinbox

	// START DPD3 Spinbox
    dpd3_spinbox = lv_spinbox_create(ui_dpd3_spinbox_panel);
    lv_obj_set_style_text_color(dpd3_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(dpd3_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(dpd3_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dpd3_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dpd3_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(dpd3_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(dpd3_spinbox, -500, 500);
    lv_spinbox_set_digit_format(dpd3_spinbox, 4, 1);
    lv_spinbox_step_prev(dpd3_spinbox);
    lv_obj_set_width(dpd3_spinbox, 100);
    lv_obj_center(dpd3_spinbox);

    h = lv_obj_get_height(dpd3_spinbox);

	btn = lv_btn_create(ui_dpd3_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, dpd3_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _dpd3_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_dpd3_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, dpd3_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _dpd3_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END DPD3 Spinbox

	// START Offset_I Spinbox
    offset_i_spinbox = lv_spinbox_create(ui_offset_i_spinbox_panel);
    lv_obj_set_style_text_color(offset_i_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(offset_i_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(offset_i_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(offset_i_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(offset_i_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(offset_i_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(offset_i_spinbox, -100, 100);
    lv_spinbox_set_digit_format(offset_i_spinbox, 3, 0);
    lv_spinbox_step_prev(offset_i_spinbox);
    lv_obj_set_width(offset_i_spinbox, 100);
    lv_obj_center(offset_i_spinbox);

    h = lv_obj_get_height(offset_i_spinbox);

	btn = lv_btn_create(ui_offset_i_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, offset_i_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _offset_i_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_offset_i_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, offset_i_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _offset_i_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END Offset_I Spinbox

	// START Offset_Q Spinbox
    offset_q_spinbox = lv_spinbox_create(ui_offset_q_spinbox_panel);
    lv_obj_set_style_text_color(offset_q_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(offset_q_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(offset_q_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(offset_q_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(offset_q_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(offset_q_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(offset_q_spinbox, -100, 100);
    lv_spinbox_set_digit_format(offset_q_spinbox, 3, 0);
    lv_spinbox_step_prev(offset_q_spinbox);
    lv_obj_set_width(offset_q_spinbox, 100);
    lv_obj_center(offset_q_spinbox);

    h = lv_obj_get_height(offset_q_spinbox);

	btn = lv_btn_create(ui_offset_q_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, offset_q_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _offset_q_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_offset_q_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, offset_q_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _offset_q_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END Offset_Q Spinbox

	// START Balance_I Spinbox
    balance_i_spinbox = lv_spinbox_create(ui_balance_i_spinbox_panel);
    lv_obj_set_style_text_color(balance_i_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(balance_i_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(balance_i_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(balance_i_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(balance_i_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(balance_i_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(balance_i_spinbox, 0, 1000);
    lv_spinbox_set_digit_format(balance_i_spinbox, 4, 1);
    lv_spinbox_step_prev(balance_i_spinbox);
    lv_obj_set_width(balance_i_spinbox, 100);
    lv_obj_center(balance_i_spinbox);

    h = lv_obj_get_height(balance_i_spinbox);

	btn = lv_btn_create(ui_balance_i_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, balance_i_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _balance_i_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_balance_i_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, balance_i_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _balance_i_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END Balance_I Spinbox

	// START Balance_Q Spinbox
    balance_q_spinbox = lv_spinbox_create(ui_balance_q_spinbox_panel);
    lv_obj_set_style_text_color(balance_q_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(balance_q_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(balance_q_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(balance_q_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(balance_q_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(balance_q_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(balance_q_spinbox, 0, 1000);
    lv_spinbox_set_digit_format(balance_q_spinbox, 4, 1);
    lv_spinbox_step_prev(balance_q_spinbox);
    lv_obj_set_width(balance_q_spinbox, 100);
    lv_obj_center(balance_q_spinbox);

    h = lv_obj_get_height(balance_q_spinbox);

	btn = lv_btn_create(ui_balance_q_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, balance_q_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _balance_q_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_balance_q_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, balance_q_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _balance_q_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END Balance_Q Spinbox

	// START TX_PWR Spinbox
    tx_pwr_spinbox = lv_spinbox_create(ui_tx_pwr_spinbox_panel);
    lv_obj_set_style_text_color(tx_pwr_spinbox, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(tx_pwr_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(tx_pwr_spinbox, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(tx_pwr_spinbox, lv_color_hex(0x464B55), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tx_pwr_spinbox, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tx_pwr_spinbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_spinbox_set_range(tx_pwr_spinbox, 0, 31);
    lv_spinbox_set_digit_format(tx_pwr_spinbox, 2, 0);
    lv_spinbox_step_prev(tx_pwr_spinbox);
    lv_obj_set_width(tx_pwr_spinbox, 100);
    lv_obj_center(tx_pwr_spinbox);

	btn = lv_btn_create(ui_tx_pwr_spinbox_panel);
    lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, tx_pwr_spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_PLUS, 0);
	lv_obj_add_event_cb(btn, _tx_pwr_spinbox_inc_event_cb, LV_EVENT_ALL,  NULL);

	btn = lv_btn_create(ui_tx_pwr_spinbox_panel);
	lv_obj_set_size(btn, h, h);
	lv_obj_align_to(btn, tx_pwr_spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
	lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_MINUS, 0);
	lv_obj_add_event_cb(btn, _tx_pwr_spinbox_dec_event_cb, LV_EVENT_ALL, NULL);
	// END TX_PWR Spinbox

	_get_xcvr_settings(false);
	_init_done = true;
}

void on_xcvr_settings_reset_clicked(lv_event_t *e)
{
	_get_xcvr_settings(true);
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

	_save_xcvr_settings();

	// write to EEEPROM
	radio_settings_save();
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
