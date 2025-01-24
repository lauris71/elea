#define __ELEA_PLANE_CPP__

//
// Libelea
//
// Copyright (C) Lauris Kaplinski 2007
//

#include "matrix3x4.h"
#include "ray.h"

#include "plane.h"

EleaPlane3f EleaPlane3fXY = { 0, 0, 1, 0 };
EleaPlane3f EleaPlane3fYX = { 0, 0, -1, 0 };
EleaPlane3f EleaPlane3fYZ = { 1, 0, 0, 0 };
EleaPlane3f EleaPlane3fZY = { -1, 0, 0, 0 };
EleaPlane3f EleaPlane3fZX = { 0, 1, 0, 0 };
EleaPlane3f EleaPlane3fXZ = { 0, -1, 0, 0 };

EleaPlane3f
elea_plane3f_from_p3 (const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2)
{
	EleaVec3f s01, s02, n;
	s01 = elea_vec3f_sub (*p1, *p0);
	s02 = elea_vec3f_sub (*p2, *p0);
	n = elea_vec3f_cross (s01, s02);
	n = elea_vec3f_normalize(n);
	return (EleaPlane3f) {n.x, n.y, n.z, -elea_vec3f_dot (n, *p0)};
}

EleaPlane3f *
elea_plane3fp_set_p3 (EleaPlane3f *dst, const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2)
{
	EleaVec3f s01, s02;
	s01 = elea_vec3f_sub (*p1, *p0);
	s02 = elea_vec3f_sub (*p2, *p0);
	dst->n = elea_vec3f_cross (s01, s02);
	dst->n = elea_vec3f_normalize(dst->n);
	dst->d = -elea_vec3f_dot (dst->n, *p0);
	return dst;
}
