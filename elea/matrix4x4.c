#define __ELEA_MATRIX4X4_C__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2021
 *
 */

#include <arikkei/arikkei-strlib.h>
#include <az/class.h>
#include <az/field.h>
#include <az/serialization.h>

#include <elea/matrix3x4.h>
#include <elea/matrix4x4.h>
#include <elea/plane.h>

typedef struct _EleaMat4x4fClass EleaMat4x4fClass;

struct _EleaMat4x4fClass {
	AZClass az_klass;
};

const EleaMat4x4f EleaMat4x4fIdentity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

static void matrix_class_init (EleaMat4x4fClass *klass);
static void matrix_init (EleaMat4x4fClass *klass, EleaMat4x4f *mat);
/* AZClass implementation */
static unsigned int matrix_get_property (const AZImplementation *impl, void *instance, unsigned int idx, const AZImplementation **prop_impl, AZValue *prop_val, AZContext *ctx);
static unsigned int matrix_set_property (const AZImplementation *impl, void *instance, unsigned int idx, const AZImplementation *prop_impl, void *prop_inst, AZContext *ctx);
static unsigned int matrix_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx);
static unsigned int matrix_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx);
static unsigned int matrix_to_string (const AZImplementation *impl, void *instance, unsigned char *buf, unsigned int len);

enum {
	/* Functions */
	NUM_FUNCTIONS,
	/* Values */
	PROP_IDENTITY = NUM_FUNCTIONS,
	PROP_TRANSLATION,
	NUM_PROPERTIES
};

static unsigned int mat_type = 0;
static EleaMat4x4fClass *mat_class;

unsigned int
elea_mat4x4f_get_type (void)
{
	if (!mat_type) {
		az_register_type (&mat_type, (const unsigned char *) "Matrix4x4f", AZ_TYPE_STRUCT, sizeof (EleaMat4x4fClass), sizeof (EleaMat4x4f), AZ_CLASS_IS_FINAL,
			(void (*) (AZClass *)) matrix_class_init,
			(void (*) (const AZImplementation *, void *)) matrix_init,
			NULL);
		mat_class = (EleaMat4x4fClass *) az_type_get_class (mat_type);
		/* Ensure required types are registered */
		elea_vec3f_get_type ();
	}
	return mat_type;
}

static void
matrix_class_init (EleaMat4x4fClass *klass)
{
	klass->az_klass.alignment = 16;
	az_class_set_num_properties ((AZClass *) klass, NUM_PROPERTIES);
	/* fixme: Implement READ_VALUE_PTR type? */
	az_class_define_property ((AZClass *) klass, PROP_IDENTITY, (const unsigned char *) "IDENTITY", ELEA_TYPE_MATRIX4X4F, 1,
		AZ_FIELD_CLASS, AZ_FIELD_READ_STORED_STATIC, AZ_FIELD_WRITE_NONE, 0, (const AZImplementation *) klass, (void *) &EleaMat4x4fIdentity);
	az_class_define_property ((AZClass *) klass, PROP_TRANSLATION, (const unsigned char *) "translation", ELEA_TYPE_VECTOR4F, 0, AZ_FIELD_INSTANCE, AZ_FIELD_READ_METHOD, AZ_FIELD_WRITE_METHOD, 0, NULL, NULL);

	klass->az_klass.get_property = matrix_get_property;
	klass->az_klass.set_property = matrix_set_property;
	klass->az_klass.serialize = matrix_serialize;
	klass->az_klass.deserialize = matrix_deserialize;
	klass->az_klass.to_string = matrix_to_string;
}

static void
matrix_init (EleaMat4x4fClass *klass, EleaMat4x4f *mat)
{
	*mat = EleaMat4x4fIdentity;
}

static unsigned int
matrix_get_property (const AZImplementation *impl, void *inst, unsigned int idx, const AZImplementation **prop_impl, AZValue *prop_val, AZContext *ctx)
{
	EleaMat4x4f *mat = (EleaMat4x4f *) inst;
	switch (idx) {
	case PROP_TRANSLATION:
		*prop_impl = (AZImplementation *) az_type_get_class (ELEA_TYPE_VECTOR4F);
		elea_mat4x4f_get_col_vec ((EleaVec4f *) prop_val, mat, 3);
		break;
	default:
		return 0;
	}
	return 1;
}

static unsigned int
matrix_set_property (const AZImplementation *impl, void *inst, unsigned int idx, const AZImplementation *prop_impl, void *prop_inst, AZContext *ctx)
{
	EleaMat4x4f *mat = (EleaMat4x4f *) inst;
	switch (idx) {
	case PROP_TRANSLATION:
		elea_mat4x4f_set_col_vec (mat, 3, (EleaVec4f *) prop_inst);
		break;
	default:
		return 0;
	}
	return 1;
}

static unsigned int
matrix_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx)
{
	EleaMat4x4f *mat = (EleaMat4x4f *) inst;
	if (dlen > 64) {
		unsigned int i;
		for (i = 0; i < 16; i++) az_serialize_float (d + 4 * i, 4, &mat->c[i]);
	}
	return 64;
}

static unsigned int
matrix_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx)
{
	EleaMat4x4f *mat = (EleaMat4x4f *) value;
	unsigned int i;
	if (slen < 64) return 0;
	for (i = 0; i < 16; i++) az_deserialize_float (&mat->c[i], s + 4 * i, 4);
	return 64;
}

static unsigned int
matrix_to_string (const AZImplementation *impl, void *instance, unsigned char *d, unsigned int dlen)
{
	EleaMat4x4f *mat = (EleaMat4x4f *) instance;
	unsigned int pos = 0, i;
	if (pos < dlen) d[pos++] = '(';
	for (i = 0; i < 15; i++) {
		pos += arikkei_dtoa_exp (d + pos, (dlen > pos) ? dlen - pos : 0, mat->c[i], 6, -5, 5);
		if (pos < dlen) d[pos++] = ',';
	}
	pos += arikkei_dtoa_exp (d + pos, (dlen > pos) ? dlen - pos : 0, mat->c[15], 6, -5, 5);
	if (pos < dlen) d[pos++] = ')';
	if (pos < dlen) d[pos] = 0;
	return pos;
}

EleaMat4x4f *
elea_mat4x4f_set_mat3x4f (EleaMat4x4f *dst, const EleaMat3x4f *src)
{
	unsigned int r, c;
	for (r = 0; r < 3; r++) {
		for (c = 0; c < 4; c++) {
			dst->c[4 * c + r] = src->c[4 * r + c];
		}
	}
	dst->c[3] = dst->c[7] = dst->c[11] = 0;
	dst->c[15] = 1;
	return dst;
}

EleaMat4x4f *
elea_mat4x4f_set_ortho (EleaMat4x4f *dst, float left, float right, float bottom, float top, float near, float far)
{
	*dst = EleaMat4x4fIdentity;
	dst->c[0] = 2 / (right - left);
	dst->c[5] = 2 / (top - bottom);
	dst->c[10] = -2 / (far - near);
	dst->c[12] = -(right + left) / (right - left);
	dst->c[13] = -(top + bottom) / (top - bottom);
	dst->c[14] = -(far + near) / (far - near);
	return dst;
}

EleaMat4x4f *
elea_mat4x4f_set_frustum (EleaMat4x4f *dst, float left, float right, float bottom, float top, float near, float far)
{
	*dst = EleaMat4x4fIdentity;
	dst->c[0] = (2 * near) / (right - left);
	dst->c[5] = (2 * near) / (top - bottom);
	dst->c[8] = (right + left) / (right - left);
	dst->c[9] = (top + bottom) / (top - bottom);
	dst->c[10] = -(far + near) / (far - near);
	dst->c[11] = -1;
	dst->c[14] = -2 * far * near / (far - near);
	dst->c[15] = 0;
	return dst;
}

static inline float
D2 (const float *v, int r0, int r1, int c0, int c1)
{
	return v[r0 + c0] * v[r1 + c1] - v[r1 + c0] * v[r0 + c1];
}

static inline float
D3 (const float *v, int r0, int r1, int r2, int c0, int c1, int c2)
{
	return v[r0 + c0] * D2 (v, r1, r2, c1, c2) - v[r0 + c1] * D2 (v, r1, r2, c0, c2) + v[r0 + c2] * D2 (v, r1, r2, c0, c1);
}

float
elea_mat4x4f_determinant (const EleaMat4x4f *mat)
{
	float D00 = D3 (mat->c, 1, 2, 3, 4, 8, 12);
	float D01 = D3 (mat->c, 1, 2, 3, 0, 8, 12);
	float D02 = D3 (mat->c, 1, 2, 3, 0, 4, 12);
	float D03 = D3 (mat->c, 1, 2, 3, 0, 4, 8);
	return mat->c[0] * D00 - mat->c[4] * D01 + mat->c[8] * D02 - mat->c[12] * D03;
}

EleaMat4x4f *
elea_mat4x4f_invert (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict mat)
{
	float D00 = D3(mat->c, 1, 2, 3, 4, 8, 12);
	float D01 = D3(mat->c, 1, 2, 3, 0, 8, 12);
	float D02 = D3(mat->c, 1, 2, 3, 0, 4, 12);
	float D03 = D3(mat->c, 1, 2, 3, 0, 4, 8);
	float d = mat->c[0] * D00 - mat->c[4] * D01 + mat->c[8] * D02 - mat->c[12] * D03;
	if (!d) {
		*dst = EleaMat4x4fIdentity;
	} else {
		float rd = 1 / d;
		dst->c[0]  =  rd * D00;
		dst->c[1]  = -rd * D3(mat->c, 1, 2, 3, 0, 8, 12);
		dst->c[2]  =  rd * D3(mat->c, 1, 2, 3, 0, 4, 12);
		dst->c[3]  = -rd * D3(mat->c, 1, 2, 3, 0, 4, 8);
		dst->c[4]  = -rd * D01;
		dst->c[5]  =  rd * D3(mat->c, 0, 2, 3, 0, 8, 12);
		dst->c[6]  = -rd * D3(mat->c, 0, 2, 3, 0, 4, 12);
		dst->c[7]  =  rd * D3(mat->c, 0, 2, 3, 0, 4, 8);
		dst->c[8]  =  rd * D02;
		dst->c[9]  = -rd * D3(mat->c, 0, 1, 3, 0, 8, 12);
		dst->c[10] =  rd * D3(mat->c, 0, 1, 3, 0, 4, 12);
		dst->c[11] = -rd * D3(mat->c, 0, 1, 3, 0, 4, 8);
		dst->c[12] = -rd * D03;
		dst->c[13] =  rd * D3(mat->c, 0, 1, 2, 0, 8, 12);
		dst->c[14] = -rd * D3(mat->c, 0, 1, 2, 0, 4, 12);
		dst->c[15] =  rd * D3(mat->c, 0, 1, 2, 0, 4, 8);
	}
	return dst;
}

EleaMat4x4f *
elea_mat4x4f_transpose (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict mat)
{
	dst->c[0] = mat->c[0];
	dst->c[1] = mat->c[4];
	dst->c[2] = mat->c[8];
	dst->c[3] = mat->c[12];
	dst->c[4] = mat->c[1];
	dst->c[5] = mat->c[5];
	dst->c[6] = mat->c[9];
	dst->c[7] = mat->c[13];
	dst->c[8] = mat->c[2];
	dst->c[9] = mat->c[6];
	dst->c[10] = mat->c[10];
	dst->c[11] = mat->c[14];
	dst->c[12] = mat->c[3];
	dst->c[13] = mat->c[7];
	dst->c[14] = mat->c[11];
	dst->c[15] = mat->c[15];
	return dst;
}

EleaMat4x4f *
elea_mat4x4f_multiply (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict lhs, const EleaMat4x4f *__restrict rhs)
{
	unsigned int i;
	for (i = 0; i < 4; i++) {
		dst->c[0 + i] = lhs->c[0 + i] * rhs->c[0] + lhs->c[4 + i] * rhs->c[1] + lhs->c[8 + i] * rhs->c[2] + lhs->c[12 + i] * rhs->c[3];
		dst->c[4 + i] = lhs->c[0 + i] * rhs->c[4] + lhs->c[4 + i] * rhs->c[5] + lhs->c[8 + i] * rhs->c[6] + lhs->c[12 + i] * rhs->c[7];
		dst->c[8 + i] = lhs->c[0 + i] * rhs->c[8] + lhs->c[4 + i] * rhs->c[9] + lhs->c[8 + i] * rhs->c[10] + lhs->c[12 + i] * rhs->c[11];
		dst->c[12 + i] = lhs->c[0 + i] * rhs->c[12] + lhs->c[4 + i] * rhs->c[13] + lhs->c[8 + i] * rhs->c[14] + lhs->c[12 + i] * rhs->c[15];
	}
	return dst;
}

EleaMat4x4f *
elea_mat4x4f_multiply_mat3x4 (EleaMat4x4f *__restrict dst, const EleaMat4x4f *__restrict lhs, const EleaMat3x4f *__restrict rhs)
{
	unsigned int i;
	for (i = 0; i < 4; i++) {
		dst->c[0 + i] = lhs->c[0 + i] * rhs->c[0] + lhs->c[4 + i] * rhs->c[4] + lhs->c[8 + i] * rhs->c[8];
		dst->c[4 + i] = lhs->c[0 + i] * rhs->c[1] + lhs->c[4 + i] * rhs->c[5] + lhs->c[8 + i] * rhs->c[9];
		dst->c[8 + i] = lhs->c[0 + i] * rhs->c[2] + lhs->c[4 + i] * rhs->c[6] + lhs->c[8 + i] * rhs->c[10];
		dst->c[12 + i] = lhs->c[0 + i] * rhs->c[3] + lhs->c[4 + i] * rhs->c[7] + lhs->c[8 + i] * rhs->c[11] + lhs->c[12 + i];
	}
	return dst;
}

void
elea_mat4x4f_get_sides_of_projection (EleaPlane3f *planes, const EleaMat4x4f *m)
{
	elea_plane3fp_set_xyzd (&planes[0], -(m->c[0] + m->c[3]), -(m->c[4] + m->c[7]), -(m->c[8]  + m->c[11]), -(m->c[12] + m->c[15]));
	elea_plane3fp_set_xyzd (&planes[1],  (m->c[0] - m->c[3]),  (m->c[4] - m->c[7]),  (m->c[8]  - m->c[11]),  (m->c[12] - m->c[15]));

	elea_plane3fp_set_xyzd (&planes[2], -(m->c[1] + m->c[3]), -(m->c[5] + m->c[7]), -(m->c[9]  + m->c[11]), -(m->c[13] + m->c[15]));
	elea_plane3fp_set_xyzd (&planes[3],  (m->c[1] - m->c[3]),  (m->c[5] - m->c[7]),  (m->c[9]  - m->c[11]),  (m->c[13] - m->c[15]));

	elea_plane3fp_set_xyzd (&planes[4], -(m->c[2] + m->c[3]), -(m->c[6] + m->c[7]), -(m->c[10] + m->c[11]), -(m->c[14] + m->c[15]));
	elea_plane3fp_set_xyzd (&planes[5],  (m->c[2] - m->c[3]),  (m->c[6] - m->c[7]),  (m->c[10] - m->c[11]),  (m->c[14] - m->c[15]));

	for (unsigned int i = 0; i <= 5; i++) {
		planes[i] = elea_plane3f_normalize (&planes[i]);
	}
}

void
elea_mat4x4f_get_corners_of_rev_projection (EleaVec3f *corners, const EleaMat4x4f *rproj)
{
	// Ceye =  ModelView X Cobj
	// Cclip = Projection X Ceye
	// Cnormdev = Cclip / perspective

	static const float Cd[] = { -1, -1, -1, 1, -1, -1, 1, 1, -1, -1, 1, -1, -1, -1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1 };
	for (int i = 0; i < 8; i++) {
		// Clip coordinates
		// Final W has to be 1
		// R[3] * Xc + R[7] * Yc + R[11] * Zc + R[15] * Wc = 1
		// R[3] * Xd * Wc + R[7] * Yd * Wc + R[11] * Zd * Wc + R[15] * Wc = 1
		// Wc * (R[3] * Xd + R[7] * Yd + R[11] * Zd + R[15]) = 1
		// Wc = 1 / (R[3] * Xd + R[7] * Yd + R[11] * Zd + R[15]);
		// f32 Wc = 1 / (c[3] * Cd[3 * i + X] + c[7] * Cd[3 * i + Y] + c[11] * Cd[3 * i + Z] + c[15]);
		float d = rproj->c[3] * Cd[3 * i + 0] + rproj->c[7] * Cd[3 * i + 1] + rproj->c[11] * Cd[3 * i + 2] + rproj->c[15];
		float Wc = 1 / d;
		float Xc = Cd[3 * i + 0] * Wc;
		float Yc = Cd[3 * i + 1] * Wc;
		float Zc = Cd[3 * i + 2] * Wc;
		// Eye coordinates
		EleaVec4f Ce;
		elea_mat4x4f_transform_vec_xyzw (&Ce, rproj, Xc, Yc, Zc, Wc);
		// Go to three dimensions
		memcpy (&corners[i], &Ce, 12);
	}
}
