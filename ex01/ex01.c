#include "mo815-3dvis.h"
#include "bitmap.h"

#define RADIOLOGIST 'R'
#define NEUROSURGEON 'N'
#define AXIAL 'A'
#define SAGITTAL 'S'
#define CORONAL 'C'

int getX(Image *img, char mode, char plane, int start, int i, int j) {
	switch (mode) {
		case RADIOLOGIST:
			switch (plane) {
				case AXIAL:
					return j;
				case SAGITTAL:
					return start;
				case CORONAL:
					return j;
			}
		case NEUROSURGEON:
			switch (plane) {
				case AXIAL:
					return img->xsize - j;
				case SAGITTAL:
					return img->xsize - start;
				case CORONAL:
					return j;
			}
	}

	return -1;
}

int getY(Image *img, char mode, char plane, int start, int i, int j) {
	switch (mode) {
		case RADIOLOGIST:
			switch (plane) {
				case AXIAL:
					return i;
				case SAGITTAL:
					return img->ysize - j;
				case CORONAL:
					return start;
			}
		case NEUROSURGEON:
			switch (plane) {
				case AXIAL:
					return i;
				case SAGITTAL:
					return j;
				case CORONAL:
					return start;
			}
	}

	return -1;
}

int getZ(Image *img, char mode, char plane, int start, int i, int j) {
	switch (mode) {
		case RADIOLOGIST:
			switch (plane) {
				case AXIAL:
					return start;
				case SAGITTAL:
					return img->zsize - i;
				case CORONAL:
					return img->zsize - i;
			}
		case NEUROSURGEON:
			switch (plane) {
				case AXIAL:
					return img->zsize - start;
				case SAGITTAL:
					return img->zsize - i;
				case CORONAL:
					return img->zsize - i;
			}
	}

	return -1;
}

int getColumns(Image *img, char plane) {
	switch (plane) {
		case AXIAL:
			return img->xsize;
		case SAGITTAL:
			return img->ysize;
		case CORONAL:
			return img->xsize;
	}

	return -1;
}

int getLines(Image *img, char plane) {
	switch (plane) {
		case AXIAL:
			return img->ysize;
		case SAGITTAL:
			return img->zsize;
		case CORONAL:
			return img->zsize;
	}

	return -1;
}

void drawPlane(Image *img, char mode, char plane, int start, char* outputFileNamePattern) {
	char outputFileName[200];
	sprintf(outputFileName, "%s_%c_%c_%d", outputFileNamePattern, mode, plane, start);

	int columns = getColumns(img, plane);
	int lines = getLines(img, plane);
	int i, j, x, y, z;

	float **corte = (float**) malloc(sizeof(float*) * lines);
	for (i = 0; i < lines; i++) {
		corte[i] = (float*) malloc(sizeof(float) * columns);
		for (j = 0; j < columns; j++) {
			x = getX(img, mode, plane, start, i, j);
			y = getY(img, mode, plane, start, i, j);
			z = getZ(img, mode, plane, start, i, j);
			corte[i][j] = img->val[z * img->ysize * img->xsize + y * img->xsize + x];
		}
	}

	saveImage(outputFileName, corte, columns, lines);

	for (y = 0; y < lines; y++) {
		free(corte[y]);
	}
	free(corte);
}

int main(int argc, char *argv[]) {
	Image *img;
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
		Error("Usage: ex01.o <input.scn> <outputFileNamePattern> <start>", "ex01.c - main");
	}

	t1 = Tic();

	img = ReadImage(argv[1]);

	drawPlane(img, RADIOLOGIST, AXIAL, atoi(argv[3]), argv[2]);
	drawPlane(img, RADIOLOGIST, SAGITTAL, atoi(argv[3]), argv[2]);
	drawPlane(img, RADIOLOGIST, CORONAL, atoi(argv[3]), argv[2]);
	drawPlane(img, NEUROSURGEON, AXIAL, atoi(argv[3]), argv[2]);
	drawPlane(img, NEUROSURGEON, SAGITTAL, atoi(argv[3]), argv[2]);
	drawPlane(img, NEUROSURGEON, CORONAL, atoi(argv[3]), argv[2]);

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
