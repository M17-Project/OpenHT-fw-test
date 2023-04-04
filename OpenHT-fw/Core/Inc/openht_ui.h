// LVGL version: 8.3.4
// Project name: OpenHT_UI

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OPENHT_UI_H
#define __OPENHT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32469i_discovery.h"
#include "stm32f4xx_hal.h"
#include <lvgl.h>

void custom_ui_init(void);

void button_matrix_event_cb(lv_event_t * e);

uint32_t get_freq_from_str(const char * str);
void get_str_from_freq(uint32_t i, char b[]);


#ifdef __cplusplus
}
#endif

#endif /* __OPENHT_UI_H */
