#include "../myLib/myCommon.h"

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

Axis getAxisForModeAndCut(char mode, char cut, int start);
Image2D getCutFromImage(Image *img, Axis axis, char* outputFileName);
