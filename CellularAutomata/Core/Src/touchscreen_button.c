/*
 * button.c
 *
 *  Created on: Sep 29, 2024
 *      Author: Max
 */

#include "fonts.h"
#include "ili9341.h"
#include "touchscreen_button.h"
#include <stdbool.h>
#include <string.h>

void Button_Draw(Button * button)
{
	if (button == NULL)
	{
		return;
	}

	uint16_t color_text, color_bg;

	if (button->is_pressed)
	{
		color_text = button->color_text_pressed;
		color_bg = button->color_bg_pressed;
	}
	else
	{
		color_text = button->color_text;
		color_bg = button->color_bg;
	}

	lcdFillRect(button->x, button->y, button->width, button->height, color_bg);
	lcdDrawRect(button->x, button->y, button->width, button->height, color_text);
	lcdDrawLine(button->x + button->width + 1, button->y,
				button->x + button->width + 1, button->y + button->height,
				color_text);
	lcdDrawLine(button->x,                 button->y + button->height + 1,
			    button->x + button->width, button->y + button->height + 1,
				color_text);

	uint16_t text_length_x = button->font->Width * strlen(button->text);
	uint16_t text_length_y = button->font->Height;

	uint16_t center_x = button->x + button->width / 2;
	uint16_t center_y = button->y + button->height / 2;

	lcdSetCursor(center_x - text_length_x / 2, center_y - text_length_y / 2);
	lcdSetTextColor(color_text, color_bg);
	lcdSetTextFont(button->font);
	lcdPrintf(button->text);
}

bool Button_IsPressed(Button * button, uint16_t x, uint16_t y)
{
	if (button == NULL)
	{
		false;
	}

	return button->x < x && x < button->x + button->width
	    && button->y < y && y < button->y + button->height;
}

void Button_SetState(Button * button, bool is_pressed)
{
	if (button == NULL)
	{
		return;
	}

	button->is_pressed = is_pressed;
}

void Button_SetText(Button * button, char text[])
{
	if (button == NULL)
	{
		return;
	}

	strcpy(button->text, text);
}
