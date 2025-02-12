#define __ELEA_GEOMETRY_CPP__

//
// Libelea
//
// Copyright (C) Lauris Kaplinski 2009
//

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "vector2.h"
#include "vector3.h"
#include "matrix3x4.h"
#include "plane.h"
#include "aabox.h"

#include "geometry.h"

unsigned int
elea_get_ray_tri_intersection (const EleaRay3f *ray, const EleaVec3f *v0, const EleaVec3f *v1, const EleaVec3f *v2, float *t)
{
/*
	vec3 v1v0 = v1 - v0;
	vec3 v2v0 = v2 - v0;
	vec3 rov0 = ro - v0;
	vec3  n = cross (v1v0, v2v0);
	vec3  q = cross (rov0, rd);
	float d = 1.0 / dot (rd, n);
	float u = d * dot (-q, v2v0);
	float v = d * dot (q, v1v0);
	float t = d * dot (-n, rov0);
	if (u < 0.0 || u>1.0 || v < 0.0 || (u + v)>1.0) t = -1.0;
	return vec3 (t, u, v);
*/
	EleaVec3f v1v0, v2v0, rov0, n, q;
	float d, u, v;
	v1v0 = elea_vec3f_sub (*v1, *v0);
	v2v0 = elea_vec3f_sub (*v2, *v0);
	rov0 = elea_vec3f_sub (ray->p, *v0);
	n = elea_vec3f_cross (v1v0, v2v0);
	q = elea_vec3f_cross (rov0, ray->d);
	d = elea_vec3f_dot (ray->d, n);
	if (fabsf (d) < ELEA_EPSILON_F) return 0;
	d = 1.0f / d;
	u = -d * elea_vec3f_dot (q, v2v0);
	v = d * elea_vec3f_dot (q, v1v0);
	if (u < 0 || v < 0 || (u + v) > 1) return 0;
	*t = -d * elea_vec3f_dot (n, rov0);
	return 1;
}

unsigned int
elea_get_ray_tri_mesh_intersection (const EleaRay3f *ray, const float *vertices, unsigned int v_stride_bytes, const unsigned int *indices, unsigned int n_indices, float *t, unsigned int *index)
{
	float t_tri, t_best = INFINITY;
	unsigned int i_best = 0;
	//elea_vec3f_div (&n_ray.d, &n_ray.d, d_len);
	for (unsigned int i = 0; i < n_indices; i += 3) {
		const EleaVec3f *p0 = (const EleaVec3f *) ((const char *) vertices + indices[i + 0] * v_stride_bytes);
		const EleaVec3f *p1 = (const EleaVec3f *) ((const char *) vertices + indices[i + 1] * v_stride_bytes);
		const EleaVec3f *p2 = (const EleaVec3f *) ((const char *) vertices + indices[i + 2] * v_stride_bytes);
		if ((p0 == p1) || (p0 == p2) || (p1 == p2)) continue;
		if (elea_get_ray_tri_intersection (ray, p0, p1, p2, &t_tri)) {
			if ((t_tri >= 0) && (t_tri < t_best)) {
				t_best = t_tri;
				i_best = i;
			}
		}
	}
	if (t_best <= 1) {
		*t = t_best;
		if (index) *index = i_best;
		return 1;
	}
	return 0;
}

unsigned int
elea_get_ellipsoid_tri_intersection (const EleaVec3f *pw0, const EleaVec3f *pw1, const EleaVec3f *pw2, const EleaMat3x4f *e2us, const EleaVec3f *v_us, const EleaAABox3f *bbox, float *t, EleaVec3f *cpus)
{
	for (int i = 0; i < 3; i++) {
		if ((pw0->c[i] < bbox->min.c[i]) && (pw1->c[i] < bbox->min.c[i]) && (pw2->c[i] < bbox->min.c[i])) return 0;
		if ((pw0->c[i] > bbox->max.c[i]) && (pw1->c[i] > bbox->max.c[i]) && (pw2->c[i] > bbox->max.c[i])) return 0;
	}
	// p[] - triangle in unit sphere space
	// Transform everything to sphere space
	EleaVec3f p[3];
	elea_mat3x4f_transform_point (&p[0], e2us, pw0);
	elea_mat3x4f_transform_point (&p[1], e2us, pw1);
	elea_mat3x4f_transform_point (&p[2], e2us, pw2);

	// Construct triangle plane
	EleaPlane3f tPlane = elea_plane3f_from_p3 (&p[0], &p[1], &p[2]);
	// signedDistance (N * p + Cp)
	// t0 = (1 - signedDistance (basePoint)) / (N * velocity)
	// t1 = (-1 - signedDistance (basePoint)) / (N * velocity)
	float signedDistance = tPlane.d;
	if (signedDistance < 0) {
		// We are at the backside of plane
		return 0;
	}
	float N_dot_velocity = elea_vec3f_dot (tPlane.n, *v_us);
	if (N_dot_velocity < 0) {
		// Velocity is directed to the plane
		float t0 = (1 - signedDistance) / N_dot_velocity;
		float t1 = (-1 - signedDistance) / N_dot_velocity;
		if ((t0 <= t1) && (t0 >= 0)) {
			// Plane intersection point
			EleaVec3f q = elea_vec3f_mul (*v_us, t0);
			q = elea_vec3f_sub (q, tPlane.n);
			// Test whether the intersection point is inside the triangle
			EleaVec3f q0, q1, q2;
			q0 = elea_vec3f_sub (p[0], q);
			q1 = elea_vec3f_sub (p[1], q);
			q2 = elea_vec3f_sub (p[2], q);
			EleaVec3f q01, q12, q20;
			q01 = elea_vec3f_cross (q0, q1);
			q12 = elea_vec3f_cross (q1, q2);
			q20 = elea_vec3f_cross (q2, q0);
			if ((elea_vec3f_dot (q01, q12) >= 0) && (elea_vec3f_dot (q12, q20) >= 0) && (elea_vec3f_dot (q20, q01) >= 0)) {
				// We hit inside of a triangle
				if (t) *t = t0;
				if (cpus) *cpus = elea_vec3f_inv (tPlane.n);
				return 1;
			}
		}
	}

	float bestt = INFINITY;
	EleaVec3f bestp = EleaVec3f0;

	float p_len2[3], vp[3];
	for (int i = 0; i < 3; i++) {
		p_len2[i] = elea_vec3f_len2 (p[i]);
		vp[i] = elea_vec3f_dot (*v_us, p[i]);
	}

	// Test against all edges
	float v2 = elea_vec3fp_len2 (v_us);
	for (int i = 0; i < 3; i++) {
		EleaVec3f d = elea_vec3f_sub (p[(i + 1) % 3], p[i]);
		float f2 = p_len2[i];
		float d2 = elea_vec3f_len2 (d);
		float dv = elea_vec3f_dot (d, *v_us);
		float fd = elea_vec3f_dot (p[i], d);
		float fv = vp[i];

		float a1 = dv * dv - d2 * v2;
		if (fabs (a1) < ELEA_EPSILON_F) continue;
		float b1 = 2 * d2 * fv - 2 * fd * dv;
		float c1 = fd * fd - f2 * d2 + d2;
		// if (fabs (c1) < Elea::EPSILON_F) continue;
		float q = b1 * b1 - 4 * a1 * c1;
		if (q < 0) continue;
		float t0 = (-b1 - sqrtf (q)) / (2 * a1);
		float t1 = (-b1 + sqrtf (q)) / (2 * a1);
		if (t0 > t1) {
			float tval = t0;
			t0 = t1;
			t1 = tval;
		}
		float a0 = d2;
		float b0 = 2 * fd - 2 * dv * t0;
		// float c0 = f2 - 2 * fv * t0 + v2 * t0 * t0 - 1;
		// q = b0 * b0 - 4 * a0 * c0;
		float s0 = -b0 / (2 * a0);

		if ((s0 >= 0) && (s0 <= 1)) {
			if ((t0 >= 0) && (t0 <= bestt)) {
				bestt = t0;
				bestp = elea_vec3f_mul (d, s0);
				bestp = elea_vec3f_add (bestp, p[i]);
			}
		}
	}

	// Test against all vertices
	for (int i = 0; i < 3; i++) {
		float f2 = p_len2[i];
		// A*t*t + B*t + C = 0
		// A = v2
		// B = -2 * (v . f)
		// C = f2 - 1
		float a = v2;
		float b = -2 * vp[i];
		float c = f2 - 1;
		// t = (-b +/- sqrt (b2 - 4ac)) / 2a
		float q = b * b - 4 * a * c;
		if (q < 0) continue;
		float t0 = (-b - sqrtf (q)) / (2 * a);
		float t1 = (-b + sqrtf (q)) / (2 * a);
		if (t0 > t1) {
			float tval = t0;
			t0 = t1;
			t1 = tval;
		}
		if ((t0 >= 0) && (t0 < bestt)) {
			bestt = t0;
			bestp = p[i];
		}
	}

	if (bestt < INFINITY) {
		if (t) *t = bestt;
		if (cpus) {
			cpus->x = bestp.x - bestt * v_us->x;
			cpus->y = bestp.y - bestt * v_us->y;
			cpus->z = bestp.z - bestt * v_us->z;
		}
		return 1;
	}

	return 0;
}

void
elea_calculate_normals (float *normals, uint32_t nstridebytes, const float *vertices, uint32_t vstridebytes, uint32_t nvertices, const uint32_t *indices, uint32_t nindices, unsigned int normalize)
{
	for (uint32_t i = 0; i < nvertices; i++) {
		float *n = (float *) ((uint8_t *) normals + i * nstridebytes);
		n[0] = n[1] = n[2] = 0;
	}

	for (uint32_t i = 0; i < nindices; i += 3) {
		for (int j = 0; j < 3; j++) {
			uint32_t i0 = indices[i + (j + 0) % 3];
			uint32_t i1 = indices[i + (j + 1) % 3];
			uint32_t i2 = indices[i + (j + 2) % 3];
			EleaVec3f *v0 = (EleaVec3f *) ((uint8_t *) vertices + i0 * vstridebytes);
			EleaVec3f *v1 = (EleaVec3f *) ((uint8_t *) vertices + i1 * vstridebytes);
			EleaVec3f *v2 = (EleaVec3f *) ((uint8_t *) vertices + i2 * vstridebytes);
			EleaVec3f v01 = elea_vec3f_sub(*v1, *v0);
			EleaVec3f vhalf = elea_vec3f_add(*v1, *v2);
			vhalf = elea_vec3f_div(vhalf, 2);
			vhalf = elea_vec3f_sub(vhalf, *v0);
			v01 = elea_vec3f_normalize(v01);
			vhalf = elea_vec3f_normalize(vhalf);
			EleaVec3f normal;
			normal = elea_vec3f_cross(v01, vhalf);
			float *n = (float *) ((uint8_t *) normals + i0 * nstridebytes);
			for (int k = 0; k < 3; k++) n[k] += normal.c[k];
		}
	}

	if (normalize) {
		for (uint32_t i = 0; i < nvertices; i++) {
			EleaVec3f *n = (EleaVec3f *) ((uint8_t *) normals + i * nstridebytes);
			elea_vec3fp_normalize(n, n);
		}
	}
}

void
elea_calculate_tangents (float *tangents, uint32_t tstridebytes, const float *vertices, uint32_t vstridebytes, const float *normals, uint32_t nstridebytes, const float *texcoords, uint32_t xstridebytes, uint32_t nvertices, const uint32_t *indices, uint32_t nindices, unsigned int normalize)
{
	for (uint32_t i = 0; i < nvertices; i++) {
		float *t = (float *) ((uint8_t *) tangents + i * tstridebytes);
		t[0] = t[1] = t[2] = 0;
	}
	// Calculate tangents
	for (uint32_t i = 0; i < nindices; i += 3) {
		for (uint32_t j = 0; j < 3; j++) {
			uint32_t i0 = indices[i + (j + 0) % 3];
			uint32_t i1 = indices[i + (j + 1) % 3];
			uint32_t i2 = indices[i + (j + 2) % 3];
			EleaVec3f *c0 = (EleaVec3f *) ((uint8_t *) vertices + i0 * vstridebytes);
			EleaVec3f *c1 = (EleaVec3f *) ((uint8_t *) vertices + i1 * vstridebytes);
			EleaVec3f *c2 = (EleaVec3f *) ((uint8_t *) vertices + i2 * vstridebytes);
			EleaVec3f v0, v1, vq;
			v0 = elea_vec3f_sub(*c1, *c0);
			v1 = elea_vec3f_sub(*c2, *c0);
			vq = elea_vec3f_add(*c0, *c1);
			// v0.normalizeSelf ();
			// vq.normalizeSelf ();
			EleaVec2f *x0 = (EleaVec2f *) ((uint8_t *) texcoords + i0 * xstridebytes);
			EleaVec2f *x1 = (EleaVec2f *) ((uint8_t *) texcoords + i1 * xstridebytes);
			EleaVec2f *x2 = (EleaVec2f *) ((uint8_t *) texcoords + i2 * xstridebytes);
			EleaVec2f t0 = elea_vec2f_sub(*x1, *x0);
			EleaVec2f t1 = elea_vec2f_sub(*x2, *x0);
			// if (fabs (t0[1]) > 0.01 && fabs (t1[1]) > 0.01) continue;
			// Ts * t0.s + Tt * t0.t = v0
			// Ts * t1.s + Tt * t1.t = v1
			// D = t0.s * t1.t - t1.s * t0.t
			// Ds = v0 * t1.t - v1 * t0.t
			// Dt = t0.s * v1 - t1.s * v0
			float D = t0.x * t1.y - t1.x * t0.y;
			EleaVec3f Ds, Dt;
			Ds = elea_vec3f_mul(v0, t1.y);
			Ds = elea_vec3f_mul_add(Ds, v1, -t0.y);
			Dt = elea_vec3f_mul(v1, t0.x);
			Dt = elea_vec3f_mul_add(Ds, v0, -t1.x);
			EleaVec3f Ts, Tt;
			Ts = elea_vec3f_div(Ds, D);
			Tt = elea_vec3f_div(Dt, D);
			EleaVec3f cv0, cv1;
			cv0 = elea_vec3f_mul(Ts, t0.x);
			cv0 = elea_vec3f_mul_add(cv0, Tt, t0.y);
			cv1 = elea_vec3f_mul(Ts, t1.x);
			cv0 = elea_vec3f_mul_add(cv0, Tt, t1.y);
			EleaVec3f q0, q1;
			q0 = elea_vec3f_sub(v0, cv0);
			q1 = elea_vec3f_sub(v1, cv1);

			EleaVec3f *t = (EleaVec3f *) ((uint8_t *) tangents + i0 * tstridebytes);
			*t = elea_vec3f_add(*t, Ts);
		}
	}
	for (uint32_t i = 0; i < nvertices; i++) {
		EleaVec3f *n = (EleaVec3f *) ((uint8_t *) normals + i * nstridebytes);
		EleaVec3f *t = (EleaVec3f *) ((uint8_t *) tangents + i * tstridebytes);
		EleaVec3f b;
		b = elea_vec3f_cross(*n, *t);
		*t = elea_vec3f_cross(b, *n);
	}
	if (normalize) {
		for (uint32_t i = 0; i < nvertices; i++) {
			EleaVec3f *t = (EleaVec3f *) ((uint8_t *) tangents + i * tstridebytes);
			elea_vec3fp_normalize(t, t);
		}
	}
}

// Sphere

// Test code for sphere

static float
longitude (const EleaVec3f *v)
{
	if ((v->z == 1) || (v->z == -1)) return 0.5f;
	float s = sqrtf (1 - v->z * v->z);
	float latitude = atan2f (v->y / s, v->x / s) / ELEA_M_2PI_F;
	if (latitude < 0) latitude = latitude + 1.0f;
	return latitude;
}

static float
latitude (const EleaVec3f *v)
{
	return 0.5f + atan2f (v->z, sqrtf (1 - v->z * v->z)) / ELEA_M_PI_F;
}

typedef struct _CBuf {
	float *vertices;
	int vstridebytes;
	float *normals;
	int nstridebytes;
	float *texcoords;
	int tstridebytes;
} CBuf;

static EleaVec3f *
cbuf_vertex(CBuf *cbuf, int idx) { return (EleaVec3f *)((char *) cbuf->vertices + idx * cbuf->vstridebytes); }
static EleaVec3f *
cbuf_normal (CBuf *cbuf, int idx) { return (EleaVec3f *)((char *) cbuf->normals + idx * cbuf->nstridebytes); }
static EleaVec2f *
cbuf_texCoord(CBuf *cbuf, int idx) { return (EleaVec2f *)((char *) cbuf->texcoords + idx * cbuf->tstridebytes); }

static void
addVertex (const EleaVec3f *vertex, CBuf *cbuf, unsigned int *vpos, unsigned int end)
{
	if (vpos) {
		if (cbuf) {
			EleaVec3f vn = elea_vec3f_normalize(*vertex);
			float s = longitude (&vn);
			float t = latitude (&vn);
			if (end && (s < 0.25f)) s += 1.0;
			if (cbuf->vertices) *cbuf_vertex(cbuf, *vpos) = *vertex;
			if (cbuf->normals) *cbuf_normal(cbuf, *vpos) = vn;
			if (cbuf->texcoords) *cbuf_texCoord(cbuf, *vpos) = elea_vec2f_from_xy(s, t);
		}
		*vpos += 1;
	}
}

typedef struct _Side {
	int v0;
	int v1;
	int vmid;
	int side0;
	int side1;
	int opposite;
} Side;

static void side_initialize (Side *side) {
	side->v0 = side->v1 = 0;
	side->vmid = side->side0 = side->side1 = side->opposite = -1;
}

static void
subdivideSide (Side *side, Side *sb, unsigned int *spos, CBuf *cbuf, unsigned int *vpos, float radius)
{
	if ((side->opposite >= 0) && (sb[side->opposite].vmid >= 0)) {
		// Opposite side has already subdivided vertices
		side->vmid = sb[side->opposite].vmid;
	} else {
		// Create new vertex
		EleaVec3f v = EleaVec3fZ;
		if (cbuf->vertices) {
			v = elea_vec3f_add(*cbuf_vertex(cbuf, side->v0), *cbuf_vertex(cbuf, side->v1));
			v = elea_vec3f_normalize(v);
			v = elea_vec3f_mul(v, radius);
		}
		unsigned int end = 0;
		if (cbuf->texcoords) {
			end = (cbuf_texCoord (cbuf, side->v0)->x > 0.5f) || (cbuf_texCoord (cbuf, side->v1)->y > 0.5f);
		}
		side->vmid = *vpos;
		addVertex (&v, cbuf, vpos, end);
	}
	side->side0 = *spos;
	sb[*spos].v0 = side->v0;
	sb[*spos].v1 = side->vmid;
	if ((side->opposite >= 0) && (sb[side->opposite].side1 >= 0)) {
		// Opposite is already divided
		sb[*spos].opposite = sb[side->opposite].side1;
		sb[sb[side->opposite].side1].opposite = *spos;
	}
	*spos += 1;
	side->side1 = *spos;
	sb[*spos].v0 = side->vmid;
	sb[*spos].v1 = side->v1;
	if ((side->opposite >= 0) && (sb[side->opposite].side0 >= 0)) {
		// Opposite is already divided
		sb[*spos].opposite = sb[side->opposite].side0;
		sb[sb[side->opposite].side0].opposite = *spos;
	}
	*spos += 1;
}

static void
subdivide (Side *sb, unsigned int *spos, CBuf *cbuf, unsigned int *vpos, unsigned int *ib, unsigned int *ipos, unsigned int s0, unsigned int s1, unsigned int s2, float radius, int level)
{
	if (level > 0) {
		// Subdivide triangle
		// Subdivide sides
		subdivideSide (&sb[s0], sb, spos, cbuf, vpos, radius);
		subdivideSide (&sb[s1], sb, spos, cbuf, vpos, radius);
		subdivideSide (&sb[s2], sb, spos, cbuf, vpos, radius);
		// Add inner sides
		unsigned int s02 = *spos;
		sb[*spos].v0 = sb[s0].vmid;
		sb[*spos].v1 = sb[s2].vmid;
		*spos += 1;
		unsigned int s10 = *spos;
		sb[*spos].v0 = sb[s1].vmid;
		sb[*spos].v1 = sb[s0].vmid;
		*spos += 1;
		unsigned int s21 = *spos;
		sb[*spos].v0 = sb[s2].vmid;
		sb[*spos].v1 = sb[s1].vmid;
		*spos += 1;
		subdivide (sb, spos, cbuf, vpos, ib, ipos, sb[s0].side0, s02, sb[s2].side1, radius, level - 1);
		subdivide (sb, spos, cbuf, vpos, ib, ipos, sb[s1].side0, s10, sb[s0].side1, radius, level - 1);
		subdivide (sb, spos, cbuf, vpos, ib, ipos, sb[s2].side0, s21, sb[s1].side1, radius, level - 1);
		unsigned int s20 = *spos;
		sb[*spos].v0 = sb[s2].vmid;
		sb[*spos].v1 = sb[s0].vmid;
		*spos += 1;
		unsigned int s01 = *spos;
		sb[*spos].v0 = sb[s0].vmid;
		sb[*spos].v1 = sb[s1].vmid;
		*spos += 1;
		unsigned int s12 = *spos;
		sb[*spos].v0 = sb[s1].vmid;
		sb[*spos].v1 = sb[s2].vmid;
		*spos += 1;
		// Opposites
		sb[s02].opposite = s20;
		sb[s20].opposite = s02;
		sb[s10].opposite = s01;
		sb[s01].opposite = s10;
		sb[s21].opposite = s12;
		sb[s12].opposite = s21;
		subdivide (sb, spos, cbuf, vpos, ib, ipos, s01, s12, s20, radius, level - 1);
		return;
	}
	// No more subdivisions - add indices of this triangle
	if (ipos) {
		if (ib) {
			ib[*ipos] = sb[s0].v0;
			ib[*ipos + 1] = sb[s1].v0;
			ib[*ipos + 2] = sb[s2].v0;
		}
		*ipos += 3;
	}
}

static void
generateBox (float *v, unsigned int v_stride_bytes, float *n, unsigned int n_stride_bytes, float *tex, unsigned int tex_stride_bytes, unsigned int *indices, const EleaVec3f corners[], int dir)
{
	static int ck[] = { 1, 2, 6, 5, 2, 3, 7, 6, 3, 0, 4, 7, 0, 1, 5, 4, 5, 6, 7, 4, 2, 1, 0, 3 };
	static int ik[] = { 0, 1, 3, 3, 1, 2 };
	for (int i = 0; i < 6; i++) {
		if (v) {
			for (int j = 0; j < 4; j++) {
				memcpy ((char *) v + (4 * i + j) * v_stride_bytes, &corners[ck[4 * i + j]], sizeof (EleaVec3f));
			}
		}
		if (n) {
			const EleaVec3f *p0 = &corners[ck[4 * i + 0]];
			const EleaVec3f *p1 = &corners[ck[4 * i + 1]];
			const EleaVec3f *p2 = &corners[ck[4 * i + 2]];
			EleaVec3f s0, s1, normal;
			s0 = elea_vec3f_sub (*p1, *p0);
			s1 = elea_vec3f_sub (*p2, *p0);
			normal = elea_vec3f_cross (s0, s1);
			normal = elea_vec3f_normalize (normal);
			if (dir < 0) normal = elea_vec3f_inv (normal);
			for (int j = 0; j < 4; j++) {
				memcpy ((char *) n + (4 * i + j) * n_stride_bytes, &normal, sizeof (EleaVec3f));
			}
		}
		if (tex) {
			for (int j = 0; j < 4; j++) {
				EleaVec2f txc = elea_vec2f_from_xy ((float) (((j + 1) & 3) >> 1), (float) (j >> 1));
				memcpy (( char *) tex + (4 * i + j) * tex_stride_bytes, &txc, sizeof (EleaVec2f));
			}
		}
		if (indices) {
			for (int j = 0; j < 6; j++) {
				indices[6 * i + j] = 4 * i + ik[j];
			}
			if (dir < 0) {
				unsigned int t = indices[6 * i + 1];
				indices[6 * i + 1] = indices[6 * i + 2];
				indices[6 * i + 2] = t;
				t = indices[6 * i + 4];
				indices[6 * i + 4] = indices[6 * i + 5];
				indices[6 * i + 5] = t;
			}
		}
	}
}

void
elea_generate_plane (float *v, unsigned int vstridebytes, float *n, unsigned int nstridebytes, float *x, unsigned int xstridebytes, unsigned int *indices, const float *p0, const float *p1, unsigned int ndivx, unsigned int ndivy, float texdimu, float texdimv, float (* height) (float, float, void *), void *data)
{
	for (unsigned int iy = 0; iy <= ndivy; iy++) {
		for (unsigned int ix = 0; ix <= ndivx; ix++) {
			unsigned int vpos = iy * (ndivx + 1) + ix;
			float vx = p0[0] + ix * (p1[0] - p0[0]) / ndivx;
			float vy = p0[1] + iy * (p1[1] - p0[1]) / ndivx;
			float vz = (height) ? height (vx, vy, data) : 0;
			if (v) elea_vec3fp_set_xyz((EleaVec3f *) ((char *) v + vpos * vstridebytes), vx, vy, vz);
			if (x) *((EleaVec2f *) ((char *) x + vpos * xstridebytes)) = elea_vec2f_from_xy(ix * texdimu / ndivx, iy * texdimv / ndivy);
			if (n) {
				EleaVec3f normal;
				if (height) {
					normal = EleaVec3f0;
					EleaVec3f pc = {vx, vy, vz};
					EleaVec3f p[4];
					for (int i = 0; i < 2; i ++) {
						float px = pc.x + ((float) ix + 2 * i - 1) * (p1[0] - p0[0]) / ndivx;
						float pz = (height) ? height (px, vy, data) : 0;
						elea_vec3fp_set_xyz(&p[2 * i], px, vy, pz);
						float py = pc.y + ((float) iy + 2 * i - 1) * (p1[1] - p0[1]) / ndivx;
						pz = (height) ? height (vx, py, data) : 0;
						elea_vec3fp_set_xyz(&p[2 * i + 1], vx, py, pz);
					}
					for (int i = 0; i < 4; i++) {
						EleaVec3f d0 = elea_vec3f_sub(p[i], pc);
						EleaVec3f d1 = elea_vec3f_sub(p[(i + 1) % 4], pc);
						EleaVec3f v = elea_vec3f_cross(d0, d1);
						normal = elea_vec3f_add(normal, v);
					}
					normal = elea_vec3f_normalize(normal);
				} else {
					normal = EleaVec3fZ;
				}
				*((EleaVec3f *) ((char *) n + vpos * nstridebytes)) = normal;
			}
		}
	}
	if (indices) {
		unsigned int ipos = 0;
		for (unsigned int iy = 0; iy < ndivy; iy++) {
			for (unsigned int ix = 0; ix < ndivx; ix++) {
				unsigned int vpos = iy * (ndivx + 1) + ix;
				indices[ipos++] = vpos;
				indices[ipos++] = vpos + 1;
				indices[ipos++] = vpos + (ndivx + 1) + 1;
				indices[ipos++] = vpos;
				indices[ipos++] = vpos + (ndivx + 1) + 1;
				indices[ipos++] = vpos + (ndivx + 1);
			}
		}
	}
}

void
elea_generate_box (float *v, unsigned int v_stride_bytes, float *n, unsigned int n_stride_bytes, float *tex, unsigned int tex_stride_bytes, unsigned int *indices, const EleaVec3f *p0, const EleaVec3f *p1, int dir)
{
	EleaVec3f corners[8];
	for (int i = 0; i < 8; i++) {
		int ix = ((i + 1) >> 1) & 1;
		int iy = (i >> 1) & 1;
		int iz = (i >> 2) & 1;
		corners[i].x = (1 - ix) * p0->x + ix * p1->x;
		corners[i].y = (1 - iy) * p0->y + iy * p1->y;
		corners[i].z = (1 - iz) * p0->z + iz * p1->z;
	}
	generateBox (v, v_stride_bytes, n, n_stride_bytes, tex, tex_stride_bytes, indices, corners, dir);
}

unsigned int
elea_get_intersection (const float *lposition, const float *lvelocity, const float *pnormal, float pdistance, const EleaMat3x4f *e2us, const EleaMat3x4f *us2e, float *t, EleaVec3f *cpus, EleaVec3f *cpobj)
{
	// Transform everything to unit sphere space
	EleaVec3f basePoint, velocity;
	elea_mat3x4f_transform_point_xyz(&basePoint, e2us, lposition[0], lposition[1], lposition[2]);
	elea_mat3x4f_transform_vec3_xyz(&velocity, e2us, lvelocity[0], lvelocity[1], lvelocity[2]);
	// Get point on plane & transform
	EleaVec3f p, normal;
	elea_mat3x4f_transform_point_xyz(&p, e2us, pnormal[0] * -pdistance, pnormal[1] * -pdistance, pnormal[2] * -pdistance);
	// Transform normal
	elea_mat3x4f_transform_vec3_xyz(&normal, e2us, pnormal[0], pnormal[1], pnormal[2]);
	float distance = -elea_vec3f_dot(normal, p);

	// signedDistance (N * p + Cp)
	// t0 = (1 - signedDistance (basePoint)) / (N * velocity)
	// t1 = (-1 - signedDistance (basePoint)) / (N * velocity)
	float signedDistance = elea_vec3f_dot (normal, basePoint) + distance;
	float N_dot_velocity = elea_vec3f_dot (normal, velocity);
	float t0 = 0;
	float t1 = 1;
	if (!N_dot_velocity) {
		// Velocity is parallel to plane
		if (signedDistance < -1) return COLLINEAR_IN;
		if (signedDistance > 1) return COLLINEAR_OUT;
		return COLLINEAR_MIXED;
	}
	// Normal collision
	t0 = ( 1 - signedDistance) / N_dot_velocity;
	t1 = (-1 - signedDistance) / N_dot_velocity;
	if (t0 <= t1) {
		// (N_dot_velocity < 0) - we touch plane from outside
		if (t) *t = t0;
		if (cpus) {
			*cpus = elea_vec3f_inv(normal);
		}
		if (cpobj) {
			EleaVec3f v = elea_vec3f_sub(basePoint, normal);
			v = elea_vec3f_mul_add(v, velocity, t0);
			elea_mat3x4f_transform_point(cpobj, us2e, &v);
		}
		return INTERSECTING_OUTSIDE;
	}
	// (N_dot_velocity > 0) - we touch plane from inside
	if (t) *t = t0;
	if (cpus) {
		*cpus = elea_vec3f_inv(normal);
	}
	if (cpobj) {
		EleaVec3f v = elea_vec3f_sub(basePoint, normal);
		v = elea_vec3f_mul_add(v, velocity, t0);
		elea_mat3x4f_transform_point(cpobj, us2e, &v);
	}
	return INTERSECTING_INSIDE;
}

unsigned int
elea_get_triangle_mesh_intersection (const float *v, uint32_t vstridebytes, const uint32_t *indices, uint32_t nindices, const EleaMat3x4f *e2us, const EleaVec3f *v_us, const EleaAABox3f *bbox, float *t, EleaVec3f *cpus, uint32_t *triangleindex)
{
	float bestt = INFINITY;
	EleaVec3f bestcp = EleaVec3f0;
	uint32_t besti = 0;

	for (uint32_t i = 0; i < nindices; i += 3) {
		// The corners of triangle
		const EleaVec3f *p0 = (const EleaVec3f *)((const char *) v + indices[i + 0] * vstridebytes);
		const EleaVec3f *p1 = (const EleaVec3f *)((const char *) v + indices[i + 1] * vstridebytes);
		const EleaVec3f *p2 = (const EleaVec3f *)((const char *) v + indices[i + 2] * vstridebytes);
		//if ((p0 == p1) || (p0 == p2) || (p1 == p2)) continue;
		float lt;
		EleaVec3f lcp;
		if (elea_get_ellipsoid_tri_intersection (p0, p1, p2, e2us, v_us, bbox, &lt, &lcp)) {
			if (lt < bestt) {
				bestt = lt;
				bestcp = lcp;
				besti = i;
			}
		}
	}

	if (bestt < INFINITY) {
		if (t) *t = bestt;
		if (cpus) *cpus = bestcp;
		if (triangleindex) *triangleindex = besti;
		return 1;
	}

	return 0;
}

static unsigned int
cage_gen_side (float *v, unsigned int v_stride_bytes, const EleaVec3f *p, unsigned int c0, unsigned int c1, int s, unsigned int n_subdiv)
{
	EleaVec3f p0, p1;
	p0 = p1 = *p;
	p1.c[c1] += s;
	unsigned int first, last;
	unsigned int vpos = 0;
	if (c1 == 2) {
		/* Secondary dimension is Z (sides vertical) */
		if (c0 == 0) {
			/* For X side draw first */
			first = 0;
			last = n_subdiv;
		} else {
			/* For Y side draw last */
			first = 1;
			last = n_subdiv + 1;
		}
	} else if (c0 == 2) {
		/* If primary dimension is Z (sides horizontal) draw first and last */
		first = 0;
		last = n_subdiv + 1;
	} else {
		/* For top and bottom draw only internal */
		first = 1;
		last = n_subdiv;
	}
	float astep = ( float) s / (n_subdiv + 1);
	for (unsigned int i = first; i <= last; i++) {
		EleaVec3f p0_l = p0;
		EleaVec3f p1_l = p1;
		p0_l.c[c0] = p0.c[c0] + i * astep;
		p1_l.c[c0] = p1.c[c0] + i * astep;
		if (v) {
			memcpy (( char *) v + vpos * v_stride_bytes, p0_l.c, 12);
			memcpy (( char *) v + (vpos + 1) * v_stride_bytes, p1_l.c, 12);
		}
		vpos += 2;
	}
	return vpos;
}

void
elea_generate_cage (float *v, unsigned int v_stride_bytes, unsigned int *indices, unsigned int n_subdivs, unsigned int *n_vertices, unsigned int *n_indices)
{
	unsigned int vpos = 0;
	static const EleaVec3f c1 = { 1, 1, 1 };
	for (unsigned int c = 0; c < 3; c++) {
		unsigned int ax2 = (c + 1) % 3;
		vpos += cage_gen_side ((v) ? v + 3 * vpos : NULL, 12, &EleaVec3f0, c, ax2, 1, n_subdivs);
		vpos += cage_gen_side ((v) ? v + 3 * vpos : NULL, 12, &EleaVec3f0, ax2, c, 1, n_subdivs);
		vpos += cage_gen_side ((v) ? v + 3 * vpos : NULL, 12, &c1, c, ax2, -1, n_subdivs);
		vpos += cage_gen_side ((v) ? v + 3 * vpos : NULL, 12, &c1, ax2, c, -1, n_subdivs);
	}
	if (indices) {
		for (unsigned int i = 0; i < vpos; i++) indices[i] = i;
	}
	if (n_vertices) *n_vertices = vpos;
	if (n_indices) *n_indices = vpos;
}

void
elea_generate_sphere (float *v, unsigned int v_stride_bytes, float *n, unsigned int n_stride_bytes, float *x, unsigned int x_stride_bytes, unsigned int *indices, float radius, int level, int dir, unsigned int *n_vertices, unsigned int *n_indices)
{
	// Subdiv 0
	// nvertices = 7;
	int nfaces = 8;
	int nsides = 24;
	// fixme: calculate proper number of new vertices
	for (int i = 0; i < level; i++) {
		// Each subdivision creates 12 sides and max 3 vertices per side
		// nvertices += 3 * nfaces;
		nsides += 12 * nfaces;
		nfaces *= 4;
	}
	// nindices = 3 * nfaces;

	Side *sides = (Side *) malloc (nsides * sizeof (Side));
	for (int i = 0; i < nsides; i++) side_initialize(&sides[i]);

	static const EleaVec3f vdata[] = { { 1, 0, 0 }, { 0, 1, 0 }, { -1, 0, 0 }, { 0, -1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 0, 0, -1 } };
	static const EleaVec2f tdata[] = { { 0, 0.5f }, { 0.25f, 0.5f }, { 0.5f, 0.5f }, { 0.75f, 0.5f }, { 1.0f, 0.5f }, { 0.5f, 1 }, { 0.5f, 0 } };
	static const int idata[] = { 0, 1, 5, 1, 2, 5, 2, 3, 5, 3, 4, 5, 4, 3, 6, 3, 2, 6, 2, 1, 6, 1, 0, 6 };

	unsigned int vpos = 0;
	unsigned int spos = 0;
	unsigned int ipos = 0;
	// Set initial vertices
	for (int i = 0; i < 7; i++) {
		if (v) *((EleaVec3f *) ((char *) v + vpos * v_stride_bytes)) = elea_vec3f_mul (vdata[i], radius);
		if (n) *((EleaVec3f *) ((char *) n + vpos * n_stride_bytes)) = elea_vec3f_mul (vdata[i], (float) dir);
		if (x) *(( EleaVec2f *) ((char *) x + vpos * x_stride_bytes)) = tdata[i];
		vpos += 1;
	}
	// Set initial sides
	for (int i = 0; i < 8; i++) {
		sides[spos].v0 = idata[3 * i + 0];
		sides[spos].v1 = idata[3 * i + 1];
		spos += 1;
		sides[spos].v0 = idata[3 * i + 1];
		sides[spos].v1 = idata[3 * i + 2];
		spos += 1;
		sides[spos].v0 = idata[3 * i + 2];
		sides[spos].v1 = idata[3 * i + 0];
		spos += 1;
	}
	// Opposites
	for (int i = 0; i < 24; i++) {
		for (int j = i + 1; j < 24; j++) {
			if ((sides[i].v0 == sides[j].v1) && (sides[i].v1 == sides[j].v0)) {
				sides[i].opposite = ( int) j;
				sides[j].opposite = ( int) i;
				break;
			}
		}
	}
	// Subdivide sides
	CBuf cbuf;
	cbuf.vertices = v;
	cbuf.vstridebytes = v_stride_bytes;
	cbuf.normals = n;
	cbuf.nstridebytes = n_stride_bytes;
	cbuf.texcoords = x;
	cbuf.tstridebytes = x_stride_bytes;
	for (int i = 0; i < 8; i++) {
		subdivide (&sides[0], &spos, &cbuf, &vpos, &indices[0], &ipos, 3 * i, 3 * i + 1, 3 * i + 2, radius, level);
	}
	if (dir < 0) {
		if (indices) {
			for (unsigned int i = 0; i < ipos; i += 3) {
				unsigned int tval = indices[i + 1];
				indices[i + 1] = indices[i + 2];
				indices[i + 2] = tval;
			}
		}
	}
	free (sides);

	if (n_vertices) *n_vertices = vpos;
	if (n_indices) *n_indices = ipos;
}
