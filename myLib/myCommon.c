#include <stdlib.h>
#include <limits.h>

#include "myCommon.h"

#define EPS 0.0001

float SIN(float theta) {
	float r = sin(theta);
	if (-EPS < r && r < EPS) {
		return 0;
	}

	return r;
}
float COS(float theta) {
	float r = cos(theta);
	if (-EPS < r && r < EPS) {
		return 0;
	}

	return r;
}

range getRange(int **img, int imageWidth, int imageHeight) {
	int	row, col;

	range r;
	r.max = INT_MIN;
	r.min = INT_MAX;
	for (row = 0; row < imageHeight; row++) {
		for (col = 0; col < imageWidth; col++) {
			if (img[row][col] < 0 || img[row][col] > (1<<16)) {
				img[row][col] = 0;
			}
			if (img[row][col] > r.max) {
				r.max = img[row][col];
			}
			if (img[row][col] < r.min) {
				r.min = img[row][col];
			}
		}
	}

	return r;
}

Image2D newImage2D(int width, int height) {
	int row, col;
	Image2D img = (Image2D) malloc(sizeof(_image2D));
	img->img = (int**) malloc(sizeof(int*) * height);
	img->width = width;
	img->height = height;

	for (row = 0; row < img->height; row++) {
		img->img[row] = (int*) malloc(sizeof(int) * img->width);
		for (col = 0; col < img->width; col++) {
			img->img[row][col] = 0;
		}
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

int initMemDinMonitoring() {
	free(malloc(1));

	return mallinfo().uordblks;
}

int checkMemDinMonitoring(int initialMemDin) {
	int finalMemDin = mallinfo().uordblks;
	if (initialMemDin != finalMemDin) {
		printf("\n\nDinamic memory was not completely deallocated %d (%d, %d)\n", finalMemDin - initialMemDin, initialMemDin, finalMemDin);
	}

	return finalMemDin - initialMemDin;
}
