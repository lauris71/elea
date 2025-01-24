#define __ELEA_RAY_CPP__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2007-2019
 *
 */

#include <stdio.h>

#include "vector3.h"
#include "plane.h"

#include "ray.h"

EleaRay3f EleaRay3fX = { 0, 0, 0, 1, 0, 0 };
EleaRay3f EleaRay3fY = { 0, 0, 0, 0, 1, 0 };
EleaRay3f EleaRay3fZ = { 0, 0, 0, 0, 0, 1 };

float
elea_ray3f_distance_point (const EleaRay3f* l, const EleaVec3f* p)
{
	EleaVec3f lp, c;
	lp = elea_vec3f_sub (*p, l->p);
	c = elea_vec3f_cross (lp, l->d);
	return sqrtf (elea_vec3f_len2 (c) / elea_vec3f_len2 (l->d));
}

float
elea_ray3f_distance_ray (const EleaRay3f *lhs, const EleaRay3f *rhs)
{
	EleaVec3f n;
	float d_lhs, d_rhs, len2;
	n = elea_vec3f_cross (lhs->d, rhs->d);
	len2 = elea_vec3f_len2 (n);
	if (len2 >= ELEA_EPSILON2_F) {
		d_lhs = -elea_vec3f_dot (n, lhs->p);
		d_rhs = -elea_vec3f_dot (n, rhs->p);
		return fabsf (d_lhs - d_rhs) / sqrtf (len2);
	} else {
		return elea_ray3f_distance_point (lhs, &rhs->p);
	}
}

unsigned int
elea_ray3f_get_closest_t (const EleaRay3f *lhs, const EleaRay3f *rhs, float* t_lhs, float* t_rhs)
{
	const EleaVec3f *u, *v;
	EleaVec3f w;
	float a, b, c, d, e, D;
	u = &lhs->d;
	v = &rhs->d;
	w = elea_vec3f_sub (lhs->p, rhs->p);
	a = elea_vec3f_dot (*u, *u);
	b = elea_vec3f_dot (*u, *v);
	c = elea_vec3f_dot (*v, *v);
	d = elea_vec3f_dot (*u, w);
	e = elea_vec3f_dot (*v, w);
	D = a * c - b * b;
	// Compute the line parameters of the two closest points
	if (!D) {
		if (t_lhs) *t_lhs = 0;
		if (t_rhs) *t_rhs = (b > c) ? d / b : e / c;
		return 0;
	}
	if (t_lhs) *t_lhs = (b * e - c * d) / D;
	if (t_rhs) *t_rhs = (a * e - b * d) / D;
	return 1;
}

unsigned int
elea_ray3f_get_closest_points (const EleaRay3f *lhs, const EleaRay3f *rhs, EleaVec3f* p_lhs, EleaVec3f* p_rhs)
{
	float t_lhs, t_rhs;
	unsigned int result = elea_ray3f_get_closest_t (lhs, rhs, &t_lhs, &t_rhs);
	if (p_lhs) elea_ray3f_get_point_at (p_lhs, lhs, t_lhs);
	if (p_rhs) elea_ray3f_get_point_at (p_rhs, rhs, t_rhs);
	return result;
}

