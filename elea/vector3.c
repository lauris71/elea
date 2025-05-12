#define __ELEA_VECTOR3_C__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <string.h>

#include <arikkei/arikkei-strlib.h>

#include <az/class.h>
#include <az/field.h>
#include <az/serialization.h>

#include "vector3.h"

struct _EleaVec3fClass {
	AZClass az_klass;
};

const EleaVec3f EleaVec3f0 = { 0, 0, 0 };
const EleaVec3f EleaVec3fX = { 1, 0, 0 };
const EleaVec3f EleaVec3fY = { 0, 1, 0 };
const EleaVec3f EleaVec3fZ = { 0, 0, 1 };

static void vec3_class_init (EleaVec3fClass *klass);
/* AZClass implementation */
static unsigned int vec3_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx);
static unsigned int vec3_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx);
static unsigned int vec3_to_string (const AZImplementation *impl, void *instance, unsigned char *buf, unsigned int len);

static unsigned int vec3f_invoke_new (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_invert (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_add (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_subtract (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_multiply (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_divide (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_dot (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_cross (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_length2 (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_length (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_normalize (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec3f_invoke_angle (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);

enum {
	/* Functions */
	FUNC_NEW,
	FUNC_INVERT,
	FUNC_ADD,
	FUNC_SUBTRACT,
	FUNC_MULTIPLY,
	FUNC_DIVIDE,
	FUNC_DOT,
	FUNC_CROSS,
	FUNC_LENGTH2,
	FUNC_LENGTH,
	FUNC_NORMALIZE,
	FUNC_ANGLE,
	NUM_FUNCTIONS,
	/* Values */
	PROP_X = NUM_FUNCTIONS,
	PROP_Y,
	PROP_Z,
	PROP_ZERO,
	PROP_UNIT_X,
	PROP_UNIT_Y,
	PROP_UNIT_Z,
	NUM_PROPERTIES
};

static unsigned int vec3_type = 0;
EleaVec3fClass *elea_vec3f_class = NULL;

unsigned int
elea_vec3f_get_type (void)
{
	if (!vec3_type) {
		az_register_type (&vec3_type, (const unsigned char *) "Vector3f", AZ_TYPE_STRUCT, sizeof (EleaVec3fClass), sizeof (EleaVec3f), AZ_FLAG_FINAL,
			(void (*) (AZClass *)) vec3_class_init,
			NULL, NULL);
		elea_vec3f_class = (EleaVec3fClass *) az_type_get_class (vec3_type);
	}
	return vec3_type;
}

static void
vec3_class_init (EleaVec3fClass *klass)
{
	az_class_set_num_properties ((AZClass *) klass, NUM_PROPERTIES);
	az_class_define_method_va ((AZClass *) klass, FUNC_INVERT, (const unsigned char *) "invert", vec3f_invoke_invert, ELEA_TYPE_VECTOR3F, 0);
	az_class_define_static_method_va ((AZClass *) klass, FUNC_NEW, (const unsigned char *) "new", vec3f_invoke_new, ELEA_TYPE_VECTOR3F, 3, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_ADD, (const unsigned char *) "add", vec3f_invoke_add, ELEA_TYPE_VECTOR3F, 1, ELEA_TYPE_VECTOR3F);
	az_class_define_method_va ((AZClass *) klass, FUNC_SUBTRACT, (const unsigned char *) "subtract", vec3f_invoke_subtract, ELEA_TYPE_VECTOR3F, 1, ELEA_TYPE_VECTOR3F);
	az_class_define_method_va ((AZClass *) klass, FUNC_MULTIPLY, (const unsigned char *) "multiply", vec3f_invoke_multiply, ELEA_TYPE_VECTOR3F, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_DIVIDE, (const unsigned char *) "divide", vec3f_invoke_divide, ELEA_TYPE_VECTOR3F, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_DOT, (const unsigned char *) "dot", vec3f_invoke_dot, AZ_TYPE_FLOAT, 1, ELEA_TYPE_VECTOR3F);
	az_class_define_method_va ((AZClass *) klass, FUNC_CROSS, (const unsigned char *) "cross", vec3f_invoke_cross, ELEA_TYPE_VECTOR3F, 1, ELEA_TYPE_VECTOR3F);
	az_class_define_method_va ((AZClass *) klass, FUNC_LENGTH2, (const unsigned char *) "length2", vec3f_invoke_length2, AZ_TYPE_FLOAT, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_LENGTH, (const unsigned char *) "length", vec3f_invoke_length, AZ_TYPE_FLOAT, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_NORMALIZE, (const unsigned char *) "normalize", vec3f_invoke_normalize, ELEA_TYPE_VECTOR3F, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_ANGLE, (const unsigned char *) "angle", vec3f_invoke_angle, AZ_TYPE_FLOAT, 1, ELEA_TYPE_VECTOR3F);

	az_class_define_property ((AZClass *) klass, PROP_X, (const unsigned char *) "x", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaVec3f, x), NULL, NULL);
	az_class_define_property ((AZClass *) klass, PROP_Y, (const unsigned char *) "y", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaVec3f, y), NULL, NULL);
	az_class_define_property ((AZClass *) klass, PROP_Z, (const unsigned char *) "z", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaVec3f, z), NULL, NULL);
	az_class_define_property ((AZClass *) klass, PROP_ZERO, (const unsigned char *) "ZERO", ELEA_TYPE_VECTOR3F, 1,
		AZ_FIELD_CLASS, AZ_FIELD_READ_STORED_STATIC, AZ_FIELD_WRITE_NONE, 0, (const AZImplementation *) klass, (void *) &EleaVec3f0);
	az_class_define_property ((AZClass *) klass, PROP_UNIT_X, (const unsigned char *) "UNIT_X", ELEA_TYPE_VECTOR3F, 1,
		AZ_FIELD_CLASS, AZ_FIELD_READ_STORED_STATIC, AZ_FIELD_WRITE_NONE, 0, (const AZImplementation *) klass, (void *) &EleaVec3fX);
	az_class_define_property ((AZClass *) klass, PROP_UNIT_Y, (const unsigned char *) "UNIT_Y", ELEA_TYPE_VECTOR3F, 1,
		AZ_FIELD_CLASS, AZ_FIELD_READ_STORED_STATIC, AZ_FIELD_WRITE_NONE, 0, (const AZImplementation *) klass, (void *) &EleaVec3fY);
	az_class_define_property ((AZClass *) klass, PROP_UNIT_Z, (const unsigned char *) "UNIT_Z", ELEA_TYPE_VECTOR3F, 1,
		AZ_FIELD_CLASS, AZ_FIELD_READ_STORED_STATIC, AZ_FIELD_WRITE_NONE, 0, (const AZImplementation *) klass, (void *) &EleaVec3fZ);
	klass->az_klass.serialize = vec3_serialize;
	klass->az_klass.deserialize = vec3_deserialize;
	klass->az_klass.to_string = vec3_to_string;
}

static unsigned int
vec3_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx)
{
	return az_serialize_floats(d, dlen, inst, 3);
}

static unsigned int
vec3_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx)
{
	return az_deserialize_floats(value, s, slen, 3);
}

static unsigned int
vec3_to_string (const AZImplementation *impl, void *instance, unsigned char *buf, unsigned int len)
{
	EleaVec3f *vec = (EleaVec3f *) instance;
	unsigned int pos = 0, i;
	if (pos < len) buf[pos++] = '(';
	for (i = 0; i < 2; i++) {
		pos += arikkei_dtoa_exp (buf + pos, (len > pos) ? len - pos : 0, vec->c[i], 6, -5, 5);
		if (pos < len) buf[pos++] = ',';
	}
	pos += arikkei_dtoa_exp (buf + pos, (len > pos) ? len - pos : 0, vec->c[2], 6, -5, 5);
	if (pos < len) buf[pos++] = ')';
	if (pos < len) buf[pos] = 0;
	return pos;
}

static unsigned int
vec3f_invoke_new (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_set_xyz ((EleaVec3f *) ret_val, arg_vals[0]->float_v, arg_vals[1]->float_v, arg_vals[2]->float_v);
	return 1;
}

static unsigned int
vec3f_invoke_invert (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_inv ((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0]);
	return 1;
}

static unsigned int
vec3f_invoke_add (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_add ((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0], (EleaVec3f *) arg_vals[1]);
	return 1;
}

static unsigned int
vec3f_invoke_subtract (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_sub ((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0], (EleaVec3f *) arg_vals[1]);
	return 1;
}

static unsigned int
vec3f_invoke_multiply (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_mul ((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0], arg_vals[1]->float_v);
	return 1;
}

static unsigned int
vec3f_invoke_divide (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_div ((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0], arg_vals[1]->float_v);
	return 1;
}

static unsigned int
vec3f_invoke_dot (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec3fp_dot ((EleaVec3f *) arg_vals[0], (EleaVec3f *) arg_vals[1]);
	return 1;
}

static unsigned int
vec3f_invoke_cross (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_cross((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0], (EleaVec3f *) arg_vals[1]);
	return 1;
}

static unsigned int
vec3f_invoke_length2 (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec3fp_len2 ((EleaVec3f *) arg_vals[0]);
	return 1;
}

static unsigned int
vec3f_invoke_length (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec3fp_len ((EleaVec3f *) arg_vals[0]);
	return 1;
}

static unsigned int
vec3f_invoke_normalize (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) elea_vec3f_class;
	elea_vec3fp_normalize ((EleaVec3f *) ret_val, (EleaVec3f *) arg_vals[0]);
	return 1;
}

static unsigned int
vec3f_invoke_angle (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec3fp_angle ((EleaVec3f *) arg_vals[0], (EleaVec3f *) arg_vals[1]);
	return 1;
}

float
elea_vec3f_angle (const EleaVec3f lhs, const EleaVec3f rhs)
{
	float a2 = elea_vec3f_len2 (lhs);
	float b2 = elea_vec3f_len2 (rhs);
	float l2 = a2 * b2;
	if (l2) {
		float ssc = elea_vec3f_dot (lhs, rhs) / sqrtf (l2);
		if (ssc > 1) ssc = 1;
		if (ssc < -1) ssc = -1;
		return acosf (ssc);
	}
	return 0;
}

EleaVec3f
elea_vec3f_get_perpendicular (const EleaVec3f src)
{
	if (fabsf (src.x) >= fabsf (src.y)) {
		if (fabsf (src.y) >= fabsf (src.z)) {
			/* X & Y */
			return (EleaVec3f) {src.y, -src.x, src.z};
		} else {
			/* X & Z*/
			return (EleaVec3f) {src.z, src.y, -src.x};
		}
	} else if (fabsf (src.x) >= fabsf (src.z)) {
		/* X & Y */
		return (EleaVec3f) {src.y, -src.x, src.z};
	} else {
		/* Y & Z */
		return (EleaVec3f) {src.x, src.z, -src.y};
	}
}
