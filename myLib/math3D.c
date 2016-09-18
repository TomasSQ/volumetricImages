#include <stdlib.h>

#include "math3D.h"

float innerProduct(Vector3D a, Vector3D b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Point3D translate(Point3D p, Vector3D inc, bool inverse) {
	Point3D q;

	q.x = p.x + (inverse ? -1.0 : 1.0) * inc.x;
	q.y = p.y + (inverse ? -1.0 : 1.0) * inc.y;
	q.z = p.z + (inverse ? -1.0 : 1.0) * inc.z;

	return q;
}

Point3D scale(Point3D p, Vector3D factor, bool inverse) {
	Point3D q;

	q.x = p.x * (inverse ? 1.0 / factor.x : factor.x);
	q.y = p.y * (inverse ? 1.0 / factor.y : factor.y);
	q.z = p.z * (inverse ? 1.0 / factor.z : factor.z);

	return q;
}

Point3D rotateZ(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;
	Point3D q = translate(p, origin, true);

	q.x = q.x * cos(theta) - q.y * sin(theta);
	q.y = q.x * sin(theta) + q.y * cos(theta);
	q.z = q.z;

	return translate(q, origin, false);
}

Point3D rotateY(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;
	Point3D q = translate(p, origin, true);

	q.x = q.x * cos(theta) + q.z * sin(theta);
	q.y = q.y;
	q.z = - q.x * sin(theta) + q.z * cos(theta);

	return translate(q, origin, false);
}

Point3D rotateX(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;
	Point3D q = translate(p, origin, true);

	q.x = q.x;
	q.y = q.y * cos(theta) - q.z * sin(theta);
	q.z = q.y * sin(theta) + q.z * cos(theta);

	return translate(q, origin, false);
}

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
