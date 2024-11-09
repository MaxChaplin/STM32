/*
 * wireframe_render_.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Max
 */

#include "wireframe_render_ili9341.h"
#include "wire2.h"
#include "wire3.h"
#include "ili9341.h"

typedef struct Pix_ {
	uint16_t x;
	uint16_t y;
} Pix;

Pix VecToPix(Vec2 * vec, DisplaySettings * ds)
{
	Pix pix = {ds->center_x + vec->x * ds->res_x,
			   ds->center_y - vec->y * ds->res_x};
	return pix;
}

Vec2 ProjectPoint(const Vec3 * vec3, double fov)
{
	double perspective_factor = 1.0 / (vec3->z * fov);
	Vec2 vec2 = {vec3->x * perspective_factor, vec3->y * perspective_factor};

	return vec2;
}

//void Wireframe_RenderSegment2(const Segment2 * l, uint16_t color,
//						      uint16_t display_size, Pix2 center)
//{
//	lcdDrawLine(center.x + l->p1.x * display_size,
//				center.y - l->p1.y * display_size,
//				center.x + l->p2.x * display_size,
//				center.y - l->p2.y * display_size,
// 				color);
//}

void RenderSide(Vec2 * vec1, Vec2 * vec2, uint16_t color, DisplaySettings * ds)
{
	Pix p1 = VecToPix(vec1, ds);
	Pix p2 = VecToPix(vec2, ds);

	lcdDrawLine(p1.x, p1.y, p2.x, p2.y, color);
}

void RenderWire2(const Wire2 * wire2, uint16_t color, DisplaySettings * ds)
{
	if (wire2 == NULL)
	{
		return;
	}

	for (uint16_t i = 0; i < wire2->num_edges; ++i)
	{
		Vec2 * p1 = &wire2->vertices[wire2->edges[i].first];
		Vec2 * p2 = &wire2->vertices[wire2->edges[i].second];
		RenderSide(p1, p2, color, ds);
	}
}

void RenderWire3(const Wire3 * wire3, uint16_t color, DisplaySettings * ds)
{
	if (wire3 == NULL)
	{
		return;
	}

	for (uint16_t i = 0; i < wire3->num_edges; ++i)
	{
		Vec3 * p1 = &wire3->vertices[wire3->edges[i].first];
		Vec3 * p2 = &wire3->vertices[wire3->edges[i].second];

		Vec2 p1_proj = ProjectPoint(p1, ds->fov);
		Vec2 p2_proj = ProjectPoint(p2, ds->fov);

		RenderSide(&p1_proj, &p2_proj, color, ds);
	}
}

void Render_Point(const Vec2 * p, uint16_t color, DisplaySettings * ds)
{
	lcdDrawPixel(ds->center_x + p->x * ds->res_x,
			     ds->center_y + p->y * ds->res_x,
				 color);
}

void RenderBubble(Bubble * bubble, uint16_t color, DisplaySettings * ds)
{
	if (bubble == NULL)
	{
		return;
	}

	double perspective_factor = 1.0 / (bubble->pos.z * ds->fov);
	Vec2 center = {bubble->pos.x * perspective_factor,
				   bubble->pos.y * perspective_factor};

	Pix p_center = VecToPix(&center, ds);

	lcdDrawCircle(p_center.x, p_center.y,
				  bubble->r * perspective_factor * ds->res_x,
				  color);
}
