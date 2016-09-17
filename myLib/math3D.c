#include <stdlib.h>

#include "math3D.h"

bool samePoint(Point3D a, Point3D b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

Point3D createPoint3D(float x, float y, float z) {
	Point3D p;

	p.x = x;
	p.y = y;
	p.z = z;

	return p;
}

Vertex createVertex(float x, float y, float z) {
	Vertex v;

	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

Vector3D createVector3D(float x, float y, float z) {
	Vector3D v;

	v.x = x;
	v.y = y;
	v.z = z;

	return v;
}

Edge createEdge(Vertex a, Vertex b) {
	Edge e;

	e.a = a;
	e.b = b;

	return e;
}

Face createFace(Vector3D normal, Vertices vertices) {
	int i;
	Face f = (Face) malloc(sizeof(_face));
	f->normal = normal; //FIXME calculate normal and receive "flip" as parameter
	f->vertices = (Vertices) malloc(sizeof(Vertex) * 4);

	for (i = 0; i < 4; i++) {
		f->vertices[i] = vertices[i];
	}

	return f;
}
