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
#include <ui/openht_ui.h>
#include <ui/ui_fpga_status_panel.h>

static lv_obj_t * status_led = NULL;
static lv_timer_t * status_timer;

static openht_fpga_status_t fpga_status = FPGA_Offline;
static char * fpga_status_str = NULL;
static bool blink = false;
lv_obj_t * mbox1;

static void _set_offline(void)
{
	blink = false;
    lv_led_set_color(status_led, lv_palette_main(LV_PALETTE_RED));
    lv_led_off(status_led);
}

static void _set_error(void)
{
	blink = true;
	lv_led_set_color(status_led, lv_palette_main(LV_PALETTE_RED));
    lv_led_on(status_led);
}

static void _set_config(void)
{
	blink = true;
    lv_led_set_color(status_led, lv_palette_main(LV_PALETTE_ORANGE));
    lv_led_on(status_led);
}

static void _set_running(void)
{
	blink = false;
    lv_led_set_color(status_led, lv_palette_main(LV_PALETTE_GREEN));
    lv_led_on(status_led);
}

static void _status_timer(lv_timer_t * timer)
{
	static openht_fpga_status_t current_fpga_status;
	static uint32_t blink_rate = 0;
	openht_fpga_status_t * system_fpga_status = timer->user_data;

	if (current_fpga_status != *system_fpga_status) {
		current_fpga_status = *system_fpga_status;

		switch(current_fpga_status){
		    case FPGA_Offline: _set_offline(); break;
		    case FPGA_Error: _set_error(); break;
		    case FPGA_Loading: _set_config(); break;
		    case FPGA_Running: _set_running(); break;
		    default: _set_offline(); break;
		}
	}

	if (blink) {
		blink_rate += 1;

		if (blink_rate % 3 == 0) {
			blink_rate = 0;
			lv_led_set_brightness(status_led, LV_LED_BRIGHT_MAX - 50);
		} else {
			lv_led_set_brightness(status_led, LV_LED_BRIGHT_MAX);
		}
	}
}

void init_fpga_status_panel()
{
	// Status: openht_fpga_status_t
	// FPGA Off:		Red LED Off (no flash)
	// FPGA Error/on:	Red LED Flashing
	// FPGA Loading/on: Orange LED Flashing
	// FPGA Running/on: Green LED On (no flash)

    status_led  = lv_led_create(ui_fpga_status_panel);
    lv_obj_align(status_led, LV_ALIGN_CENTER, 0, 0);

    _set_offline();
    fpga_status_str = openht_get_fpga_status_str(FPGA_Offline);

    status_timer = lv_timer_create(_status_timer, 250,  &fpga_status);
}

static void _event_cb(lv_event_t * e)
{
    lv_msgbox_close(mbox1);
}

void on_fpga_status_clicked(lv_event_t *e)
{
    static const char * btns[] ={"OK", ""};

    mbox1 = lv_msgbox_create(NULL, "FPGA Status", fpga_status_str, btns, false); //true);
    lv_obj_add_event_cb(mbox1, _event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
}

// call to set the ui fpga status
void set_fpga_status(openht_fpga_status_t status)
{
	fpga_status = status;
    fpga_status_str = openht_get_fpga_status_str(status);
}


