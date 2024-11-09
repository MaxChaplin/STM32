/*
 * wire33d.c
 *
 *  Created on: Aug 31, 2024
 *      Author: Max
 */

#include "wire3.h"
#include <stdint.h>
#include <stdlib.h>
#include <vector3.h>

Wire3 * Wire3_Create(uint16_t num_vertices, uint16_t num_edges)
{
	Wire3 * wire3 = malloc(sizeof(Wire3));

	if (wire3 == NULL)
	{
		return NULL;
	}

	wire3->vertices = malloc(sizeof(Vec3) * num_vertices);

	if (wire3->vertices == NULL)
	{
		return NULL;
	}

	wire3->edges = malloc(sizeof(Side) * num_edges);

	if (wire3->edges == NULL)
	{
		return NULL;
	}

	wire3->num_vertices = num_vertices;
	wire3->num_edges = num_edges;

	return wire3;
}

Wire3 * Wire3_MakeCube(double side)
{
	double halfside = side / 2;

	Wire3 * wire3 = Wire3_Create(8, 12);
	if (wire3 == NULL)
	{
		return NULL;
	}

	Vec3 pts[] = {{.x =  halfside, .y =  halfside, .z =  halfside},
				  {.x =  halfside, .y =  halfside, .z = -halfside},
				  {.x =  halfside, .y = -halfside, .z =  halfside},
				  {.x =  halfside, .y = -halfside, .z = -halfside},
				  {.x = -halfside, .y =  halfside, .z =  halfside},
				  {.x = -halfside, .y =  halfside, .z = -halfside},
				  {.x = -halfside, .y = -halfside, .z =  halfside},
				  {.x = -halfside, .y = -halfside, .z = -halfside}
				 };

	Side sides[] = {{0, 1}, {0, 2}, {0, 4}, {3, 1},
					{3, 2}, {3, 7}, {5, 1}, {5, 4},
					{5, 7}, {6, 2}, {6, 4}, {6, 7}};

	for (int i = 0; i < 8; ++i)
	{
		wire3->vertices[i] = pts[i];
	}

	for (int i = 0; i < 12; ++i)
	{
		wire3->edges[i] = sides[i];
	}

	return wire3;
}

Wire3 * Wire3_MakeOctahedron(double size)
{
	double halfsize = size / 2;
	Wire3 * wire3 = Wire3_Create(6, 12);
	if (wire3 == NULL)
	{
		return NULL;
	}

	Vec3 pts[] = {{.x =  halfsize, .y = 0, .z = 0},
				  {.x = -halfsize, .y = 0, .z = 0},
				  {.x = 0, .y =  halfsize, .z = 0},
				  {.x = 0, .y = -halfsize, .z = 0},
				  {.x = 0, .y = 0, .z =  halfsize},
				  {.x = 0, .y = 0, .z = -halfsize},
				 };

	Side sides[] = {{0, 2}, {0, 3}, {0, 4}, {0, 5},
					{1, 2}, {1, 3}, {1, 4}, {1, 5},
					{2, 4}, {4, 3}, {3, 5}, {5, 2}};

	for (int i = 0; i < 6; ++i)
	{
		wire3->vertices[i] = pts[i];
	}

	for (int i = 0; i < 12; ++i)
	{
		wire3->edges[i] = sides[i];
	}

	return wire3;
}

void Wire3_Move(Wire3 * wire3, const Vec3 * vec)
{
	if (wire3 == NULL)
	{
		return;
	}

	for(uint16_t i = 0; i < wire3->num_vertices; ++i)
	{
		wire3->vertices[i] = Sum_Vec3(wire3->vertices + i, vec);
	}
}

void Wire3_Transform(Wire3 * wire3, const Trans3 * trans)
{
	if (wire3 == NULL)
	{
		return;
	}

	for(uint16_t i = 0; i < wire3->num_vertices; ++i)
	{
		wire3->vertices[i] = Vec3_Transform(&wire3->vertices[i], trans);
	}
}

void Bubble_Transform(Bubble * bubble, const Trans3 * trans)
{
	if (bubble == NULL)
	{
		return;
	}

	bubble->pos = Vec3_Transform(&bubble->pos, trans);
}
