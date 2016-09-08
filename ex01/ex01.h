#include "mo815-3dvis.h"

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


void drawCut(Image *img, Axis axis, char* outputFileName);
