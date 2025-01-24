#define __ELEA_AABOX3_CPP__

/*
* Libelea
*
* Copyright (C) Lauris Kaplinski 2007-2018
*
*/

#include "types.h"
#include "matrix3x4.h"
#include "ray.h"
#include "plane.h"
#include "polyhedron.h"

#include "aabox.h"

const EleaAABox3f EleaAABox3fEmpty = { INFINITY, INFINITY, INFINITY, -INFINITY, -INFINITY, -INFINITY };
const EleaAABox3f EleaAABox3fZero = { 0, 0, 0, 0, 0, 0 };
const EleaAABox3f EleaAABox3fInfinite = { -INFINITY, -INFINITY, -INFINITY, INFINITY, INFINITY, INFINITY };

EleaAABox3f*
elea_aabox3f_grow_fv (EleaAABox3f* dst, const EleaAABox3f* src, const float* p, unsigned int n_points, unsigned int stride_bytes)
{
	if (!n_points && (dst != src)) {
		*dst = *src;
	} else {
		unsigned int i;
		elea_aabox3f_grow_xyz (dst, src, p[0], p[1], p[2]);
		for (i = 1; i < n_points; i++) {
			float* v = ( float*) (( char*) p + i * stride_bytes);
			elea_aabox3f_grow_xyz (dst, dst, v[0], v[1], v[2]);
		}
	}
	return dst;
}

EleaAABox3f *
elea_aabox3f_grow_fv_mat (EleaAABox3f* dst, const EleaAABox3f* src, const float* p, unsigned int n_points, unsigned int stride_bytes, const EleaMat3x4f* m)
{
	if (!n_points) {
		if (dst != src) *dst = *src;
	} else {
		unsigned int i;
		elea_aabox3f_grow_xyz (dst, src, m->c[0] * p[0] + m->c[1] * p[1] + m->c[2] * p[2] + m->c[3], m->c[4] * p[0] + m->c[5] * p[1] + m->c[6] * p[2] + m->c[7], m->c[8] * p[0] + m->c[9] * p[1] + m->c[10] * p[2] + m->c[11]);
		for (i = 1; i < n_points; i++) {
			float* v = ( float*) (( char*) p + i * stride_bytes);
			elea_aabox3f_grow_xyz (dst, dst, m->c[0] * v[0] + m->c[1] * v[1] + m->c[2] * v[2] + m->c[3], m->c[4] * v[0] + m->c[5] * v[1] + m->c[6] * v[2] + m->c[7], m->c[8] * v[0] + m->c[9] * v[1] + m->c[10] * v[2] + m->c[11]);
		}
	}
	return dst;
}

unsigned int
elea_aabox3f_intersect_ray (const EleaAABox3f* box, const EleaRay3f* ray, float* p0, float* p1)
{
	if ((box->c[0] > box->c[3]) || (box->c[1] > box->c[4]) || (box->c[2] > box->c[5])) return 0;
	*p0 = -INFINITY;
	*p1 = INFINITY;
	for (unsigned int i = 0; i < 3; i++) {
		if (ray->d.c[i]) {
			float t0 = (box->c[i] - ray->p.c[i]) / ray->d.c[i];
			float t1 = (box->c[3 + i] - ray->p.c[i]) / ray->d.c[i];
			float tmax = (t0 >= t1) ? t0 : t1;
			float tmin = (t0 <= t1) ? t0 : t1;
			if (tmin > *p0) *p0 = tmin;
			if (tmax < *p1) *p1 = tmax;
		} else {
			if ((ray->p.c[i] < box->c[i]) || (ray->p.c[i] > box->c[3 + i])) {
				/* Completely outside */
				*p0 = INFINITY;
				*p1 = -INFINITY;
				return 0;
			}
		}
	}
	return *p0 <= *p1;
}

unsigned int
elea_polyhedron3f_test_aabox (const EleaPolyhedron3f* poly, const EleaAABox3f* box)
{
	unsigned int i, j;
	if (elea_aabox3f_is_empty (box)) return ELEA_POSITION_OUT;
	EleaVec3f p[2];
	p[0] = box->min;
	p[1] = box->max;
	int totalin = 0;
	for (i = 0; i < poly->n; i++) {
		int incount = 8;
		int ptin = 1;
		for (j = 0; j < 8; j++) {
			float pos = elea_plane3f_get_position_xyz (&poly->p[i], p[j & 1].x, p[(j & 2) >> 1].y, p[j >> 2].z);
			if (pos > 0) {
				// We are above specific plane
				incount -= 1;
				ptin = 0;
			}
		}
		if (!incount) return ELEA_POSITION_OUT;
		totalin += ptin;
	}
	if (totalin == 6) return ELEA_POSITION_IN;
	return ELEA_POSITION_INTERSECT;
}
