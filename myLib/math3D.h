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

Point3D translate(Point3D p, Vector3D inc, bool inverse);
Point3D scale(Point3D p, Vector3D factor, bool inverse);
Point3D rotateZ(Point3D p, Point3D origin, float theta, bool inverse);
Point3D rotateY(Point3D p, Point3D origin, float theta, bool inverse);
Point3D rotateX(Point3D p, Point3D origin, float theta, bool inverse);

float innerProduct(Vector3D a, Vector3D b);
Point3D project(Vector3D planeRotation, Point3D p, int D);
#endif
