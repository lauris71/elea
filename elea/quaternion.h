#ifndef __ELEA_QUATERNION_H__
#define __ELEA_QUATERNION_H__

/*
* Libelea
*
* Copyright (C) Lauris Kaplinski 2007-2019
*
*/

typedef struct _EleaQuatf EleaQuatf;

#include <math.h>
#include <string.h>

#include <elea/types.h>

extern EleaQuatf EleaQuatf0;

#ifdef __cplusplus
extern "C" {
#endif

struct _EleaQuatf {
	union {
		struct {
			float i, j, k, r;
		};
		float c[4];
	};
};

unsigned int elea_quatf_get_type (void);

static inline EleaQuatf
elea_quatf_from_ijkr(float i, float j, float k, float r)
{
	return (EleaQuatf) {i, j, k, r};
}

static inline EleaQuatf
elea_quatf_from_floatv(const float *v)
{
	return (EleaQuatf) {v[0], v[1], v[2], v[3]};
}

static inline EleaQuatf
elea_quatf_from_ijk (float i, float j, float k)
{
	float w2 = i * i + j * j + k * k;
	if (w2 <= 1) {
		return (EleaQuatf) {i, j, k, sqrtf (1 - w2)};
	} else {
		float d = sqrtf (w2);
		return (EleaQuatf) {i / d, j / d, k / d, 0};
	}
}

static inline EleaQuatf *
elea_quatfp_set_ijkr (EleaQuatf *dst, float i, float j, float k, float r)
{
	dst->i = i;
	dst->j = j;
	dst->k = k;
	dst->r = r;
	return dst;
}

static inline EleaQuatf *
elea_quatfp_set_floatv (EleaQuatf *dst, const float *v)
{
	memcpy (dst, v, sizeof (EleaQuatf));
	return dst;
}

static inline EleaQuatf *
elea_quatfp_set_ijk (EleaQuatf *dst, float i, float j, float k)
{
	float w2 = i * i + j * j + k * k;
	if (w2 <= 1) {
		dst->i = i;
		dst->j = j;
		dst->k = k;
		dst->r = sqrtf (1 - w2);
	} else {
		float d = sqrtf (w2);
		dst->i = i / d;
		dst->j = j / d;
		dst->k = k / d;
		dst->r = 0;
	}
	return dst;
}

static inline float
elea_quatf_norm (const EleaQuatf q)
{
	return sqrtf (q.i * q.i + q.j * q.j + q.k * q.k + q.r * q.r);
}

static inline float
elea_quatfp_norm (const EleaQuatf *q)
{
	return sqrtf (q->i * q->i + q->j * q->j + q->k * q->k + q->r * q->r);
}

static inline float
elea_quatf_norm2 (const EleaQuatf q)
{
	return q.i * q.i + q.j * q.j + q.k * q.k + q.r * q.r;
}

static inline float
elea_quatfp_norm2 (const EleaQuatf *q)
{
	return q->i * q->i + q->j * q->j + q->k * q->k + q->r * q->r;
}

static inline EleaQuatf
elea_quatf_negate (const EleaQuatf src)
{
	return (EleaQuatf) {-src.i, -src.j, -src.k, -src.r};
}

static inline EleaQuatf *
elea_quatfp_negate (EleaQuatf *dst, const EleaQuatf *src)
{
	dst->i = -src->i;
	dst->j = -src->j;
	dst->k = -src->k;
	dst->r = -src->r;
	return dst;
}

static inline EleaQuatf
elea_quatf_conjugate (const EleaQuatf src)
{
	return (EleaQuatf) {-src.i, -src.j, -src.k, src.r};
}

static inline EleaQuatf *
elea_quatfp_conjugate (EleaQuatf *dst, const EleaQuatf *src)
{
	dst->i = -src->i;
	dst->j = -src->j;
	dst->k = -src->k;
	dst->r = src->r;
	return dst;
}

static inline EleaQuatf
elea_quatf_inverse (const EleaQuatf src)
{
	float n2 = src.i * src.i + src.j * src.j + src.k * src.k + src.r * src.r;
	return (EleaQuatf) {-src.i / n2, -src.j / n2, -src.k / n2, src.r / n2};
}

static inline EleaQuatf *
elea_quatfp_inverse (EleaQuatf *dst, const EleaQuatf *src)
{
	float n2 = src->i * src->i + src->j * src->j + src->k * src->k + src->r * src->r;
	dst->i = -src->i / n2;
	dst->j = -src->j / n2;
	dst->k = -src->k / n2;
	dst->r = src->r / n2;
	return dst;
}

static inline EleaQuatf
elea_quatf_normalize (const EleaQuatf src)
{
	float norm = elea_quatf_norm (src);
	if (!norm) return elea_quatf_from_ijkr (0, 0, 0, 1);
	return (EleaQuatf) {src.i / norm, src.j / norm, src.k / norm, src.r / norm};
}

static inline EleaQuatf *
elea_quatfp_normalize (EleaQuatf *dst, const EleaQuatf *src)
{
	float norm = elea_quatfp_norm (src);
	if (!norm) return elea_quatfp_set_ijkr (dst, 0, 0, 0, 1);
	dst->i = src->i / norm;
	dst->j = src->j / norm;
	dst->k = src->k / norm;
	dst->r = src->r / norm;
	return dst;
}

static inline EleaQuatf
elea_quatf_mul_scalar (const EleaQuatf src, float v)
{
	return (EleaQuatf) {src.i * v, src.j * v, src.k * v, src.r * v};
}

static inline EleaQuatf *
elea_quatfp_mul_scalar (EleaQuatf *dst, const EleaQuatf *src, float v)
{
	dst->i = src->i * v;
	dst->j = src->j * v;
	dst->k = src->k * v;
	dst->r = src->r * v;
	return dst;
}

static inline EleaQuatf
elea_quatf_mul (const EleaQuatf lhs, const EleaQuatf rhs)
{
	return (EleaQuatf) {
		lhs.r * rhs.i + lhs.i * rhs.r + lhs.j * rhs.k - lhs.k * rhs.j,
		lhs.r * rhs.j + lhs.j * rhs.r + lhs.k * rhs.i - lhs.i * rhs.k,
		lhs.r * rhs.k + lhs.k * rhs.r + lhs.i * rhs.j - lhs.j * rhs.i,
		lhs.r * rhs.r - lhs.i * rhs.i - lhs.j * rhs.j - lhs.k * rhs.k
	};
}

static inline EleaQuatf *
elea_quatfp_mul (EleaQuatf * __restrict dst, const EleaQuatf * lhs, const EleaQuatf * rhs)
{
	dst->i = lhs->r * rhs->i + lhs->i * rhs->r + lhs->j * rhs->k - lhs->k * rhs->j;
	dst->j = lhs->r * rhs->j + lhs->j * rhs->r + lhs->k * rhs->i - lhs->i * rhs->k;
	dst->k = lhs->r * rhs->k + lhs->k * rhs->r + lhs->i * rhs->j - lhs->j * rhs->i;
	dst->r = lhs->r * rhs->r - lhs->i * rhs->i - lhs->j * rhs->j - lhs->k * rhs->k;
	return dst;
}

static inline float
elea_quatf_dot (const EleaQuatf lhs, const EleaQuatf rhs) {
	return lhs.i * rhs.i + lhs.j * rhs.j + lhs.k * rhs.k + lhs.r * rhs.r;
}

static inline float
elea_quatfp_dot (const EleaQuatf *lhs, const EleaQuatf *rhs) {
	return lhs->i * rhs->i + lhs->j * rhs->j + lhs->k * rhs->k + lhs->r * rhs->r;
}

EleaQuatf *elea_quatfp_slerp_normalized (EleaQuatf *dst, const EleaQuatf *lhs, const EleaQuatf *rhs, float t);

static inline EleaQuatf
elea_quatf_slerp_normalized (const EleaQuatf lhs, const EleaQuatf rhs, float t)
{
	EleaQuatf dst;
	return *elea_quatfp_slerp_normalized(&dst, &lhs, &rhs, t);
}

#ifdef __cplusplus
}
#endif

#endif

