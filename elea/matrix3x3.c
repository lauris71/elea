#define __ELEA_MATRIX3X3_CPP__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2020
 *
 */

#include <az/field.h>
#include <az/extend.h>

#include <elea/aabox.h>
#include <elea/private.h>

#include <elea/matrix3x3.h>

typedef struct _EleaMat3x3fClass EleaMat3x3fClass;

struct _EleaMat3x3fClass {
	AZClass az_klass;
};

const EleaMat3x3f EleaMat3x3fIdentity = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

static void matrix_class_init (EleaMat3x3fClass *klass);
static void matrix_init (EleaMat3x3fClass *klass, EleaMat3x3f *mat);
/* AZClass implementation */
static unsigned int matrix_get_property (const AZImplementation *impl, void *instance, unsigned int idx, const AZImplementation **prop_impl, AZValue *prop_val, AZContext *ctx);
static unsigned int matrix_set_property (const AZImplementation *impl, void *instance, unsigned int idx, const AZImplementation *prop_impl, void *prop_inst, AZContext *ctx);

enum {
	/* Functions */
	NUM_FUNCTIONS,
	/* Values */
	NUM_PROPERTIES
};

static unsigned int mat_type = 0;

unsigned int
elea_mat3x3f_get_type (void)
{
	if (!mat_type) {
		az_register_type (&mat_type, (const unsigned char *) "Matrix3x3f", AZ_TYPE_STRUCT, sizeof (EleaMat3x3fClass), sizeof (EleaMat3x3f), AZ_FLAG_FINAL, 0, 0,
			(void (*) (AZClass *)) matrix_class_init,
			(void (*) (const AZImplementation *, void *)) matrix_init,
			NULL);
	}
	return mat_type;
}

static void
matrix_class_init (EleaMat3x3fClass *klass)
{
	klass->az_klass.get_property = matrix_get_property;
	klass->az_klass.set_property = matrix_set_property;
	klass->az_klass.serialize = vec_serialize;
	klass->az_klass.deserialize = vec_deserialize;
	klass->az_klass.to_string = vec_to_string;
}

static void
matrix_init (EleaMat3x3fClass *klass, EleaMat3x3f *mat)
{
	*mat = EleaMat3x3fIdentity;
}

static unsigned int
matrix_get_property (const AZImplementation *impl, void *inst, unsigned int idx, const AZImplementation **prop_impl, AZValue *prop_val, AZContext *ctx)
{
	return 0;
}

static unsigned int
matrix_set_property (const AZImplementation *impl, void *inst, unsigned int idx, const AZImplementation *prop_impl, void *prop_inst, AZContext *ctx)
{
	return 0;
}

EleaMat3x3f *
elea_mat3x3f_transpose (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict mat)
{
	/* Transpose rotation */
	dst->c[0] = mat->c[0];
	dst->c[1] = mat->c[3];
	dst->c[2] = mat->c[6];
	dst->c[3] = mat->c[1];
	dst->c[4] = mat->c[4];
	dst->c[5] = mat->c[7];
	dst->c[6] = mat->c[2];
	dst->c[7] = mat->c[5];
	dst->c[8] = mat->c[8];
	return dst;
}

EleaMat3x3f *
elea_mat3x3f_transpose_self (EleaMat3x3f* mat)
{
	float t;
	/* Transpose rotation */
	t = mat->c[1];
	mat->c[1] = mat->c[3];
	mat->c[3] = t;
	t = mat->c[2];
	mat->c[2] = mat->c[6];
	mat->c[6] = t;
	t = mat->c[5];
	mat->c[5] = mat->c[7];
	mat->c[7] = t;
	return mat;
}

/* 2x2 minor by row and col addresses */

static inline float
D2 (const float *v, int r0, int r1, int c0, int c1)
{
	return v[r0 + c0] * v[r1 + c1] - v[r1 + c0] * v[r0 + c1];
}

/* 3x3 minor by row and col addresses */

static inline float
D3 (const float *v, int r0, int r1, int r2, int c0, int c1, int c2)
{
	return v[r0 + c0] * D2 (v, r1, r2, c1, c2) - v[r0 + c1] * D2 (v, r1, r2, c0, c2) + v[r0 + c2] * D2 (v, r1, r2, c0, c1);
}

EleaMat3x3f *
elea_mat3x3f_invert (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict mat)
{
	/* Determinant */
	float D = D3 (mat->c, 0, 1, 2, 0, 3, 6);
	if (!D) {
		*dst = EleaMat3x3fIdentity;
	} else {
		float rd = 1 / D;
		/* Inversion */
		dst->c[0] = rd * D2 (mat->c, 1, 2, 3, 6);
		dst->c[1] = -rd * D2 (mat->c, 0, 2, 3, 6);
		dst->c[2] = rd * D2 (mat->c, 0, 1, 3, 6);

		dst->c[3] = -rd * D2 (mat->c, 1, 2, 0, 6);
		dst->c[4] = rd * D2 (mat->c, 0, 2, 0, 6);
		dst->c[5] = -rd * D2 (mat->c, 0, 1, 0, 6);

		dst->c[6] = rd * D2 (mat->c, 1, 2, 0, 3);
		dst->c[7] = -rd * D2 (mat->c, 0, 2, 0, 3);
		dst->c[8] = rd * D2 (mat->c, 0, 1, 0, 3);
	}
	return dst;
}

EleaMat3x3f *
elea_mat3x3f_invert_self (EleaMat3x3f *mat)
{
	EleaMat3x3f inv;
	elea_mat3x3f_invert (&inv, mat);
	*mat = inv;
	return mat;
}

EleaMat3x3f *
elea_mat3x3f_multiply (EleaMat3x3f * __restrict dst, const EleaMat3x3f * __restrict lhs, const EleaMat3x3f * __restrict rhs)
{
	unsigned int i;
	for (i = 0; i < 4; i++) {
		dst->c[3 * i + 0] = lhs->c[0] * rhs->c[3 * i + 0] + lhs->c[3] * rhs->c[3 * i + 1] + lhs->c[6] * rhs->c[3 * i + 2];
		dst->c[3 * i + 1] = lhs->c[1] * rhs->c[3 * i + 0] + lhs->c[4] * rhs->c[3 * i + 1] + lhs->c[7] * rhs->c[3 * i + 2];
		dst->c[3 * i + 2] = lhs->c[2] * rhs->c[3 * i + 0] + lhs->c[5] * rhs->c[3 * i + 1] + lhs->c[8] * rhs->c[3 * i + 2];
	}
	return dst;
}
