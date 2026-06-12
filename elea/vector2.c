#define __ELEA_VECTOR2_C__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <string.h>

#include <az/extend.h>

#include <elea/private.h>

#include "vector2.h"

typedef struct _EleaVec2fClass EleaVec2fClass;

struct _EleaVec2fClass {
	AZClass az_klass;
};

const EleaVec2f EleaVec2f0 = { 0, 0 };
const EleaVec2f EleaVec2fX = { 1, 0 };
const EleaVec2f EleaVec2fY = { 0, 1 };

static void vec2_class_init (EleaVec2fClass *klass);

static unsigned int vec2f_invoke_new (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int vec2f_invoke_angle (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);

enum {
	/* Functions */
	FUNC_NEW,
	FUNC_INVERT,
	FUNC_ADD,
	FUNC_SUBTRACT,
	FUNC_MULTIPLY,
	FUNC_DIVIDE,
	FUNC_DOT,
	FUNC_LENGTH2,
	FUNC_LENGTH,
	FUNC_NORMALIZE,
	FUNC_ANGLE,
	NUM_FUNCTIONS,
	/* Values */
	PROP_X = NUM_FUNCTIONS,
	PROP_Y,
	NUM_PROPERTIES
};

static unsigned int vec2_type = 0;
static EleaVec2fClass *vec2_class = NULL;

unsigned int
elea_vec2f_get_type (void)
{
	if (!vec2_type) {
		az_register_type (&vec2_type, (const unsigned char *) "Vector2f", AZ_TYPE_STRUCT, sizeof (EleaVec2fClass), sizeof (EleaVec2f), AZ_FLAG_FINAL, 0, NUM_PROPERTIES,
			(void (*) (AZClass *)) vec2_class_init,
			NULL, NULL);
		vec2_class = (EleaVec2fClass *) az_type_get_class (vec2_type);
	}
	return vec2_type;
}

static void
vec2_class_init (EleaVec2fClass *klass)
{
	az_class_define_static_method_va ((AZClass *) klass, FUNC_NEW, (const unsigned char *) "new", vec2f_invoke_new, ELEA_TYPE_VECTOR2F, 2, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_INVERT, (const unsigned char *) "invert", vec_invoke_invert, ELEA_TYPE_VECTOR2F, 0, NULL);
	az_class_define_method_va ((AZClass *) klass, FUNC_ADD, (const unsigned char *) "add", vec_invoke_add, ELEA_TYPE_VECTOR2F, 1, ELEA_TYPE_VECTOR2F);
	az_class_define_method_va ((AZClass *) klass, FUNC_SUBTRACT, (const unsigned char *) "subtract", vec_invoke_subtract, ELEA_TYPE_VECTOR2F, 1, ELEA_TYPE_VECTOR2F);
	az_class_define_method_va ((AZClass *) klass, FUNC_MULTIPLY, (const unsigned char *) "multiply", vec_invoke_multiply, ELEA_TYPE_VECTOR2F, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_DIVIDE, (const unsigned char *) "divide", vec_invoke_divide, ELEA_TYPE_VECTOR2F, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_DOT, (const unsigned char *) "dot", vec_invoke_dot, AZ_TYPE_FLOAT, 1, ELEA_TYPE_VECTOR2F);
	az_class_define_method_va ((AZClass *) klass, FUNC_LENGTH2, (const unsigned char *) "length2", vec_invoke_length2, AZ_TYPE_FLOAT, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_LENGTH, (const unsigned char *) "length", vec_invoke_length, AZ_TYPE_FLOAT, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_NORMALIZE, (const unsigned char *) "normalize", vec_invoke_normalize, ELEA_TYPE_VECTOR2F, 0);
	az_class_define_method_va ((AZClass *) klass, FUNC_ANGLE, (const unsigned char *) "angle", vec2f_invoke_angle, AZ_TYPE_FLOAT, 1, ELEA_TYPE_VECTOR2F);

	az_class_define_property ((AZClass *) klass, PROP_X, (const unsigned char *) "x", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaVec2f, x), NULL, NULL);
	az_class_define_property ((AZClass *) klass, PROP_Y, (const unsigned char *) "y", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaVec2f, y), NULL, NULL);
	klass->az_klass.serialize = vec_serialize;
	klass->az_klass.deserialize = vec_deserialize;
	klass->az_klass.to_string = vec_to_string;
}

static unsigned int
vec2f_invoke_new (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) vec2_class;
	*((EleaVec2f *) ret_val) = elea_vec2f_from_xy (arg_vals[0]->float_v, arg_vals[1]->float_v);
	return 1;
}

static unsigned int
vec2f_invoke_angle (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
	*ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec2fp_angle ((EleaVec2f *) &arg_vals[0], (EleaVec2f *) &arg_vals[1]);
	return 1;
}

float
elea_vec2fp_angle (const EleaVec2f *lhs, const EleaVec2f *rhs)
{
	float a2 = elea_vec2fp_len2 (lhs);
	float b2 = elea_vec2fp_len2 (rhs);
	float l2 = a2 * b2;
	if (l2) {
		float ssc = elea_vec2fp_dot (lhs, rhs) / sqrtf (l2);
		if (ssc > 1) ssc = 1;
		if (ssc < -1) ssc = -1;
		return acosf (ssc);
	}
	return 0;
}

