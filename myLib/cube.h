#ifndef CUBE_3D
#define CUBE_3D

#include "math3D.h"

typedef struct {
	Face* faces;
	Edge* edges;
	Vertices vertices;
	int nVertices;
	int nEdges;
	int nFaces;
} _cube;

typedef _cube* Cube;


Cube createCube(Point3D origin, float size);
void destroCube(Cube cube);

#endif
