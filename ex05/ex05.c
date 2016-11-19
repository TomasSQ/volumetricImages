#include <stdlib.h>

#include "ex05.h"

#define EXTRA_STEP_FACTOR 2

Image2D getSlice(Point3D origin, Vector3D normal, Image* image) {
	int imageSize = MAX(MAX(image->xsize, image->ysize), image->zsize);
	Image2D slice = newImage2D(imageSize, imageSize);
	Vector3D cubeScale = createVector3D(image->xsize * 2, image->ysize * 2, 10);
	Vector3D rotation = NULL;
	Point3D dp = translate(createPoint3D(image->xsize / 2, image->ysize / 2, image->zsize / 2), origin, true);

	Cube cube = createCube(origin, cubeScale);
	rotation = alignCube(cube, normal);

	drawSquare(slice, cube->faces[0]->vertices, image, rotation, origin, dp);

	destroyCube(cube);
	free(rotation);
	free(cubeScale);

	return slice;
}
