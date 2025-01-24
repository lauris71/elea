#ifndef __ELEA_VECTOR2_H__
#define __ELEA_VECTOR2_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2025
 *
 */

#define ELEA_TYPE_VECTOR2F elea_vec2f_get_type ()

#include <math.h>
#include <string.h>

#include <elea/types.h>

#ifdef __cplusplus
extern "C" {
#endif

const extern EleaVec2f EleaVec2f0;
const extern EleaVec2f EleaVec2fX;
const extern EleaVec2f EleaVec2fY;

struct _EleaVec2f {
	union {
		struct {
			float x, y;
		};
		float c[2];
	};
};

static inline EleaVec2f
elea_vec2f_from_xy (float x, float y)
{
	return (EleaVec2f) {x, y};
}

static inline EleaVec2f
elea_vec2f_from_floatv (float *v)
{
	return (EleaVec2f) {v[0], v[1]};
}

static inline EleaVec2f *
elea_vec2fp_set_xy (EleaVec2f *dst, float x, float y)
{
	dst->x = x;
	dst->y = y;
	return dst;
}

static inline EleaVec2f *
elea_vec2fp_set_floatv (EleaVec2f *dst, const float *v)
{
	dst->x = v[0];
	dst->y = v[1];
	return dst;
}

static inline EleaVec2f
elea_vec2f_inv (const EleaVec2f vec)
{
	return (EleaVec2f) {-vec.x, -vec.y};
}

static inline EleaVec2f *
elea_vec2fp_inv (EleaVec2f *dst, const EleaVec2f *vec)
{
	dst->x = -vec->x;
	dst->y = -vec->y;
	return dst;
}

static inline EleaVec2f
elea_vec2f_add_xy(const EleaVec2f vec, float x, float y)
{
	return (EleaVec2f) {vec.x + x, vec.y + y};
}

static inline EleaVec2f *
elea_vec2fp_add_xy(EleaVec2f *dst, const EleaVec2f *vec, float x, float y)
{
	dst->x = vec->x + x;
	dst->y = vec->y + y;
	return dst;
}

static inline EleaVec2f
elea_vec2f_add_floats(const EleaVec2f vec, const float *v)
{
	return (EleaVec2f) {vec.x + v[0], vec.y + v[1]};
}

static inline EleaVec2f *
elea_vec2fp_add_floats(EleaVec2f *dst, const EleaVec2f *vec, const float *v)
{
	dst->x = vec->x + v[0];
	dst->y = vec->y + v[1];
	return dst;
}

static inline EleaVec2f
elea_vec2f_add (const EleaVec2f lhs, const EleaVec2f rhs)
{
	return (EleaVec2f) {lhs.x + rhs.x, lhs.y + rhs.y};
}

static inline EleaVec2f *
elea_vec2fp_add (EleaVec2f *dst, const EleaVec2f *lhs, const EleaVec2f *rhs)
{
	dst->x = lhs->x + rhs->x;
	dst->y = lhs->y + rhs->y;
	return dst;
}

static inline EleaVec2f
elea_vec2f_sub_xy (const EleaVec2f vec, float x, float y)
{
	return (EleaVec2f) {vec.x - x, vec.y - y};
}

static inline EleaVec2f *
elea_vec2fp_sub_xy (EleaVec2f *dst, const EleaVec2f *vec, float x, float y)
{
	dst->x = vec->x - x;
	dst->y = vec->y - y;
	return dst;
}

static inline EleaVec2f
elea_vec2f_sub_floats(const EleaVec2f vec, float *v)
{
	return (EleaVec2f) {vec.x - v[0], vec.y - v[1]};
}

static inline EleaVec2f *
elea_vec2fp_sub_floats(EleaVec2f *dst, const EleaVec2f *vec, float *v)
{
	dst->x = vec->x - v[0];
	dst->y = vec->y - v[1];
	return dst;
}

static inline EleaVec2f
elea_vec2f_sub (const EleaVec2f lhs, const EleaVec2f rhs)
{
	return (EleaVec2f) {lhs.x - rhs.x, lhs.y - rhs.y};
}

static inline EleaVec2f *
elea_vec2fp_sub (EleaVec2f *dst, const EleaVec2f *lhs, const EleaVec2f *rhs)
{
	dst->x = lhs->x - rhs->x;
	dst->y = lhs->y - rhs->y;
	return dst;
}

static inline EleaVec2f
elea_vec2f_mul (const EleaVec2f vec, float v)
{
	return (EleaVec2f) {vec.x * v, vec.y * v};
}

static inline EleaVec2f *
elea_vec2fp_mul (EleaVec2f *dst, const EleaVec2f *vec, float v)
{
	dst->x = vec->x * v;
	dst->y = vec->y * v;
	return dst;
}

static inline EleaVec2f
elea_vec2f_div (const EleaVec2f vec, float v)
{
	return (EleaVec2f) {vec.x / v, vec.y / v};
}

static inline EleaVec2f *
elea_vec2fp_div (EleaVec2f *dst, const EleaVec2f *vec, float v)
{
	dst->x = vec->x / v;
	dst->y = vec->y / v;
	return dst;
}

static inline float
elea_vec2f_dot(const EleaVec2f lhs, const EleaVec2f rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

static inline float
elea_vec2fp_dot(const EleaVec2f *lhs, const EleaVec2f *rhs)
{
	return lhs->x * rhs->x + lhs->y * rhs->y;
}

static inline float
elea_vec2f_len (const EleaVec2f vec)
{
	return sqrtf (vec.x * vec.x + vec.y * vec.y);
}

static inline float
elea_vec2fp_len (const EleaVec2f *vec)
{
	return sqrtf (vec->x * vec->x + vec->y * vec->y);
}

static inline float
elea_vec2f_len2 (const EleaVec2f vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

static inline float
elea_vec2fp_len2 (const EleaVec2f *vec)
{
	return vec->x * vec->x + vec->y * vec->y;
}

static inline EleaVec2f
elea_vec2f_normalize (const EleaVec2f src)
{
	float len = elea_vec2f_len (src);
	if (!len) return elea_vec2f_from_xy (1, 0);
	return (EleaVec2f) {src.x / len, src.y / len};
}

static inline EleaVec2f *
elea_vec2fp_normalize (EleaVec2f *dst, const EleaVec2f *vec)
{
	float len = elea_vec2fp_len (vec);
	if (!len) elea_vec2fp_set_xy (dst, 1, 0);
	elea_vec2fp_set_xy (dst, vec->x / len, vec->y / len);
	return dst;
}

static inline unsigned int
elea_vec2f_equal (const EleaVec2f lhs, const EleaVec2f rhs)
{
	return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

static inline unsigned int
elea_vec2fp_equal (const EleaVec2f *lhs, const EleaVec2f *rhs)
{
	return (lhs->x == rhs->x) && (lhs->y == rhs->y);
}

float elea_vec2fp_angle(const EleaVec2f *lhs, const EleaVec2f *rhs);

static inline float
elea_vec2f_angle(const EleaVec2f lhs, const EleaVec2f rhs)
{
	return elea_vec2fp_angle(&lhs, &rhs);
}

#ifdef __cplusplus
};
#endif

#endif
