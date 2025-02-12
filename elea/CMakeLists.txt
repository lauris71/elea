set(ELEA_HEADERS
	aabox.h
	collision.h
	color.h
	geometry.h
	matrix3x3.h
	matrix3x4.h
	matrix4x4.h
	plane.h
	polyhedron.h
	quaternion.h
	ray.h
	types.h
	vector2.h
	vector3.h
	vector4.h
)

set(ELEA_SOURCES
	aabox.c
	collision.c
	color.c
	geometry.c
	matrix3x3.c
	matrix3x4.c
	matrix4x4.c
	plane.c
	quaternion.c
	ray.c
	vector2.c
	vector3.c
	vector4.c
)

add_library(elea STATIC
	${ELEA_HEADERS}
	${ELEA_SOURCES}
)

set_property(TARGET elea PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(elea PUBLIC
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/arikkei
    ${CMAKE_SOURCE_DIR}/az
)
