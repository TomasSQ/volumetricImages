#ifndef EX_03
#define EX_03

#include "../myLib/math3D.h"
#include "../myLib/cube.h"
#include "../myLib/myCommon.h"

void drawLine(Image2D image, Point3D start, Point3D end, int intensity);

void visibleFaces(Vector3D planeRotation, Vector3D* normals, bool* visibleFaces, int nFaces);

#endif
