#ifndef __ELEA_VECTOR4_H__
#define __ELEA_VECTOR4_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#define ELEA_TYPE_VECTOR4F elea_vec4f_get_type ()

#include <math.h>
#include <string.h>

#include <arikkei/arikkei-utils.h>
#include <elea/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern EleaVec4f EleaVec4f0;
extern EleaVec4f EleaVec4fX;
extern EleaVec4f EleaVec4fY;
extern EleaVec4f EleaVec4fZ;
extern EleaVec4f EleaVec4fW;
extern EleaVec4f EleaVec4fX1;
extern EleaVec4f EleaVec4fY1;
extern EleaVec4f EleaVec4fZ1;

struct _EleaVec4f {
	union {
		struct {
			float x, y, z, w;
		};
		float c[4];
	};
};

unsigned int elea_vec4f_get_type(void);

static inline EleaVec4f
elea_vec4f_from_xyzw (float x, float y, float z, float w)
{
	return (EleaVec4f) {x, y, z, w};
}

static inline EleaVec4f
elea_vec4f_from_floatv (float *v)
{
	return (EleaVec4f) {v[0], v[1], v[2], v[3]};
}

static inline EleaVec4f *
elea_vec4fp_set_xyzw (EleaVec4f *dst, float x, float y, float z, float w)
{
	dst->x = x;
	dst->y = y;
	dst->z = z;
	dst->w = w;
	return dst;
}

static inline EleaVec4f *
elea_vec4fp_set_floatv (EleaVec4f *dst, const float *v)
{
	memcpy (dst, v, 12);
	return dst;
}

static inline EleaVec4f
elea_vec4f_inv (const EleaVec4f vec)
{
	return (EleaVec4f) {-vec.x, -vec.y, -vec.z, -vec.w};
}

static inline EleaVec4f *
elea_vec4fp_inv (EleaVec4f *dst, const EleaVec4f *src)
{
	dst->x = -src->x;
	dst->y = -src->y;
	dst->z = -src->z;
	dst->w = -src->w;
	return dst;
}

static inline EleaVec4f
elea_vec4f_add_xyzw (const EleaVec4f lhs, float x, float y, float z, float w)
{
	return (EleaVec4f) {lhs.x + x, lhs.y + y, lhs.z + z, lhs.w + w};
}

static inline EleaVec4f *
elea_vec4fp_add_xyzw (EleaVec4f *dst, const EleaVec4f *lhs, float x, float y, float z, float w)
{
	dst->x = lhs->x + x;
	dst->y = lhs->y + y;
	dst->z = lhs->z + z;
	dst->w = lhs->w + w;
	return dst;
}

static inline EleaVec4f
elea_vec4f_add (const EleaVec4f lhs, const EleaVec4f rhs)
{
	return (EleaVec4f) {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

static inline EleaVec4f *
elea_vec4fp_add (EleaVec4f *dst, const EleaVec4f *lhs, const EleaVec4f *rhs)
{
	dst->x = lhs->x + rhs->x;
	dst->y = lhs->y + rhs->y;
	dst->z = lhs->z + rhs->z;
	dst->w = lhs->w + rhs->w;
	return dst;
}

static inline EleaVec4f
elea_vec4f_sub_xyzw (const EleaVec4f lhs, float x, float y, float z, float w)
{
	return (EleaVec4f) {lhs.x - x, lhs.y - y, lhs.z - z, lhs.w - w};
}

static inline EleaVec4f *
elea_vec4fp_sub_xyzw (EleaVec4f *dst, const EleaVec4f *lhs, float x, float y, float z, float w)
{
	dst->x = lhs->x - x;
	dst->y = lhs->y - y;
	dst->z = lhs->z - z;
	dst->w = lhs->w - w;
	return dst;
}

static inline EleaVec4f
elea_vec4f_sub (const EleaVec4f lhs, const EleaVec4f rhs)
{
	return (EleaVec4f) {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

static inline EleaVec4f *
elea_vec4fp_sub (EleaVec4f *dst, const EleaVec4f *lhs, const EleaVec4f *rhs)
{
	dst->x = lhs->x - rhs->x;
	dst->y = lhs->y - rhs->y;
	dst->z = lhs->z - rhs->z;
	dst->w = lhs->w - rhs->w;
	return dst;
}

static inline EleaVec4f
elea_vec4f_mul (const EleaVec4f vec, float v)
{
	return (EleaVec4f) {vec.x * v, vec.y * v, vec.z * v, vec.w * v};
}

static inline EleaVec4f *
elea_vec4fp_mul (EleaVec4f *dst, const EleaVec4f *vec, float v)
{
	dst->x = vec->x * v;
	dst->y = vec->y * v;
	dst->z = vec->z * v;
	dst->w = vec->w * v;
	return dst;
}

static inline EleaVec4f
elea_vec4f_div (const EleaVec4f vec, float v)
{
	return (EleaVec4f) {vec.x / v, vec.y / v, vec.z / v, vec.w / v};
}

static inline EleaVec4f *
elea_vec4fp_div (EleaVec4f *dst, const EleaVec4f *vec, float v)
{
	dst->x = vec->x / v;
	dst->y = vec->y / v;
	dst->z = vec->z / v;
	dst->w =  vec->w / v;
	return dst;
}

static inline float
elea_vec4f_dot (const EleaVec4f lhs, const EleaVec4f rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
}

static inline float
elea_vec4fp_dot (const EleaVec4f *lhs, const EleaVec4f *rhs)
{
	return lhs->x * rhs->x + lhs->y * rhs->y + lhs->z * rhs->z + lhs->w * rhs->w;
}

static inline float
elea_vec4f_len (const EleaVec4f v)
{
	return sqrtf (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

static inline float
elea_vec4fp_len (const EleaVec4f *v)
{
	return sqrtf (v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

static inline float
elea_vec4f_len2 (const EleaVec4f v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

static inline float
elea_vec4fp_len2 (const EleaVec4f *v)
{
	return v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
}

static inline EleaVec4f
elea_vec4f_normalize (const EleaVec4f vec)
{
	float len = elea_vec4f_len(vec);
	if (len) {
		return (EleaVec4f) {vec.x / len, vec.y / len, vec.z / len, vec.w / len};
	} else {
		return elea_vec4f_from_xyzw(1, 0, 0, 0);
	}
}

static inline EleaVec4f *
elea_vec4fp_normalize (EleaVec4f *dst, const EleaVec4f *vec)
{
	float len = elea_vec4fp_len(vec);
	if (len) {
		return elea_vec4fp_div(dst, vec, len);
	} else {
		return &EleaVec4fX;
	}

}

static inline unsigned int
elea_vec4f_equal (const EleaVec4f lhs, const EleaVec4f rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w);
}

static inline unsigned int
elea_vec4fp_equal (const EleaVec4f *lhs, const EleaVec4f *rhs)
{
	return (lhs->x == rhs->x) && (lhs->y == rhs->y) && (lhs->z == rhs->z) && (lhs->w == rhs->w);
}

#ifdef __cplusplus
};
#endif

#endif
