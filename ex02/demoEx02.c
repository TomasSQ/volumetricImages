#include "../ex01/bitmap.h"

#include "ex02.h"

int main(int argc, char *argv[]) {
	Image *img = NULL;
	Image *label = NULL;
	Image2D cut = NULL;
	Image2D cutLabel = NULL;
	Image2D cutColored = NULL;
	timer *t1, *t2;
	char outputFileName[200];
	int i;

	/*--------------------------------------------------------*/
	void *trash = malloc(1);
	struct mallinfo info;
	int MemDinInicial, MemDinFinal;
	free(trash);
	info = mallinfo();
	MemDinInicial = info.uordblks;

	/*--------------------------------------------------------*/

	if (argc != 4 && argc != 5) {
		Error("Usage: ex02.o <input.scn> <window> <level> [input-label.scn]", "ex02.c - main");
	}

	t1 = Tic();

	img = ReadImage(argv[1]);
	printf("Image size: %d x %d x %d\n", img->xsize, img->ysize, img->zsize);

	if (argc == 5) {
		label = ReadImage(argv[4]);
		printf("Label size: %d x %d x %d\n", label->xsize, label->ysize, label->zsize);
	}

	for (i = 0; i < img->zsize; i++) {
		cut = getCutFromImage(img, getAxisForModeAndCut(RADIOLOGIST, CORONAL, i), argv[2]);
		if (label != NULL) {
			printf("Coloring using label\n");
			cutLabel = getCutFromImage(label, getAxisForModeAndCut(RADIOLOGIST, CORONAL, i), argv[2]);
		}

		// saveImage("before", cut->img, cut->width, cut->height);
		ajustWindowAndLevel(atof(argv[2]), atof(argv[3]), cut->img, cut->width, cut->height);
		//saveImage("after", cut->img, cut->width, cut->height);
		if (cutLabel == NULL) {
			cutColored = coloredImage2D(cut);
		} else {
			cutColored = coloredLabelImage2D(cut, cutLabel);
		}
		sprintf(outputFileName, "out/afterColored_%04d", i);
		saveColoredImage(outputFileName, cutColored->r, cutColored->g, cutColored->b, cutColored->width, cutColored->height);

		freeImage2D(cut);
		freeColoredImage2D(cutColored);
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
