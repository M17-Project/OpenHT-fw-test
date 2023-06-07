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

#ifndef __OPENHT_UI_H
#define __OPENHT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32469i_discovery.h"
#include "stm32f4xx_hal.h"
#include <lvgl.h>
#include "user_settings.h"

#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

#define EMPTY_FREQ "_.___.___.___"
#define END_POS 13
#define THOU_POS 9
#define MEG_POS 5
#define GIG_POS 1

extern char callsign_str[10];
extern settings_t user_settings;
extern char * callsign_prefix;
extern char * mode_prefix;
extern char * ctcss_options_str;

void custom_ui_init(void);

void numpad_btnmatrix_event_cb(lv_event_t *e);
void qwertypad_btnmatrix_event_cb(lv_event_t *e);
freq_t get_freq_from_str(const char *str);
void get_str_from_freq(uint32_t i, char b[], int prepend_blank);
void update_callsign();
bool validate_freq(uint32_t *freq);

void on_userbutton_press(void);
void on_userbutton_release(void);

#ifdef __cplusplus
}
#endif

#endif /* __OPENHT_UI_H */
