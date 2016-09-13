#include "bitmap.h"
#include "ex01.h"

void test(Image *img, char mode, char cut, int start, char* outputFileNamePattern) {
	char outputFileName[200];
	Image2D image2D;

	sprintf(outputFileName, "%s_%c_%c_%04d", outputFileNamePattern, mode, cut, start);
	image2D = getCutFromImage(img, getAxisForModeAndCut(mode, cut, start), outputFileName);
	saveImage(outputFileName, image2D->img, image2D->width, image2D->height);

	freeImage2D(image2D);
}

int main(int argc, char *argv[]) {
	Image *img;
	timer *t1, *t2;
	int i;

	/*--------------------------------------------------------*/
	void *trash = malloc(1);
	struct mallinfo info;
	int MemDinInicial, MemDinFinal;
	free(trash);
	info = mallinfo();
	MemDinInicial = info.uordblks;

	/*--------------------------------------------------------*/

	if (argc != 4) {
		Error("Usage: ex01.o <input.scn> <outputFileNamePattern> <start>", "ex01.c - main");
	}

	t1 = Tic();

	img = ReadImage(argv[1]);

	printf("Image size: %d x %d x %d\n", img->xsize, img->ysize, img->zsize);

	for (i = 1; i < img->zsize; i++) {
		test(img, RADIOLOGIST, AXIAL, i, argv[2]);
		test(img, NEUROSURGEON, AXIAL, i, argv[2]);
	}

	for (i = 1; i < img->xsize; i++) {
		test(img, RADIOLOGIST, SAGITTAL, i, argv[2]);
		test(img, NEUROSURGEON, SAGITTAL, i, argv[2]);
	}

	for (i = 1; i < img->ysize; i++) {
		test(img, RADIOLOGIST, CORONAL, i, argv[2]);
		test(img, NEUROSURGEON, CORONAL, i, argv[2]);
	}

	t2 = Toc();
	fprintf(stdout, "Done in %f ms\n",CompTime(t1,t2));

	DestroyImage(img);

	/* ------------------------------------------------------ */

	info = mallinfo();
	MemDinFinal = info.uordblks;
	if (MemDinInicial != MemDinFinal) {
		printf("\n\nDinamic memory was not completely deallocated (%d, %d)\n", MemDinInicial, MemDinFinal);
	}

	return 0;
}
