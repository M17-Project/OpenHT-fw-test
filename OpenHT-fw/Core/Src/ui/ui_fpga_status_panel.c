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

static lv_coord_t height;
static lv_coord_t width;
static lv_obj_t * led1 = NULL;
static lv_timer_t * timer;
static char * fpga_status_str = NULL;
lv_obj_t * mbox1;

static void _status_timer(lv_timer_t * timer)
{
	uint32_t * user_data = timer->user_data;
	*user_data += 1;

	if (*user_data % 3 == 0) {
		*user_data = 0;
		//lv_led_set_color(led1, lv_color_hex(0x000000)); //lv_palette_main(LV_PALETTE_RED));
		//lv_led_off(led1);
		lv_led_set_brightness(led1, LV_LED_BRIGHT_MAX - 50);
	} else {
		//lv_led_set_color(led1, lv_palette_main(LV_PALETTE_GREEN));
		//lv_led_on(led1);
		lv_led_set_brightness(led1, LV_LED_BRIGHT_MAX);
	}
}

void init_fpga_status_panel()
{
	// Status: openht_fpga_status_t
	// FPGA Off:		Red LED Off (no flash)
	// FPGA Error/on:	Red LED Flashing
	// FPGA Loading/on: Orange LED Flashing
	// FPGA Running/on: Green LED On (no flash)

//	height = lv_obj_get_style_height(ui_fpga_status_panel, LV_PART_MAIN) * 0.6;
//	width = lv_obj_get_style_width(ui_fpga_status_panel, LV_PART_MAIN) * 0.6;

    led1  = lv_led_create(ui_fpga_status_panel);
    lv_obj_align(led1, LV_ALIGN_CENTER, 0, 0);
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_RED));
//    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_GREEN));
//    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_ORANGE));
    lv_led_off(led1);
//    lv_led_set_brightness(led1, 100);
//    lv_led_on(led1);

    fpga_status_str = openht_get_fpga_status_str(FPGA_Offline);
    static uint32_t user_data = 0;
    timer = lv_timer_create(_status_timer, 250,  &user_data);
	lv_timer_pause(timer);

    /*
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.radius = 14;
    rect_dsc.bg_opa = LV_OPA_COVER;
    rect_dsc.bg_grad.dir = LV_GRAD_DIR_HOR;
    rect_dsc.bg_grad.stops[0].color = lv_palette_main(LV_PALETTE_RED);
    rect_dsc.bg_grad.stops[1].color = lv_palette_main(LV_PALETTE_BLUE);
    rect_dsc.border_width = 2;
    rect_dsc.border_opa = LV_OPA_90;
    rect_dsc.border_color = lv_color_white();
    rect_dsc.shadow_width = 5;
    rect_dsc.shadow_ofs_x = 5;
    rect_dsc.shadow_ofs_y = 5;

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_palette_main(LV_PALETTE_ORANGE);

    static uint8_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(50, 50)];

    lv_obj_t * canvas = lv_canvas_create(ui_fpga_status_panel);
    lv_canvas_set_buffer(canvas, cbuf, width, height, LV_IMG_CF_TRUE_COLOR);
    lv_obj_center(canvas);
    lv_canvas_fill_bg(canvas, lv_color_hex(0x14191E), LV_OPA_COVER);

    //lv_canvas_fill_bg(canvas, lv_palette_main(LV_PALETTE_RED), LV_OPA_TRANSP);

    lv_canvas_draw_rect(canvas, 0, 0, width, height, &rect_dsc);

//    lv_canvas_draw_text(canvas, 40, 20, 100, &label_dsc, "Some text on text canvas");
*/
}

static void _event_cb(lv_event_t * e)
{
    //lv_obj_t * obj = lv_event_get_current_target(e);
    //LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
    lv_msgbox_close(mbox1);
}

void on_fpga_status_clicked(lv_event_t *e)
{
    static const char * btns[] ={"OK", ""};

    mbox1 = lv_msgbox_create(NULL, "FPGA Status", fpga_status_str, btns, false); //true);
    lv_obj_add_event_cb(mbox1, _event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
}

static void _set_offline(void)
{
	lv_timer_pause(timer);
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_RED));
    lv_led_off(led1);
}

static void _set_error(void)
{
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_RED));
    lv_led_on(led1);
	lv_timer_resume(timer);
}

static void _set_config(void)
{
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_ORANGE));
    lv_led_on(led1);
	lv_timer_resume(timer);
}

static void _set_running(void)
{
	lv_timer_pause(timer);
    lv_led_set_color(led1, lv_palette_main(LV_PALETTE_GREEN));
    lv_led_on(led1);
}

void set_fpga_status(openht_fpga_status_t status)
{
    fpga_status_str = openht_get_fpga_status_str(status);

	switch(status){
	    case FPGA_Offline: _set_offline(); break;
	    case FPGA_Error: _set_error(); break;
	    case FPGA_Loading: _set_config(); break;
	    case FPGA_Running: _set_running(); break;
	    default: _set_offline(); break;
	}
}


