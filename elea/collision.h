#ifndef __ELEA_COLLISION_H__
#define __ELEA_COLLISION_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2022
 *
 */

typedef struct _EleaCollision EleaCollision;
typedef struct _EleaSphere EleaSphere;
typedef struct _EleaCapsule EleaCapsule;
typedef struct _EleaCollidable EleaCollidable;

#include <stdint.h>

#include <arikkei/arikkei-utils.h>

#include <elea/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _EleaCollision {
	EleaVec3f p;
	EleaVec3f n;
	float depth;
};

struct _EleaSphere {
	EleaVec3f c;
	float r;
};

struct _EleaCapsule {
	EleaVec3f p0;
	EleaVec3f p1;
	float r;
};

struct _EleaCollidable {
	union {
		EleaSphere sphere;
		EleaCapsule capsule;
	};
};

/* Both collision point and normal are given at s1 surface */
unsigned int elea_sphere_sphere_collision (EleaCollision *result, const EleaSphere *s0, const EleaSphere *s1);
unsigned int elea_sphere_tri_collision (EleaCollision *result, const EleaSphere *s0, const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2);

#ifdef __cplusplus
};
#endif

#endif
