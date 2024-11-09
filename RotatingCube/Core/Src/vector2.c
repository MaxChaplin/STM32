/*
 * vector2d.c
 *
 *  Created on: Aug 11, 2024
 *      Author: Max
 */

#include <math.h>
#include <vector2.h>

Mat22 mat22_i = {.xx = 1, .xy = 0, .yx = 0, .yy = 1};

/* ------------------------------ Operations ------------------------------ */

Vec2 Sum_Vec2(const Vec2 * v1, const Vec2 * v2)
{
	Vec2 sum = {.x = v1->x + v2->x, .y = v1->y + v2->y};
	return sum;
}

Mat22 Sum_Mat22(const Mat22 * m1, const Mat22 * m2)
{
	Mat22 sum = {.xx = m1->xx + m2->xx, .xy = m1->xy + m2->xy,
				 .yx = m1->yx + m2->yx, .yy = m1->yy + m2->yy};
	return sum;
}

Mat22 Diff_Mat22(const Mat22 * m1, const Mat22 * m2)
{
	Mat22 sum = {.xx = m1->xx - m2->xx, .xy = m1->xy - m2->xy,
				 .yx = m1->yx - m2->yx, .yy = m1->yy - m2->yy};
	return sum;
}

double Dot_Vec2_Vec2(const Vec2 * v1, const Vec2 * v2)
{
	return v1->x*v2->x + v1->y*v2->y;
}

Vec2 Dot_Mat22_Vec2(const Mat22 * m, const Vec2 * v)
{
	Vec2 product = {.x = m->xx * v->x + m->xy * v->y,
					.y = m->yx * v->x + m->yy * v->y};
	return product;
}

Mat22 Dot_Mat22_Mat22(const Mat22 * m1, const Mat22 * m2)
{
	Mat22 product = {.xx = m1->xx * m2->xx + m1->yx * m2->xy,
					 .xy = m1->xy * m2->xx + m1->yy * m2->xy,
					 .yx = m1->xx * m2->yx + m1->yx * m2->yy,
					 .yy = m1->xy * m2->yx + m1->yy * m2->yy};
	return product;
}

/* -------------------------- Rotation matrix --------------------------- */

Mat22 Mat22_Rot(double angle)
{
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	Mat22 mat = {.xx =  cos_angle, .xy =  sin_angle,
				 .yx = -sin_angle, .yy =  cos_angle};
	return mat;
}

/* --------------------------- Transformations ---------------------------- */

Vec2 Vec2_Transform(const Vec2 * v, const Trans2 * t)
{
	Vec2 prod = Dot_Mat22_Vec2(&t->a, v);

	return Sum_Vec2(&prod, &t->b);
}

Trans2 Trans2_Rot(double angle, const Vec2 * pivot)
{
	Mat22 rot = Mat22_Rot(angle);
	Mat22 comp = Diff_Mat22(&mat22_i, &rot);
	Trans2 trans = {.a = rot, .b = Dot_Mat22_Vec2(&comp, pivot)};

	return trans;
}
