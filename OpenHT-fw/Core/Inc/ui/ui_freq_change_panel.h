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

#ifndef __UI_FREQ_CHANGE_PANEL_H
#define __UI_FREQ_CHANGE_PANEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>

void init_freq_change_panel(void);
bool update_split_mode(void);

#ifdef __cplusplus
}
#endif

#endif /* __UI_FREQ_CHANGE_PANEL_H */
