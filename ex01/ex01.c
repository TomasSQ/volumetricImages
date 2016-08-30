#include "mo815-3dvis.h"

int main(int argc, char *argv[]) {
	Image *img;
	timer	*t1, *t2;

	/*--------------------------------------------------------*/
	void *trash = malloc(1);
	struct mallinfo info;
	int MemDinInicial, MemDinFinal;
	free(trash);
	info = mallinfo();
	MemDinInicial = info.uordblks;

	/*--------------------------------------------------------*/

	if (argc != 3) {
		Error("Usage: ex01.o <input.scn> <output pattern>", "ex01.c - main");
	}

	t1 = Tic();

	img = ReadImage(argv[1]);
	DestroyImage(img);

	t2 = Toc();
	fprintf(stdout, "Done in %f ms\n",CompTime(t1,t2));

	/* ------------------------------------------------------ */

	info = mallinfo();
	MemDinFinal = info.uordblks;
	if (MemDinInicial != MemDinFinal) {
		printf("\n\nDinamic memory was not completely deallocated (%d, %d)\n", MemDinInicial, MemDinFinal);
	}

	return 0;
}
