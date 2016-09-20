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

void drawSquare(Image2D image, Vertex a, Vertex b, Vertex c, Vertex d) {
	drawLine(image, a, b, 255);
	drawLine(image, b, c, 255);
	drawLine(image, c, d, 255);
	drawLine(image, d, a, 255);
}

void drawFace(Image2D image, Vertices vertices, int face, Vector3D planeRotation) {
	float center = 250;
	switch (face) {
		case 0:
			drawSquare(image, project(planeRotation, vertices[0], center), project(planeRotation, vertices[1], center), project(planeRotation, vertices[2], center), project(planeRotation, vertices[3], center));
			break;
		case 1:
			drawSquare(image, project(planeRotation, vertices[4], center), project(planeRotation, vertices[5], center), project(planeRotation, vertices[6], center), project(planeRotation, vertices[7], center));
			break;
		case 2:
			drawSquare(image, project(planeRotation, vertices[0], center), project(planeRotation, vertices[4], center), project(planeRotation, vertices[7], center), project(planeRotation, vertices[3], center));
			break;
		case 3:
			drawSquare(image, project(planeRotation, vertices[1], center), project(planeRotation, vertices[5], center), project(planeRotation, vertices[6], center), project(planeRotation, vertices[2], center));
			break;
		case 4:
			drawSquare(image, project(planeRotation, vertices[0], center), project(planeRotation, vertices[1], center), project(planeRotation, vertices[5], center), project(planeRotation, vertices[4], center));
			break;
		case 5:
			drawSquare(image, project(planeRotation, vertices[2], center), project(planeRotation, vertices[3], center), project(planeRotation, vertices[7], center), project(planeRotation, vertices[6], center));
			break;
	}
}

void testCube(char* name, Vector3D planeRotation) {
	int i;
	bool* visible = (bool*) malloc(sizeof(bool) * 6);
	Point3D origin = createPoint3D(250, 250, 250);
	Cube cube = createCube(origin, 100);

	for (i = 0; i < cube->nVertices; i++) {
		cube->vertices[i] = rotateX(cube->vertices[i], origin, planeRotation->x, false);
		cube->vertices[i] = rotateY(cube->vertices[i], origin, planeRotation->y, false);
	}
	recalculateNormals(cube);

	visibleFaces(planeRotation, cube->faces, visible, 6);

	Image2D image = newImage2D(500, 500);
	for (i = 0; i < 6; i++) {
		if (visible[i]) {
			drawFace(image, cube->vertices, i, planeRotation);
		}
	}

	saveImage(name, image->img, image->width, image->height);
	freeImage2D(image);
	free(visible);
}

void drawCube() {
	char nome[200];
	float i = 0;
	for (i = 0; i < 2 * PI; i += 0.1) {
		sprintf(nome, "out/cube_%f", i);
		testCube(nome, createVector3D(i, i, 0));
	}
}

int main(int argc, char* argv[]) {
	//drawStar();

	drawCube();

	return 0;
}
