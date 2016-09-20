#include <stdlib.h>

#include "math3D.h"

Point3D project(Vector3D planeRotation, Point3D p, int D) {
	Point3D p2 = rotateX(p, createPoint3D(D, D, D), planeRotation->x, false);
	p2 = rotateY(p2, createPoint3D(D, D, D), planeRotation->y, false);
	p2->x += 0;
	p2->y += 0;
	p2->z = 0;

	return p2;
}

Vector3D vectorProduct(Vector3D a, Vector3D b) {
	return createVector3D(
		a->y * b->z - a->z * b->y,
		a->z * b->x - a->x * b->z,
		a->x * b->y - a->y * b->x);
}

Vector3D calculateNormal(Vertices vertices, int vCount) {
	if (vCount <= 2) {
		return createVector3D(0, 0, 0);
	}
	Vector3D a = translate(vertices[0], vertices[1], false);
	Vector3D b = translate(vertices[vCount - 2], vertices[vCount - 1], false);

	return normalizedVector3D(vectorProduct(a, b));
}

float innerProduct(Vector3D a, Vector3D b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

float moduleOfVector3D(Vector3D a) {
	return sqrt(innerProduct(a, a));
}

Vector3D normalizedVector3D(Vector3D a) {
	float module = moduleOfVector3D(a);
	return createVector3D(
		a->x / module,
		a->y / module,
		a->z / module);
}

Point3D translate(Point3D p, Vector3D inc, bool inverse) {
	return createPoint3D(
		p->x + (inverse ? -1.0 : 1.0) * inc->x,
		p->y + (inverse ? -1.0 : 1.0) * inc->y,
		p->z + (inverse ? -1.0 : 1.0) * inc->z);
}

Point3D scale(Point3D p, Vector3D factor, bool inverse) {
	return createPoint3D(
		p->x * (inverse ? 1.0 / factor->x : factor->x),
		p->y * (inverse ? 1.0 / factor->y : factor->y),
		p->z * (inverse ? 1.0 / factor->z : factor->z));
}

Point3D rotateZ(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;
	Point3D q = translate(p, origin, true);

	Point3D q2 = createPoint3D(
		q->x * COS(theta) + q->y * SIN(theta),
		-q->x * SIN(theta) + q->y * COS(theta),
		q->z);

	free(q);

	q = translate(q2, origin, false);
	free(q2);
	return q;
}

Point3D rotateY(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;
	Point3D q = translate(p, origin, true);

	Point3D q2 = createPoint3D(
		q->x * COS(theta) + q->z * SIN(theta),
		q->y,
		-q->x * SIN(theta) + q->z * COS(theta));

	free(q);

	q = translate(q2, origin, false);
	free(q2);
	return q;
}

Point3D rotateX(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;
	Point3D q = translate(p, origin, true);

	Point3D q2 = createPoint3D(
		q->x,
		q->y * COS(theta) - q->z * SIN(theta),
		q->y * SIN(theta) + q->z * COS(theta));

	free(q);

	q = translate(q2, origin, false);
	free(q2);
	return q;
}

bool samePoint(Point3D a, Point3D b) {
	return a->x == b->x && a->y == b->y && a->z == b->z;
}

Point3D createPoint3D(float x, float y, float z) {
	Point3D p = (Point3D) malloc(sizeof(_point3D));

	p->x = x;
	p->y = y;
	p->z = z;

	return p;
}

Vertex createVertex(float x, float y, float z) {
	Vertex v = (Vertex) malloc(sizeof(_point3D));

	v->x = x;
	v->y = y;
	v->z = z;

	return v;
}

Vector3D createVector3D(float x, float y, float z) {
	Vector3D v = (Vector3D) malloc(sizeof(_point3D));

	v->x = x;
	v->y = y;
	v->z = z;

	return v;
}

Edge createEdge(Vertex a, Vertex b) {
	Edge e = (Edge) malloc(sizeof(_edge));

	e->a = a;
	e->b = b;

	return e;
}

Face createFace(Vertices vertices) {
	int i;
	Face f = (Face) malloc(sizeof(_face));
	f->normal = calculateNormal(vertices, 4);
	f->vertices = (Vertices) malloc(sizeof(Vertex) * 4);

	for (i = 0; i < 4; i++) {
		f->vertices[i] = vertices[i];
	}

	return f;
}
