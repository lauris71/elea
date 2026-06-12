#ifndef __ELEA_PRIVATE_H__
#define __ELEA_PRIVATE_H__

/*
 * Elea
 *
 * Copyright (C) Lauris Kaplinski 2007-2026
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <az/az.h>

unsigned int vec_serialize (const AZImplementation *impl, void *inst, unsigned char *d, unsigned int dlen, AZContext *ctx);
unsigned int vec_deserialize (const AZImplementation *impl, AZValue *value, const unsigned char *s, unsigned int slen, AZContext *ctx);
unsigned int vec_to_string (const AZImplementation *impl, void *inst, unsigned char *buf, unsigned int len);

unsigned int vec_invoke_invert(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_add(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_subtract(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_multiply(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_divide(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_dot(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_length(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_length2(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);
unsigned int vec_invoke_normalize(const AZImplementation *arg_impls[], const AZValue *arg_vals[], const AZImplementation **ret_impl, AZValue64 *ret_val, AZContext *ctx);

#ifdef __cplusplus
};
#endif

#endif

