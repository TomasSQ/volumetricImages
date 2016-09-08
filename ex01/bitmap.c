#include "bitmap.h"

void writeImage(char* imageName, unsigned char *img, int imageWidth, int imageHeight);

unsigned char getColor(float color) {
	return (unsigned char) (color > 255 ? color / 255.0 : color);
}

void saveImage(char* imageName, float **img, int imageWidth, int imageHeight) {
	saveColoredImage(imageName, img, img, img, imageWidth, imageHeight);
}

void saveColoredImage(char* imageName, float **r, float **g, float **b, int imageWidth, int imageHeight) {
	int x, y;
	unsigned char *img = (unsigned char*) malloc(sizeof(unsigned char) * 3 * imageWidth * imageHeight);

	for (y = 0; y < imageHeight; y++) {
		for (x = 0; x < imageWidth; x++) {
			img[(x + y * imageWidth) * 3 + 2] = getColor(r[y][x]);
			img[(x + y * imageWidth) * 3 + 1] = getColor(g[y][x]);
			img[(x + y * imageWidth) * 3 + 0] = getColor(b[y][x]);
		}
	}

	writeImage(imageName, img, imageWidth, imageHeight);

	free(img);
}

void writeImage(char* imageName, unsigned char *img, int imageWidth, int imageHeight) {
	FILE *file;

	int i;
	int filesize = 40 + 14 + 3 * imageWidth * imageHeight;
	unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
	unsigned char bmppad[3] = {0, 0, 0};

	unsigned char bitMapFileHeader[14] = {'B','M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
	bitMapFileHeader[2] = (unsigned char) filesize;
	bitMapFileHeader[3] = (unsigned char) (filesize >> 8);
	bitMapFileHeader[4] = (unsigned char) (filesize >> 16);
	bitMapFileHeader[5] = (unsigned char) (filesize >> 24);

	bmpinfoheader[4] = (unsigned char) imageWidth;
	bmpinfoheader[5] = (unsigned char) (imageWidth >> 8);
	bmpinfoheader[6] = (unsigned char) (imageWidth >> 16);
	bmpinfoheader[7] = (unsigned char) (imageWidth >> 24);
	bmpinfoheader[8] = (unsigned char) imageHeight;
	bmpinfoheader[9] = (unsigned char) (imageHeight >> 8);
	bmpinfoheader[10] = (unsigned char) (imageHeight >> 16);
	bmpinfoheader[11] = (unsigned char) (imageHeight >> 24);

	char imgNameWithExtension[200];
	sprintf(imgNameWithExtension, "%s.bmp", imageName);
 	file = fopen(imgNameWithExtension, "wb");

	fwrite(bitMapFileHeader, 1, 14, file);
	fwrite(bmpinfoheader, 1, 40, file);

	for (i = 0; i < imageHeight; i++) {
		fwrite(img + (imageWidth * (imageHeight - i - 1) * 3), 3, imageWidth, file);
		fwrite(bmppad, 1, (4 - (imageWidth * 3) % 4) % 4, file);
	}

	fclose(file);
}
