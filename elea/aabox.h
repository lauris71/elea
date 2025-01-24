#ifndef __ELEA_AABOX3_H__
#define __ELEA_AABOX3_H__

/*
* Libelea
*
* Copyright (C) Lauris Kaplinski 2007-2019
*
*/

typedef struct _EleaAABox3f EleaCuboid3f;
typedef struct _EleaAABox3f EleaAABox3f;

#include <string.h>

#include <arikkei/arikkei-utils.h>
#include <elea/matrix3x4.h>
#include <elea/types.h>
#include <elea/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

const extern EleaAABox3f EleaAABox3fEmpty;
const extern EleaAABox3f EleaAABox3fZero;
const extern EleaAABox3f EleaAABox3fInfinite;

struct _EleaAABox3f {
	union {
		struct {
			float x0, y0, z0, x1, y1, z1;
		};
		struct {
			EleaVec3f min, max;
		};
		float c[6];
	};
};

ELEA_INLINE EleaAABox3f *
elea_aabox3f_set_values (EleaAABox3f *dst, float x0, float y0, float z0, float x1, float y1, float z1)
{
	dst->x0 = x0;
	dst->y0 = y0;
	dst->z0 = z0;
	dst->x1 = x1;
	dst->y1 = y1;
	dst->z1 = z1;
	return dst;
}

ELEA_INLINE EleaAABox3f*
elea_aabox3f_set_minmax (EleaAABox3f* dst, const EleaVec3f *min, const EleaVec3f *max)
{
	memcpy (&dst->min, min, sizeof (EleaVec3f));
	memcpy (&dst->max, max, sizeof (EleaVec3f));
	return dst;
}

ELEA_INLINE EleaAABox3f*
elea_aabox3f_set (EleaAABox3f* dst, const EleaAABox3f *src)
{
	memcpy (dst, src, sizeof (EleaAABox3f));
	return dst;
}

ELEA_INLINE unsigned int
elea_aabox3f_is_empty (const EleaAABox3f *cub)
{
	return (cub->x0 > cub->x1) || (cub->y0 > cub->y1) || (cub->z0 > cub->z1);
}

ELEA_INLINE unsigned int
elea_aabox3f_is_empty_or_zero (const EleaAABox3f *cub)
{
	return (cub->x0 >= cub->x1) || (cub->y0 >= cub->y1) || (cub->z0 >= cub->z1);
}

ELEA_INLINE EleaVec3f *
elea_aabox3f_get_center (EleaVec3f *dst, const EleaAABox3f* cub)
{
	dst->x = (cub->x0 + cub->x1) / 2;
	dst->y = (cub->y0 + cub->y1) / 2;
	dst->z = (cub->z0 + cub->z1) / 2;
	return dst;
}

#define elea_aabox3f_corner_x(cub,i) (cub)->c[3 * (i & 1)]
#define elea_aabox3f_corner_y(cub,i) (cub)->c[3 * ((i & 2) >> 1) + 1]
#define elea_aabox3f_corner_z(cub,i) (cub)->c[3 * ((i & 4) >> 2) + 2]

ELEA_INLINE EleaAABox3f*
elea_aabox3f_grow_xyz (EleaAABox3f* dst, const EleaAABox3f* src, float x, float y, float z)
{
	dst->x0 = fminf (src->x0, x);
	dst->y0 = fminf (src->y0, y);
	dst->z0 = fminf (src->z0, z);
	dst->x1 = fmaxf (src->x1, x);
	dst->y1 = fmaxf (src->y1, y);
	dst->z1 = fmaxf (src->z1, z);
	return dst;
}

ELEA_INLINE EleaAABox3f*
elea_aabox3f_grow_p (EleaAABox3f* dst, const EleaAABox3f* src, const EleaVec3f *p)
{
	dst->x0 = fminf (src->x0, p->x);
	dst->y0 = fminf (src->y0, p->y);
	dst->z0 = fminf (src->z0, p->z);
	dst->x1 = fmaxf (src->x1, p->x);
	dst->y1 = fmaxf (src->y1, p->y);
	dst->z1 = fmaxf (src->z1, p->z);
	return dst;
}

ELEA_INLINE EleaAABox3f *
elea_aabox3f_grow_p_mat (EleaAABox3f *dst, const EleaAABox3f *src, const EleaVec3f *p, const EleaMat3x4f *m)
{
	return elea_aabox3f_grow_xyz (dst, src, m->c[0] * p->x + m->c[1] * p->y + m->c[2] * p->z + m->c[3], m->c[4] * p->x + m->c[5] * p->y + m->c[6] * p->z + m->c[7], m->c[8] * p->x + m->c[9] * p->y + m->c[10] * p->z + m->c[11]);
}

EleaAABox3f *elea_aabox3f_grow_fv (EleaAABox3f *dst, const EleaAABox3f *src, const float *p, unsigned int n_points, unsigned int stride_bytes);
EleaAABox3f* elea_aabox3f_grow_fv_mat (EleaAABox3f* dst, const EleaAABox3f* src, const float* p, unsigned int n_points, unsigned int stride_bytes, const EleaMat3x4f* m);

ELEA_INLINE EleaAABox3f*
elea_aabox3f_intersection (EleaAABox3f *dst, const EleaAABox3f *lhs, const EleaAABox3f *rhs)
{
	dst->x0 = fmaxf (lhs->x0, rhs->x0);
	dst->y0 = fmaxf (lhs->y0, rhs->y0);
	dst->z0 = fmaxf (lhs->z0, rhs->z0);
	dst->x1 = fminf (lhs->x1, rhs->x1);
	dst->y1 = fminf (lhs->y1, rhs->y1);
	dst->z1 = fminf (lhs->z1, rhs->z1);
	return dst;
}

ELEA_INLINE EleaAABox3f*
elea_aabox3f_union (EleaAABox3f *dst, const EleaAABox3f *lhs, const EleaAABox3f *rhs)
{
	dst->x0 = fminf (lhs->x0, rhs->x0);
	dst->y0 = fminf (lhs->y0, rhs->y0);
	dst->z0 = fminf (lhs->z0, rhs->z0);
	dst->x1 = fmaxf (lhs->x1, rhs->x1);
	dst->y1 = fmaxf (lhs->y1, rhs->y1);
	dst->z1 = fmaxf (lhs->z1, rhs->z1);
	return dst;
}

ELEA_INLINE float
elea_aabox3f_get_distance_xyz (const EleaAABox3f* cub, float x, float y, float z)
{
	float dx = fmaxf (fmaxf (cub->x0 - x, 0), x - cub->x1);
	float dy = fmaxf (fmaxf (cub->y0 - y, 0), y - cub->y1);
	float dz = fmaxf (fmaxf (cub->z0 - z, 0), z - cub->z1);
	return sqrtf (dx * dx + dy * dy + dz * dz);
}

ELEA_INLINE float
elea_aabox3f_get_distance_p (const EleaAABox3f* cub, const EleaVec3f *p)
{
	float dx = fmaxf (fmaxf (cub->x0 - p->x, 0), p->x - cub->x1);
	float dy = fmaxf (fmaxf (cub->y0 - p->y, 0), p->y - cub->y1);
	float dz = fmaxf (fmaxf (cub->z0 - p->z, 0), p->z - cub->z1);
	return sqrtf (dx * dx + dy * dy + dz * dz);
}

ELEA_INLINE float
elea_aabox3f_test_isect (const EleaAABox3f *lhs, const EleaAABox3f *rhs)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		if (lhs->min.c[i] > rhs->max.c[i]) return 0;
		if (lhs->max.c[i] < rhs->min.c[i]) return 0;
	}
	return 1;
}

unsigned int elea_aabox3f_intersect_ray (const EleaAABox3f *box, const EleaRay3f *ray, float *p0, float *p1);

#ifdef __cplusplus
}
#endif

#endif

