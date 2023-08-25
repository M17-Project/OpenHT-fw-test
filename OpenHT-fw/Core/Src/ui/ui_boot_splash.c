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

#include "ui/openht_ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "stm32f4xx_hal_rng.h"

extern RNG_HandleTypeDef hrng;

#define NUM_MSGS 12
static char * silly_boot_status[NUM_MSGS] = {
		"/*       reticulating splines         */",
		"/*  buffering for hyperspace bypass   */",
		"/*         wiggling tracks            */",
		"/*  inquiring about second breakfast  */",
		"/*harmonising crystal frequency chakra*/",
		"/*      tuning sonic screwdriver      */",
		"/*      applying hertz to volts       */",
		"/*      untangling all quantums       */",
		"/*         2.4G hertz a lot           */",
		"/*      multiplying six by nine       */",
		"/*         oh no, not again           */",
		"/*     setting volume to eleven       */",
		};

static char default_status[] =
		"/*            boot status             */";

static lv_obj_t * boot_splash = NULL;
static lv_timer_t * update_status_timer = NULL;
static lv_obj_t * ui_splash_status_label = NULL;

static uint32_t scale_distribution(uint32_t unscaledNum, uint32_t minAllowed, uint32_t maxAllowed, uint32_t min, uint32_t max);

static void _status_timer(lv_timer_t * timer)
{
	// get the next random number
	uint32_t rnd = HAL_RNG_GetRandomNumber(&hrng);

	// scale it down to the array
	uint32_t scaled_rnd = scale_distribution(rnd, 0, NUM_MSGS-1, 0, UINT32_MAX);

    lv_label_set_text(ui_splash_status_label, silly_boot_status[scaled_rnd]);
}

void start_boot_splash()
{
	// create the boot splash screen
	boot_splash = lv_obj_create(NULL);
    lv_obj_clear_flag(boot_splash, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(boot_splash, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(boot_splash, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // create the wiggles image
    lv_obj_t * ui_splash_wiggles_image = lv_img_create(boot_splash);
    lv_img_set_src(ui_splash_wiggles_image, &ui_img_wiggles_png);
    lv_obj_set_width(ui_splash_wiggles_image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_splash_wiggles_image, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_splash_wiggles_image, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_splash_wiggles_image, -300);
    lv_obj_clear_flag(ui_splash_wiggles_image, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // create the "OpenHT" logo text using 48 pt RobotoBold font
    lv_obj_t * ui_splash_openht_label = lv_label_create(boot_splash);
    lv_obj_set_width(ui_splash_openht_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_splash_openht_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_splash_openht_label, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_splash_openht_label, -200);
    lv_label_set_text(ui_splash_openht_label, "OpenHT");
    lv_obj_clear_flag(ui_splash_openht_label, LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags
    lv_obj_set_style_text_color(ui_splash_openht_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_splash_openht_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_splash_openht_label, &ui_font_RobotoBold_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    // create the M17 logo image
    lv_obj_t * ui_splash_image = lv_img_create(boot_splash);
    lv_img_set_src(ui_splash_image, &ui_img_m17_logo_300_w_png);
    lv_obj_set_width(ui_splash_image, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_splash_image, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_splash_image, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_splash_image, +100);
    lv_obj_clear_flag(ui_splash_image, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE);

    // create the silly status text label which will pull random phrases from array
    ui_splash_status_label = lv_label_create(boot_splash);
    lv_obj_set_width(ui_splash_status_label, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_splash_status_label, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_splash_status_label, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(ui_splash_status_label, -20);
    lv_label_set_text(ui_splash_status_label, default_status);
    lv_obj_clear_flag(ui_splash_status_label, LV_OBJ_FLAG_GESTURE_BUBBLE);      /// Flags
    lv_obj_set_style_text_color(ui_splash_status_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_splash_status_label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_splash_status_label, &ui_font_RobotoMonoRegular_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // display the screen
    lv_disp_load_scr(boot_splash);

    // create the timer used to display random status messages
    update_status_timer = lv_timer_create(_status_timer, 500, NULL);
}

void stop_boot_splash()
{
	// delete timer before screen so that we don't call on deleted components
	if (update_status_timer != NULL) {
		lv_timer_del(update_status_timer);
		update_status_timer = NULL;
	}

	// delete the screen
	if (boot_splash != NULL) {
		lv_obj_del(boot_splash);
	}
}

// scale the number distribution
static uint32_t scale_distribution(uint32_t unscaled_num, uint32_t scaled_min, uint32_t scaled_max, uint32_t unscaled_min, uint32_t unscaled_max)
{
	float f = 1.0 * (unscaled_num - unscaled_min) / (unscaled_max - unscaled_min);
	return roundf((scaled_max - scaled_min) * f) + scaled_min;
}

