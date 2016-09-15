#include "ex02.h"

typedef struct {
	int r;
	int g;
	int b;
} RGB;

float linearTransform(float I, float k1, float k2, float I1, float I2) {
	if (I < I1) {
		return k1;
	}
	if (I >= I2) {
		return k2;
	}

	return (k2 - k1) / (I2 - I1) * (I - I1) + k1;
}

void ajustWindowAndLevel(float window, float level, int** img, int imageWidth, int imageHeight) {
	float H = getRange(img, imageWidth, imageHeight).max;
	float k2 = H;
	float k1 = 0.0;
	float I1, I2, aux;
	int row, col;

	level = level / 100.0 * H;
	window = window / 100.0 * H;

	I1 = level - window / 2;
	I2 = level + window / 2;
	if (I1 > I2) {
		aux = I1;
		I1 = I2;
		I2 = aux;
	}

	for (row = 0; row < imageHeight; row++) {
		for (col = 0; col < imageWidth; col++) {
			img[row][col] = (int) linearTransform(img[row][col], k1, k2, I1, I2);
		}
	}
}

RGB getColor(float intensity, float maxIntensity) {
	RGB rgb;
	float V = (6 - 2) * intensity / maxIntensity + 1;

	rgb.r = 255 * MAX(0, (3 - ABS(V - 4) - ABS(V - 5)) / 2.0);
	rgb.g = 255 * MAX(0, (4 - ABS(V - 2) - ABS(V - 4)) / 2.0);
	rgb.b = 255 * MAX(0, (3 - ABS(V - 1) - ABS(V - 2)) / 2.0);

	return rgb;
}

Image2D colored(Image2D img, Image2D label, float maxIntensity) {
	int row, col;
	RGB rgb;
	Image2D colored = newColoredImage2D(img->width, img->height);

	for (row = 0; row < colored->height; row ++) {
		for (col = 0; col < colored->width; col ++) {
			if ((col + row) % 2 != 0) {
				colored->r[row][col] = img->img[row][col];
				colored->g[row][col] = img->img[row][col];
				colored->b[row][col] = img->img[row][col];
			} else {
				rgb = getColor(label->img[row][col], maxIntensity);
				colored->r[row][col] = rgb.r;
				colored->g[row][col] = rgb.g;
				colored->b[row][col] = rgb.b;
			}
		}
	}

	return colored;
}

Image2D coloredImage2D(Image2D img) {
	return colored(img, img, getRange(img->img, img->width, img->height).max);
}

Image2D coloredLabelImage2D(Image2D img, Image2D label) {
	return colored(img, label, getRange(label->img, label->width, label->height).max);
}
