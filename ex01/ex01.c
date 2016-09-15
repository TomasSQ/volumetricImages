#include "ex01.h"

#define RADIOLOGIST 'R'
#define NEUROSURGEON 'N'
#define AXIAL 'A'
#define SAGITTAL 'S'
#define CORONAL 'C'

Axis getAxis(AxisType fixedAxis, AxisType firstAxis, bool invertedFixedAxis, bool invertedFirstAxis, bool invertedSecondAxis, int start) {
	Axis axis;

	axis.start = start;
	AxisOrientation fixed;
	fixed.axis = fixedAxis;
	fixed.inverted = invertedFixedAxis;
	axis.fixedAxis = fixed;

	AxisOrientation first;
	first.axis = firstAxis;
	first.inverted = invertedFirstAxis;
	axis.firstAxis = first;

	AxisOrientation second;
	if (fixedAxis == X) {
		second.axis = firstAxis == Y ? Z : Y;
	} else if (fixedAxis == Y) {
		second.axis = firstAxis == X ? Z : X;
	} else {
		second.axis = firstAxis == X ? Y : X;
	}
	second.inverted = invertedSecondAxis;
	axis.secondAxis = second;

	return axis;
}

Axis getAxisForModeAndCut(char mode, char cut, int start) {
	if (cut == 'C') {
		return getAxis(Y, X, false, false, true, start);
	}

	if (mode == 'R') {
		if (cut == 'A') {
			return getAxis(Z, X, false, false, false, start);
		}
		return getAxis(X, Y, false, true, true, start);
	}

	if (cut == 'A') {
		return getAxis(Z, X, true, true, false, start);
	}
	return getAxis(X, Y, true, false, true, start);
}

int getVoxelX(Image *img, Axis axis, int row, int col) {
	if (axis.fixedAxis.axis == X) {
		return axis.fixedAxis.inverted ? img->xsize - axis.start - 1 : axis.start;
	}

	if (axis.firstAxis.axis == X) {
		return axis.firstAxis.inverted ? img->xsize - col - 1 : col;
	}

	return axis.secondAxis.inverted ? img->xsize - row - 1 : row;
}

int getVoxelY(Image *img, Axis axis, int row, int col) {
	if (axis.fixedAxis.axis == Y) {
		return axis.fixedAxis.inverted ? img->ysize - axis.start - 1 : axis.start;
	}

	if (axis.firstAxis.axis == Y) {
		return axis.firstAxis.inverted ? img->ysize - col - 1 : col;
	}

	return axis.secondAxis.inverted ? img->ysize - row - 1 : row;
}

int getVoxelZ(Image *img, Axis axis, int row, int col) {
	if (axis.fixedAxis.axis == Z) {
		return axis.fixedAxis.inverted ? img->zsize - axis.start - 1 : axis.start;
	}

	if (axis.firstAxis.axis == Z) {
		return axis.firstAxis.inverted ? img->zsize - col - 1 : col;
	}

	return axis.secondAxis.inverted ? img->zsize - row - 1 : row;
}

Voxel getVoxel(Image *img, Axis axis, int row, int col) {
	Voxel v;

	v.x = getVoxelX(img, axis, row, col);
	v.y = getVoxelY(img, axis, row, col);
	v.z = getVoxelZ(img, axis, row, col);

	return v;
}

int getColumns(Image *img, Axis axis) {
	switch (axis.firstAxis.axis) {
		case X:
			return img->xsize;
		case Y:
			return img->ysize;
		default:
			return img->zsize;
	}
}

int getLines(Image *img, Axis axis) {
	switch (axis.secondAxis.axis) {
		case X:
			return img->xsize;
		case Y:
			return img->ysize;
		default:
			return img->zsize;
	}
}

Image2D getCutFromImage(Image *img, Axis axis, char* outputFileName) {
	int columns = getColumns(img, axis);
	int lines = getLines(img, axis);
	int row, col;
	Voxel v;
	Image2D image2D = newImage2D(columns, lines);

	for (row = 0; row < image2D->height; row++) {
		for (col = 0; col < image2D->width; col++) {
			v = getVoxel(img, axis, row, col);
			image2D->img[row][col] = img->val[v.z * img->ysize * img->xsize + v.y * img->xsize + v.x];
		}
	}

	return image2D;
}

Image2D newImage2D(int width, int height) {
	int row;
	Image2D img = (Image2D) malloc(sizeof(_image2D));
	img->img = (int**) malloc(sizeof(int*) * height);
	img->width = width;
	img->height = height;

	for (row = 0; row < img->height; row++) {
		img->img[row] = (int*) malloc(sizeof(int) * img->width);
	}

	return img;
}

void freeImage2D(Image2D img) {
	int row;

	for (row = 0; row < img->height; row++) {
		free(img->img[row]);
	}

	free(img->img);

	free(img);
}

Image2D newColoredImage2D(int width, int height) {
	int row;
	Image2D img = newImage2D(width, height);
	img->r = (int**) malloc(sizeof(int*) * height);
	img->g = (int**) malloc(sizeof(int*) * height);
	img->b = (int**) malloc(sizeof(int*) * height);

	for (row = 0; row < img->height; row++) {
		img->r[row] = (int*) malloc(sizeof(int) * img->width);
		img->g[row] = (int*) malloc(sizeof(int) * img->width);
		img->b[row] = (int*) malloc(sizeof(int) * img->width);
	}

	return img;
}

void freeColoredImage2D(Image2D img) {
	int row;

	for (row = 0; row < img->height; row++) {
		free(img->img[row]);
		free(img->r[row]);
		free(img->g[row]);
		free(img->b[row]);
	}

	free(img->img);
	free(img->r);
	free(img->g);
	free(img->b);

	free(img);
}
