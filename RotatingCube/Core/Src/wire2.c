/*
 * wire22.c
 *
 *  Created on: Aug 30, 2024
 *      Author: Max
 */

#include "wire2.h"
#include <stdint.h>
#include <stdlib.h>
#include <vector2.h>

Wire2 * Wire2_Create(uint16_t num_vertices, uint16_t num_edges)
{
	Wire2 * wire2 = malloc(sizeof(Wire2));

	if (wire2 == NULL)
	{
		return NULL;
	}

	wire2->vertices = malloc(sizeof(Vec2) * num_vertices);

	if (wire2->vertices == NULL)
	{
		return NULL;
	}

	wire2->edges = malloc(sizeof(Side) * num_edges);

	if (wire2->edges == NULL)
	{
		return NULL;
	}

	wire2->num_vertices = num_vertices;
	wire2->num_edges = num_edges;

	return wire2;
}

Wire2 * Wire2_MakeSquare(Vec2 pos, double side)
{
	double halfside = side / 2;

	Wire2 * wire2 = Wire2_Create(4, 4);
	if (wire2 == NULL)
	{
		return NULL;
	}

	Vec2 pts[] = {{pos.x + halfside, pos.y + halfside},
			      {pos.x + halfside, pos.y - halfside},
				  {pos.x - halfside, pos.y + halfside},
				  {pos.x - halfside, pos.y - halfside},
				 };

	Side sides[] = {{0, 1}, {0, 2}, {1, 3}, {2, 3}};

	for (int i = 0; i < 4; ++i)
	{
		wire2->vertices[i] = pts[i];
		wire2->edges[i] = sides[i];
	}

	return wire2;
}

void Wire2_Transform(Wire2 * wire2, const Trans2 * trans)
{
	if (wire2 == NULL)
	{
		return;
	}

	for(uint16_t i = 0; i < wire2->num_vertices; ++i)
	{
		wire2->vertices[i] = Vec2_Transform(&wire2->vertices[i], trans);
	}
}
