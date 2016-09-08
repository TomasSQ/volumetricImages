#ifndef _BIT_MAP_HELPER_
#define _BIT_MAP_HELPER_

#include <stdio.h>
#include <stdlib.h>

void saveImage(char* imageName, float **img, int imageWidth, int imageHeight);
void saveColoredImage(char* imageName, float **r, float **g, float **b, int imageWidth, int imageHeight);

#endif
