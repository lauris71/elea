#define __ELEA_VECTOR4_C__

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
/* AZClass implementation */
static unsigned int vec4_serialize(const AZImplementation* impl, void* inst, unsigned char* d, unsigned int dlen, AZContext* ctx);
static unsigned int vec4_deserialize(const AZImplementation* impl, AZValue* value, const unsigned char* s, unsigned int slen, AZContext* ctx);
static unsigned int vec4_to_string(const AZImplementation *impl, void* instance, unsigned char* buf, unsigned int len);

static unsigned int vec4f_invoke_new(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_invert(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_add(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_subtract(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_multiply(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_divide(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_dot(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_length2(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_length(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int vec4f_invoke_normalize(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);

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
	if (!vec4_type) {
		vec4_class = ( EleaVec4fClass *) az_register_type(&vec4_type, (const unsigned char*) "Vector4f", AZ_TYPE_STRUCT, sizeof(EleaVec4fClass), sizeof(EleaVec4f), AZ_FLAG_FINAL,
			(void (*) (AZClass*)) vec4_class_init,
			NULL, NULL);
	}
	return vec4_type;
}

static void
vec4_class_init(EleaVec4fClass* klass)
{
	az_class_set_num_properties(( AZClass*) klass, NUM_PROPERTIES);
	az_class_define_method_va ((AZClass*) klass, FUNC_INVERT, (const unsigned char*) "invert", vec4f_invoke_invert, ELEA_TYPE_VECTOR4F, 0);
	az_class_define_static_method_va ((AZClass*) klass, FUNC_NEW, (const unsigned char*) "new", vec4f_invoke_new, ELEA_TYPE_VECTOR4F, 4, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass*) klass, FUNC_ADD, (const unsigned char*) "add", vec4f_invoke_add, ELEA_TYPE_VECTOR4F, 1, ELEA_TYPE_VECTOR4F );
	az_class_define_method_va ((AZClass*) klass, FUNC_SUBTRACT, (const unsigned char*) "subtract", vec4f_invoke_subtract, ELEA_TYPE_VECTOR4F, 1, ELEA_TYPE_VECTOR4F );
	az_class_define_method_va ((AZClass*) klass, FUNC_MULTIPLY, (const unsigned char*) "multiply", vec4f_invoke_multiply, ELEA_TYPE_VECTOR4F, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass*) klass, FUNC_DIVIDE, (const unsigned char*) "divide", vec4f_invoke_divide, ELEA_TYPE_VECTOR4F, 1, AZ_TYPE_FLOAT );
	az_class_define_method_va ((AZClass*) klass, FUNC_DOT, (const unsigned char*) "dot", vec4f_invoke_dot, AZ_TYPE_FLOAT, 1, ELEA_TYPE_VECTOR4F );
	az_class_define_method_va ((AZClass*) klass, FUNC_LENGTH2, (const unsigned char*) "length2", vec4f_invoke_length2, AZ_TYPE_FLOAT, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_LENGTH, (const unsigned char*) "length", vec4f_invoke_length, AZ_TYPE_FLOAT, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_NORMALIZE, (const unsigned char*) "normalize", vec4f_invoke_normalize, ELEA_TYPE_VECTOR4F, 0 );

	az_class_define_property(( AZClass*) klass, PROP_X, (const unsigned char*) "x", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, x), NULL, NULL);
	az_class_define_property(( AZClass*) klass, PROP_Y, (const unsigned char*) "y", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, y), NULL, NULL);
	az_class_define_property(( AZClass*) klass, PROP_Z, (const unsigned char*) "z", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, z), NULL, NULL);
	az_class_define_property(( AZClass*) klass, PROP_Z, (const unsigned char*) "w", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaVec4f, w), NULL, NULL);
	klass->az_klass.serialize = vec4_serialize;
	klass->az_klass.deserialize = vec4_deserialize;
	klass->az_klass.to_string = vec4_to_string;
}

static unsigned int
vec4_serialize(const AZImplementation* impl, void* inst, unsigned char* d, unsigned int dlen, AZContext* ctx)
{
	return az_serialize_floats(d, dlen, inst, 4);
}

static unsigned int
vec4_deserialize(const AZImplementation* impl, AZValue* value, const unsigned char* s, unsigned int slen, AZContext* ctx)
{
	return az_deserialize_floats(value, s, slen, 4);
}

static unsigned int
vec4_to_string(const AZImplementation *impl, void* instance, unsigned char* buf, unsigned int len)
{
	EleaVec4f* vec = ( EleaVec4f*) instance;
	unsigned int pos = 0, i;
	if (pos < len) buf[pos++] = '(';
	for (i = 0; i < 3; i++) {
		pos += arikkei_dtoa_exp(buf + pos, (len > pos) ? len - pos : 0, vec->c[i], 6, -5, 5);
		if (pos < len) buf[pos++] = ',';
	}
	pos += arikkei_dtoa_exp(buf + pos, (len > pos) ? len - pos : 0, vec->c[2], 6, -5, 5);
	if (pos < len) buf[pos++] = ')';
	if (pos < len) buf[pos] = 0;
	return pos;
}

static unsigned int
vec4f_invoke_new(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) vec4_class;
	elea_vec4fp_set_xyzw((EleaVec4f *) &ret_val, arg_vals[0]->float_v, arg_vals[1]->float_v, arg_vals[2]->float_v, arg_vals[3]->float_v);
	return 1;
}

static unsigned int
vec4f_invoke_invert(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) vec4_class;
	elea_vec4fp_inv ((EleaVec4f *) ret_val, (EleaVec4f *) &arg_vals[0]);
	return 1;
}

static unsigned int
vec4f_invoke_add(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) vec4_class;
	elea_vec4fp_add((EleaVec4f*) ret_val, (EleaVec4f *) &arg_vals[0], (EleaVec4f *) &arg_vals[1]);
	return 1;
}

static unsigned int
vec4f_invoke_subtract(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) vec4_class;
	elea_vec4fp_sub((EleaVec4f*) ret_val, (EleaVec4f *) &arg_vals[0], (EleaVec4f *) &arg_vals[1]);
	return 1;
}

static unsigned int
vec4f_invoke_multiply(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) vec4_class;
	elea_vec4fp_mul((EleaVec4f*) ret_val, (EleaVec4f *) &arg_vals[0], arg_vals[1]->float_v);
	return 1;
}

static unsigned int
vec4f_invoke_divide(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) vec4_class;
	elea_vec4fp_div((EleaVec4f*) ret_val, (EleaVec4f *) &arg_vals[0], arg_vals[1]->float_v);
	return 1;
}

static unsigned int
vec4f_invoke_dot(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) az_type_get_class(AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec4fp_dot ((EleaVec4f *) &arg_vals[0], (EleaVec4f *) &arg_vals[1]);
	return 1;
}

static unsigned int
vec4f_invoke_length2(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) az_type_get_class(AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec4fp_len2 ((EleaVec4f *) &arg_vals[0]);
	return 1;
}

static unsigned int
vec4f_invoke_length(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) az_type_get_class(AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_vec4fp_len ((EleaVec4f *) &arg_vals[0]);
	return 1;
}

static unsigned int
vec4f_invoke_normalize(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = ( AZImplementation*) vec4_class;
	elea_vec4fp_normalize((EleaVec4f *) ret_val, (EleaVec4f *) &arg_vals[0]);
	return 1;
}

