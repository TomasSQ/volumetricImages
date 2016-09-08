#include "mo815-3dvis.h"

#define RADIOLOGIST 'R'
#define NEUROSURGEON 'N'
#define AXIAL 'A'
#define SAGITTAL 'S'
#define CORONAL 'C'

typedef enum {
	X,
	Y,
	Z
} AxisType;

typedef enum {
	false,
	true
} bool;

typedef struct {
	AxisType axis;
	bool inverted;
} AxisOrientation;

typedef struct {
	AxisOrientation fixedAxis;
	AxisOrientation firstAxis;
	AxisOrientation secondAxis;
	int start;
} Axis;

typedef struct {
	float** img;
	float** r;
	float** g;
	float** b;
	int width;
	int height;
} _image2D;
typedef _image2D* Image2D;

Axis getAxisForModeAndCut(char mode, char cut, int start);
Image2D getCutFromImage(Image *img, Axis axis, char* outputFileName);

Image2D newImage2D(int width, int height);
Image2D newColoredImage2D(int width, int height);
void freeImage2D(Image2D img);
void freeColoredImage2D(Image2D img);
