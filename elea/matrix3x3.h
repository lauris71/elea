#ifndef __ELEA_MATRIX3X3_H__
#define __ELEA_MATRIX3X3_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2020
 *
 */

/*
 * Column-major matrix
 */

#define ELEA_TYPE_MATRIX3X3F elea_mat3x3f_get_type ()

#include <math.h>
#include <string.h>

#include <arikkei/arikkei-utils.h>
#include <elea/types.h>
#include <elea/vector3.h>
#include <elea/vector4.h>

#ifdef __cplusplus
extern "C" {
#endif

const extern EleaMat3x3f EleaMat3x3fIdentity;

struct _EleaMat3x3f {
	float c[9];
};

unsigned int elea_mat3x3f_get_type (void);

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_set_values (EleaMat3x3f *dst, float a00, float a10, float a20, float a01, float a11, float a21, float a02, float a12, float a22)
{
	dst->c[0] = a00;
	dst->c[1] = a10;
	dst->c[2] = a20;
	dst->c[3] = a01;
	dst->c[4] = a11;
	dst->c[5] = a21;
	dst->c[6] = a02;
	dst->c[7] = a12;
	dst->c[8] = a22;
	return dst;
}

ELEA_INLINE const EleaMat3x3f *
elea_mat3x3f_set (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict src)
{
	memcpy (dst, src, sizeof (EleaMat3x3f));
	return dst;
}

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_set_row_vec_xyz (EleaMat3x3f *dst, unsigned int r, float x, float y, float z)
{
	dst->c[r] = x;
	dst->c[r + 3] = y;
	dst->c[r + 6] = z;
	return dst;
}

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_set_row_vec (EleaMat3x3f *dst, unsigned int r, const EleaVec3f *v)
{
	dst->c[r] = v->x;
	dst->c[r + 3] = v->y;
	dst->c[r + 6] = v->z;
	return dst;
}

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_set_col_vec_xyz (EleaMat3x3f *dst, unsigned int c, float x, float y, float z)
{
	dst->c[3 * c] = x;
	dst->c[3 * c + 1] = y;
	dst->c[3 * c + 2] = z;
	return dst;
}

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_set_col_vec (EleaMat3x3f *dst, unsigned int c, const EleaVec3f* v)
{
	memcpy (&dst->c[3 * c], v, sizeof (EleaVec3f));
	return dst;
}

ELEA_INLINE EleaVec3f *
elea_mat3x3f_get_row_vec (EleaVec3f *dst, const EleaMat3x3f *mat, unsigned int r)
{
	dst->x = mat->c[r];
	dst->y = mat->c[3 + r];
	dst->z = mat->c[6 + r];
	return dst;
}

ELEA_INLINE const float*
elea_mat3x3f_get_col (const EleaMat3x3f* mat, unsigned int c)
{
	return &mat->c[3 * c];
}

ELEA_INLINE EleaVec3f *
elea_mat3x3f_get_col_vec (EleaVec3f *dst, const EleaMat3x3f *mat, unsigned int c)
{
	memcpy (dst, &mat->c[3 * c], sizeof (EleaVec3f));
	return dst;
}

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_set_scaling_xyz (EleaMat3x3f* dst, float sx, float sy, float sz)
{
	*dst = EleaMat3x3fIdentity;
	dst->c[0] = sx;
	dst->c[4] = sy;
	dst->c[8] = sz;
	return dst;
}

EleaMat3x3f *elea_mat3x3f_transpose (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict mat);
EleaMat3x3f *elea_mat3x3f_transpose_self (EleaMat3x3f *mat);
EleaMat3x3f *elea_mat3x3f_invert (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict mat);
EleaMat3x3f *elea_mat3x3f_invert_self (EleaMat3x3f *mat);

EleaMat3x3f *elea_mat3x3f_multiply (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict lhs, const EleaMat3x3f * __restrict rhs);

ELEA_INLINE EleaMat3x3f *
elea_mat3x3f_multiply_self_right (EleaMat3x3f * __restrict mat, const EleaMat3x3f * __restrict rhs)
{
	EleaMat3x3f lhs;
	memcpy (lhs.c, mat, 36);
	return elea_mat3x3f_multiply (mat, &lhs, rhs);
}

ELEA_INLINE EleaMat3x3f*
elea_mat3x3f_multiply_self_left (EleaMat3x3f * __restrict mat, const EleaMat3x3f * __restrict lhs)
{
	EleaMat3x3f rhs;
	memcpy (rhs.c, mat, 36);
	return elea_mat3x3f_multiply (mat, lhs, &rhs);
}

ARIKKEI_INLINE EleaVec3f*
elea_mat3x3f_transform_vec_xyz (EleaVec3f* d, const EleaMat3x3f* mat, float x, float y, float z)
{
	d->x = mat->c[0] * x + mat->c[3] * y + mat->c[6] * z;
	d->y = mat->c[1] * x + mat->c[4] * y + mat->c[7] * z;
	d->z = mat->c[2] * x + mat->c[5] * y + mat->c[8] * z;
	return d;
}

ARIKKEI_INLINE EleaVec3f*
elea_mat3x3f_transform_vec (EleaVec3f* d, const EleaMat3x3f* mat, const EleaVec3f* v)
{
	return elea_mat3x3f_transform_vec_xyz (d, mat, v->x, v->y, v->z);
}

#ifdef __cplusplus
};
#endif

#endif
