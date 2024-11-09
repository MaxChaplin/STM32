/*
 * vector2d.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Max
 */

#ifndef INC_VECTOR2_H_
#define INC_VECTOR2_H_

typedef struct Vec2_ {
	double x;
	double y;
} Vec2;

typedef struct Mat22_ {
	double xx;
	double xy;
	double yx;
	double yy;
} Mat22;

typedef struct Trans2_ {
	Mat22 a;
	Vec2 b;
} Trans2;

Vec2 Sum_Vec2(const Vec2 * v1, const Vec2 * v2);

double Dot_Vec2_Vec2(const Vec2 * v1, const Vec2 * v2);

Vec2 Dot_Mat22_Vec2(const Mat22 * m, const Vec2 * v);

Trans2 Trans2_Rot(double angle, const Vec2 * pivot);

Vec2 Vec2_Transform(const Vec2 * v, const Trans2 * t);

#endif /* INC_VECTOR2_H_ */
