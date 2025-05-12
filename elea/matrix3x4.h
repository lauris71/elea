#ifndef __ELEA_MATRIX3X4_H__
#define __ELEA_MATRIX3X4_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

/*
 * Row-major matrix for affine transformations
 */

#define ELEA_TYPE_MATRIX3X4F elea_mat3x4f_get_type ()

#include <math.h>
#include <string.h>

#include <arikkei/arikkei-utils.h>
#include <elea/types.h>
#include <elea/vector3.h>
#include <elea/vector4.h>

#ifdef __cplusplus
extern "C" {
#endif

const extern EleaMat3x4f EleaMat3x4fIdentity;

struct _EleaMat3x4f {
	float c[12];
} ARIKKEI_ALIGN_16;

unsigned int elea_mat3x4f_get_type (void);

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_values (EleaMat3x4f *dst, float a00, float a01, float a02, float a03, float a10, float a11, float a12, float a13, float a20, float a21, float a22, float a23)
{
	dst->c[0] = a00;
	dst->c[1] = a01;
	dst->c[2] = a02;
	dst->c[3] = a03;
	dst->c[4] = a10;
	dst->c[5] = a11;
	dst->c[6] = a12;
	dst->c[7] = a13;
	dst->c[8] = a20;
	dst->c[9] = a21;
	dst->c[10] = a22;
	dst->c[11] = a23;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_columns (EleaMat3x4f *dst, const EleaVec3f *c0, const EleaVec3f *c1, const EleaVec3f *c2, const EleaVec3f *c3)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		dst->c[4 * i + 0] = c0->c[i];
		dst->c[4 * i + 1] = c1->c[i];
		dst->c[4 * i + 2] = c2->c[i];
		dst->c[4 * i + 3] = c3->c[i];
	}
        return dst;
}

ELEA_INLINE const EleaMat3x4f *
elea_mat3x4f_set (EleaMat3x4f * __restrict dst, const EleaMat3x4f * __restrict src)
{
	memcpy (dst, src, sizeof (EleaMat3x4f));
	return dst;
}

ARIKKEI_INLINE EleaMat3x4f *
elea_mat3x4f_set_translate_xyz (EleaMat3x4f *dst, float x, float y, float z)
{
	memcpy (dst, EleaMat3x4fIdentity.c, 48);
	dst->c[3] = x;
	dst->c[7] = y;
	dst->c[11] = z;
	return dst;
}

ARIKKEI_INLINE EleaMat3x4f *
elea_mat3x4f_set_translate (EleaMat3x4f *dst, const EleaVec3f * __restrict v)
{
	return elea_mat3x4f_set_translate_xyz (dst, v->x, v->y, v->z);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_scale_xyz (EleaMat3x4f* dst, float sx, float sy, float sz)
{
	*dst = EleaMat3x4fIdentity;
	dst->c[0] = sx;
	dst->c[5] = sy;
	dst->c[10] = sz;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_scale (EleaMat3x4f *dst, const EleaVec3f *scale)
{
	return elea_mat3x4f_set_scale_xyz (dst, scale->x, scale->y, scale->z);
}

EleaMat3x4f *elea_mat3x4f_set_rotate (EleaMat3x4f *dst, const EleaQuatf *q);
EleaMat3x4f *elea_mat3x4f_set_look_at (EleaMat3x4f *dst, const EleaVec3f *vp, const EleaVec3f *tgt, const EleaVec3f *up);

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_row_vec3_xyz (EleaMat3x4f *dst, unsigned int r, float x, float y, float z)
{
	dst->c[4 * r] = x;
	dst->c[4 * r + 1] = y;
	dst->c[4 * r + 2] = z;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_row_vec3 (EleaMat3x4f *dst, unsigned int r, const EleaVec3f *v)
{
	memcpy (&dst->c[4 * r], v, sizeof (EleaVec3f));
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_row_vec4_xyzw (EleaMat3x4f *dst, unsigned int r, float x, float y, float z, float w)
{
	dst->c[4 * r] = x;
	dst->c[4 * r + 1] = y;
	dst->c[4 * r + 2] = z;
	dst->c[4 * r + 3] = w;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_row_vec4 (EleaMat3x4f *dst, unsigned int r, const EleaVec4f *v)
{
	memcpy (&dst->c[4 * r], v, sizeof (EleaVec4f));
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_col_vec_xyz (EleaMat3x4f *dst, unsigned int c, float x, float y, float z)
{
	dst->c[c] = x;
	dst->c[4 + c] = y;
	dst->c[8 + c] = z;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_set_col_vec (EleaMat3x4f *dst, unsigned int c, const EleaVec3f* v)
{
	dst->c[c] = v->x;
	dst->c[4 + c] = v->y;
	dst->c[8 + c] = v->z;
	return dst;
}

ELEA_INLINE const float *
elea_mat3x4f_get_row (const EleaMat3x4f *mat, unsigned int r)
{
	return &mat->c[4 * r];
}

ELEA_INLINE EleaVec3f *
elea_mat3x4f_get_row_vec3 (EleaVec3f *dst, const EleaMat3x4f *mat, unsigned int r)
{
	memcpy (dst, &mat->c[4 * r], sizeof (EleaVec3f));
	return dst;
}

ELEA_INLINE EleaVec4f *
elea_mat3x4f_get_row_vec4 (EleaVec4f *dst, const EleaMat3x4f *mat, unsigned int r)
{
	memcpy (dst, &mat->c[4 * r], sizeof (EleaVec4f));
	return dst;
}

ELEA_INLINE EleaVec3f *
elea_mat3x4f_get_col_vec (EleaVec3f *dst, const EleaMat3x4f *mat, unsigned int c)
{
	dst->x = mat->c[c];
	dst->y = mat->c[4 + c];
	dst->z = mat->c[8 + c];
	return dst;
}

#define elea_mat3x4f_set_translation_xyz(m,x,y,z) elea_mat3x4f_set_col_vec_xyz (m, 3, x, y, z)
#define elea_mat3x4f_set_translation(m,v) elea_mat3x4f_set_col_vec (m, 3, v)
#define elea_mat3x4f_get_translation(d,m) elea_mat3x4f_get_col_vec (d, m, 3)

EleaMat3x4f *elea_mat3x4f_set_rotation (EleaMat3x4f *dst, const EleaQuatf *q);
EleaMat3x4f *elea_mat3x4f_set_rotation_angles_xyz (EleaMat3x4f *dst, float x, float y, float z);
EleaMat3x4f *elea_mat3x4f_set_rotation_angles_axes (EleaMat3x4f *dst, const float *angles, const unsigned int *axes);
EleaMat3x4f *elea_mat3x4f_set_rotation_axis_xyz_angle (EleaMat3x4f *dst, float x, float y, float z, float angle);
ARIKKEI_INLINE EleaMat3x4f *
elea_mat3x4f_set_rotation_axis_angle (EleaMat3x4f *dst, const EleaVec3f *axis, float angle)
{
	return elea_mat3x4f_set_rotation_axis_xyz_angle (dst, axis->x, axis->y, axis->z, angle);
}
EleaMat3x4f *elea_mat3x4f_set_mirror(EleaMat3x4f *dst, const EleaPlane3f *plane);

float *elea_mat3x4f_get_angles (float *dst, EleaMat3x4f *mat);
float *elea_mat3x4f_get_angles_base (float *dst, EleaMat3x4f *mat, const float * __restrict base);
float *elea_mat3x4f_get_angles_ordered (float *dst, EleaMat3x4f *mat, const int * __restrict order);
float *elea_mat3x4f_get_angles_ordered_base (float *dst, EleaMat3x4f *mat, const int * __restrict order, const float * __restrict base);

EleaMat3x4f *elea_mat3x4f_invert_normalized (EleaMat3x4f * __restrict dst, const EleaMat3x4f * __restrict mat);
EleaMat3x4f *elea_mat3x4f_invert_self_normalized (EleaMat3x4f *mat);
EleaMat3x4f *elea_mat3x4f_invert (EleaMat3x4f* __restrict dst, const EleaMat3x4f* __restrict mat);
EleaMat3x4f *elea_mat3x4f_invert_self (EleaMat3x4f *mat);

EleaMat3x4f *elea_mat3x4f_multiply (EleaMat3x4f * __restrict dst, const EleaMat3x4f * __restrict lhs, const EleaMat3x4f * __restrict rhs);

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_multiply_self_right (EleaMat3x4f * __restrict mat, const EleaMat3x4f * __restrict rhs)
{
	EleaMat3x4f lhs;
	memcpy (lhs.c, mat, 48);
	return elea_mat3x4f_multiply (mat, &lhs, rhs);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_multiply_self_left (EleaMat3x4f * __restrict mat, const EleaMat3x4f * __restrict lhs)
{
	EleaMat3x4f rhs;
	memcpy (rhs.c, mat, 48);
	return elea_mat3x4f_multiply (mat, lhs, &rhs);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_right_xyz (EleaMat3x4f *dst, const EleaMat3x4f * __restrict src, float x, float y, float z)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		memcpy (&dst->c[4 * i], &src->c[4 * i], 12);
		dst->c[4 * i + 3] = src->c[4 * i + 0] * x + src->c[4 * i + 1] * y + src->c[4 * i + 2] * z + src->c[4 * i + 3];
	}
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_right (EleaMat3x4f *dst, const EleaMat3x4f * __restrict src, const EleaVec3f * __restrict t)
{
	return elea_mat3x4f_translate_right_xyz (dst, src, t->x, t->y, t->z);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_self_right_xyz (EleaMat3x4f *dst, float x, float y, float z)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		dst->c[4 * i + 3] += dst->c[4 * i + 0] * x + dst->c[4 * i + 1] * y + dst->c[4 * i + 2] * z;
	}
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_self_right (EleaMat3x4f *dst, const EleaVec3f * __restrict s)
{
	return elea_mat3x4f_translate_self_right_xyz (dst, s->x, s->y, s->z);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_left_xyz (EleaMat3x4f *dst, const EleaMat3x4f * __restrict src, float x, float y, float z)
{
	memcpy (dst, src, sizeof (EleaMat3x4f));
	dst->c[3] = src->c[3] + x;
	dst->c[7] = src->c[7] + y;
	dst->c[11] = src->c[11] + z;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_left (EleaMat3x4f *dst, const EleaMat3x4f * __restrict src, const EleaVec3f * __restrict t)
{
	return elea_mat3x4f_translate_left_xyz (dst, src, t->x, t->y, t->z);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_self_left_xyz (EleaMat3x4f *dst, float x, float y, float z)
{
	dst->c[3] += x;
	dst->c[7] += y;
	dst->c[11] += z;
	return dst;
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_translate_self_left (EleaMat3x4f *dst, const EleaVec3f * __restrict s)
{
	return elea_mat3x4f_translate_self_left_xyz (dst, s->x, s->y, s->z);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_rotate_left_axis_xyz_angle (EleaMat3x4f *dst, const EleaMat3x4f * __restrict src, float x, float y, float z, float angle)
{
	EleaMat3x4f m;
	elea_mat3x4f_set_rotation_axis_xyz_angle (&m, x, y, z, angle);
	m.c[3] = m.c[7] = m.c[11] = 0;
	return elea_mat3x4f_multiply (dst, &m, src);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_rotate_left_axis_angle (EleaMat3x4f *dst, const EleaMat3x4f *src, const EleaVec3f *axis, float angle)
{
	return elea_mat3x4f_rotate_left_axis_xyz_angle (dst, src, axis->x, axis->y, axis->z, angle);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_rotate_right_axis_xyz_angle (EleaMat3x4f *dst, const EleaMat3x4f *__restrict src, float x, float y, float z, float angle)
{
	EleaMat3x4f m;
	elea_mat3x4f_set_rotation_axis_xyz_angle (&m, x, y, z, angle);
	m.c[3] = m.c[7] = m.c[11] = 0;
	return elea_mat3x4f_multiply (dst, src, &m);
}

ELEA_INLINE EleaMat3x4f *
elea_mat3x4f_rotate_right_axis_angle (EleaMat3x4f *dst, const EleaMat3x4f *__restrict src, const EleaVec3f *axis, float angle)
{
	return elea_mat3x4f_rotate_right_axis_xyz_angle (dst, src, axis->x, axis->y, axis->z, angle);
}

ELEA_INLINE EleaMat3x4f*
elea_mat3x4f_scale_right_xyz (EleaMat3x4f* dst, const EleaMat3x4f * __restrict src, float x, float y, float z)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		dst->c[4 * i + 0] = src->c[4 * i + 0] * x;
		dst->c[4 * i + 1] = src->c[4 * i + 1] * y;
		dst->c[4 * i + 2] = src->c[4 * i + 2] * z;
		dst->c[4 * i + 3] = src->c[4 * i + 3];
	}
	return dst;
}

ELEA_INLINE EleaMat3x4f*
elea_mat3x4f_scale_right (EleaMat3x4f* dst, const EleaMat3x4f * __restrict src, const EleaVec3f * __restrict s)
{
	return elea_mat3x4f_scale_right_xyz (dst, src, s->x, s->y, s->z);
}

ELEA_INLINE EleaMat3x4f*
elea_mat3x4f_scale_self_right_xyz (EleaMat3x4f* dst, float x, float y, float z)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		dst->c[4 * i + 0] *= x;
		dst->c[4 * i + 1] *= y;
		dst->c[4 * i + 2] *= z;
	}
	return dst;
}

ELEA_INLINE EleaMat3x4f*
elea_mat3x4f_scale_self_right (EleaMat3x4f* dst, const EleaVec3f * __restrict s)
{
	return elea_mat3x4f_scale_self_right_xyz (dst, s->x, s->y, s->z);
}

ARIKKEI_INLINE EleaVec3f*
elea_mat3x4f_transform_point_xyz (EleaVec3f* d, const EleaMat3x4f* mat, float x, float y, float z)
{
	d->x = mat->c[0] * x + mat->c[1] * y + mat->c[2] * z + mat->c[3];
	d->y = mat->c[4] * x + mat->c[5] * y + mat->c[6] * z + mat->c[7];
	d->z = mat->c[8] * x + mat->c[9] * y + mat->c[10] * z + mat->c[11];
	return d;
}

ARIKKEI_INLINE EleaVec3f*
elea_mat3x4f_transform_point (EleaVec3f* d, const EleaMat3x4f* mat, const EleaVec3f* v)
{
	return elea_mat3x4f_transform_point_xyz (d, mat, v->x, v->y, v->z);
}

ARIKKEI_INLINE EleaVec3f*
elea_mat3x4f_transform_vec3_xyz (EleaVec3f* d, const EleaMat3x4f* mat, float x, float y, float z)
{
	d->x = mat->c[0] * x + mat->c[1] * y + mat->c[2] * z;
	d->y = mat->c[4] * x + mat->c[5] * y + mat->c[6] * z;
	d->z = mat->c[8] * x + mat->c[9] * y + mat->c[10] * z;
	return d;
}

ARIKKEI_INLINE EleaVec3f *
elea_mat3x4f_transform_vec3 (EleaVec3f* d, const EleaMat3x4f* mat, const EleaVec3f* v)
{
	return elea_mat3x4f_transform_vec3_xyz (d, mat, v->x, v->y, v->z);
}

EleaPlane3f *elea_mat3x4f_transform_plane (EleaPlane3f *d, const EleaMat3x4f *mat, EleaPlane3f *s);
EleaAABox3f *elea_mat3x4f_transform_aab (EleaAABox3f *d, const EleaMat3x4f *mat, const EleaAABox3f *s);

#ifdef __cplusplus
};
#endif

#endif
