/*
 * shape2.h
 *
 *  Created on: Aug 30, 2024
 *      Author: Max
 */

#ifndef INC_WIRE2_H_
#define INC_WIRE2_H_

#include <stdint.h>
#include <vector2.h>

typedef struct Side_ {
	uint16_t first;
	uint16_t second;
} Side;

typedef struct Wire2_ {
	Vec2 * vertices;
	Side * edges;
	uint16_t num_vertices;
	uint16_t num_edges;
} Wire2;

Wire2 * Wire2_Create(uint16_t num_vertices, uint16_t num_edges);

Wire2 * Wire2_MakeSquare(Vec2 pos, double side);

void Wire2_Transform(Wire2 * wire2, const Trans2 * trans);

#endif /* INC_WIRE2_H_ */
