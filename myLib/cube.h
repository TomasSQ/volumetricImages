#ifndef CUBE_3D
#define CUBE_3D

#include "math3D.h"

typedef struct {
	Point3D origin;
	Vector3D size;
	Face* faces;
	Edge* edges;
	Vertices vertices;
	int nVertices;
	int nEdges;
	int nFaces;
} _cube;

typedef _cube* Cube;


Cube createCube(Point3D origin, Vector3D size);

void updateCube(Cube cube);

void destroyCube(Cube cube);

#endif
