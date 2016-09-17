#ifndef MATH_3D
#define MATH_3D

#include "myCommon.h"

typedef struct {
	float x;
	float y;
	float z;
} Point3D, Vertex, Vector3D;

typedef Vertex* Vertices;

typedef struct {
	Vertex a;
	Vertex b;
} Edge;

typedef struct {
	Vector3D normal;
	Vertices vertices;
} _face;
typedef _face* Face;

Point3D createPoint3D(float x, float y, float z);
Vertex createVertex(float x, float y, float z);
Vector3D createVector3D(float x, float y, float z);

bool samePoint(Point3D a, Point3D b);

Edge createEdge(Vertex a, Vertex b);

Face createFace(Vector3D normal, Vertices vertices);

Face* createCube(Vector3D* normals, Vertices vertices);

#endif
