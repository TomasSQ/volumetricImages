#ifndef MY_COMMON
#define MY_COMMON

#include "mo815-3dvis.h"

#define ABS(x) ((x >= 0)?x:-x)

typedef enum {
	false,
	true
} bool;

typedef struct {
	int** img;
	int** r;
	int** g;
	int** b;
	int width;
	int height;
} _image2D;
typedef _image2D* Image2D;

typedef struct {
	int min;
	int max;
} range;

Image2D newImage2D(int width, int height);
Image2D newColoredImage2D(int width, int height);
void freeImage2D(Image2D img);
void freeColoredImage2D(Image2D img);

range getRange(int **img, int imageWidth, int imageHeight);

#endif
