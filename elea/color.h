#ifndef __ELEA_COLOR_H__
#define __ELEA_COLOR_H__

//
// Libelea
//
// Copyright (C) Lauris Kaplinski 2007-2019
//

#define ELEA_TYPE_COLOR4F elea_color4f_get_type ()

#include <math.h>
#include <stdint.h>
#include <string.h>

#include <elea/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __ELEA_COLOR_C__
extern EleaColor4f EleaColor4fTransparent;
extern EleaColor4f EleaColor4fBlack;
extern EleaColor4f EleaColor4fWhite;
extern EleaColor4f EleaColor4fRed;
extern EleaColor4f EleaColor4fGreen;
extern EleaColor4f EleaColor4fBlue;
#endif

struct _EleaColor4f {
	union {
		struct {
			float r, g, b, a;
		};
		float c[4];
	};
};

unsigned int elea_color4f_get_type (void);

static inline EleaColor4f
elea_color4f_from_rgba(float r, float g, float b, float a)
{
	return (EleaColor4f) {r, g, b, a};
}

static inline EleaColor4f
elea_color4f_from_floats(const float *v)
{
	return (EleaColor4f) {v[0], v[1], v[2], v[3]};
}

static inline EleaColor4f
elea_color4f_from_rgba32(uint32_t rgba)
{
	return (EleaColor4f) {((rgba >> 24) & 0xff) / 255.0f, ((rgba >> 16) & 0xff) / 255.0f, ((rgba >> 8) & 0xff) / 255.0f, (rgba & 0xff) / 255.0f};
}

static inline EleaColor4f *
elea_color4fp_set_rgba(EleaColor4f *dst, float r, float g, float b, float a)
{
	dst->r = r;
	dst->g = g;
	dst->b = b;
	dst->a = a;
	return dst;
}

static inline EleaColor4f *
elea_color4fp_set_floatv(EleaColor4f *dst, const float c[])
{
	memcpy (dst, c, sizeof (EleaColor4f));
	return dst;
}

static inline EleaColor4f*
elea_color4f_set_rgba32(EleaColor4f* dst, const uint32_t rgba)
{
	dst->r = ((rgba >> 24) & 0xff) / 255.0f;
	dst->g = ((rgba >> 16) & 0xff) / 255.0f;
	dst->b = ((rgba >> 8) & 0xff) / 255.0f;
	dst->a = (rgba & 0xff) / 255.0f;
	return dst;
}

static inline EleaColor4f *
elea_color4fp_set_srgba32 (EleaColor4f *dst, uint32_t rgba)
{
	unsigned int i;
	for (i = 0; i < 3; i++) {
		float v = ((rgba >> ((3 - i) * 8)) & 0xff) / 255.0f;
		if (v <= 0.04045f) {
			dst->c[i] = v / 12.92f;
		} else {
			dst->c[i] = powf ((v + 0.055f) / 1.055f, 2.4f);
		}
	}
	dst->a = (rgba & 0xff) / 255.0f;
	return dst;
}

static inline EleaColor4f
elea_color4f_from_srgba32 (uint32_t rgba)
{
	EleaColor4f dst;
	return *elea_color4fp_set_srgba32(&dst, rgba);
}

static inline EleaColor4f
elea_color4f_add_rgb (const EleaColor4f lhs, float r, float g, float b)
{
	return (EleaColor4f) {lhs.r + r, lhs.g + g, lhs.b + b, lhs.a};
}

static inline EleaColor4f *
elea_color4fp_add_rgb (EleaColor4f *dst, const EleaColor4f *lhs, float r, float g, float b)
{
	dst->r = lhs->r + r;
	dst->g = lhs->g + g;
	dst->b = lhs->b + b;
	dst->a = lhs->a;
	return dst;
}

static inline EleaColor4f
elea_color4f_add (const EleaColor4f lhs, const EleaColor4f rhs)
{
	return (EleaColor4f) {lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b, lhs.a};
}

static inline EleaColor4f *
elea_color4fp_add (EleaColor4f *dst, const EleaColor4f *lhs, const EleaColor4f *rhs)
{
	dst->r = lhs->r + rhs->r;
	dst->g = lhs->g + rhs->g;
	dst->b = lhs->b + rhs->b;
	dst->a = lhs->a;
	return dst;
}

static inline EleaColor4f
elea_color4f_sub_rgb (const EleaColor4f lhs, float r, float g, float b)
{
	return (EleaColor4f) {lhs.r - r, lhs.g - g, lhs.b - b, lhs.a};
}

static inline EleaColor4f *
elea_color4fp_sub_rgb (EleaColor4f *dst, const EleaColor4f *lhs, float r, float g, float b)
{
	dst->r = lhs->r - r;
	dst->g = lhs->g - g;
	dst->b = lhs->b - b;
	dst->a = lhs->a;
	return dst;
}

static inline const EleaColor4f
elea_color4f_sub (const EleaColor4f lhs, const EleaColor4f rhs)
{
	return (EleaColor4f) {lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b, lhs.a};
}

static inline const EleaColor4f *
elea_color4fp_sub (EleaColor4f *dst, const EleaColor4f *lhs, const EleaColor4f *rhs)
{
	dst->r = lhs->r - rhs->r;
	dst->g = lhs->g - rhs->g;
	dst->b = lhs->b - rhs->b;
	dst->a = lhs->a;
	return dst;
}

static inline const EleaColor4f
elea_color4f_mul (const EleaColor4f lhs, float v)
{
	return (EleaColor4f) {lhs.r * v, lhs.g * v, lhs.b * v, lhs.a};
}

static inline const EleaColor4f *
elea_color4fp_mul (EleaColor4f *dst, const EleaColor4f *lhs, float v)
{
	dst->r = lhs->r * v;
	dst->g = lhs->g * v;
	dst->b = lhs->b * v;
	dst->a = lhs->a;
	return dst;
}

static inline const EleaColor4f
elea_color4f_div (const EleaColor4f lhs, float v)
{
	return (EleaColor4f) {lhs.r / v, lhs.g / v, lhs.b / v, lhs.a};
}

static inline const EleaColor4f *
elea_color4fp_div (EleaColor4f*dst, const EleaColor4f *lhs, float v)
{
	dst->r = lhs->r / v;
	dst->g = lhs->g / v;
	dst->b = lhs->b / v;
	dst->a = lhs->a;
	return dst;
}

static inline EleaColor4f
elea_color4f_mul_add (const EleaColor4f lhs, const EleaColor4f rhs, float v)
{
	return (EleaColor4f) {lhs.r + v * rhs.r, lhs.g + v * rhs.g,  lhs.b + v * rhs.b, lhs.a};
}

static inline EleaColor4f *
elea_color4fp_mul_add (EleaColor4f *dst, const EleaColor4f *lhs, const EleaColor4f *rhs, float v)
{
	dst->r = lhs->r + v * rhs->r;
	dst->g = lhs->g + v * rhs->g;
	dst->b = lhs->b + v * rhs->b;
	dst->a = lhs->a;
	return dst;
}

static inline unsigned int
elea_color4f_equal (const EleaColor4f lhs, const EleaColor4f rhs)
{
	return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b) && (lhs.a == rhs.a);
}

static inline unsigned int
elea_color4fp_equal (const EleaColor4f *lhs, const EleaColor4f *rhs)
{
	return (lhs->r == rhs->r) && (lhs->g == rhs->g) && (lhs->b == rhs->b) && (lhs->a == rhs->a);
}

static inline uint32_t
elea_color4f_get_rgba(const EleaColor4f c)
{
return ((uint32_t) (c.r * 255.9999f) << 24) | ((uint32_t) (c.g * 255.9999) << 16) | ((uint32_t) (c.g * 255.9999) << 8) | (uint32_t) (c.a * 255.9999);
}

static inline uint32_t
elea_color4fp_get_rgba(const EleaColor4f *c)
{
return ((uint32_t) (c->r * 255.9999f) << 24) | ((uint32_t) (c->g * 255.9999) << 16) | ((uint32_t) (c->g * 255.9999) << 8) | (uint32_t) (c->a * 255.9999);
}

float elea_color4fp_hue(const EleaColor4f *c);

static inline float
elea_color4f_hue(const EleaColor4f c)
{
	return elea_color4fp_hue(&c);
}

float elea_color4fp_saturation(const EleaColor4f *c);

static inline float
elea_color4f_saturation(const EleaColor4f c)
{
	return elea_color4fp_saturation(&c);
}

static inline float
elea_color4fp_value (const EleaColor4f *c)
{
	if (c->r >= c->g) {
		return (c->r >= c->b) ? c->r : c->b;
	} else {
		return (c->g >= c->b) ? c->g : c->b;
	}
}

static inline float
elea_color4f_value(const EleaColor4f c)
{
	return elea_color4fp_value(&c);
}

EleaColor4f *elea_color4fp_set_hsv(EleaColor4f *dst, float h, float s, float v, float a);

static inline
EleaColor4f *elea_color4f_set_hue(EleaColor4f *dst, float hue)
{
	return elea_color4fp_set_hsv(dst, hue, elea_color4fp_saturation(dst), elea_color4fp_value(dst), dst->a);
}

static inline
EleaColor4f *elea_color4f_set_saturation(EleaColor4f *dst, float saturation)
{
	return elea_color4fp_set_hsv(dst, elea_color4fp_hue(dst), saturation, elea_color4fp_value(dst), dst->a);
}

static inline
EleaColor4f *elea_color4fp_set_value(EleaColor4f *dst, float value)
{
	return elea_color4fp_set_hsv(dst, elea_color4fp_hue(dst), elea_color4fp_saturation(dst), value, dst->a);
}

#ifdef __cplusplus
};
#endif

#endif

