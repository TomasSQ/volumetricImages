#include <stdlib.h>
#include <stdio.h>

#include "../myLib/bitmap.h"

#include "ex03.h"

void drawStar() {
	int i, max, step = 10;

	Image2D image = newImage2D(500, 500);
	max = image->width / 2 / step;

	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step + i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step + i * 10, 0), 255);

	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, 0, 0), createPoint3D(0, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, 0, 0), createPoint3D(image->width, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(i * step, image->height, 0), createPoint3D(0, max * step + i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine(image, createPoint3D(image->width - i * step, image->height, 0), createPoint3D(image->width, max * step + i * 10, 0), 255);

	saveImage("out/star", image->img, image->width, image->height);
	freeImage2D(image);
}

void drawSquare(Image2D image, Vertices vertices) {
	int i;

	for (i = 0; i < 3; i++) {
		drawLine(image, vertices[i], vertices[i + 1], 255);
	}

	drawLine(image, vertices[3], vertices[0], 255);
}

void testCube(char* name, Vector3D planeRotation) {
	int i;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);
	Point3D origin = createPoint3D(250, 250, 250);
	Cube cube = createCube(origin, 100);

	for (i = 0; i < cube->nVertices; i++) {
		rotateX(cube->vertices[i], origin, planeRotation->x, false);
		rotateY(cube->vertices[i], origin, planeRotation->y, false);
		rotateZ(cube->vertices[i], origin, planeRotation->y, false);
	}

	updateCube(cube);

	visibleFaces(planeRotation, cube->faces, visible, 6);

	Image2D image = newImage2D(500, 500);
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			drawSquare(image, cube->faces[i]->vertices);
		}
	}

	saveImage(name, image->img, image->width, image->height);
	freeImage2D(image);
	free(visible);
}

void drawCube() {
	char nome[200];
	float i = PI / 4;
	for (i = 0; i < 2 * PI; i += 0.1) {
		sprintf(nome, "out/cube_%f", i);
		testCube(nome, createVector3D(i, i, 0));
	}
}

void testMath() {
	Point3D p, p2, p3, p4, origin;
	Vector3D v1, v2, normal;
	Vertices vertices;
	Face face;

	p = createPoint3D(1, 1, 1);
	origin = createVector3D(250, 250, 250);
	printf("init   (1.0000, 1.0000, 1.0000) %s\n", toStringPoint3D(p));
	printf("scale  (15.0000, 25.0000, 10.0000) %s\n", toStringPoint3D(scale(p, createVector3D(15, 25, 10), false)));
	printf("-scale (1.0000, 1.0000, 1.0000) %s\n", toStringPoint3D(scale(p, createVector3D(15, 25, 10), true)));
	printf("scale  (100.0000, 100.0000, 100.0000) %s\n", toStringPoint3D(scale(p, createVector3D(100, 100, 100), false)));
	printf("relO   (350.0000, 350.0000, 350.0000) %s\n", toStringPoint3D(translate(p, origin, false)));
	printf("rotX   (350.0000, 250.0000, 391.4214) %s\n", toStringPoint3D(rotateX(p, origin, PI / 4, false)));
	printf("rotX   (420.7107, 250.0000, 279.2893) %s\n", toStringPoint3D(rotateY(p, origin, PI / 4, false)));
	printf("rotX   (420.7107, 250.0000, 279.2893) %s\n", toStringPoint3D(rotateY(p, origin, PI / 4, false)));

	p = createPoint3D(-1, 1, 1);
	p2 = createPoint3D(1, 1, 1);
	p3 = createPoint3D(1, -1, 1);
	p4 = createPoint3D(-1, -1, 1);
	v1 = translate(p2, p, true);
	v2 = translate(p3, p, true);
	normal = vectorProduct(v1, v2);
	printf("V1     (2.000, 0.000, 0.000) %s\n", toStringPoint3D(v1));
	printf("V2     (2.000, -2.000, 0.000) %s\n", toStringPoint3D(v2));
	printf("normal (0.000, 0.000, -4.000) %s\n", toStringPoint3D(normal));
	printf("unit   (0.000, 0.000, -1.000) %s\n", toStringPoint3D(normalizedVector3D(normal)));

	p = createPoint3D(-2, 2, 2);
	p2 = createPoint3D(2, 2, 2);
	p3 = createPoint3D(2, -2, 2);
	p4 = createPoint3D(-2, -2, 2);
	vertices = (Vertices) malloc(sizeof(Vertex) * 4);
	vertices[0] = p;
	vertices[1] = p2;
	vertices[2] = p3;
	vertices[3] = p4;
	face = createFace(vertices, false);
	normal = calculateNormal(face->vertices, 4, createVector3D(1, 1, 1), createVector3D(0, 0, 0));
	printf("normal (0.000, 0.000, -1.000) %s\n", toStringPoint3D(normal));
}

int main(int argc, char* argv[]) {
	testMath();
	//drawStar();

	drawCube();

	return 0;
}
