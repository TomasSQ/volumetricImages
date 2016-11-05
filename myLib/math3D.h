#ifndef MATH_3D
#define MATH_3D

#include "myCommon.h"

typedef struct {
	float x;
	float y;
	float z;
} _point3D;

typedef _point3D* Point3D;
typedef _point3D* Vertex;
typedef _point3D* Vector3D;

typedef Vertex* Vertices;

typedef struct {
	Vertex a;
	Vertex b;
} _edge;

typedef _edge* Edge;

typedef struct {
	Vector3D normal;
	bool invertedNormal;
	Vertices vertices;
} _face;
typedef _face* Face;

typedef struct {
	float x;
	float y;
	float z;
	int n;
} Inc;

Point3D createPoint3D(float x, float y, float z);
Vertex createVertex(float x, float y, float z);
Vector3D createVector3D(float x, float y, float z);

char* toStringPoint3D(Point3D p);

bool samePoint(Point3D a, Point3D b);

Edge createEdge(Vertex a, Vertex b);

Face createFace(Vertices vertices, bool invertedNormal);

Point3D translate(Point3D p, Vector3D inc, bool inverse);
Point3D scale(Point3D p, Vector3D factor, bool inverse);
Point3D rotateZ(Point3D p, Point3D origin, float theta, bool inverse);
Point3D rotateY(Point3D p, Point3D origin, float theta, bool inverse);
Point3D rotateX(Point3D p, Point3D origin, float theta, bool inverse);
Point3D rotate(Point3D p, Point3D origin, Vector3D rotation, float theta, bool inverse);

float angleBetweenVectors(Vector3D a, Vector3D b);

float innerProduct(Vector3D a, Vector3D b);
float moduleOfVector3D(Vector3D a);
Vector3D vectorProduct(Vector3D a, Vector3D b);
Vector3D normalizedVector3D(Vector3D a);
Vector3D calculateNormal(Vertices vertices, int vCount);
Vector3D calculateFaceNormal(Face face);
Inc getInc(Point3D start, Point3D end, bool ignoreZ);

Point3D copy(Point3D destination, Point3D origin);

#endif
