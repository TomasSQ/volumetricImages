#include <limits.h>

#include "../myLib/myCommon.h"
#include "bitmap.h"

void writeImage(char* imageName, unsigned char *img, int imageWidth, int imageHeight);

void saveImage(char* imageName, int **img, int imageWidth, int imageHeight) {
	FILE *fp = NULL;
	unsigned char *data = NULL;
	int row, col;
	range r = getRange(img, imageWidth, imageHeight);

	char imgNameWithExtension[200];
	sprintf(imgNameWithExtension, "%s.pgm", imageName);
	fp = fopen(imgNameWithExtension, "w");
	if (r.max - r.min >= 0 && r.max - r.min < 256) {
		fprintf(fp,"P5\n");
		fprintf(fp,"%d %d\n", imageWidth, imageHeight);
		fprintf(fp,"%d\n", 255);

		data = (unsigned char*) malloc(sizeof(unsigned char) * imageWidth * imageHeight);
		for (row = 0; row < imageHeight; row++) {
			for (col = 0; col < imageWidth; col++) {
				data[row * imageWidth + col] = (unsigned char) (img[row][col] - r.min);
			}
		}

		fwrite(data, sizeof(unsigned char), imageWidth * imageHeight, fp);
		free(data);
	} else {
		fprintf(fp, "P2\n");
		fprintf(fp, "%d %d\n", imageWidth, imageHeight);
		fprintf(fp, "%d\n", r.max - r.min);

		for (row = 0; row < imageHeight; row++) {
			for (col = 0; col < imageWidth; col++) {
				fprintf(fp, "%d ", (int) (img[row][col] - r.min));
			}
			if (row % 17 == 0) {
				fprintf(fp, "\n");
			}
		}

	}
	fclose(fp);
}

void saveColoredImage(char* imageName, int **r, int **g, int **b, int imageWidth, int imageHeight) {
	FILE *fp=NULL;
	int	row, col;
	int maxR = getRange(r, imageWidth, imageHeight).max;
	int maxG = getRange(g, imageWidth, imageHeight).max;
	int maxB = getRange(b, imageWidth, imageHeight).max;
	char imgNameWithExtension[200];
	sprintf(imgNameWithExtension, "%s.pgm", imageName);

	fp = fopen(imgNameWithExtension, "w");
	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", imageWidth, imageHeight);
	fprintf(fp, "255\n");

	for (row = 0; row < imageHeight; row++) {
		for (col = 0; col < imageWidth; col++) {
			fputc((unsigned char) (r[row][col] * 1.0 / maxR * 256), fp);
			fputc((unsigned char) (g[row][col] * 1.0 / maxG * 256), fp);
			fputc((unsigned char) (b[row][col] * 1.0 / maxB * 256), fp);
		}
	}

	fclose(fp);
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
