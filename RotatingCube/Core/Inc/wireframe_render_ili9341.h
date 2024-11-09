/*
 * wireframe_render_ili9341.h
 *
 *  Created on: Aug 30, 2024
 *      Author: Max
 */

#ifndef INC_WIREFRAME_RENDER_ILI9341_H_
#define INC_WIREFRAME_RENDER_ILI9341_H_

#include "vector2.h"
#include "vector3.h"
#include "wire2.h"
#include "wire3.h"

typedef struct DisplaySettings_ {
	uint16_t res_x;
	uint16_t res_y;
	uint16_t center_x;
	uint16_t center_y;
    double fov;
} DisplaySettings;

Vec2 ProjectPoint(const Vec3 * vec3, double fov);

void RenderPoint(const Vec2 * vec, uint16_t color, DisplaySettings * ds);

void RenderWire2(const Wire2 * shape, uint16_t color, DisplaySettings * ds);

void RenderWire3(const Wire3 * wire3, uint16_t color, DisplaySettings * ds);

void RenderBubble(Bubble * bubble, uint16_t color, DisplaySettings * ds);

#endif /* INC_WIREFRAME_RENDER_ILI9341_H_ */
