#ifndef __ELEA_MATRIX4X4_H__
#define __ELEA_MATRIX4X4_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2021
 *
 */

/*
 * Column-major matrix
 */

#define ELEA_TYPE_MATRIX4X4F elea_mat4x4f_get_type ()

#include <math.h>
#include <string.h>

#include <arikkei/arikkei-utils.h>
#include <elea/types.h>
#include <elea/vector3.h>
#include <elea/vector4.h>

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

#ifdef __cplusplus
extern "C" {
#endif

const extern EleaMat4x4f EleaMat4x4fIdentity;

struct _EleaMat4x4f {
	float c[16];
} ARIKKEI_ALIGN_16;

unsigned int elea_mat4x4f_get_type (void);

EleaMat4x4f *elea_mat4x4f_set_mat3x4f (EleaMat4x4f *dst, const EleaMat3x4f *src);
EleaMat4x4f *elea_mat4x4f_set_ortho (EleaMat4x4f *dst, float left, float right, float bottom, float top, float near, float far);
ARIKKEI_INLINE
EleaMat4x4f *elea_mat4x4f_set_ortho_wh (EleaMat4x4f *dst, float width, float height, float near, float far)
{
	return elea_mat4x4f_set_ortho (dst, -width / 2, width / 2, -height / 2, height / 2, near, far);
}
EleaMat4x4f *elea_mat4x4f_set_frustum (EleaMat4x4f *dst, float left, float right, float bottom, float top, float near, float far);
ARIKKEI_INLINE
EleaMat4x4f *elea_mat4x4f_set_frustum_fov (EleaMat4x4f *dst, float fovx, float aspect, float near, float far)
{
	float x2 = fovx / 2;
	float y2 = x2 / aspect;
	return elea_mat4x4f_set_frustum (dst, -x2, x2, -y2, y2, near, far);
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_set_row_vec_xyzw (EleaMat4x4f *dst, unsigned int r, float x, float y, float z, float w)
{
	dst->c[r] = x;
	dst->c[r + 4] = y;
	dst->c[r + 8] = z;
	dst->c[r + 12] = w;
	return dst;
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_set_row_vec (EleaMat4x4f *dst, unsigned int r, const EleaVec4f *v)
{
	return elea_mat4x4f_set_row_vec_xyzw (dst, r, v->x, v->y, v->z, v->w);
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_set_col_vec_xyzw (EleaMat4x4f *dst, unsigned int c, float x, float y, float z, float w)
{
	dst->c[4 * c] = x;
	dst->c[4 * c + 1] = y;
	dst->c[4 * c + 2] = z;
	dst->c[4 * c + 3] = w;
	return dst;
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_set_col_vec (EleaMat4x4f *dst, unsigned int c, const EleaVec4f *v)
{
	return elea_mat4x4f_set_col_vec_xyzw (dst, c, v->x, v->y, v->z, v->w);
}

ELEA_INLINE EleaVec4f *
elea_mat4x4f_get_row_vec (EleaVec4f *dst, const EleaMat4x4f *mat, unsigned int r)
{
	dst->x = mat->c[r];
	dst->y = mat->c[4 + r];
	dst->z = mat->c[8 + r];
	dst->w = mat->c[12 + r];
	return dst;
}

ELEA_INLINE EleaVec4f *
elea_mat4x4f_get_col_vec (EleaVec4f *dst, const EleaMat4x4f *mat, unsigned int c)
{
	memcpy (dst, &mat->c[4 * c], sizeof (EleaVec4f));
	return dst;
}

#define elea_mat4x4f_set_translation_xyz(m,x,y,z) elea_mat4x4f_set_col_vec_xyzw (m, 3, x, y, z, 1)

float elea_mat4x4f_determinant (const EleaMat4x4f *mat);
EleaMat4x4f *elea_mat4x4f_invert (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict mat);
ARIKKEI_INLINE
EleaMat4x4f *elea_mat4x4f_invert_self (EleaMat4x4f *mat) {
	EleaMat4x4f tmp;
	elea_mat4x4f_invert (&tmp, mat);
	memcpy (mat, &tmp, 64);
        return mat;
}
EleaMat4x4f *elea_mat4x4f_transpose (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict mat);

EleaMat4x4f *elea_mat4x4f_multiply (EleaMat4x4f * __restrict dst, const EleaMat4x4f * __restrict lhs, const EleaMat4x4f * __restrict rhs);

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_multiply_self_right (EleaMat4x4f * __restrict mat, const EleaMat4x4f * __restrict rhs)
{
	EleaMat4x4f lhs;
	memcpy (lhs.c, mat, 64);
	return elea_mat4x4f_multiply (mat, &lhs, rhs);
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_multiply_self_left (EleaMat4x4f * __restrict mat, const EleaMat4x4f * __restrict lhs)
{
	EleaMat4x4f rhs;
	memcpy (rhs.c, mat, 64);
	return elea_mat4x4f_multiply (mat, lhs, &rhs);
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_translate_right_xyz (EleaMat4x4f *dst, const EleaMat4x4f *__restrict src, float x, float y, float z)
{
	memcpy (dst, src, 48);
	dst->c[12] = src->c[0] * x + src->c[4] * y + src->c[8] * z + src->c[12];
	dst->c[13] = src->c[1] * x + src->c[5] * y + src->c[9] * z + src->c[13];
	dst->c[14] = src->c[2] * x + src->c[6] * y + src->c[10] * z + src->c[14];
	dst->c[15] = src->c[15];
	return dst;
}

ELEA_INLINE EleaMat4x4f *
elea_mat4x4f_translate_left_xyz (EleaMat4x4f *dst, const EleaMat4x4f *__restrict src, float x, float y, float z)
{
	memcpy (dst, src, 48);
	dst->c[12] = src->c[12] + x;
	dst->c[13] = src->c[13] + y;
	dst->c[14] = src->c[14] + z;
	dst->c[15] = src->c[15];
	return dst;
}

EleaMat4x4f *elea_mat4x4f_multiply_mat3x4 (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict lhs, const EleaMat3x4f *__restrict rhs);

ARIKKEI_INLINE EleaVec4f *
elea_mat4x4f_transform_vec_xyzw (EleaVec4f *d, const EleaMat4x4f *mat, float x, float y, float z, float w)
{
	d->x = mat->c[0] * x + mat->c[4] * y + mat->c[8] * z + mat->c[12] * w;
	d->y = mat->c[1] * x + mat->c[5] * y + mat->c[9] * z + mat->c[13] * w;
	d->z = mat->c[2] * x + mat->c[6] * y + mat->c[10] * z + mat->c[14] * w;
	d->w = mat->c[3] * x + mat->c[7] * y + mat->c[11] * z + mat->c[15] * w;
	return d;
}

ARIKKEI_INLINE EleaVec4f *
elea_mat4x4f_transform_vec (EleaVec4f *d, const EleaMat4x4f *mat, const EleaVec4f *v)
{
	return elea_mat4x4f_transform_vec_xyzw (d, mat, v->x, v->y, v->z, v->w);
}

/**
 * @brief transform 3D point
 * 
 * Transform 3D *point* (i.e. w = 1) with 4x4 matrix.
 * @param d destination vector
 * @param mat matrix
 * @param x x-coordinate
 * @param y y-coordinate
 * @param z z-coordinate
 * @return destination 
 */
ARIKKEI_INLINE EleaVec3f *
elea_mat4x4f_transform_vec3_xyz0 (EleaVec3f *d, const EleaMat4x4f *mat, float x, float y, float z)
{
	d->x = mat->c[0] * x + mat->c[4] * y + mat->c[8] * z;
	d->y = mat->c[1] * x + mat->c[5] * y + mat->c[9] * z;
	d->z = mat->c[2] * x + mat->c[6] * y + mat->c[10] * z;
	return d;
}

/**
 * @brief transform 3D vector
 * 
 * Transform 3D *vector* (i.e. w = 0) with 4x4 matrix.
 * @param d destination vector
 * @param mat matrix
 * @param x x-coordinate
 * @param y y-coordinate
 * @param z z-coordinate
 * @return destination 
 */
ARIKKEI_INLINE EleaVec3f *
elea_mat4x4f_transform_vec3_xyz1 (EleaVec3f *d, const EleaMat4x4f *mat, float x, float y, float z)
{
	d->x = mat->c[0] * x + mat->c[4] * y + mat->c[8] * z + mat->c[12];
	d->y = mat->c[1] * x + mat->c[5] * y + mat->c[9] * z + mat->c[13];
	d->z = mat->c[2] * x + mat->c[6] * y + mat->c[10] * z + mat->c[14];
	return d;
}

ARIKKEI_INLINE EleaVec3f *
elea_mat4x4f_transform_vec3_0 (EleaVec3f *d, const EleaMat4x4f *mat, const EleaVec3f *v)
{
	return elea_mat4x4f_transform_vec3_xyz0 (d, mat, v->x, v->y, v->z);
}

ARIKKEI_INLINE EleaVec3f *
elea_mat4x4f_transform_vec3_1 (EleaVec3f *d, const EleaMat4x4f *mat, const EleaVec3f *v)
{
	return elea_mat4x4f_transform_vec3_xyz1 (d, mat, v->x, v->y, v->z);
}

/**
 * @brief Find the sides of projection frustum/cuboid
 * 
 * Get all 6 sides of projection shape (frustum for perspective, cuboid for orthographic).
 * Planes are normalized and facing outwards.
 * The order of planes is LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR
 * @param planes destination planes in view space
 * @param m projection matrix
 */

void elea_mat4x4f_get_sides_of_projection (EleaPlane3f *planes, const EleaMat4x4f *m);
/**
 * @brief Find the corners of projection fustum/cuboid
 * 
 * Get all 8 corners of projection shape (frustum for perspective, cuboid for orthographic)
 * given *reverse* projection matrix.
 * @param corners destination points in view space
 * @param rproj reverse projection matrix
 */
void elea_mat4x4f_get_corners_of_rev_projection (EleaVec3f *corners, const EleaMat4x4f *rproj);

#ifdef __cplusplus
};
#endif

#endif
