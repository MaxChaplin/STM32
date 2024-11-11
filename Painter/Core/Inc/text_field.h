/*
 * text_field.h
 *
 *  Created on: Oct 1, 2024
 *      Author: Max
 */

#ifndef INC_TEXT_FIELD_H_
#define INC_TEXT_FIELD_H_

#include "fonts.h"

typedef enum TextAlignment {
	ALIGN_BEGIN,
	ALIGN_CENTER,
	ALIGN_END,
} TextAlignment;

typedef struct TextField_ {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;

	sFONT * font;

	uint16_t color_text;
	uint16_t color_bg;
	uint16_t color_frame;

	TextAlignment horizontal_alignment;
	TextAlignment vertical_alignment;

	char text[256];
} TextField;

void TextField_Draw(TextField * field);

void TextField_SetText(TextField * field, char text[]);

#endif /* INC_TEXT_FIELD_H_ */
