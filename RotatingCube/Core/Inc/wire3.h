/*
 * solid3d.h
 *
 *  Created on: Aug 31, 2024
 *      Author: Max
 */

#ifndef INC_WIRE3_H_
#define INC_WIRE3_H_

#include <stdint.h>
#include <vector3.h>
#include <wire2.h>
#include <wire3.h>

typedef struct Wire3_ {
	Vec3 * vertices;
	Side * edges;
	uint16_t num_vertices;
	uint16_t num_edges;
} Wire3;

typedef struct Bubble_ {
	Vec3 pos;
	double r;
} Bubble;

Wire3 * Wire3_Create(uint16_t num_vertices, uint16_t num_edges);

Wire3 * Wire3_MakeCube(double side);

Wire3 * Wire3_MakeOctahedron(double side);

void Wire3_Move(Wire3 * wire3, const Vec3 * vec);

void Wire3_Transform(Wire3 * wire3, const Trans3 * trans);

void Bubble_Transform(Bubble * bubble, const Trans3 * trans);

#endif /* INC_WIRE3_H_ */
