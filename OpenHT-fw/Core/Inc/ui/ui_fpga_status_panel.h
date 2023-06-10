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

#ifndef __UI_FPGA_STATUS_PANEL_H
#define __UI_FPGA_STATUS_PANEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>
#include "openht_types.h"

void init_fpga_status_panel(void);
void set_fpga_status(openht_fpga_status_t status);

#ifdef __cplusplus
}
#endif

#endif /* __UI_FPGA_STATUS_PANEL_H */
