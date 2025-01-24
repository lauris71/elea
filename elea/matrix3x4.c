#define __ELEA_MATRIX3X4_C__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <arikkei/arikkei-strlib.h>
#include <az/class.h>
#include <az/field.h>
#include <az/serialization.h>

#include <elea/aabox.h>
#include <elea/matrix3x4.h>
#include <elea/plane.h>
#include <elea/quaternion.h>

typedef struct _EleaMat3x4fClass EleaMat3x4fClass;

struct _EleaMat3x4fClass {
	AZClass az_klass;
};

const EleaMat3x4f EleaMat3x4fIdentity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 };

static void matrix_class_init (EleaMat3x4fClass *klass);
static void matrix_init (EleaMat3x4fClass *klass, EleaMat3x4f *mat);
/* AZClass implementation */
static unsigned int matrix_get_property (const AZImplementation *impl, void *instance, unsigned int idx, const AZImplementation **prop_impl, AZValue *prop_val, AZContext *ctx);
static unsigned int matrix_set_property (const AZImplementation *impl, void *instance, unsigned int idx, const AZImplementation *prop_impl, void *prop_inst, AZContext *ctx);
static unsigned int matrix_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx);
static unsigned int matrix_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx);
static unsigned int matrix_to_string (const AZImplementation* impl, void *instance, unsigned char *buf, unsigned int len);

static unsigned int matrix_call_invert (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int matrix_call_invertNormalized (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int matrix_call_transformPoint3 (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int matrix_call_transformVector3 (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);

enum {
	/* Functions */
	FUNC_INVERT,
	FUNC_INVERT_NORMALIZED,
	FUNC_TRANSFORM_POINT3,
	FUNC_TRANSFORM_POINT3_XYZ,
	FUNC_TRANSFORM_VECTOR3,
	FUNC_TRANSFORM_VECTOR3_XYZ,
	NUM_FUNCTIONS,
	/* Values */
	PROP_IDENTITY = NUM_FUNCTIONS,
	PROP_TRANSLATION,
	NUM_PROPERTIES
};

static unsigned int mat_type = 0;
static EleaMat3x4fClass *mat_class;

unsigned int
elea_mat3x4f_get_type (void)
{
	if (!mat_type) {
		az_register_type (&mat_type, (const unsigned char *) "Matrix3x4f", AZ_TYPE_STRUCT, sizeof (EleaMat3x4fClass), sizeof (EleaMat3x4f), AZ_CLASS_IS_FINAL,
			(void (*) (AZClass *)) matrix_class_init,
			(void (*) (const AZImplementation *, void *)) matrix_init,
			NULL);
		mat_class = (EleaMat3x4fClass *) az_type_get_class (mat_type);
		/* Ensure required types are registered */
		elea_vec3f_get_type ();
	}
	return mat_type;
}

static void
matrix_class_init (EleaMat3x4fClass *klass)
{
	klass->az_klass.alignment = 16;
	az_class_set_num_properties ((AZClass *) klass, NUM_PROPERTIES);
	az_class_define_method_va ((AZClass *) klass, FUNC_INVERT, (const unsigned char *) "invert", matrix_call_invert, ELEA_TYPE_MATRIX3X4F, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_INVERT_NORMALIZED, (const unsigned char *) "invertNormalized", matrix_call_invertNormalized, ELEA_TYPE_MATRIX3X4F, 0);

	az_class_define_method_va ((AZClass *) klass, FUNC_TRANSFORM_POINT3, (const unsigned char *) "transformPoint3", matrix_call_transformPoint3, ELEA_TYPE_VECTOR3F, 1, ELEA_TYPE_VECTOR3F);
	az_class_define_method_va ((AZClass *) klass, FUNC_TRANSFORM_POINT3_XYZ, (const unsigned char *) "transformPoint3", matrix_call_transformPoint3, ELEA_TYPE_VECTOR3F, 3, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_TRANSFORM_VECTOR3, (const unsigned char *) "transformVector3", matrix_call_transformVector3, ELEA_TYPE_VECTOR3F, 1, ELEA_TYPE_VECTOR3F);
	az_class_define_method_va ((AZClass *) klass, FUNC_TRANSFORM_VECTOR3_XYZ, (const unsigned char *) "transformVector3", matrix_call_transformVector3, ELEA_TYPE_VECTOR3F, 3, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);

	/* fixme: Implement READ_VALUE_PTR type? */
	az_class_define_property ((AZClass *) klass, PROP_IDENTITY, (const unsigned char *) "IDENTITY", ELEA_TYPE_MATRIX3X4F, 1,
		AZ_FIELD_CLASS, AZ_FIELD_READ_STORED_STATIC, AZ_FIELD_WRITE_NONE, 0, (const AZImplementation *) klass, (void *) &EleaMat3x4fIdentity);
	az_class_define_property ((AZClass *) klass, PROP_TRANSLATION, (const unsigned char *) "translation", ELEA_TYPE_VECTOR3F, 0, AZ_FIELD_INSTANCE, AZ_FIELD_READ_METHOD, AZ_FIELD_WRITE_METHOD, 0, NULL, NULL);

	klass->az_klass.get_property = matrix_get_property;
	klass->az_klass.set_property = matrix_set_property;
	klass->az_klass.serialize = matrix_serialize;
	klass->az_klass.deserialize = matrix_deserialize;
	klass->az_klass.to_string = matrix_to_string;
}

static void
matrix_init (EleaMat3x4fClass *klass, EleaMat3x4f *mat)
{
	*mat = EleaMat3x4fIdentity;
}

static unsigned int
matrix_get_property (const AZImplementation *impl, void *inst, unsigned int idx, const AZImplementation **prop_impl, AZValue *prop_val, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) inst;
	switch (idx) {
	case PROP_TRANSLATION:
		*prop_impl = (AZImplementation *) az_type_get_class (ELEA_TYPE_VECTOR3F);
		elea_mat3x4f_get_translation ((EleaVec3f *) prop_val, mat);
		break;
	default:
		return 0;
	}
	return 1;
}

static unsigned int
matrix_set_property (const AZImplementation *impl, void *inst, unsigned int idx, const AZImplementation *prop_impl, void *prop_inst, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) inst;
	switch (idx) {
	case PROP_TRANSLATION:
		elea_mat3x4f_set_translation (mat, (EleaVec3f *) prop_inst);
		break;
	default:
		return 0;
	}
	return 1;
}

static unsigned int
matrix_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) inst;
	if (dlen > 48) {
		unsigned int i;
		for (i = 0; i < 12; i++) az_serialize_float (d + 4 * i, 4, &mat->c[i]);
	}
	return 48;
}

static unsigned int
matrix_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) value;
	unsigned int i;
	if (slen < 48) return 0;
	for (i = 0; i < 12; i++) az_deserialize_float (&mat->c[i], s + 4 * i, 4);
	return 48;
}

static unsigned int
matrix_to_string (const AZImplementation* impl, void *instance, unsigned char *d, unsigned int dlen)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) instance;
	unsigned int pos = 0, i;
	if (pos < dlen) d[pos++] = '(';
	for (i = 0; i < 11; i++) {
		pos += arikkei_dtoa_exp (d + pos, (dlen > pos) ? dlen - pos : 0, mat->c[i], 6, -5, 5);
		if (pos < dlen) d[pos++] = ',';
	}
	pos += arikkei_dtoa_exp (d + pos, (dlen > pos) ? dlen - pos : 0, mat->c[11], 6, -5, 5);
	if (pos < dlen) d[pos++] = ')';
	if (pos < dlen) d[pos] = 0;
	return pos;
}

static unsigned int
matrix_call_invert (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) arg_vals[0];
	*ret_impl = (const AZImplementation *) mat_class;
	elea_mat3x4f_invert ((EleaMat3x4f *) ret_val, mat);
	return 1;
}

static unsigned int
matrix_call_invertNormalized (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) arg_vals[0];
	*ret_impl = (const AZImplementation *) mat_class;
	elea_mat3x4f_invert_normalized ((EleaMat3x4f *) ret_val, mat);
	return 1;
}

static unsigned int
matrix_call_transformPoint3 (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) arg_vals[0];
	EleaVec3f *dst = (EleaVec3f *) ret_val;
	if (arg_impls[1]->type == AZ_TYPE_FLOAT) {
		elea_mat3x4f_transform_point_xyz (dst, mat, arg_vals[1]->float_v, arg_vals[2]->float_v, arg_vals[3]->float_v);
	} else {
		elea_mat3x4f_transform_point (dst, mat, (EleaVec3f *) arg_vals[1]);
	}
	*ret_impl = (const AZImplementation *) elea_vec3f_class;
	return 1;
}

static unsigned int
matrix_call_transformVector3 (const AZImplementation **arg_impls, const AZValue **arg_vals, const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	EleaMat3x4f *mat = (EleaMat3x4f *) arg_vals[0];
	EleaVec3f *dst = (EleaVec3f *) ret_val;
	if (arg_impls[1]->type == AZ_TYPE_FLOAT) {
		elea_mat3x4f_transform_vec3_xyz (dst, mat, arg_vals[1]->float_v, arg_vals[2]->float_v, arg_vals[3]->float_v);
	} else {
		elea_mat3x4f_transform_vec3 (dst, mat, (EleaVec3f *) arg_vals[1]);
	}
	*ret_impl = (const AZImplementation *) elea_vec3f_class;
	return 1;
}

EleaMat3x4f *
elea_mat3x4f_set_rotate (EleaMat3x4f* dst, const EleaQuatf* q)
{
	elea_mat3x4f_set_translation_xyz (dst, 0, 0, 0);
	elea_mat3x4f_set_rotation (dst, q);
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_set_look_at (EleaMat3x4f *dst, const EleaVec3f *vp, const EleaVec3f *tgt, const EleaVec3f *up)
{
	EleaVec3f dir, pd;
	dir = elea_vec3f_sub (*tgt, *vp);
	if (!elea_vec3f_len2 (dir)) {
		if (!elea_vec3fp_len (up)) {
			/* Neither direction nor up defined */
			*dst = EleaMat3x4fIdentity;
			elea_mat3x4f_set_translate_xyz (dst, vp->x, vp->y, vp->z);
			return dst;
		} else {
			/* Choose arbitrary dir as right */
			pd = elea_vec3f_get_perpendicular(*up);
			dir = elea_vec3f_cross (*up, pd);
		}
	}
	EleaVec3f axx, axy, axz;
	axz = elea_vec3f_inv (dir);
	axz = elea_vec3f_normalize (axz);
	axx = elea_vec3f_cross (*up, axz);
	if (!elea_vec3f_len2 (axx)) {
		axx = elea_vec3f_get_perpendicular (axz);
	}
	axx = elea_vec3f_normalize (axx);
	axy = elea_vec3f_cross (axz, axx);
	elea_mat3x4f_set_values (dst, axx.x, axy.x, axz.x, vp->x, axx.y, axy.y, axz.y, vp->y, axx.z, axy.z, axz.z, vp->z);
	return dst;
}


EleaMat3x4f *
elea_mat3x4f_set_rotation (EleaMat3x4f* dst, const EleaQuatf* q)
{
	dst->c[0] = 1 - 2 * (q->j * q->j + q->k * q->k);
	dst->c[1] = 2 * (q->i * q->j - q->k * q->r);
	dst->c[2] = 2 * (q->i * q->k + q->j * q->r);

	dst->c[4] = 2 * (q->i * q->j + q->k * q->r);
	dst->c[5] = 1 - 2 * (q->i * q->i + q->k * q->k);
	dst->c[6] = 2 * (q->j * q->k - q->i * q->r);

	dst->c[8] = 2 * (q->i * q->k - q->j * q->r);
	dst->c[9] = 2 * (q->j * q->k + q->i * q->r);
	dst->c[10] = 1 - 2 * (q->i * q->i + q->j * q->j);
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_set_rotation_angles_xyz (EleaMat3x4f *dst, float x, float y, float z)
{
	float c1 = cosf (x);
	float s1 = sinf (x);
	float c2 = cosf (y);
	float s2 = sinf (y);
	float c3 = cosf (z);
	float s3 = sinf (z);
	dst->c[0] = c2 * c3;
	dst->c[1] = -c2 * s3;
	dst->c[2] = s2;
	dst->c[4] = c1 * s3 + s1 * s2 * c3;
	dst->c[5] = c1 * c3 - s1 * s2 * s3;
	dst->c[6] = -s1 * c2;
	dst->c[8] = s1 * s3 - c1 * s2 * c3;
	dst->c[9] = s1 * c3 + c1 * s2 * s3;
	dst->c[10] = c1 * c2;
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_set_rotation_angles_axes (EleaMat3x4f *dst, const float *angles, const unsigned int *axes)
{
	static const float signs[] = { 1, -1, 1, -1, 1, -1 };
	unsigned int order = 2 * axes[0] + (((axes[0] + 1) % 3) != axes[1]);
	float sign = signs[order];
	EleaMat3x4f m;
	unsigned int r, c;
	elea_mat3x4f_set_rotation_angles_xyz (&m, angles[0], angles[1], sign * angles[2]);
	if (order & 1) {
		m.c[2] *= sign;
		m.c[6] *= sign;
		m.c[8] *= sign;
		m.c[9] *= sign;
	}
	for (r = 0; r < 3; r++) {
		for (c = 0; c < 3; c++) {
			dst->c[4 * axes[r] + axes[c]] = m.c[4 * r + c];
		}
	}
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_set_rotation_axis_xyz_angle (EleaMat3x4f *dst, float x, float y, float z, float angle)
{
	float d2 = x * x + y * y + z * z;
	if (!d2) {
		z = 1;
		angle = 0;
		d2 = 1;
	}
	float id = 1 / sqrtf (d2);
	x *= id;
	y *= id;
	z *= id;
	float sa = sinf (angle);
	float ca = cosf (angle);
	float xs = x * sa;
	float ys = y * sa;
	float zs = z * sa;
	float ca_1 = 1 - ca;

	dst->c[0] = (ca_1 * x * x) + ca;
	dst->c[1] = (ca_1 * x * y) - zs;
	dst->c[2] = (ca_1 * z * x) + ys;

	dst->c[4] = (ca_1 * x * y) + zs;
	dst->c[5] = (ca_1 * y * y) + ca;
	dst->c[6] = (ca_1 * y * z) - xs;

	dst->c[8] = (ca_1 * z * x) - ys;
	dst->c[9] = (ca_1 * y * z) + xs;
	dst->c[10] = (ca_1 * z * z) + ca;
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_set_mirror(EleaMat3x4f *dst, const EleaPlane3f *plane)
{
	float k = -plane->d;
	dst->c[0] = 1 - 2 * plane->n.x * plane->n.x;
	dst->c[1] = -2 * plane->n.x * plane->n.y;
	dst->c[2] = -2 * plane->n.x * plane->n.z;
	dst->c[3] = 2 * plane->n.x * k;
	dst->c[4] = -2 * plane->n.y * plane->n.x;
	dst->c[5] = 1 - 2 * plane->n.y * plane->n.y;
	dst->c[6] = -2 * plane->n.y * plane->n.z;
	dst->c[7] = 2 * plane->n.y * k;
	dst->c[8] = -2 * plane->n.z * plane->n.x;
	dst->c[9] = -2 * plane->n.z * plane->n.y;
	dst->c[10] = 1 - 2 * plane->n.z * plane->n.z;
	dst->c[11] = 2 * plane->n.z * k;
	return dst;
}

float *
elea_mat3x4f_get_angles (float* dst, EleaMat3x4f* mat)
{
	float aX, aY, aZ;

	aY = asinf (mat->c[2]);
	if (fabsf (mat->c[2]) < 0.99999f) {
		// Over 0.25 degrees from right angle
		float C = cosf (aY);
		float trX = mat->c[10] / C;
		float trY = -mat->c[6] / C;
		aX = atan2f (trY, trX);
		trX = mat->c[0] / C;
		trY = -mat->c[1] / C;
		aZ = atan2f (trY, trX);
	} else {
		// Less than 0.25 degrees from right angle
		// Gimbal lock occured
		// Keep X rotation the same as base
		aX = 0;
		float Sz = mat->c[4];
		aZ = asinf (Sz);
	}

	dst[0] = elea_normalize_angle (aX);
	dst[1] = elea_normalize_angle (aY);
	dst[2] = elea_normalize_angle (aZ);
	return dst;
}

float *
elea_mat3x4f_get_angles_base (float *dst, EleaMat3x4f *mat, const float * __restrict base)
{
	float aX, aY, aZ;
	float aX2, aY2, aZ2;

	aY = asinf (mat->c[2]);
	aY2 = (aY >= 0) ? ELEA_M_PI_F - aY : -ELEA_M_PI_F - aY;
	if (fabsf (mat->c[2]) < 0.99999f) {
		// Over 0.25 degrees from right angle
		float C = cosf (aY);
		float C2 = cosf (aY2);
		float trX = mat->c[10] / C;
		float trY = -mat->c[6] / C;
		aX = atan2f (trY, trX);
		aX2 = atan2f (-mat->c[6] / C2, mat->c[10] / C2);
		trX = mat->c[0] / C;
		trY = -mat->c[1] / C;
		aZ = atan2f (trY, trX);
		aZ2 = atan2f (-mat->c[1] / C2, mat->c[0] / C2);
	} else {
		// Less than 0.25 degrees from right angle
		// Gimbal lock occured
		// Keep X rotation the same as base
		aX = aX2 = base[0];
		float Sx = sinf (aX);
		float Cx = cosf (aX);
		if (fabs (Cx) > fabs (Sx)) {
			float Sz = (mat->c[4] + (Sx / Cx) * mat->c[8]) / (Sx * Sx / Cx + Cx);
			aZ = aZ2 = asinf (Sz);
		} else {
			float Sz = (mat->c[8] + (Cx / Sx) * mat->c[4]) / (Cx * Cx / Sx + Sx);
			aZ = aZ2 = asinf (Sz);
		}
	}

	float d1X = elea_normalize_angle (aX - base[0]);
	float d1Y = elea_normalize_angle (aY - base[1]);
	float d1Z = elea_normalize_angle (aZ - base[2]);
	float d2X = elea_normalize_angle (aX2 - base[0]);
	float d2Y = elea_normalize_angle (aY2 - base[1]);
	float d2Z = elea_normalize_angle (aZ2 - base[2]);

	if ((d1X * d1X + d1Y * d1Y + d1Z * d1Z) < (d2X * d2X + d2Y * d2Y + d2Z * d2Z)) {
		dst[0] = aX;
		dst[1] = aY;
		dst[2] = aZ;
	} else {
		dst[0] = aX2;
		dst[1] = aY2;
		dst[2] = aZ2;
	}
	return dst;
}

float *
elea_mat3x4f_get_angles_ordered (float *dst, EleaMat3x4f *mat, const int * __restrict order)
{
	static const float base[] = { 0, 0, 0 };
	return elea_mat3x4f_get_angles_ordered_base(dst, mat, order, base);
}

float *
elea_mat3x4f_get_angles_ordered_base (float *dst, EleaMat3x4f *mat, const int * __restrict order, const float * __restrict base)
{
	float sign = ((order[1] == (order[0] + 1)) || (order[2] == (order[1] + 1))) ? 1.0f : -1.0f;
	// Permute matrix to switched axes space
	EleaMat3x4f rs;
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			rs.c[4 * row + col] = mat->c[4 * order[row] + order[col]];
		}
	}
	rs.c[2] *= sign;
	rs.c[6] *= sign;
	rs.c[8] *= sign;
	rs.c[9] *= sign;
	// New base
	float nbase[3];
	for (int i = 0; i < 3; i++) nbase[i] = base[i];
	nbase[2] *= sign;
	// Decompose to Euler angles
	elea_mat3x4f_get_angles_base(dst, &rs, nbase);
	dst[2] *= sign;
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_invert_normalized (EleaMat3x4f * __restrict dst, const EleaMat3x4f * __restrict mat)
{
	/* Transpose rotation */
	dst->c[0] = mat->c[0];
	dst->c[1] = mat->c[4];
	dst->c[2] = mat->c[8];
	dst->c[4] = mat->c[1];
	dst->c[5] = mat->c[5];
	dst->c[6] = mat->c[9];
	dst->c[8] = mat->c[2];
	dst->c[9] = mat->c[6];
	dst->c[10] = mat->c[10];
	/* Transform translation */
	dst->c[3] = -(dst->c[0] * mat->c[3] + dst->c[1] * mat->c[7] + dst->c[2] * mat->c[11]);
	dst->c[7] = -(dst->c[4] * mat->c[3] + dst->c[5] * mat->c[7] + dst->c[6] * mat->c[11]);
	dst->c[11] = -(dst->c[8] * mat->c[3] + dst->c[9] * mat->c[7] + dst->c[10] * mat->c[11]);
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_invert_self_normalized (EleaMat3x4f* mat)
{
	float t, m3, m7, m11;
	/* Transpose rotation */
	t = mat->c[1];
	mat->c[1] = mat->c[4];
	mat->c[4] = t;
	t = mat->c[2];
	mat->c[2] = mat->c[8];
	mat->c[8] = t;
	t = mat->c[6];
	mat->c[6] = mat->c[9];
	mat->c[9] = t;
	/* Transform translation */
	m3 = mat->c[3];
	m7 = mat->c[7];
	m11 = mat->c[11];
	mat->c[3] = -(mat->c[0] * m3 + mat->c[1] * m7 + mat->c[2] * m11);
	mat->c[7] = -(mat->c[4] * m3 + mat->c[5] * m7 + mat->c[6] * 11);
	mat->c[11] = -(mat->c[8] * m3 + mat->c[9] * m7 + mat->c[10] * m11);
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

EleaMat3x4f *
elea_mat3x4f_invert (EleaMat3x4f * __restrict dst, const EleaMat3x4f * __restrict mat)
{
	/* Determinant */
	float D = D3 (mat->c, 0, 4, 8, 0, 1, 2);
	if (!D) {
		*dst = EleaMat3x4fIdentity;
	} else {
		float rd = 1 / D;
		/* Inversion */
		dst->c[0] = rd * D2 (mat->c, 4, 8, 1, 2);
		dst->c[1] = -rd * D2 (mat->c, 0, 8, 1, 2);
		dst->c[2] = rd * D2 (mat->c, 0, 4, 1, 2);
		dst->c[3] = -rd * D3 (mat->c, 0, 4, 8, 1, 2, 3);

		dst->c[4] = -rd * D2 (mat->c, 4, 8, 0, 2);
		dst->c[5] = rd * D2 (mat->c, 0, 8, 0, 2);
		dst->c[6] = -rd * D2 (mat->c, 0, 4, 0, 2);
		dst->c[7] = rd * D3 (mat->c, 0, 4, 8, 0, 2, 3);

		dst->c[8] = rd * D2 (mat->c, 4, 8, 0, 1);
		dst->c[9] = -rd * D2 (mat->c, 0, 8, 0, 1);
		dst->c[10] = rd * D2 (mat->c, 0, 4, 0, 1);
		dst->c[11] = -rd * D3 (mat->c, 0, 4, 8, 0, 1, 3);
	}
	return dst;
}

EleaMat3x4f *
elea_mat3x4f_invert_self (EleaMat3x4f *mat)
{
	EleaMat3x4f inv;
	elea_mat3x4f_invert (&inv, mat);
	*mat = inv;
	return mat;
}

EleaMat3x4f *
elea_mat3x4f_multiply (EleaMat3x4f * __restrict dst, const EleaMat3x4f * __restrict lhs, const EleaMat3x4f * __restrict rhs)
{
	unsigned int i;
	for (i = 0; i < 4; i++) {
		dst->c[0 + i] = lhs->c[0] * rhs->c[0 + i] + lhs->c[1] * rhs->c[4 + i] + lhs->c[2] * rhs->c[8 + i];
		dst->c[4 + i] = lhs->c[4] * rhs->c[0 + i] + lhs->c[5] * rhs->c[4 + i] + lhs->c[6] * rhs->c[8 + i];
		dst->c[8 + i] = lhs->c[8] * rhs->c[0 + i] + lhs->c[9] * rhs->c[4 + i] + lhs->c[10] * rhs->c[8 + i];
	}
	dst->c[3] += lhs->c[3];
	dst->c[7] += lhs->c[7];
	dst->c[11] += lhs->c[11];
	return dst;
}

EleaPlane3f *
elea_mat3x4f_transform_plane (EleaPlane3f *d, const EleaMat3x4f *mat, EleaPlane3f *s)
{
	EleaVec3f p = elea_vec3f_mul (s->n, -s->d);
	elea_mat3x4f_transform_point (&p, mat, &p);
	elea_mat3x4f_transform_vec3 (&d->n, mat, &s->n);
	d->d = -elea_vec3f_dot (p, d->n);

	return d;
}

EleaAABox3f *
elea_mat3x4f_transform_aab (EleaAABox3f *d, const EleaMat3x4f *mat, const EleaAABox3f *s)
{
	EleaAABox3f b = EleaAABox3fEmpty;
	EleaVec3f p;
	unsigned int i;
	for (i = 0; i < 8; i++) {
		unsigned int ix = i & 1;
		unsigned int iy = (i & 2) >> 1;
		unsigned int iz = (i & 4) >> 2;
		p.x = s->c[3 * ix];
		p.y = s->c[3 * iy + 1];
		p.z = s->c[3 * iz + 2];
		elea_aabox3f_grow_p_mat (&b, &b, &p, mat);
	}
	*d = b;
	return d;
}
