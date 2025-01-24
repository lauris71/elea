#define __ELEA_QUATERNION_CPP__

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

#include "quaternion.h"

typedef struct _EleaQuatfClass EleaQuatfClass;

struct _EleaQuatfClass {
	AZClass az_klass;
};

EleaQuatf EleaQuatf0 = { 0, 0, 0, 1 };

static void quat_class_init (EleaQuatfClass* klass);
/* AZClass implementation */
static unsigned int quat_serialize (const AZImplementation* impl, void* inst, unsigned char* d, unsigned int dlen, AZContext* ctx);
static unsigned int quat_deserialize (const AZImplementation* impl, AZValue* value, const unsigned char* s, unsigned int slen, AZContext* ctx);
static unsigned int quat_to_string (const AZImplementation* impl, void* instance, unsigned char* d, unsigned int dlen);

static unsigned int quat_invoke_new (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
static unsigned int quat_invoke_norm(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_norm2(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_negate(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_conjugate(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_inverse(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_normalize(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_multiply_scalar(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_multiply(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_dot(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);
static unsigned int quat_invoke_slerp(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx);

enum {
	/* Functions */
	FUNC_NEW,
	FUNC_NORM,
	FUNC_NORM2,
	FUNC_NEGATE,
	FUNC_CONJUGATE,
	FUNC_INVERSE,
	FUNC_NORMALIZE,
	FUNC_MULTIPLY_SCALAR,
	FUNC_MULTIPLY,
	FUNC_DOT,
	FUNC_SLERP,
	NUM_FUNCTIONS,
	/* Values */
	PROP_I = NUM_FUNCTIONS,
	PROP_J,
	PROP_K,
	PROP_R,
	NUM_PROPERTIES
};

static unsigned int quat_type = 0;
static EleaQuatfClass * quat_class = NULL;

unsigned int
elea_quatf_get_type (void)
{
	if (!quat_type) {
		quat_class = (EleaQuatfClass *) az_register_type (&quat_type, (const unsigned char*) "Quaternionf", AZ_TYPE_STRUCT, sizeof (EleaQuatfClass), sizeof (EleaQuatf), AZ_CLASS_IS_FINAL,
			(void (*) (AZClass*)) quat_class_init,
			NULL, NULL);
	}
	return quat_type;
}

static void
quat_class_init (EleaQuatfClass* klass)
{
	klass->az_klass.alignment = 4;
	az_class_set_num_properties (( AZClass*) klass, NUM_PROPERTIES);
	az_class_define_static_method_va ((AZClass *) klass, FUNC_NEW, (const unsigned char *) "new", quat_invoke_new, ELEA_TYPE_QUATF, 4, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass*) klass, FUNC_NORM, (const unsigned char*) "norm", quat_invoke_norm, AZ_TYPE_FLOAT, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_NORM2, (const unsigned char*) "norm2", quat_invoke_norm2, AZ_TYPE_FLOAT, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_NEGATE, (const unsigned char*) "negate", quat_invoke_negate, ELEA_TYPE_QUATF, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_CONJUGATE, (const unsigned char*) "conjugate", quat_invoke_conjugate, ELEA_TYPE_QUATF, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_INVERSE, (const unsigned char*) "inverse", quat_invoke_inverse, ELEA_TYPE_QUATF, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_NORMALIZE, (const unsigned char*) "normalize", quat_invoke_normalize, ELEA_TYPE_QUATF, 0 );
	az_class_define_method_va ((AZClass*) klass, FUNC_MULTIPLY_SCALAR, (const unsigned char *) "multiply", quat_invoke_multiply_scalar, ELEA_TYPE_QUATF, 1, AZ_TYPE_FLOAT);
	az_class_define_method_va ((AZClass*) klass, FUNC_MULTIPLY, (const unsigned char *) "multiply", quat_invoke_multiply, ELEA_TYPE_QUATF, 1, ELEA_TYPE_QUATF);
	az_class_define_method_va ((AZClass*) klass, FUNC_DOT, (const unsigned char *) "dot", quat_invoke_dot, AZ_TYPE_FLOAT, 1, ELEA_TYPE_QUATF);
	az_class_define_method_va ((AZClass*) klass, FUNC_SLERP, (const unsigned char *) "slerp", quat_invoke_slerp, ELEA_TYPE_QUATF, 2, ELEA_TYPE_QUATF, AZ_TYPE_FLOAT);

	az_class_define_property (( AZClass*) klass, PROP_I, (const unsigned char*) "i", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaQuatf, i), NULL, NULL);
	az_class_define_property (( AZClass*) klass, PROP_J, (const unsigned char*) "j", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaQuatf, j), NULL, NULL);
	az_class_define_property (( AZClass*) klass, PROP_K, (const unsigned char*) "k", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaQuatf, k), NULL, NULL);
	az_class_define_property (( AZClass*) klass, PROP_R, (const unsigned char*) "r", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET (EleaQuatf, r), NULL, NULL);
	klass->az_klass.serialize = quat_serialize;
	klass->az_klass.deserialize = quat_deserialize;
	klass->az_klass.to_string = quat_to_string;
}

static unsigned int
quat_serialize (const AZImplementation* impl, void* inst, unsigned char* d, unsigned int dlen, AZContext* ctx)
{
	return az_serialize_floats (d, dlen, inst, 4);
}

static unsigned int
quat_deserialize (const AZImplementation* impl, AZValue* value, const unsigned char* s, unsigned int slen, AZContext* ctx)
{
	return az_deserialize_floats (value, s, slen, 4);
}

static unsigned int
quat_to_string (const AZImplementation* impl, void* instance, unsigned char* d, unsigned int dlen)
{
	EleaQuatf* col = ( EleaQuatf*) instance;
	unsigned int pos = 0, i;
	if (pos < dlen) d[pos++] = '(';
	for (i = 0; i < 3; i++) {
		pos += arikkei_dtoa_exp (d + pos, (dlen > pos) ? dlen - pos : 0, col->c[i], 6, -5, 5);
		if (pos < dlen) d[pos++] = ',';
	}
	pos += arikkei_dtoa_exp (d + pos, (dlen > pos) ? dlen - pos : 0, col->c[2], 6, -5, 5);
	if (pos < dlen) d[pos++] = ')';
	if (pos < dlen) d[pos] = 0;
	return pos;
}

static unsigned int
quat_invoke_new(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_set_ijkr((EleaQuatf *) &ret_val, arg_vals[0]->float_v, arg_vals[1]->float_v, arg_vals[2]->float_v, arg_vals[3]->float_v);
	return 1;
}

static unsigned int
quat_invoke_norm(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) az_type_get_class(AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_quatfp_norm((EleaQuatf *) &arg_vals[0]);
	return 1;
}

static unsigned int
quat_invoke_norm2(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) az_type_get_class(AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_quatfp_norm2((EleaQuatf *) &arg_vals[0]);
	return 1;
}

static unsigned int
quat_invoke_negate(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_negate((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0]);
	return 1;
}

static unsigned int
quat_invoke_conjugate(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_conjugate((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0]);
	return 1;
}

static unsigned int
quat_invoke_inverse(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_inverse((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0]);
	return 1;
}

static unsigned int
quat_invoke_normalize(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_normalize((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0]);
	return 1;
}

static unsigned int
quat_invoke_multiply_scalar(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_mul_scalar((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0], arg_vals[1]->float_v);
	return 1;
}
static unsigned int
quat_invoke_multiply(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_mul((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0], (EleaQuatf *) arg_vals[1]);
	return 1;
}

static unsigned int
quat_invoke_dot(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) az_type_get_class(AZ_TYPE_FLOAT);
	ret_val->value.float_v = elea_quatfp_dot((EleaQuatf *) arg_vals[0], (EleaQuatf *) arg_vals[1]);
	return 1;
}

static unsigned int
quat_invoke_slerp(const AZImplementation* arg_impls[], const AZValue* arg_vals[], const AZImplementation** ret_impl, AZValue64* ret_val, AZContext* ctx)
{
	*ret_impl = (AZImplementation*) quat_class;
	elea_quatfp_slerp_normalized((EleaQuatf *) ret_val, (EleaQuatf *) arg_vals[0], (EleaQuatf *) arg_vals[1], arg_vals[2]->float_v);
	return 1;
}

#define DOT_THRESHOLD 0.995f

EleaQuatf *
elea_quatfp_slerp_normalized (EleaQuatf *dst, const EleaQuatf *lhs, const EleaQuatf *rhs, float t)
{
	EleaQuatf b;
	float dot;
	unsigned int i;
	dot = elea_quatfp_dot(lhs, rhs);
	if (dot < 0) {
		dot = -dot;
		elea_quatfp_negate (&b, rhs);
	} else {
		b = *rhs;
	}
	if (dot > DOT_THRESHOLD) {
		for (i = 0; i < 4; i++) dst->c[i] = lhs->c[i] + t * (b.c[i] - lhs->c[i]);
		elea_quatfp_normalize(dst, dst);
		return dst;
	}
	float theta_0 = acosf (dot);
	float theta = theta_0 * t;
	float cos_theta = cosf (theta);
	/* float sin_theta = sinf (theta); */
	float sin_theta = sqrtf (1 - cos_theta * cos_theta);
	/* float sin_theta_0 = sinf (theta_0); */
	float sin_theta_0 = sqrtf (1 - dot * dot);

	float s0 = cos_theta - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
	float s1 = sin_theta / sin_theta_0;

	for (i = 0; i < 4; i++) dst->c[i] = s0 * lhs->c[i] + s1 * b.c[i];
	return dst;
}

