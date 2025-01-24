#ifndef __ELEA_GEOMETRY_H__
#define __ELEA_GEOMETRY_H__

//
// Libelea
//
// Copyright (C) Lauris Kaplinski 2009-2011
//

#include <math.h>
#include <string.h>

#include <elea/types.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int elea_get_ray_tri_intersection (const EleaRay3f *ray, const EleaVec3f *v0, const EleaVec3f *v1, const EleaVec3f *v2, float* t);
unsigned int elea_get_ray_tri_mesh_intersection (const EleaRay3f *ray, const float *vertices, unsigned int v_stride_bytes, const unsigned int *indices, unsigned int n_indices, float *t, unsigned int *index);

///
/// Get the smallest intersection time and collision point of moving ellipsoid and triangle.
/// @param p0 Vertex 1 of CCW triangle
/// @param p1 Vertex 2 of CCW triangle
/// @param p2 Vertex 3 of CCW triangle
/// @param e2us the transformation matrix from ellipsoid to unit sphere (scaling + rotation + translation)
/// @param vus the speed vector of unit sphere (in unit sphere space)
/// @param bbox the bounding box of full unit sphere move (in triangle space)
/// @param t the pointer to a variable where the intersection time will be stored
/// @param cpus the pointer to a variable where the interection point on unit sphere (i.e. x2 + y2 + z2 = 1) will be stored
/// @return true if intersection in range 0..1 was found, false otherwise
///
/// Only CCW intersections in time range 0..1 are calculated
///

unsigned int elea_get_ellipsoid_tri_intersection (const EleaVec3f *p0, const EleaVec3f *p1, const EleaVec3f *p2, const EleaMat3x4f *e2us, const EleaVec3f *v_us, const EleaAABox3f *bbox, float *t, EleaVec3f *cpus);

//unsigned int elea_get_ray_plane_intersection (const float *p, const float *dir, const float *plane_n, float plane_dist, const Matrix3x4f *e2us, float *t, float *p);

///
/// Generate axis-aligned cuboid with correct normals and texture coordinates.
/// @param v the buffer for generated vertices (NULL is allowed)
/// @param v_stride_bytes the stride of vertices in destination buffer (in bytes)
/// @param n the buffer for generated normals (NULL is allowed)
/// @param n_stride_bytes the stride of normals in destination buffer (in bytes)
/// @param x the buffer for generated texture coordinates (NULL is allowed)
/// @param x_stride_bytes the stride of texture coordinates in destination buffer (in bytes)
/// @param indices the buffer for generated indices (NULL is allowed)
/// @param p0 a corner of the box
/// @param p1 the opposite corner of the box
/// @param dir the direction of normals (1 outside, -1 inside)
///
/// All buffer locations are allowed to be NULL.
/// Generated box will always have 24 vertices and 36 indices
///

void elea_generate_box (float *v, unsigned int v_stride_bytes, float *n, unsigned int n_stride_bytes, float *x, unsigned int x_stride_bytes, unsigned int *indices, const EleaVec3f *p0, const EleaVec3f *p1, int dir);
void elea_generate_cage (float *v, unsigned int v_stride_bytes, unsigned int *indices, unsigned int n_subdivs, unsigned int *n_vertices, unsigned int *n_indices);

///
/// Generate subdivided octahedron based sphere with correct normals and texture coordinates.
/// @param v the buffer for generated vertices (NULL is allowed)
/// @param v_stride_bytes the stride of vertices in destination buffer (in bytes)
/// @param n the buffer for generated normals (NULL is allowed)
/// @param n_stride_bytes the stride of normals in destination buffer (in bytes)
/// @param x the buffer for generated texture coordinates (NULL is allowed)
/// @param x_stride_bytes the stride of texture coordinates in destination buffer (in bytes)
/// @param indices the buffer for generated indices (NULL is allowed)
/// @param radius the radius of generated sphere
/// @param level the number of subdivisions to perform
/// @param dir the direction of normals (1 outside, -1 inside)
/// @param n_vertices the pointer to a variable where the number of generated vertices will be stored
/// @param n_indices the pointer to a variable where the number of generated indices will be stored
///
/// All buffer locations are allowed to be NULL in which case only the number of vertices and indices are written.
/// So one can calculate the correct size of buffers in the first run and fill the buffers on the second run.
///

void elea_generate_sphere (float *v, unsigned int v_stride_bytes, float *n, unsigned int n_stride_bytes, float *x, unsigned int x_stride_bytes, unsigned int *indices, float radius, int level, int dir, unsigned int *n_vertices, unsigned int *n_indices);

///
/// Calculate smooth vertex normals by averaging face normals at vertices.
/// @param n the buffer for generated normals
/// @param nstridebytes the stride of normals in destination buffer (in bytes)
/// @param v the buffer of vertices
/// @param vstridebytes the stride of vertices in buffer (in bytes)
/// @param nvertices the number of vertices (and generated normals) in buffer
/// @param indices the buffer of indices
/// @param nindices the number of indices in buffer
/// @param normalize true if normal vectors should be normalized, false otherwise
///
/// Indices are processed in groups of 3 as triangles. If the total number of indices is not divisible by 3 that last ones are ignored.
///

void elea_calculate_normals (float *n, uint32_t nstridebytes, const float *v, uint32_t vstridebytes, uint32_t nvertices, const uint32_t *indices, uint32_t nindices, unsigned int normalize);

///
/// Calculate tangents by projecting and averaging dS to normal plane.
/// @param t the buffer for generated tangents
/// @param tstridebytes the stride of tangents in destination buffer (in bytes)
/// @param v the buffer of vertices
/// @param vstridebytes the stride of vertices in buffer (in bytes)
/// @param n the buffer of normals
/// @param nstridebytes the stride of normals in buffer (in bytes)
/// @param x the buffer of texture coordinates
/// @param xstridebytes the stride of texture coordinates in buffer (in bytes)
/// @param nvertices the number of vertices (and generated tangents) in buffer
/// @param indices the buffer of indices
/// @param nindices the number of indices in buffer
/// @param normalize true if tangent vectors should be normalized, false otherwise
///
/// Indices are processed in groups of 3 as triangles. The total number of indices has to be divisible by 3
///

void elea_calculate_tangents (float *t, uint32_t tstridebytes, const float *v, uint32_t vstridebytes, const float *n, uint32_t nstridebytes, const float *x, uint32_t xstridebytes, uint32_t nvertices, const uint32_t *indices, uint32_t nindices, unsigned int normalize);

///
/// Generate axis-aligned subdivided plane with correct normals and texture coordinates.
/// @param v the buffer for generated vertices (NULL is allowed)
/// @param vstridebytes the stride of vertices in destination buffer (in bytes)
/// @param n the buffer for generated normals (NULL is allowed)
/// @param nstridebytes the stride of normals in destination buffer (in bytes)
/// @param x the buffer for generated texture coordinates (NULL is allowed)
/// @param xstridebytes the stride of texture coordinates in destination buffer (in bytes)
/// @param indices the buffer for generated indices (NULL is allowed)
/// @param p0 a corner of the plane (x and y)
/// @param p1 the opposite corner of the plane (x and y)
/// @param ndivx the number of divisions in x direction
/// @param ndivy the number of divisions in y direction
/// @param texdimu the texture dimension in u direction
/// @param texdimv the texture dimension in v direction
/// @param height the callback to determine height
/// @param data the parameter to height callback
///
/// All buffer locations and height callback are allowed to be NULL.
/// Height callback has to give correct results for the outside of the plane if normal calculation is needed
///

void elea_generate_plane (float *v, unsigned int vstridebytes, float *n, unsigned int nstridebytes, float *x, unsigned int xstridebytes, unsigned int *indices, const float *p0, const float *p1, unsigned int ndivx, unsigned int ndivy, float texdimu, float texdimv, float (* height) (float, float, void *), void *data);

///
/// Get the intersection time and collision point of moving ellipsoid and plane.
/// @param lposition the base point of movement line
/// @param lvelocity the velocity vector of movement line
/// @param pnormal the plane normal
/// @param pdistance the plane distance
/// @param e2us the transformation matrix from ellipsoid to unit sphere (scaling + rotation)
/// @param us2e the transformation matrix from unit sphere to ellipsoid (scaling + rotation)
/// @param t the pointer to a variable where the intersection time will be stored
/// @param cpus the pointer to a variable where the intersection point in unit sphere space will be stored
/// @param cpobject the pointer to a variable where the intersection point in object space will be stored
/// @return PlanePosition of intersection
///
/// If velocity is not collinear with plane the intersection time and point is written (the moment when ellipsoid is touching plane from outer side).
///

unsigned int elea_get_intersection (const float *lposition, const float *lvelocity, const float *pnormal, float pdistance, const EleaMat3x4f *e2us, const EleaMat3x4f *us2e, float *t, EleaVec3f *cpus, EleaVec3f *cpobj);

///
/// Get the smallest intersection time and collision point of moving ellipsoid and triangle mesh.
/// @param v the buffer of vertices
/// @param vstridebytes the stride of vertices in buffer (in bytes)
/// @param indices the buffer of indices
/// @param nindices the number of indices in buffer
/// @param e2us the transformation matrix from ellipsoid to unit sphere (scaling + rotation)
/// @param v_us the speed vector of unit sphere (in unit sphere space)
/// @param bbox the bounding box of full unit sphere move (in mesh space)
/// @param t the pointer to a variable where the intersection time will be stored
/// @param cpus the pointer to a variable where the intersection point in unit sphere (i.e. x2 + y2 + z2 = 1) will be stored
/// @param triangleindex the pointer to a variable where the index of triangle (first corner) will be stored
/// @return true if intersection in range 0..1 was found, false otherwise
///
/// Only CCW intersections in time range 0-1 are calculated
/// Indices are processed in groups of 3 as triangles. The total number of indices has to be divisible by 3
///
unsigned int elea_get_triangle_mesh_intersection (const float *v, uint32_t vstridebytes, const uint32_t *indices, uint32_t nindices, const EleaMat3x4f *e2us, const EleaVec3f *v_us, const EleaAABox3f *bbox, float *t, EleaVec3f *cpus, uint32_t *triangleindex);

#ifdef __cplusplus
};
#endif

#endif
