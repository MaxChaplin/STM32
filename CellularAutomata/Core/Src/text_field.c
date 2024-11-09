/*
 * text_field.c
 *
 *  Created on: Oct 1, 2024
 *      Author: Max
 */

#include "fonts.h"
#include "ili9341.h"
#include "text_field.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

uint16_t GetCursorPos(uint16_t field_pos, uint16_t field_size, uint16_t text_size, TextAlignment alignment)
{
	if (alignment == ALIGN_BEGIN)
	{
		return field_pos;
	}
	else if (alignment == ALIGN_CENTER)
	{
		return field_pos + (field_size - text_size) / 2;
	}

	return field_pos + field_size;
}

uint16_t NumOfLines(char text[])
{
	uint16_t num_of_lines = 1;
	uint16_t text_len = strlen(text);

	for (size_t i = 0; i < text_len; ++i)
	{
		if (text[i] == '\n')
		{
			++num_of_lines;
		}
	}

	return num_of_lines;
}

uint16_t LongestLine(char text[])
{
	uint16_t line_len_max = 0;
	uint16_t line_len = 0;
	uint16_t text_len = strlen(text);

	for (size_t i = 0; i < text_len; ++i)
	{
		if (text[i] == '\n')
		{
			line_len = 0;
		}
		else if (isprint((int)text[i])) /* Consider \t */
		{
			++line_len;

			if (line_len > line_len_max)
			{
				line_len_max = line_len;
			}
		}
	}

	return line_len_max;
}

void TextField_Draw(TextField * field)
{
	if (field == NULL)
	{
		return;
	}

	lcdFillRect(field->x, field->y, field->width, field->height, field->color_bg);
	lcdDrawRect(field->x, field->y, field->width, field->height, field->color_frame);

	uint16_t text_width = LongestLine(field->text) * field->font->Width;
	uint16_t text_height = NumOfLines(field->text) * field->font->Height;

//	uint16_t center_x = field->x + field->width / 2;
//	uint16_t center_y = field->y + field->height / 2;

//	lcdSetCursor(center_x - text_width / 2, center_y - text_height / 2);
	lcdSetCursor(GetCursorPos(field->x, field->width,  text_width,  field->horizontal_alignment),
				 GetCursorPos(field->y, field->height, text_height, field->vertical_alignment)
				 );
	lcdSetTextColor(field->color_text, field->color_bg);
	lcdSetTextFont(field->font);
	lcdPrintf(field->text);
}

void TextField_SetText(TextField * field, char text[])
{
	if (field == NULL)
	{
		return;
	}

	strcpy(field->text, text);

}
