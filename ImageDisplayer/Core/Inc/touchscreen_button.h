/*
 * XPT2046_button.h
 *
 *  Created on: Sep 29, 2024
 *      Author: Max
 */

#ifndef INC_TOUCHSCREEN_BUTTON_H_
#define INC_TOUCHSCREEN_BUTTON_H_

#include "fonts.h"
#include <stdbool.h>

typedef struct Button_ {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;

	sFONT * font;

	bool is_pressed;

	uint16_t color_text;
	uint16_t color_bg;
	uint16_t color_text_pressed;
	uint16_t color_bg_pressed;

	char text[8];

} Button;

void Button_Draw(Button * button);

bool Button_IsPressed(Button * button, uint16_t x, uint16_t y);

void Button_SetState(Button * button, bool is_pressed);

void Button_SetText(Button * button, char text[]);

#endif /* INC_TOUCHSCREEN_BUTTON_H_ */
