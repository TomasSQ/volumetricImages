#include <stdlib.h>
#include <stdio.h>

#include "../myLib/bitmap.h"

#include "ex03.h"

void testStar() {
	int i, max, step = 10;

	Image2D image = newImage2D(500, 500);
	max = image->width / 2 / step;

	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(image->width - i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step + i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(image->width - i * step, image->height / 2, 0), createPoint3D(image->width / 2, max * step + i * 10, 0), 255);

	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(i * step, 0, 0), createPoint3D(0, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(image->width - i * step, 0, 0), createPoint3D(image->width, max * step - i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(i * step, image->height, 0), createPoint3D(0, max * step + i * 10, 0), 255);
	for (i = 0; i <= max; i++)
		drawLine2D(image, createPoint3D(image->width - i * step, image->height, 0), createPoint3D(image->width, max * step + i * 10, 0), 255);

	saveImage("out/star", image->img, image->width, image->height);
	freeImage2D(image);
}

void testCube(Image* image) {
	char name[200];
	float i = PI / 4;
	float inc = 0.05;
	int sign;
	Point3D origin = createPoint3D(image->xsize / 2, image->ysize / 2, image->zsize / 2);
	Vector3D planeRotation = NULL;
	Vector3D scaleFactor = NULL;
	Cube cube = createCube(origin, createVector3D(50, 50, 50));
	printf("%d %d %d\n", image->xsize, image->ysize, image->zsize);

	for (i = 0; i < 2 * PI; i += inc) {
		sign = ((int) i) % 2 == 0 ? 1 : -1;
		sprintf(name, "out/cube_%f", i);

		planeRotation = createVector3D(-inc, 3 * inc, -inc);
		scaleFactor = createVector3D(1 + sign * inc, 1 + sign * inc, 1 + sign * inc);

		scaleCube(cube, scaleFactor);
		rotateCube(cube, planeRotation);

		Image2D cubeImage = render(planeRotation , cube, image);
		saveImage(name, cubeImage->img, cubeImage->width, cubeImage->height);
		freeImage2D(cubeImage);
	}
}

void testSlice(Image* image) {
	char name[200];
	float i = PI / 4;
	float inc = 0.05;
	Point3D origin = createPoint3D(image->xsize / 2, image->ysize / 2, image->zsize / 2);
	printf("%d %d %d\n", image->xsize, image->ysize, image->zsize);

	for (i = 0; i < 2 * PI; i += inc) {
		sprintf(name, "out/slice_%f", i);

		Image2D slice = getSlice(origin, createVector3D(i, i, 0), image);
		saveImage(name, slice->img, slice->width, slice->height);
		freeImage2D(slice);
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
	normal = calculateNormal(face->vertices, 4);
	printf("normal (0.000, 0.000, -1.000) %s\n", toStringPoint3D(normal));

	printf("p2     (2.000, 2.000, 2.000) %s\n", toStringPoint3D(p2));
	printf("f->p2  (2.000, 2.000, 2.000) %s\n", toStringPoint3D(face->vertices[1]));
	scale(p2, createVector3D(1.5, 2, 2.5), false);
	printf("p2     (3.000, 4.000, 5.000) %s\n", toStringPoint3D(p2));
	printf("f->p2  (3.000, 4.000, 5.000) %s\n", toStringPoint3D(face->vertices[1]));
}

int main(int argc, char* argv[]) {
	testMath();
	testStar();
	if (argv[1][0] == 'c') {
		testCube(ReadImage(argv[2]));
		system("convert -delay 0 -loop 0 out/cube_*.pgm ~/Desktop/cube.gif");
	} else if (argv[1][0] == 's') {
		testSlice(ReadImage(argv[2]));
		system("convert -delay 0 -loop 0 out/slice_*.pgm ~/Desktop/slice.gif");
	}

	return 0;
}
