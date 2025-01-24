#ifndef __ELEA_TYPES_H__
#define __ELEA_TYPES_H__

//
// Libelea
//
// Copyright (C) Lauris Kaplinski 2007-2019
//

typedef struct _EleaAABox3f EleaAABox3f;
typedef struct _EleaColor4f EleaColor4f;
typedef struct _EleaMat3x3f EleaMat3x3f;
typedef struct _EleaMat3x4f EleaMat3x4f;
typedef struct _EleaMat4x4f EleaMat4x4f;
typedef struct _EleaPlane3f EleaPlane3f;
typedef struct _EleaQuatf EleaQuatf;
typedef struct _EleaRay3f EleaRay3f;
typedef struct _EleaVec2f EleaVec2f;
typedef struct _EleaVec3f EleaVec3f;
typedef struct _EleaVec4f EleaVec4f;

#include <math.h>

#include <arikkei/arikkei-utils.h>

#define ELEA_A16 ARIKKEI_ALIGN_16
#define ELEA_INLINE ARIKKEI_INLINE

#define ELEA_EPSILON_F 1e-12f
#define ELEA_EPSILON2_F 1e-24f
#define ELEA_EPSILON_2_F 1e-6f

#define ELEA_M_PI_F 3.1415926535897932384626433832795f
#define ELEA_M_2PI_F 6.283185307179586476925286766559f
#define ELEA_M_PI2_F 1.5707963267948966192313216916398f
#define ELEA_M_E_F 2.7182818284590452353602874713526f

#define ELEA_X 0
#define ELEA_Y 1
#define ELEA_Z 2
#define ELEA_W 3

#define ELEA_R 0
#define ELEA_G 1
#define ELEA_B 2
#define ELEA_A 3

#define ELEA_AXES_XYZ 0
#define ELEA_AXES_XZY 1
#define ELEA_AXES_YZX 2
#define ELEA_AXES_YXZ 3
#define ELEA_AXES_ZXY 4
#define ELEA_AXES_ZYX 5

#define ELEA_POSITION_IN 1
#define ELEA_POSITION_OUT 2
#define ELEA_POSITION_INTERSECT 0

enum EleaPlanePosition { INTERSECTING_INSIDE, INTERSECTING_OUTSIDE, COLLINEAR_IN, COLLINEAR_MIXED, COLLINEAR_OUT };

#ifdef __cplusplus
extern "C" {
#endif

ARIKKEI_INLINE void
elea_axes_from_axis_order (unsigned int order, unsigned int *axes)
{
	axes[0] = order / 2;
	axes[1] = ((order + 3) % 6) / 2;
	axes[2] = 2 - (order % 3);
}

ARIKKEI_INLINE unsigned int
elea_axis_order_from_axes (unsigned int ax_0, unsigned int ax_1, unsigned int ax_2)
{
	return 2 * ax_0 + (((ax_0 + 1) % 3) != ax_1);
}

ARIKKEI_INLINE float
elea_normalize_angle (float angle)
{
	if (angle <= -ELEA_M_PI_F) {
		return angle + ELEA_M_2PI_F;
	} else if (angle > ELEA_M_PI_F) {
		return angle - ELEA_M_2PI_F;
	}
	return angle;
}

ARIKKEI_INLINE float
elea_interpolate_angle (float a0, float a1, float t)
{
	if ((a0 + ELEA_M_PI_F) < a1) {
		a0 += ELEA_M_2PI_F;
	} else if ((a1 + ELEA_M_PI_F) < a0) {
		a1 += ELEA_M_2PI_F;
	}
	return elea_normalize_angle (t * a0 + (1 - t) * a1);
}

#ifdef __cplusplus
};
#endif

#endif

