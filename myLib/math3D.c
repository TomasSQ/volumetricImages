#include <stdio.h>
#include <stdlib.h>

#include "math3D.h"

Point3D copy(Point3D p, Point3D q) {
	p->x = q->x;
	p->y = q->y;
	p->z = q->z;

	return p;
}

float angleBetweenVectors(Vector3D a, Vector3D b) {
	float modules = moduleOfVector3D(a) * moduleOfVector3D(b);

	return modules > 0.00001 || modules < -0.0001 ? acos(innerProduct(a, b) / modules) : 0;
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
	Vector3D v0 = copy(createVector3D(0, 0, 0), vertices[0]);
	Vector3D v1 = copy(createVector3D(0, 0, 0), vertices[1]);
	Vector3D v2 = copy(createVector3D(0, 0, 0), vertices[2]);

	v1 = translate(v1, v0, true);
	v2 = translate(v2, v0, true);
	Vector3D v = vectorProduct(v1, v2);

	free(v0);
	free(v1);
	free(v2);

	return normalizedVector3D(v);
}

Vector3D calculateFaceNormal(Face face) {
	Vector3D reverse = createVector3D(-1, -1, -1);

	if (face->normal != NULL) {
		free(face->normal);
	}

	face->normal = calculateNormal(face->vertices, 4);
	if (face->invertedNormal) {
		scale(face->normal, reverse, false);
	}

	free(reverse);

	return face->normal;
}

float innerProduct(Vector3D a, Vector3D b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

float moduleOfVector3D(Vector3D a) {
	return sqrt(innerProduct(a, a));
}

Vector3D normalizedVector3D(Vector3D a) {
	float module = moduleOfVector3D(a);
	if (-0.00001 < module && module < 0.00001) {
		a->x = 0;
		a->y = 0;
		a->z = 0;

		return a;
	}

	a->x /= module,
	a->y /= module,
	a->z /= module;

	return a;
}

Point3D translate(Point3D p, Vector3D inc, bool inverse) {
	if (inc == NULL) {
		return p;
	}
	p->x += (inverse ? -1.0 : 1.0) * inc->x,
	p->y += (inverse ? -1.0 : 1.0) * inc->y,
	p->z += (inverse ? -1.0 : 1.0) * inc->z;

	return p;
}

Point3D scale(Point3D p, Vector3D factor, bool inverse) {
	p->x *= (inverse ? 1.0 / factor->x : factor->x),
	p->y *= (inverse ? 1.0 / factor->y : factor->y),
	p->z *= (inverse ? 1.0 / factor->z : factor->z);

	return p;
}

Point3D rotateZ(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;

	translate(p, origin, true);

	Point3D q = createPoint3D(
		p->x * COS(theta) + p->y * SIN(theta),
		-p->x * SIN(theta) + p->y * COS(theta),
		p->z);

	translate(q, origin, false);

	copy(p, q);
	free(q);
	return p;
}

Point3D rotateY(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;

	translate(p, origin, true);

	Point3D q = createPoint3D(
		p->x * COS(theta) + p->z * SIN(theta),
		p->y,
		-p->x * SIN(theta) + p->z * COS(theta));

	translate(q, origin, false);

	copy(p, q);
	free(q);
	return p;
}

Point3D rotateX(Point3D p, Point3D origin, float theta, bool inverse) {
	theta = inverse ? -theta : theta;

	translate(p, origin, true);

	Point3D q = createPoint3D(
		p->x,
		p->y * COS(theta) - p->z * SIN(theta),
		p->y * SIN(theta) + p->z * COS(theta));

	translate(q, origin, false);

	copy(p, q);
	free(q);
	return p;
}

Point3D rotate(Point3D p, Point3D origin, Vector3D rotation, float theta, bool inverse) {
	float thetaX = atan(rotation->y / rotation->z);
	float thetaY = atan(rotation->x / cos(thetaX));
	float thetaZ = theta;

	rotateX(p, origin, thetaX, false);
	rotateY(p, origin, thetaY, false);
	rotateZ(p, origin, thetaZ, false);
	rotateY(p, origin, thetaY, true);
	rotateX(p, origin, thetaX, true);

	return p;
}

bool samePoint(Point3D a, Point3D b) {
	return a->x == b->x && a->y == b->y && a->z == b->z;
}

Inc getInc(Point3D start, Point3D end, bool ignoreZ) {
	float deltaX, deltaY, deltaZ;

	Inc inc;
	inc.n = 0;
	inc.x = 0;
	inc.y = 0;
	inc.z = 0;

	if (samePoint(start, end)) {
		return inc;
	}

	deltaX = end->x - start->x;
	deltaY = end->y - start->y;
	deltaZ = end->z - start->z;

	if (ABS(deltaX) >= ABS(deltaY) && (ignoreZ || ABS(deltaX) >= ABS(deltaZ))) {
		inc.n = ABS(deltaX) + 1;
		inc.x = SIGN(deltaX);
		inc.y = inc.x * deltaY / deltaX;
		inc.z = inc.x * deltaZ / deltaX;
	} else if (ABS(deltaY) >= ABS(deltaX) && (ignoreZ || ABS(deltaY) >= ABS(deltaZ))) {
		inc.n = ABS(deltaY) + 1;
		inc.y = SIGN(deltaY);
		inc.x = inc.y * deltaX / deltaY;
		inc.z = inc.y * deltaZ / deltaY;
	} else if (!ignoreZ) {
		inc.n = ABS(deltaZ) + 1;
		inc.z = SIGN(deltaZ);
		inc.x = inc.z * deltaX / deltaZ;
		inc.y = inc.z * deltaY / deltaZ;
	}

	return inc;
}

Point3D createPoint3D(float x, float y, float z) {
	Point3D p = (Point3D) malloc(sizeof(_point3D));

	p->x = x;
	p->y = y;
	p->z = z;

	return p;
}

Vertex createVertex(float x, float y, float z) {
	return createPoint3D(x, y, z);
}

Vector3D createVector3D(float x, float y, float z) {
	return createPoint3D(x, y, z);
}

Edge createEdge(Vertex a, Vertex b) {
	Edge e = (Edge) malloc(sizeof(_edge));

	e->a = a;
	e->b = b;

	return e;
}

Face createFace(Vertices vertices, bool invertedNormal) {
	int i;

	Face f = (Face) malloc(sizeof(_face));

	f->normal = NULL;
	f->invertedNormal = invertedNormal;
	f->vertices = (Vertices) malloc(sizeof(Vertex) * 4);

	for (i = 0; i < 4; i++) {
		f->vertices[i] = vertices[i];
	}

	f->normal = calculateFaceNormal(f);

	return f;
}

char* toStringPoint3D(Point3D p) {
	char* str = (char*) malloc(sizeof(char) * (2 + 2 + 2 + 3 * (1 + 4 + 1 + 4)));
	sprintf(str, "(%.4f, %.4f, %.4f)", p->x, p->y, p->z);

	return str;
}
