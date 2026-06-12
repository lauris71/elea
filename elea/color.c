#define __ELEA_COLOR_CPP__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <math.h>

#include <az/class.h>
#include <az/field.h>
#include <az/extend.h>

#include <elea/private.h>

#include "color.h"

typedef struct _EleaColor4fClass EleaColor4fClass;

struct _EleaColor4fClass {
	AZClass az_klass;
};

const EleaColor4f EleaColor4fTransparent = { 0, 0, 0, 0 };
const EleaColor4f EleaColor4fBlack = { 0, 0, 0, 1 };
const EleaColor4f EleaColor4fWhite = { 1, 1, 1, 1 };
const EleaColor4f EleaColor4fRed = { 1, 0, 0, 1 };
const EleaColor4f EleaColor4fGreen = { 0, 1, 0, 1 };
const EleaColor4f EleaColor4fBlue = { 0, 0, 1, 1 };

static void color_class_init (EleaColor4fClass *klass);

enum {
	/* Functions */
	NUM_FUNCTIONS,
	/* Values */
	PROP_R = NUM_FUNCTIONS,
	PROP_G,
	PROP_B,
	PROP_A,
	NUM_PROPERTIES
};

static unsigned int color_type = 0;

unsigned int
elea_color4f_get_type (void)
{
	if (!color_type) {
		az_register_type (&color_type, (const unsigned char *) "Color4f", AZ_TYPE_STRUCT, sizeof (EleaColor4fClass), sizeof (EleaColor4f), AZ_FLAG_FINAL, 0, NUM_PROPERTIES,
			(void (*) (AZClass *)) color_class_init,
			NULL, NULL);
	}
	return color_type;
}

static void
color_class_init (EleaColor4fClass *klass)
{
	az_class_define_property(( AZClass*) klass, PROP_R, (const unsigned char*) "r", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaColor4f, r), NULL, NULL);
	az_class_define_property(( AZClass*) klass, PROP_G, (const unsigned char*) "g", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaColor4f, g), NULL, NULL);
	az_class_define_property(( AZClass*) klass, PROP_B, (const unsigned char*) "b", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaColor4f, b), NULL, NULL);
	az_class_define_property(( AZClass*) klass, PROP_A, (const unsigned char*) "a", AZ_TYPE_FLOAT, 1, AZ_FIELD_INSTANCE, AZ_FIELD_READ_VALUE, 0, ARIKKEI_OFFSET(EleaColor4f, a), NULL, NULL);
	klass->az_klass.serialize = vec_serialize;
	klass->az_klass.deserialize = vec_deserialize;
	klass->az_klass.to_string = vec_to_string;
}

float
elea_color4fp_hue(const EleaColor4f *c)
{
	float max = ((c->r >= c->g) && (c->r >= c->b)) ? c->r : (c->g >= c->b) ? c->g : c->b;
	float min = ((c->r <= c->g) && (c->r <= c->b)) ? c->r : (c->g <= c->b) ? c->g : c->b;
	float delta = max - min;
	float hue = 0.0;
	if (delta) {
		if (c->r == max) {
			hue = (c->g - c->b) / delta;
		} else if (c->g == max) {
			hue = 2.0f + (c->b - c->r) / delta;
		} else {
			hue = 4.0f + (c->r - c->g) / delta;
		}
		hue /= 6;
		if (hue < 0) hue += 1;
	}
	return hue;
}

float
elea_color4fp_saturation(const EleaColor4f *c)
{
	float max = ((c->r >= c->g) && (c->r >= c->b)) ? c->r : (c->g >= c->b) ? c->g : c->b;
	float min = ((c->r <= c->g) && (c->r <= c->b)) ? c->r : (c->g <= c->b) ? c->g : c->b;
	float delta = max - min;
	return (max > 0) ? delta / max : 0;
}

EleaColor4f *
elea_color4fp_set_hsv(EleaColor4f *dst, float h, float s, float v, float a)
{
	float d = h * 5.999999f;
	float f = d - floorf (d);
	float w = v * (1 - s);
	float q = v * (1 - (s * f));
	float t = v * (1 - (s * (1 - f)));

	if (d < 1) {
		dst->r = v;
		dst->g = t;
		dst->b = w;
	} else if (d < 2) {
		dst->r = q;
		dst->g = v;
		dst->b = w;
	} else if (d < 3) {
		dst->r = w;
		dst->g = v;
		dst->b = t;
	} else if (d < 4) {
		dst->r = w;
		dst->g = q;
		dst->b = v;
	} else if (d < 5) {
		dst->r = t;
		dst->g = w;
		dst->b = v;
	} else {
		dst->r = v;
		dst->g = w;
		dst->b = q;
	}
	dst->a = a;
	return dst;
}

