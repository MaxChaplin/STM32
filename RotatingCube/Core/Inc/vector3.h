/*
 * vector3d.h
 *
 *  Created on: Aug 11, 2024
 *      Author: Max
 */

#ifndef INC_VECTOR3_H_
#define INC_VECTOR3_H_

#include <vector2.h>

typedef struct Vec3_ Vec3;

typedef struct Mat33_ Mat33;

struct Vec3_ {
	double x;
	double y;
	double z;
};

struct Mat33_ {
	double xx;
	double xy;
	double xz;
	double yx;
	double yy;
	double yz;
	double zx;
	double zy;
	double zz;
};

typedef struct Trans3_ {
	Mat33 a;
	Vec3 b;
} Trans3;

Vec3 Sum_Vec3(const Vec3 * v1, const Vec3 * v2);

Vec3 Diff_Vec3(const Vec3 * v1, const Vec3 * v2);

double Dot_Vec3_Vec3(const Vec3 v1, const Vec3 v2);

double Vec3_Dot(const Vec3 v1, const Vec3 v2);

Vec3 Dot_Mat33_Vec3(const Mat33 * m, const Vec3 * v);

Vec3 Vec3_Transform(const Vec3 * v, const Trans3 * t);

Trans3 Trans3_Rot(double angle, const Mat33 * mat_rot, const Vec3 * pivot);

Trans3 Trans3_RotX(double angle, const Vec3 * pivot);

Trans3 Trans3_RotY(double angle, const Vec3 * pivot);

Trans3 Trans3_RotZ(double angle, const Vec3 * pivot);

#endif /* INC_VECTOR3_H_ */
