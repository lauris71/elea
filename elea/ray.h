#ifndef __ELEA_RAY_H__
#define __ELEA_RAY_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

typedef struct _EleaRay3f EleaRay3f;

#include <math.h>
#include <string.h>

#include <elea/types.h>
#include <elea/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

extern EleaRay3f EleaRay3fX;
extern EleaRay3f EleaRay3fY;
extern EleaRay3f EleaRay3fZ;

struct _EleaRay3f {
	union {
		struct {
			EleaVec3f p;
			EleaVec3f d;
		};
		float c[6];
	};
};

ARIKKEI_INLINE EleaRay3f *
elea_ray3f_set_xyz (EleaRay3f *dst, float px, float py, float pz, float dx, float dy, float dz)
{
	dst->p.x = px;
	dst->p.y = py;
	dst->p.z = pz;
	dst->d.x = dx;
	dst->d.y = dy;
	dst->d.z = dz;
	return dst;
}

ARIKKEI_INLINE const EleaRay3f *
elea_ray3f_set_pd (EleaRay3f *dst, const EleaVec3f *p, const EleaVec3f *d)
{
	memcpy (&dst->p, p, 12);
	memcpy (&dst->d, d, 12);
	return dst;
}

ARIKKEI_INLINE const EleaRay3f *
elea_ray3f_set (EleaRay3f *dst, const EleaRay3f *src)
{
	memcpy (dst, src, 24);
	return dst;
}

ARIKKEI_INLINE const EleaVec3f *
elea_ray3f_get_point_at (EleaVec3f *dst, const EleaRay3f *l, float t)
{
	dst->x = l->p.x + t * l->d.x;
	dst->y = l->p.y + t * l->d.y;
	dst->z = l->p.z + t * l->d.z;
	return dst;
}

/* fixme: These are misleading names because we treat these rays as infinite lines */
float elea_ray3f_distance_point (const EleaRay3f *l, const EleaVec3f *p);
float elea_ray3f_distance_ray (const EleaRay3f* lhs, const EleaRay3f* rhs);
unsigned int elea_ray3f_get_closest_t (const EleaRay3f* lhs, const EleaRay3f* rhs, float* t_lhs, float* t_rhs);
unsigned int elea_ray3f_get_closest_points (const EleaRay3f* lhs, const EleaRay3f* rhs, EleaVec3f *p_lhs, EleaVec3f *p_rhs);

#ifdef __cplusplus
};
#endif

#endif

