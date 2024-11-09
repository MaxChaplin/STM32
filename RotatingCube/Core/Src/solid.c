/*
 * polygon.c
 *
 *  Created on: Sep 4, 2024
 *      Author: Max
 */

#include "solid.h"
#include "vector3.h"
#include <stdint.h>
#include <stdlib.h>

uint8_t Polygon_IsFrontface(Vec3 * p1, Vec3 * p2, Vec3 * p3)
{
	Vec3 p12 = Diff_Vec3(p1, p2);
	Vec3 p13 = Diff_Vec3(p1, p3);

	/* Check if the cross product faces toward the viewer. */
	return p12.y * p13.z - p13.y * p12.z < 0;
}

Solid * Solid_Create(uint16_t num_vertices, uint16_t num_polygons)
{
	Solid * solid = malloc(sizeof(Solid));

	if (solid == NULL)
	{
		return NULL;
	}

	solid->vertices = malloc(sizeof(Vec3) * num_vertices);

	if (solid->vertices == NULL)
	{
		return NULL;
	}

	solid->polygons = malloc(sizeof(Polygon) * num_polygons);

	if (solid->polygons == NULL)
	{
		return NULL;
	}

	solid->num_vertices = num_vertices;
	solid->num_polygons = num_polygons;

	return solid;
}
