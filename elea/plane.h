#ifndef __ELEA_PLANE_H__
#define __ELEA_PLANE_H__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <string.h>

#include <elea/ray.h>
#include <elea/vector3.h>

#ifdef __cplusplus
extern "C" {
#endif
	
extern EleaPlane3f EleaPlane3fXY;
extern EleaPlane3f EleaPlane3fYX;
extern EleaPlane3f EleaPlane3fYZ;
extern EleaPlane3f EleaPlane3fZY;
extern EleaPlane3f EleaPlane3fZX;
extern EleaPlane3f EleaPlane3fXZ;

struct _EleaPlane3f {
	union {
		struct {
			EleaVec3f n;
			float d;
		};
		float c[4];
	};
};

static inline EleaPlane3f
elea_plane3f_from_xyzd(float nx, float ny, float nz, float d)
{
	return (EleaPlane3f) {nx, ny, nz, d};
}

static inline EleaPlane3f
elea_plane3f_from_floatv(const float *v)
{
	return (EleaPlane3f) {v[0], v[1], v[2], v[3]};
}

static inline EleaPlane3f
elea_plane3f_from_nd (const EleaVec3f *n, float d)
{
	return (EleaPlane3f) {n->x, n->y, n->z, d};
}

static inline EleaPlane3f
elea_plane3f_from_pn (const EleaVec3f *p, const EleaVec3f *n)
{
	return (EleaPlane3f) {n->x, n->y, n->z, -elea_vec3fp_dot(n, p)};
}

EleaPlane3f elea_plane3f_from_p3 (const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2);

static inline EleaPlane3f *
elea_plane3fp_set_xyzd (EleaPlane3f* dst, float nx, float ny, float nz, float d)
{
	dst->n.x = nx;
	dst->n.y = ny;
	dst->n.z = nz;
	dst->d = d;
	return dst;
}

static inline EleaPlane3f *
elea_plane3fp_set_nd (EleaPlane3f* dst, const EleaVec3f *n, float d)
{
	memcpy (&dst->n, n, sizeof (EleaVec3f));
	dst->d = d;
	return dst;
}

static inline EleaPlane3f *
elea_plane3fp_set_pn (EleaPlane3f *dst, const EleaVec3f *p, const EleaVec3f *n)
{
	memcpy (&dst->n, n, sizeof (EleaVec3f));
	dst->d = -elea_vec3fp_dot (n, p);
	return dst;
}

EleaPlane3f *elea_plane3fp_set_p3 (EleaPlane3f *dst, const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2);

static inline EleaPlane3f
elea_plane3f_normalize (const EleaPlane3f *src)
{
	float len = elea_vec3f_len(src->n);
	if (len) {
		return (EleaPlane3f) {src->n.x / len, src->n.y / len, src->n.z / len, src->d / len};
	} else {
		return elea_plane3f_from_xyzd(0, 0, 1, 0);
	}
}

static inline float
elea_plane3f_get_position_xyz (const EleaPlane3f* plane, float x, float y, float z)
{
	return plane->n.x * x + plane->n.y * y + plane->n.z * z + plane->d;
}

static inline float
elea_plane3f_get_position (const EleaPlane3f* plane, const EleaVec3f *vec)
{
	return plane->n.x * vec->x + plane->n.y * vec->y + plane->n.z * vec->z + plane->d;
}

static inline unsigned int
elea_plane3f_get_intersection_t (const EleaPlane3f *plane, const EleaRay3f *line, float *t)
{
	/*
	 * x = x0 + mu * (x1 - x0)
	 * y = y0 + mu * (y1 - y0)
	 * z = z0 + mu * (z1 - z0)
	 * Nx * x + Ny * y + Nz * z + D = 0
	 * Nx*x0 + Ny*y0 + Nz * z0 + mu*(Nx*(x1-x0) + Ny*(y1-y0) + Nz*(z1-z0)) + D = 0
	 * mu = (Nx*x0 + Ny*y0 + Nz*z0 + D) / (Nx*(x1-x0) + Ny*(y1-y0) + Nz*(z1-z0))
	 */
	float den = elea_vec3f_dot (plane->n, line->d);
	if (!den) return 0;
	if (t) *t = -(elea_vec3f_dot (plane->n, line->p) + plane->d) / den;
	return 1;
}

static inline unsigned int
elea_plane3f_get_intersection_p (const EleaPlane3f *plane, const EleaRay3f *line, EleaVec3f *p)
{
	/*
	 * x = x0 + mu * (x1 - x0)
	 * y = y0 + mu * (y1 - y0)
	 * z = z0 + mu * (z1 - z0)
	 * Nx * x + Ny * y + Nz * z + D = 0
	 * Nx*x0 + Ny*y0 + Nz * z0 + mu*(Nx*(x1-x0) + Ny*(y1-y0) + Nz*(z1-z0)) + D = 0
	 * mu = (Nx*x0 + Ny*y0 + Nz*z0 + D) / (Nx*(x1-x0) + Ny*(y1-y0) + Nz*(z1-z0))
	 */
	float den = elea_vec3f_dot (plane->n, line->d);
	if (!den) return 0;
	if (p) {
		float t = -(elea_vec3f_dot (plane->n, line->p) + plane->d) / den;
		elea_ray3f_get_point_at (p, line, t);
	}
	return 1;
}

#ifdef __cplusplus
};
#endif

#endif

