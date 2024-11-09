/*
 * polygon.h
 *
 *  Created on: Sep 4, 2024
 *      Author: Max
 */

#ifndef INC_SOLID_H_
#define INC_SOLID_H_

#include "vector3.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct Polygon_ {
	uint16_t p1;
	uint16_t p2;
	uint16_t p3;
} Polygon;

typedef struct Solid_ {
	Vec3 * vertices;
	Polygon * polygons;
	uint16_t num_vertices;
	uint16_t num_polygons;
} Solid;

uint8_t Polygon_IsFrontface(Vec3 * p1, Vec3 * p2, Vec3 * p3);

Solid * Solid_Create(uint16_t num_vertices, uint16_t num_polygons);

Solid * Solid_CreateTetrahedron(uint16_t side);

#endif /* INC_SOLID_H_ */
