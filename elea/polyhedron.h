#ifndef __ELEA_POLYHEDRON_H__
#define __ELEA_POLYHEDRON_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 */

typedef struct _EleaPolyhedron3f EleaPolyhedron3f;

#include <stdint.h>
#include <string.h>

#include <elea/types.h>
#include <elea/plane.h>

#ifdef __cplusplus
extern "C" {
#endif

struct _EleaPolyhedron3f {
	uint32_t n;
	uint32_t _dummy;
	EleaPlane3f p[8];
};

unsigned int elea_polyhedron3f_test_aabox (const EleaPolyhedron3f* poly, const EleaAABox3f* box);

#ifdef __cplusplus
};
#endif

#endif
