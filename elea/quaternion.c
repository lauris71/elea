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

enum {
	/* Functions */
	NUM_FUNCTIONS,
	/* Values */
	PROP_I = NUM_FUNCTIONS,
	PROP_J,
	PROP_K,
	PROP_R,
	NUM_PROPERTIES
};

static unsigned int quat_type = 0;

unsigned int
elea_quatf_get_type (void)
{
	if (!quat_type) {
		az_register_type (&quat_type, (const unsigned char*) "Quaternionf", AZ_TYPE_STRUCT, sizeof (EleaQuatfClass), sizeof (EleaQuatf), AZ_CLASS_IS_FINAL,
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

