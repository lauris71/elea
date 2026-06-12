#define __ELEA_PRIVATE_C__

/*
 * Elea
 *
 * Copyright (C) Lauris Kaplinski 2007-2026
 *
 */

#include <math.h>

#include <arikkei/arikkei-strlib.h>

#include <az/class.h>
#include <az/serialization.h>
#include <az/value.h>

#include <elea/private.h>

unsigned int
vec_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) impl;
    unsigned int n_vals = klass->instance_size / 4;
	return az_serialize_floats(d, dlen, inst, n_vals);
}

unsigned int
vec_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) impl;
    unsigned int n_vals = klass->instance_size / 4;
	return az_deserialize_floats(value, s, slen, n_vals);
}

unsigned int
vec_to_string (const AZImplementation *impl, void *inst, unsigned char *buf, unsigned int len)
{
    const AZClass *klass = (const AZClass *) impl;
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (float *) inst;
	unsigned int pos = 0, i;
	if (pos < len) buf[pos++] = '(';
	for (i = 0; i < (n_vals - 1); i++) {
		pos += arikkei_dtoa_exp (buf + pos, (len > pos) ? len - pos : 0, c[i], 6, -5, 5);
		if (pos < len) buf[pos++] = ',';
	}
	pos += arikkei_dtoa_exp (buf + pos, (len > pos) ? len - pos : 0, c[n_vals - 1], 6, -5, 5);
	if (pos < len) buf[pos++] = ')';
	if (pos < len) buf[pos] = 0;
	return pos;
}

unsigned int
vec_invoke_invert (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (const float *) arg_vals[0];
    float *d = (float *) ret_val;
    for (unsigned int i = 0; i < n_vals; i++) d[i] = -c[i];
    *ret_impl = arg_impls[0];
    return 1;
}

unsigned int
vec_invoke_add (const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *lhs = (const float *) arg_vals[0];
    const float *rhs = (const float *) arg_vals[1];
    float *d = (float *) ret_val;
    for (unsigned int i = 0; i < n_vals; i++) d[i] = lhs[i] + rhs[i];
    *ret_impl = arg_impls[0];
    return 1;
}

unsigned int
vec_invoke_subtract(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *lhs = (const float *) arg_vals[0];
    const float *rhs = (const float *) arg_vals[1];
    float *d = (float *) ret_val;
    for (unsigned int i = 0; i < n_vals; i++) d[i] = lhs[i] - rhs[i];
    *ret_impl = arg_impls[0];
    return 1;
}

unsigned int
vec_invoke_multiply(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (const float *) arg_vals[0];
    float v = arg_vals[1]->float_v;
    float *d = (float *) ret_val;
    for (unsigned int i = 0; i < n_vals; i++) d[i] = c[i] * v;
    *ret_impl = arg_impls[0];
    return 1;
}

unsigned int
vec_invoke_divide(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (const float *) arg_vals[0];
    float v = arg_vals[1]->float_v;
    float *d = (float *) ret_val;
    for (unsigned int i = 0; i < n_vals; i++) d[i] = c[i] / v;
    *ret_impl = arg_impls[0];
    return 1;
}

unsigned int
vec_invoke_dot(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *l = (const float *) arg_vals[0];
    const float *r = (const float *) arg_vals[1];
    float sum = 0;
    for (unsigned int i = 0; i < n_vals; i++) sum += l[i] * r[i];
    *ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
    ret_val->value.float_v = sum;
    return 1;
}

unsigned int
vec_invoke_length2(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (const float *) arg_vals[0];
    float sum = 0;
    for (unsigned int i = 0; i < n_vals; i++) sum += c[i] * c[i];
    *ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
    ret_val->value.float_v = sum;
    return 1;
}

unsigned int
vec_invoke_length(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (const float *) arg_vals[0];
    float sum = 0;
    for (unsigned int i = 0; i < n_vals; i++) sum += c[i] * c[i];
    *ret_impl = (AZImplementation *) az_type_get_class (AZ_TYPE_FLOAT);
    ret_val->value.float_v = sqrtf (sum);
    return 1;
}

unsigned int
vec_invoke_normalize(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx)
{
    const AZClass *klass = (const AZClass *) arg_impls[0];
    unsigned int n_vals = klass->instance_size / 4;
    const float *c = (const float *) arg_vals[0];
    float *d = (float *) ret_val;
    float sum = 0;
    for (unsigned int i = 0; i < n_vals; i++) sum += c[i] * c[i];
    float len = sqrtf (sum);
    if (len) {
        for (unsigned int i = 0; i < n_vals; i++) d[i] = c[i] / len;
    } else {
        d[0] = 1;
        for (unsigned int i = 1; i < n_vals; i++) d[i] = 0;
    }
    *ret_impl = arg_impls[0];
    return 1;
}
