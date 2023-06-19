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

#include "ui/ui_mode_change_panel.h"

#include "ui/openht_ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

void init_mode_change_panel(void)
{
	char labels[128];
	char *label = labels;

	for (int i = 0; i < openht_mode_count; i++) {
		if (i > 0) {
			*label++ = '\n';
		}
		int mode_str_len = strlen(openht_mode_data[i].mode_name);
		strlcpy(label, openht_mode_data[i].mode_name, sizeof(labels) - (label - labels));

		label += mode_str_len;
	}

    lv_roller_set_options(ui_mode_roller, labels, LV_ROLLER_MODE_NORMAL);

	lv_roller_set_selected(ui_mode_roller, radio_settings_get_mode(), LV_ANIM_OFF);
}

void on_mode_clicked(lv_event_t *e)
{
	lv_obj_clear_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

}

void on_mode_cancel_clicked(lv_event_t *e)
{
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);
}

void on_mode_ok_clicked(lv_event_t *e)
{
	lv_obj_add_flag(ui_mode_change_panel, LV_OBJ_FLAG_HIDDEN);
	lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_CLICKABLE);

	char roller_str[15];

	lv_roller_get_selected_str(ui_mode_roller, roller_str, sizeof(roller_str));
	lv_label_set_text_fmt(ui_header_mode_label, "%s%s", mode_prefix, roller_str);

	radio_settings_set_mode(lv_roller_get_selected(ui_mode_roller));
	radio_settings_save();
}
