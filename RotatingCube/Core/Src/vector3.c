/*
 * vector3d.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Max
 */

#include <math.h>
#include <vector2.h>
#include <vector3.h>

Mat33 mat33_i = {.xx = 1, .xy = 0, .xz = 0,
				 .yx = 0, .yy = 1, .yz = 0,
				 .zx = 0, .zy = 0, .zz = 1};

/* ------------------------------ Operations ------------------------------ */

Vec3 Sum_Vec3(const Vec3 * v1, const Vec3 * v2)
{
	Vec3 sum = {.x = v1->x + v2->x, .y = v1->y + v2->y, .z = v1->z + v2->z};
	return sum;
}

Vec3 Diff_Vec3(const Vec3 * v1, const Vec3 * v2)
{
	Vec3 sum = {.x = v1->x - v2->x, .y = v1->y - v2->y, .z = v1->z - v2->z};
	return sum;
}

Mat33 Sum_Mat33(const Mat33 * m1, const Mat33 * m2)
{
	Mat33 sum = {.xx = m1->xx + m2->xx, .xy = m1->xy + m2->xy, .xz = m1->xz + m2->xz,
				 .yx = m1->yx + m2->yx, .yy = m1->yy + m2->yy, .yz = m1->yz + m2->yz,
				 .zx = m1->zx + m2->zx, .zy = m1->zy + m2->zy, .zz = m1->zz + m2->zz
				};
	return sum;
}

Mat33 Diff_Mat33(const Mat33 * m1, const Mat33 * m2)
{
	Mat33 diff = {.xx = m1->xx - m2->xx, .xy = m1->xy - m2->xy, .xz = m1->xz - m2->xz,
				  .yx = m1->yx - m2->yx, .yy = m1->yy - m2->yy, .yz = m1->yz - m2->yz,
				  .zx = m1->zx - m2->zx, .zy = m1->zy - m2->zy, .zz = m1->zz - m2->zz
				 };
	return diff;
}

double Dot_Vec3_Vec3(const Vec3 v1, const Vec3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vec3 Dot_Mat33_Vec3(const Mat33 * m, const Vec3 * v)
{
	Vec3 product = {.x = m->xx*v->x + m->xy*v->y + m->xz*v->z,
					.y = m->yx*v->x + m->yy*v->y + m->yz*v->z,
					.z = m->zx*v->x + m->zy*v->y + m->zz*v->z};
	return product;
}

/* -------------------------- Rotation matrices --------------------------- */

Mat33 Mat33_RotX(double angle)
{
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	Mat33 mat = {.xx = 1, .xy = 0,          .xz = 0,
				 .yx = 0, .yy = cos_angle,  .yz = sin_angle,
				 .zx = 0, .zy = -sin_angle, .zz = cos_angle};

	return mat;
}

Mat33 Mat33_RotY(double angle)
{
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	Mat33 mat = {.xx = cos_angle,  .xy = 0, .xz = sin_angle,
				 .yx = 0,          .yy = 1, .yz = 0,
				 .zx = -sin_angle, .zy = 0, .zz = cos_angle};

	return mat;
}

Mat33 Mat33_RotZ(double angle)
{
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	Mat33 mat = {.xx = cos_angle,  .xy = sin_angle, .xz = 0,
				 .yx = -sin_angle, .yy = cos_angle, .yz = 0,
				 .zx = 0,          .zy = 0,         .zz = 1};

	return mat;
}

/* --------------------------- Transformations ---------------------------- */

Vec3 Vec3_Transform(const Vec3 * v, const Trans3 * t)
{
	Vec3 prod = Dot_Mat33_Vec3(&t->a, v);

	return Sum_Vec3(&prod, &t->b);
}

Trans3 Trans3_Rot(double angle, const Mat33 * mat_rot, const Vec3 * axis_point)
{
	Mat33 comp = Diff_Mat33(&mat33_i, mat_rot);
	Trans3 trans = {.a = *mat_rot, .b = Dot_Mat33_Vec3(&comp, axis_point)};

	return trans;
}

Trans3 Trans3_RotX(double angle, const Vec3 * pivot)
{
	Mat33 mat_rot = Mat33_RotX(angle);
	return Trans3_Rot(angle, &mat_rot, pivot);
}

Trans3 Trans3_RotY(double angle, const Vec3 * pivot)
{
	Mat33 mat_rot = Mat33_RotY(angle);
	return Trans3_Rot(angle, &mat_rot, pivot);
}

Trans3 Trans3_RotZ(double angle, const Vec3 * pivot)
{
	Mat33 mat_rot = Mat33_RotZ(angle);
	return Trans3_Rot(angle, &mat_rot, pivot);
}
