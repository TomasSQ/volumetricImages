#include <stdlib.h>

#include "ex03.h"

void visibleFaces(Vector3D planeRotation, Vector3D* normals, bool* visibleFaces, int nFaces) {
	int i = 0;

	Point3D origin = createPoint3D(0, 0, 0);
	Vector3D normal = createVector3D(0, 0, -1);

	normal = rotateX(normal, origin, planeRotation.x, false);
	normal = rotateY(normal, origin, planeRotation.y, false);

	for (i = 0; i < nFaces; i++) {
		visibleFaces[i] = innerProduct(normal, normals[i]) > 0;
	}
}

void draw(Image2D image, Point3D p, float dx, float dy, int n, int intensity) {
	int i;
	for (i = 0; i < n; i++) {
		if (0 <= p.y && p.y < image->height && 0 <= p.x && p.x < image->width) {
			image->img[(int) p.y][(int) p.x] = intensity;
		}
		p.x += dx;
		p.y += dy;
	}
}

void drawLine(Image2D image, Point3D start, Point3D end, int intensity) {
	int n;
	float deltaX, deltaY, dx, dy;

	if (samePoint(start, end)) {
		n = 0;
		dx = 0;
		dy = 0;
	} else {
		deltaX = end.x - start.x;
		deltaY = end.y - start.y;

		if (ABS(deltaX) >= ABS(deltaY)) {
			n = ABS(deltaX) + 1;
			dx = SIGN(deltaX);
			dy = dx * deltaY / deltaX;
		} else {
			n = ABS(deltaY) + 1;
			dy = SIGN(deltaY);
			dx = dy * deltaX / deltaY;
		}
	}

	draw(image, start, dx, dy, n, intensity);
}

Face* createCube(Vector3D* normals, Vertices vertices) {
	int i = 0;
	Face* cube = (Face*) malloc(sizeof(Face) * 6);

	Vertices faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);
	faceVertices[0] = vertices[0];
	faceVertices[1] = vertices[1];
	faceVertices[2] = vertices[2];
	faceVertices[3] = vertices[3];
	cube[i] = createFace(normals[i], faceVertices);
	i++;

	faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);
	faceVertices[0] = vertices[4];
	faceVertices[1] = vertices[5];
	faceVertices[2] = vertices[6];
	faceVertices[3] = vertices[7];
	cube[i] = createFace(normals[i], faceVertices);
	i++;

	faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);
	faceVertices[0] = vertices[0];
	faceVertices[1] = vertices[4];
	faceVertices[2] = vertices[7];
	faceVertices[3] = vertices[3];
	cube[i] = createFace(normals[i], faceVertices);
	i++;

	faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);
	faceVertices[0] = vertices[1];
	faceVertices[1] = vertices[5];
	faceVertices[2] = vertices[6];
	faceVertices[3] = vertices[2];
	cube[i] = createFace(normals[i], faceVertices);
	i++;

	faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);
	faceVertices[0] = vertices[0];
	faceVertices[1] = vertices[4];
	faceVertices[2] = vertices[5];
	faceVertices[3] = vertices[2];
	cube[i] = createFace(normals[i], faceVertices);
	i++;

	faceVertices = (Vertices) malloc(sizeof(Vertex) * 4);
	faceVertices[0] = vertices[3];
	faceVertices[1] = vertices[2];
	faceVertices[2] = vertices[6];
	faceVertices[3] = vertices[7];
	cube[i] = createFace(normals[i], faceVertices);
	i++;

	return cube;
}
