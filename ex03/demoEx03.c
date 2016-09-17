#include <stdlib.h>
#include <stdio.h>

#include "../ex01/bitmap.h"

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
}

int main(int argc, char* argv[]) {
	Vector3D* normals = (Vector3D*) malloc(sizeof(Vector3D) * 6);
	normals[0] = createVector3D( 0,  0, -1);
	normals[1] = createVector3D( 0,  0,  1);
	normals[2] = createVector3D(-1,  0,  0);
	normals[3] = createVector3D( 1,  0,  0);
	normals[4] = createVector3D( 0, -1,  0);
	normals[5] = createVector3D( 0,  1,  0);

	printf("samePoint: 1 == %d\n", samePoint(normals[0], normals[0]));
	printf("samePoint: 0 == %d\n", samePoint(normals[1], normals[0]));

	Vertices vertices = (Vertices) malloc(sizeof(Vertex) * 8);
	vertices[0] = createVertex( 0,  0,  0);
	vertices[1] = createVertex( 1,  0,  0);
	vertices[2] = createVertex( 1,  1,  0);
	vertices[3] = createVertex( 0,  1,  0);
	vertices[4] = createVertex( 0,  0,  1);
	vertices[5] = createVertex( 1,  0,  1);
	vertices[6] = createVertex( 1,  1,  1);
	vertices[7] = createVertex( 0,  1,  1);

	Face* cube = createCube(normals, vertices);

	drawStar();

	return 0;
}
