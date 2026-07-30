#define __ELEA_VECTOR4_C__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <string.h>

#include <az/types.h>
#include <az/extend.h>

#include <elea/private.h>

#include "vector4.h"

typedef struct _EleaVec4fClass EleaVec4fClass;

struct _EleaVec4fClass {
	AZClass az_klass;
};

EleaVec4f EleaVec4f0 = { 0, 0, 0, 0 };
EleaVec4f EleaVec4fX = { 1, 0, 0, 0 };
EleaVec4f EleaVec4fY = { 0, 1, 0, 0 };
EleaVec4f EleaVec4fZ = { 0, 0, 1, 0 };
EleaVec4f EleaVec4fW = { 0, 0, 0, 1 };
EleaVec4f EleaVec4fX1 = { 1, 0, 0, 1 };
EleaVec4f EleaVec4fY1 = { 0, 1, 0, 1 };
EleaVec4f EleaVec4fZ1 = { 0, 0, 1, 1 };

static void vec4_class_init(EleaVec4fClass* klass);

static unsigned int vec4f_invoke_new(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);

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
	NUM_FUNCTIONS,
	/* Values */
	PROP_X = NUM_FUNCTIONS,
	PROP_Y,
	PROP_Z,
	PROP_W,
	NUM_PROPERTIES
};

static unsigned int vec4_type = 0;
static EleaVec4fClass* vec4_class = NULL;

unsigned int
elea_vec4f_get_type(void)
{
	unsigned int t = AZ_TYPE_READ(vec4_type);
	if (t) return t;
	AZ_TYPES_LOCK();
	if (!vec4_type) {
		vec4_class = ( EleaVec4fClass *) az_register_type(&vec4_type, (const unsigned char*) "Vector4f", AZ_TYPE_STRUCT, sizeof(EleaVec4fClass), sizeof(EleaVec4f), AZ_FLAG_FINAL, 0, NUM_PROPERTIES,
			(void (*) (AZClass*)) vec4_class_init,
			NULL, NULL);
	}
	t = vec4_type;
	AZ_TYPES_UNLOCK();
	return t;
}

static void
vec4_class_init(EleaVec4fClass* klass)
{
	az_class_define_method_va ((AZClass *) klass, FUNC_INVERT, (const unsigned char*) "invert", vec_invoke_invert, ELEA_TYPE_VECTOR4F, 0);
	az_class_define_static_method_va ((AZClass *) klass, FUNC_NEW, (const unsigned char*) "new", vec4f_invoke_new, ELEA_TYPE_VECTOR4F, 4, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_ADD, (const unsigned char*) "add", vec_invoke_add, ELEA_TYPE_VECTOR4F, 1, ELEA_TYPE_VECTOR4F );
	az_class_define_method_va ((AZClass *) klass, FUNC_SUBTRACT, (const unsigned char*) "subtract", vec_invoke_subtract, ELEA_TYPE_VECTOR4F, 1, ELEA_TYPE_VECTOR4F );
	az_class_define_method_va ((AZClass *) klass, FUNC_MULTIPLY, (const unsigned char*) "multiply", vec_invoke_multiply, ELEA_TYPE_VECTOR4F, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass *) klass, FUNC_DIVIDE, (const unsigned char*) "divide", vec_invoke_divide, ELEA_TYPE_VECTOR4F, 1, AZ_TYPE_FLOAT );
	az_class_define_method_va ((AZClass *) klass, FUNC_DOT, (const unsigned char*) "dot", vec_invoke_dot, AZ_TYPE_FLOAT, 1, ELEA_TYPE_VECTOR4F );
	az_class_define_method_va ((AZClass *) klass, FUNC_LENGTH2, (const unsigned char*) "length2", vec_invoke_length2, AZ_TYPE_FLOAT, 0 );
	az_class_define_method_va ((AZClass *) klass, FUNC_LENGTH, (const unsigned char*) "length", vec_invoke_length, AZ_TYPE_FLOAT, 0 );
	az_class_define_method_va ((AZClass *) klass, FUNC_NORMALIZE, (const unsigned char*) "normalize", vec_invoke_normalize, ELEA_TYPE_VECTOR4F, 0 );

	az_class_define_property((AZClass *) klass, PROP_X, (const unsigned char*) "x", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, x), NULL, NULL);
	az_class_define_property((AZClass *) klass, PROP_Y, (const unsigned char*) "y", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, y), NULL, NULL);
	az_class_define_property((AZClass *) klass, PROP_Z, (const unsigned char*) "z", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, z), NULL, NULL);
	az_class_define_property((AZClass *) klass, PROP_W, (const unsigned char*) "w", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, w), NULL, NULL);
	klass->az_klass.serialize = vec_serialize;
	klass->az_klass.deserialize = vec_deserialize;
	klass->az_klass.to_string = vec_to_string;
}

static unsigned int
vec4f_invoke_new(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) vec4_class;
	elea_vec4fp_set_xyzw((EleaVec4f *) &ret_val, arg_vals[0]->float_v, arg_vals[1]->float_v, arg_vals[2]->float_v, arg_vals[3]->float_v);
	return 1;
}

