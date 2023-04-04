// LVGL version: 8.3.4
// Project name: OpenHT_UI

#include "openht_ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <lvgl.h>

#include "lvgl_ui/ui.h"


static lv_obj_t * currentActiveTextAreaFreq = NULL;
static char * currentFreeqStr = NULL;

#define EMPTY_FREQ "___.___.___"
#define END_POS 11

static char rxfreqstr[] = EMPTY_FREQ;
static char txfreqstr[] = EMPTY_FREQ;

static void create_number_pad(lv_obj_t * lv_obj);
static void end_input_text_area(void);
static void update_cursor_pos(lv_obj_t * textAreaFreq);
static void update_active_text_area_freq(lv_obj_t * newTextAreaFreq, char freqstr[]);

void custom_ui_init(void)
{
	// test inputs...
	//uint32_t test = get_freq_from_str("_39.123.___");
	//get_str_from_freq(test, rxfreqstr);
	//get_str_from_freq(0, rxfreqstr);

	// remove the border for the UI placeholder
	// Hint: using border in SquareLine Studio allows visibility while moving
	// things around, then set border to none at runtime...
	lv_obj_set_style_border_side(ui_panel_numpad, LV_BORDER_SIDE_NONE, LV_PART_MAIN);
	create_number_pad(ui_panel_numpad);

	//lv_obj_set_style_anim_time((lv_textarea_t *)ui_text_area_rx_freq, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);
	//lv_obj_set_style_anim_time((lv_textarea_t *)ui_text_area_tx_freq, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);
}

void button_matrix_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt = lv_btnmatrix_get_btn_text(obj, id);

        LV_LOG_USER("%s was pressed\n", txt);

        if (currentActiveTextAreaFreq == NULL)
        	return;

        // for future use...(or not!)
        if ((strcmp(txt, "*") == 0) ||
        	(strcmp(txt, ".") == 0) ||
			(strcmp(txt, "#") == 0))
        	return;

        // get current cursor position
        uint32_t curs_pos = lv_textarea_get_cursor_pos(currentActiveTextAreaFreq);

        // move cursor left
        if(strcmp(txt, LV_SYMBOL_LEFT) == 0) {
        	if (curs_pos == 0) curs_pos = 10;
        	else {
				curs_pos = curs_pos - 1;
				if (curs_pos == 3) curs_pos = curs_pos - 1;
				if (curs_pos == 7) curs_pos = curs_pos - 1;
	        if (curs_pos == END_POS) curs_pos = 0;
        	}
        } // move cursor right
        else if(strcmp(txt, LV_SYMBOL_RIGHT) == 0) {
			curs_pos = curs_pos + 1;
			if (curs_pos == 3) curs_pos = curs_pos + 1;
			if (curs_pos == 7) curs_pos = curs_pos + 1;
	        if (curs_pos == END_POS) curs_pos = 0;
        } // end input
        else if(strcmp(txt, LV_SYMBOL_OK) == 0) {
        	curs_pos = END_POS;
        }
        else {
			if (curs_pos == END_POS) curs_pos = 0;
			currentFreeqStr[curs_pos] = *txt;

			// update the text
			lv_textarea_set_text(currentActiveTextAreaFreq, currentFreeqStr);

			// update cursor position...
			curs_pos = curs_pos + 1;
			if (curs_pos == 3) curs_pos = curs_pos + 1;
			if (curs_pos == 7) curs_pos = curs_pos + 1;
        }

        // update cursor position
        lv_textarea_set_cursor_pos(currentActiveTextAreaFreq, curs_pos);

		if (curs_pos == END_POS){
			end_input_text_area();
		}
    }
}

static void end_input_text_area()
{
	if (currentActiveTextAreaFreq == NULL) {
		return;
	}

    lv_textarea_set_cursor_pos(currentActiveTextAreaFreq, END_POS);
	lv_obj_set_style_border_side(currentActiveTextAreaFreq, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

	//const char * txt = lv_textarea_get_text(currentActiveTextAreaFreq);
	uint32_t freq = get_freq_from_str(currentFreeqStr);
	get_str_from_freq(freq, currentFreeqStr);
	lv_textarea_set_text(currentActiveTextAreaFreq, currentFreeqStr);
	currentFreeqStr = NULL;
	currentActiveTextAreaFreq = NULL;

	/* log output... */
    char buffer[15];
    snprintf(buffer, 15, "%d", get_freq_from_str(rxfreqstr));
    lv_label_set_text(ui_label_test_rx, buffer);
    snprintf(buffer, 15, "%d", get_freq_from_str(txfreqstr));
    lv_label_set_text(ui_label_test_tx, buffer);

}

// array matrix of buttons
static const char * btnm_map[] = {"7", "8", "9", "\n",
								  "4", "5", "6", "\n",
                                  "1", "2", "3", "\n",
                                  //"*", "0", ".", "#", "\n",
								  "0", LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, LV_SYMBOL_OK, ""
                                 };

static void create_number_pad(lv_obj_t * lv_obj)
{
	// create button matrix and assign as parent: lv_obj
    lv_obj_t * btnm1 = lv_btnmatrix_create(lv_obj);
    lv_btnmatrix_set_map(btnm1, btnm_map);
    lv_btnmatrix_set_btn_width(btnm1, 9, 7);
    lv_btnmatrix_set_btn_width(btnm1, 10, 4);
    lv_btnmatrix_set_btn_width(btnm1, 11, 4);
    lv_btnmatrix_set_btn_width(btnm1, 12, 5);

    // get the style width/height of the parent.
    // Using this because actual width and height are not defined yet since not visible
    lv_coord_t w = lv_obj_get_style_width(lv_obj, LV_PART_MAIN);
    lv_coord_t h = lv_obj_get_style_height(lv_obj, LV_PART_MAIN);
    lv_obj_align(btnm1, LV_ALIGN_DEFAULT, 0, 0);
    lv_obj_set_width(btnm1, w);
    lv_obj_set_height(btnm1, h);

    // area between buttons
    lv_obj_set_style_border_side(btnm1, LV_BORDER_SIDE_NONE, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btnm1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btnm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // the buttons in the matrix
    lv_obj_set_style_bg_color(btnm1, lv_color_hex(0x191C26), LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btnm1, lv_color_hex(0x37B9F5), LV_PART_ITEMS | LV_STATE_PRESSED);

    lv_obj_set_style_text_font(btnm1, &lv_font_montserrat_42, LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btnm1, lv_color_hex(0xFFFFFF), LV_PART_ITEMS | LV_STATE_DEFAULT);

    // callback handler
    lv_obj_add_event_cb(btnm1, button_matrix_event_cb, LV_EVENT_ALL, NULL);
}

static void update_active_text_area_freq(lv_obj_t * newTextAreaFreq, char freqstr[])
{
	if (newTextAreaFreq == NULL)
		return;

	end_input_text_area();
	currentActiveTextAreaFreq = newTextAreaFreq;
	currentFreeqStr = freqstr;
	lv_obj_set_style_border_side(currentActiveTextAreaFreq, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
	//lv_obj_set_style_anim_time(currentActiveTextAreaFreq, 0, LV_PART_CURSOR | LV_STATE_DEFAULT);

	update_cursor_pos(currentActiveTextAreaFreq);
}

static void update_cursor_pos(lv_obj_t * textAreaFreq)
{
	uint32_t curs_pos = lv_textarea_get_cursor_pos((lv_obj_t *)textAreaFreq);
    if (curs_pos == 3) curs_pos = curs_pos + 1;
    if (curs_pos == 7) curs_pos = curs_pos + 1;
    if (curs_pos == END_POS) curs_pos = END_POS - 1;
    lv_textarea_set_cursor_pos((lv_obj_t *)textAreaFreq, curs_pos);
}

void on_screen_pressed(lv_event_t * e)
{
	end_input_text_area();
}

void on_rx_click(lv_event_t * e)
{
	if (currentActiveTextAreaFreq == ui_text_area_rx_freq){
		update_cursor_pos(ui_text_area_rx_freq);
		return;
	}
	update_active_text_area_freq(ui_text_area_rx_freq, rxfreqstr);
}

void on_tx_click(lv_event_t * e)
{
	if (currentActiveTextAreaFreq == ui_text_area_tx_freq){
		update_cursor_pos(ui_text_area_tx_freq);
		return;
	}
	update_active_text_area_freq(ui_text_area_tx_freq, txfreqstr);
}

uint32_t get_freq_from_str(const char * str)
{
	uint32_t num = 0;

    // converting string to number
    for (int i = 0; str[i] != '\0'; i++) {
    	int test = str[i] - 48;

    	// if it is a digit 0-9
    	if(test >= 0 && test <= 9){
    		num = num * 10 + test;
    	}

    	// if it is a "_" treat as a 0
    	if(test == 47){
    		num = num * 10;
    	}
    }

	return num;
}

void get_str_from_freq(uint32_t i, char b[])
{
    char const digit[] = "0123456789";
    char* p = b;

    if(i==0){
        strcpy(p, EMPTY_FREQ);
        return;
    }

    //Move to end of str
    int str_i;
    for (str_i = 0; b[str_i] != '\0'; ++str_i);
    p = p + str_i;

    int thousands = 0;
    do{ // add digits backwards, inserting thousands separator
    	if (thousands == 3) {
    		*--p = '.';
    		thousands = 0;
    	} else {
    		if (i){
    			*--p = digit[i%10];
    			i = i/10;
    		}
    		else { // prepend underscores
    			*--p = '_';
    		}
			thousands++;
    	}
    }while(p > b);
}

void on_xmit_button_press(lv_event_t * e)
{
	BSP_LED_On(LED_RED);
}

void on_xmit_button_release(lv_event_t * e)
{
	BSP_LED_Off(LED_RED);
}
