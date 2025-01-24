#define __ELEA_COLLISION_C__

/*
 * Libelea
 *
 * Copyright (C) Lauris Kaplinski 2022
 *
 */

#include <math.h>

#include <elea/collision.h>


static float
elea_seg_get_closest_t_to_point (const EleaVec3f *a0, const EleaVec3f *a1, const EleaVec3f *p)
{
	EleaVec3f ab = elea_vec3f_sub (*a1, *a0);
	EleaVec3f ap = elea_vec3f_sub (*p, *a0);
	float t = elea_vec3f_dot (ap, ab) / elea_vec3f_len2 (ab);
	return (t < 0) ? 0 : (t > 1) ? 1 : t;
}

static void
elea_seg_get_closest_pt_to_point (EleaVec3f *d, const EleaVec3f *a0, const EleaVec3f *a1, const EleaVec3f *p)
{
	EleaVec3f ab = elea_vec3f_sub (*a1, *a0);
	float t = elea_seg_get_closest_t_to_point (a0, a1, p);
	*d = elea_vec3f_mul_add (*a0, ab, t);
}

static void
elea_seg_get_closest_pt_to_seg (EleaVec3f *da, EleaVec3f *db, const EleaVec3f *a0, const EleaVec3f *a1, const EleaVec3f *b0, const EleaVec3f *b1)
{
	EleaVec3f a, a_n, b, a0b0, a0b1;
	float d_b0, d_b1;
	EleaVec3f p_b0, p_b1;
	float t;
	a = elea_vec3f_sub (*a1, *a0);
	b = elea_vec3f_sub (*b1, *b0);
	a0b0 = elea_vec3f_sub (*b0, *a0);
	a0b1 = elea_vec3f_sub (*b1, *a0);
	/* Normalized directions */
	a_n = elea_vec3f_normalize(a);
	/* Find B projections onto A direction */
	d_b0 = elea_vec3f_dot (a_n, a0b0);
	d_b1 = elea_vec3f_dot (a_n, a0b1);
	/* Find B projection onto A plane */
	p_b0 = elea_vec3f_mul_add (a0b0, a_n, -d_b0);
	p_b1 = elea_vec3f_mul_add (a0b1, a_n, -d_b1);
	/* Find the closest point on B, using projected B and A0 */
	t = elea_seg_get_closest_t_to_point (&p_b0, &p_b1, a0);
	*db = elea_vec3f_mul_add (*b0, b, t);
	/* Find closest point on A */
	elea_seg_get_closest_pt_to_point (da, a0, a1, db);
}

static void
elea_tri_get_closest_pt_to_point (EleaVec3f *d, const EleaVec3f *a, const EleaVec3f *b, const EleaVec3f *c, const EleaVec3f *p)
{
	EleaVec3f ab, bc, ca, n, ap, bp, cp, l, pl, best;
	float len2, best2;
	/* Find plane normal (not normalized) */
	ab = elea_vec3f_sub (*b, *a);
	bc = elea_vec3f_sub (*c, *b);
	ca = elea_vec3f_sub (*a, *c);
	n = elea_vec3f_cross (ab, bc);
	/* Check whether point projection is inside triangle */
	ap = elea_vec3f_sub (*p, *a);
	l = elea_vec3f_cross (ap, ab);
	if (elea_vec3f_dot (l, n) <= 0) {
		bp = elea_vec3f_sub (*p, *b);
		l = elea_vec3f_cross (bp, bc);
		if (elea_vec3f_dot (l, n) <= 0) {
			cp = elea_vec3f_sub (*p, *c);
			l = elea_vec3f_cross (cp, ca);
			if (elea_vec3f_dot (l, n) <= 0) {
				float dist;
				/* Point is inside triangle, find projection */
				n = elea_vec3f_normalize(n);
				dist = elea_vec3f_dot (ap, n);
				*d = elea_vec3f_mul_add (*p, n, -dist);
				return;
			}
		}
	}
	/* Point is outside, test against all edges */
	elea_seg_get_closest_pt_to_point (&best, a, b, p);
	pl = elea_vec3f_sub (best, *p);
	best2 = elea_vec3f_len2 (pl);
	elea_seg_get_closest_pt_to_point (&l, b, c, p);
	l = elea_vec3f_sub (pl, *p);
	len2 = elea_vec3f_len2 (pl);
	if (len2 < best2) best = l;
	elea_seg_get_closest_pt_to_point (&l, c, a, p);
	l = elea_vec3f_sub (pl, *p);
	len2 = elea_vec3f_len2 (pl);
	if (len2 < best2) best = l;
	*d = best;
}

unsigned int
elea_sphere_sphere_collision (EleaCollision *result, const EleaSphere *s0, const EleaSphere *s1)
{
	EleaVec3f d10 = elea_vec3f_sub (s0->c, s1->c);
	float dist = elea_vec3f_len (d10);
	if (dist > (s0->r + s1->r)) return 0;
	result->depth = dist - s0->r - s1->r;
	result->n = elea_vec3f_normalize(d10);
	result->p = elea_vec3f_mul_add (s1->c, result->n, s1->r);
	return 1;
}

unsigned int
elea_sphere_tri_collision (EleaCollision *result, const EleaSphere *s0, const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2)
{
	EleaVec3f s01, s12, s20, n, pc;
	EleaVec3f d, c;
	float dist;
	/* Plane normal */
	s01 = elea_vec3f_sub (*p1, *p0);
	s12 = elea_vec3f_sub (*p2, *p1);
	s20 = elea_vec3f_sub (*p0, *p2);
	n = elea_vec3f_cross (s01, s12);
	n = elea_vec3f_normalize(n);
	/* Signed distance between sphere and plane */
	d = elea_vec3f_sub (s0->c, *p0);
	dist = elea_vec3f_dot (d, n);
	if ((dist < -s0->r) || (dist > s0->r)) return 0;
	/* Project center to triangle plane */
	pc = elea_vec3f_mul_add (s0->c, n, -dist);
	/* Check whether it is inside triangle */
	d = elea_vec3f_sub (pc, *p0);
	c = elea_vec3f_cross (d, s01);
	if (elea_vec3f_dot (c, n) <= 0) {
		d = elea_vec3f_sub (pc, *p1);
		c = elea_vec3f_cross (d, s12);
		if (elea_vec3f_dot (c, n) <= 0) {
			d = elea_vec3f_sub (pc, *p2);
			c = elea_vec3f_cross (d, s20);
			if (elea_vec3f_dot (c, n) <= 0) {
				/* Point is inside triangle */
				result->p = pc;
				result->n = n;
				d = elea_vec3f_sub (s0->c, pc);
				result->depth = s0->r - elea_vec3f_len (d);
				return 1;
			}
		}
	}
	/* Projection is outside triangle */
	float r2 = s0->r * s0->r;
	EleaVec3f best;
	float d2, best_d2 = INFINITY;
	/* Side 0-1 */
	elea_seg_get_closest_pt_to_point (&pc, p0, p1, &s0->c);
	d = elea_vec3f_sub (s0->c, pc);
	d2 = elea_vec3f_len2 (d);
	if ((d2 <= r2) && (d2 < best_d2)) {
		best = pc;
		best_d2 = d2;
	}
	/* Side 1-2 */
	elea_seg_get_closest_pt_to_point (&pc, p1, p2, &s0->c);
	d = elea_vec3f_sub (s0->c, pc);
	d2 = elea_vec3f_len2 (d);
	if ((d2 <= r2) && (d2 < best_d2)) {
		best = pc;
		best_d2 = d2;
	}
	/* Side 2-0 */
	elea_seg_get_closest_pt_to_point (&pc, p2, p0, &s0->c);
	d = elea_vec3f_sub (s0->c, pc);
	d2 = elea_vec3f_len2 (d);
	if ((d2 <= r2) && (d2 < best_d2)) {
		best = pc;
		best_d2 = d2;
	}
	if (best_d2 <= r2) {
		result->p = pc;
		d = elea_vec3f_sub (s0->c, pc);
		result->n = elea_vec3f_normalize(d);
		result->depth = s0->r - elea_vec3f_len (d);
		return 1;
	}
	return 0;
}

unsigned int
elea_sphere_capsule_collision (EleaCollision *result, const EleaSphere *s0, const EleaVec3f *p0, const EleaVec3f *p1, float r)
{
	EleaVec3f p, d;
	float l2;
	elea_seg_get_closest_pt_to_point (&p, p0, p1, &s0->c);
	d = elea_vec3f_sub (s0->c, p);
	l2 = elea_vec3f_len2 (d);
	if (l2 > (s0->r + r)) return 0;
	result->n = elea_vec3f_normalize(d);
	result->p = elea_vec3f_mul_add (p, result->n, r);
	result->depth = s0->r + r - sqrtf (l2);
	return 1;
}

unsigned int
elea_capsule_capsule_collision (EleaCollision *result, const EleaVec3f *a0, const EleaVec3f *a1, float ra, const EleaVec3f *b0, const EleaVec3f *b1, float rb)
{
	EleaVec3f pa, pb, d;
	float l2;
	elea_seg_get_closest_pt_to_seg (&pa, &pb, a0, a1, b0, b1);
	d = elea_vec3f_sub (pa, pb);
	l2 = elea_vec3f_len2 (d);
	if (l2 > (ra + rb)) return 0;
	result->n = elea_vec3f_normalize(d);
	result->p = elea_vec3f_mul_add (pb, result->n, rb);
	result->depth = ra + rb - sqrtf (l2);
	return 1;
}

unsigned int
elea_capsule_tri_collision (EleaCollision *result, const EleaVec3f *a, const EleaVec3f *b, float r, const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2)
{
	EleaVec3f s01, s12, s20, p_n, c_n, d, is, tp, cp;
	float scale, t, len2;
	/* Plane normal */
	s01 = elea_vec3f_sub (*p1, *p0);
	s12 = elea_vec3f_sub (*p2, *p1);
	s20 = elea_vec3f_sub (*p0, *p2);
	p_n = elea_vec3f_cross (s01, s12);
	p_n = elea_vec3f_normalize(p_n);
	/* Capsule normal */
	c_n = elea_vec3f_sub (*b, *a);
	c_n = elea_vec3f_normalize(c_n);
	/* Capsule axis/plane intersection */
	scale = elea_vec3f_dot (p_n, c_n);
	d = elea_vec3f_sub (*p0, *a);
	t = elea_vec3f_dot (p_n, d) / scale;
	is = elea_vec3f_mul_add (*a, c_n, t);
	/* Find closest point at tri to intersection */
	elea_tri_get_closest_pt_to_point (&tp, p0, p1, p2, &is);
	/* Find closest point on line segment to triangle point */
	elea_seg_get_closest_pt_to_point (&cp, a, b, &tp);
	/* Distance vector */
	d = elea_vec3f_sub (cp, tp);
	len2 = elea_vec3f_len2 (d);
	if (len2 > (r * r)) return 0;
	result->p = tp;
	result->n = elea_vec3f_normalize(d);
	result->depth = r - sqrtf (len2);
	return 1;
}
