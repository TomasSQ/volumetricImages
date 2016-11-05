#include <stdlib.h>
#include <stdio.h>

#include "../myLib/bitmap.h"

#include "ex05.h"

void testSlice(Image* image) {
	char name[200];
	float i = 0;//PI / 4;
	float inc = 0.05;
	float xinc = 1 * (1 * inc);
	float yinc = 1 * (3 * inc);
	float zinc = 1 * (1 * inc);
	Point3D origin = createPoint3D(image->xsize / 2, image->ysize / 2, image->zsize / 2);
	Vector3D normal = createVector3D(0, 0, -1);
	printf("%d %d %d\n", image->xsize, image->ysize, image->zsize);

	for (i = 0; i < 2 * PI; i += inc) {
		sprintf(name, "out/slice_%f", i);

		normal = normalizedVector3D(rotateZ(rotateY(rotateX(normal, NULL, xinc, false), NULL, yinc, false), NULL, zinc, false));
		Image2D slice = getSlice(origin, normal, image);
		saveImage(name, slice->img, slice->width, slice->height);
		freeImage2D(slice);

		printf("%s done\n", name);
	}
	free(normal);

	system("convert -delay 0 -loop 0 out/slice_*.pgm ~/Desktop/slice.gif");
}

void testSlice2(Image* image) {
	char name[200];
	int i = 0;
	Point3D maxP = createPoint3D(image->xsize / 2, image->ysize, image->zsize / 2);
	Point3D minP = createPoint3D(image->xsize / 2, 0, image->zsize / 2);
	Inc inc = getInc(minP, maxP, false);

	Point3D origin = copy(createPoint3D(0, 0, 0), minP);
	Vector3D normal = createVector3D(0, -1, 0);
	//normal = normalizedVector3D(rotateY(rotateX(normal, NULL, PI / 4, false), NULL, PI / 4, false));

	for (i = 0; i < inc.n; i++) {
		sprintf(name, "out/slice_line_%03d", i);

		Image2D slice = getSlice(origin, normal, image);
		saveImage(name, slice->img, slice->width, slice->height);
		freeImage2D(slice);

		origin->x += inc.x;
		origin->y += inc.y;
		origin->z += inc.z;

		printf("%s done\n", name);
	}
	free(origin);
	free(normal);
	free(maxP);
	free(minP);

	system("convert -delay 0 -loop 0 out/slice_line_*.pgm ~/Desktop/slice_line.gif");
}

int main(int argc, char* argv[]) {
	int initialMemDin = initMemDinMonitoring();
	testSlice2(ReadImage(argv[2]));

	checkMemDinMonitoring(initialMemDin);

	return 0;
}
