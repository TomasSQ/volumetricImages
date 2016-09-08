#include "../ex01/bitmap.h"
#include "ex02.h"

float linearTransform(float I, float k1, float k2, float I1, float I2) {
	if (I < I1) {
		return k1;
	}
	if (I >= I2) {
		return k2;
	}

	return (k2 - k1) / (I2 - I1) * (I - I1) + k1;
}

void ajustWindowAndLevel(float window, float level, float** img, int imageWidth, int imageHeight) {
	float H = 255.0; /*Mudar para 2^b - 1, com b sendo numero de bits para cada canal de cores*/
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
			img[row][col] = linearTransform(img[row][col], k1, k2, I1, I2);
		}
	}
}

int main(int argc, char *argv[]) {
	Image *img;
	Image2D cut;
	timer *t1, *t2;

	/*--------------------------------------------------------*/
	void *trash = malloc(1);
	struct mallinfo info;
	int MemDinInicial, MemDinFinal;
	free(trash);
	info = mallinfo();
	MemDinInicial = info.uordblks;

	/*--------------------------------------------------------*/

	if (argc != 4) {
		Error("Usage: ex02.o <input.scn> <window> <level>", "ex02.c - main");
	}

	t1 = Tic();

	img = ReadImage(argv[1]);
	printf("Image size: %d x %d x %d\n", img->xsize, img->ysize, img->zsize);

	cut = getCutFromImage(img, getAxisForModeAndCut(RADIOLOGIST, SAGITTAL, 103), argv[2]);

	saveImage("before", cut->img, cut->width, cut->height);
	ajustWindowAndLevel(atof(argv[2]), atof(argv[3]), cut->img, cut->width, cut->height);
	saveImage("after", cut->img, cut->width, cut->height);

	t2 = Toc();
	fprintf(stdout, "Done in %f ms\n",CompTime(t1,t2));

	DestroyImage(img);
	freeImage2D(cut);

	/* ------------------------------------------------------ */

	info = mallinfo();
	MemDinFinal = info.uordblks;
	if (MemDinInicial != MemDinFinal) {
		printf("\n\nDinamic memory was not completely deallocated (%d, %d)\n", MemDinInicial, MemDinFinal);
	}

	return 0;
}
