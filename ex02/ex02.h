#include "../ex01/ex01.h"

/*
	level e window sao porgentagens de H (intensidade maxima)
	k1 = 0, k2 = H, I1 < I2
	level = I1+I2/2
	window = I2 - I1
	I1 = level - window / 2
	I2 = level + window / 2

	cada pixel I de img recebera
	k1 se I < I1
	(k2 - k1) / (I2 - I1) * (I - I1) + k1 se I1 <= I < I2
	k2 se I >= I2
*/
void ajustWindowAndLevel(float window, float level, int** img, int imageWidth, int imageHeight);

Image2D coloredImage2D(Image2D img);

Image2D coloredLabelImage2D(Image2D img, Image2D label);
