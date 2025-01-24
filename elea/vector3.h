#ifndef __ELEA_VECTOR3_H__
#define __ELEA_VECTOR3_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

typedef struct _EleaVec3fClass EleaVec3fClass;

#define ELEA_TYPE_VECTOR3F elea_vec3f_get_type ()

#include <math.h>
#include <string.h>

#include <elea/types.h>

#ifdef __cplusplus
extern "C" {
#endif

const extern EleaVec3f EleaVec3f0;
const extern EleaVec3f EleaVec3fX;
const extern EleaVec3f EleaVec3fY;
const extern EleaVec3f EleaVec3fZ;

struct _EleaVec3f {
	union {
		struct {
			float x, y, z;
		};
		float c[3];
	};
};

extern EleaVec3fClass *elea_vec3f_class;

unsigned int elea_vec3f_get_type (void);

static inline EleaVec3f
elea_vec3f_from_xyz (float x, float y, float z)
{
	return (EleaVec3f) {x, y, z};
}

static inline EleaVec3f
elea_vec3f_from_floatv (float *v)
{
	return (EleaVec3f) {v[0], v[1], v[2]};
}

static inline EleaVec3f *
elea_vec3fp_set_xyz (EleaVec3f *dst, float x, float y, float z)
{
	dst->x = x;
	dst->y = y;
	dst->z = z;
	return dst;
}

static inline EleaVec3f *
elea_vec3fp_set_floatv (EleaVec3f *dst, const float *v)
{
	memcpy (dst, v, 12);
	return dst;
}

static inline EleaVec3f
elea_vec3f_inv (const EleaVec3f vec)
{
	return (EleaVec3f) {-vec.x, -vec.y, -vec.z};
}

static inline EleaVec3f *
elea_vec3fp_inv (EleaVec3f *dst, const EleaVec3f *vec)
{
	dst->x = -vec->x;
	dst->y = -vec->y;
	dst->z = -vec->z;
	return dst;
}

static inline EleaVec3f
elea_vec3f_add_xyz (const EleaVec3f lhs, float x, float y, float z)
{
	return (EleaVec3f) {lhs.x + x, lhs.y + y, lhs.z + z};
}

static inline EleaVec3f *
elea_vec3fp_add_xyz (EleaVec3f *dst, const EleaVec3f *vec, float x, float y, float z)
{
	dst->x = vec->x + x;
	dst->y = vec->y + y;
	dst->z = vec->z + z;
	return dst;
}

static inline EleaVec3f
elea_vec3f_add (const EleaVec3f lhs, const EleaVec3f rhs)
{
	return (EleaVec3f) {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

static inline EleaVec3f *
elea_vec3fp_add (EleaVec3f *dst, const EleaVec3f *lhs, const EleaVec3f *rhs)
{
	dst->x = lhs->x + rhs->x;
	dst->y = lhs->y + rhs->y;
	dst->z = lhs->z + rhs->z;
	return dst;
}

static inline EleaVec3f
elea_vec3f_sub_xyz (const EleaVec3f vec, float x, float y, float z)
{
	return (EleaVec3f) {vec.x - x, vec.y - y, vec.z - z};
}

static inline EleaVec3f *
elea_vec3fp_sub_xyz (EleaVec3f *dst, const EleaVec3f *vec, float x, float y, float z)
{
	dst->x = vec->x - x;
	dst->y = vec->y - y;
	dst->z = vec->z - z;
	return dst;
}

static inline EleaVec3f
elea_vec3f_sub (const EleaVec3f lhs, const EleaVec3f rhs)
{
	return (EleaVec3f) {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

static inline EleaVec3f *
elea_vec3fp_sub (EleaVec3f *dst, const EleaVec3f *lhs, const EleaVec3f *rhs)
{
	dst->x = lhs->x - rhs->x;
	dst->y = lhs->y - rhs->y;
	dst->z = lhs->z - rhs->z;
	return dst;
}

static inline EleaVec3f
elea_vec3f_mul (const EleaVec3f vec, float v)
{
	return (EleaVec3f) {vec.x * v, vec.y * v, vec.z * v};
}

static inline EleaVec3f *
elea_vec3fp_mul (EleaVec3f *dst, const EleaVec3f *src, float v)
{
	dst->x = src->x * v;
	dst->y = src->y * v;
	dst->z = src->z * v;
	return dst;
}

static inline EleaVec3f
elea_vec3f_div (const EleaVec3f vec, float v)
{
	return (EleaVec3f) {vec.x / v, vec.y / v, vec.z / v};
}

static inline EleaVec3f *
elea_vec3fp_div (EleaVec3f *dst, const EleaVec3f *src, float v)
{
	dst->x = src->x / v;
	dst->y = src->y / v;
	dst->z = src->z / v;
	return dst;
}

static inline EleaVec3f
elea_vec3f_mul_add (const EleaVec3f lhs, const EleaVec3f rhs, float v)
{
	return (EleaVec3f) {lhs.x + rhs.x * v, lhs.y + rhs.y * v, lhs.z + rhs.z * v};
}

static inline EleaVec3f
elea_vec3f_interpolate (const EleaVec3f lhs, const EleaVec3f rhs, float s)
{
	return (EleaVec3f) {(1 - s) * lhs.x + s * rhs.x, (1 - s) * lhs.y + s * rhs.y, (1 - s) * lhs.z + s * rhs.z};
}

static inline EleaVec3f *
elea_vec3fp_interpolate (EleaVec3f *dst, const EleaVec3f *lhs, const EleaVec3f *rhs, float s)
{
	dst->x = (1 - s) * lhs->x + s * rhs->x;
	dst->y = (1 - s) * lhs->y + s * rhs->y;
	dst->z = (1 - s) * lhs->z + s * rhs->z;
	return dst;
}

static inline float
elea_vec3f_dot (const EleaVec3f lhs, const EleaVec3f rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

static inline float
elea_vec3fp_dot (const EleaVec3f *lhs, const EleaVec3f *rhs)
{
	return lhs->x * rhs->x + lhs->y * rhs->y + lhs->z * rhs->z;
}

static inline float
elea_vec3f_len (const EleaVec3f v)
{
	return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline float
elea_vec3fp_len (const EleaVec3f *v)
{
	return sqrtf (v->x * v->x + v->y * v->y + v->z * v->z);
}

static inline float
elea_vec3f_len2 (const EleaVec3f v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

static inline float
elea_vec3fp_len2 (const EleaVec3f *v)
{
	return v->x * v->x + v->y * v->y + v->z * v->z;
}

static inline EleaVec3f
elea_vec3f_normalize (const EleaVec3f vec)
{
	float len = elea_vec3f_len(vec);
	if (!len) return elea_vec3f_from_xyz(1, 0, 0);
	return (EleaVec3f) {vec.x / len, vec.y / len, vec.z / len};
}

static inline EleaVec3f *
elea_vec3fp_normalize (EleaVec3f *dst, const EleaVec3f *src)
{
	float len = elea_vec3fp_len(src);
	if (len) {
		elea_vec3fp_div(dst, src, len);
	} else {
		elea_vec3fp_set_xyz(dst, 1, 0, 0);
	}
	return dst;
}

static inline EleaVec3f
elea_vec3f_cross (const EleaVec3f lhs, const EleaVec3f rhs)
{
	return (EleaVec3f) {
		lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.z * rhs.x - lhs.x * rhs.z,
		lhs.x * rhs.y - lhs.y * rhs.x
	};
}

static inline EleaVec3f *
elea_vec3fp_cross (EleaVec3f *dst, const EleaVec3f *lhs, const EleaVec3f *rhs)
{
	dst->x = lhs->y * rhs->z - lhs->z * rhs->y;
	dst->y = lhs->z * rhs->x - lhs->x * rhs->z;
	dst->z = lhs->x * rhs->y - lhs->y * rhs->x;
	return dst;
}

static inline unsigned int
elea_vec3f_equal (const EleaVec3f lhs, const EleaVec3f rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

static inline unsigned int
elea_vec3fp_equal (const EleaVec3f *lhs, const EleaVec3f *rhs)
{
	return (lhs->x == rhs->x) && (lhs->y == rhs->y) && (lhs->z == rhs->z);
}

float elea_vec3f_angle (const EleaVec3f lhs, const EleaVec3f rhs);

static inline float
elea_vec3fp_angle (const EleaVec3f *lhs, const EleaVec3f *rhs)
{
	return elea_vec3f_angle(*lhs, *rhs);
}

/* Get arbitrary perpendicular vector with the same length */
EleaVec3f elea_vec3f_get_perpendicular (const EleaVec3f src);

static inline EleaVec3f *
elea_vec3fp_get_perpendicular (EleaVec3f *dst, const EleaVec3f *src)
{
	*dst = elea_vec3f_get_perpendicular(*src);
	return dst;
}

#ifdef __cplusplus
};
#endif

#endif
