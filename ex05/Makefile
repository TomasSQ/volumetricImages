FLAGS = -O3 -Wall -Wno-unused-result -std=gnu99 -pedantic
MO815-3DVIS-LIB = -L../libmo815-3dvis/lib -lmo815-3dvis -lm
MO815-3DVIS-FLAGS = -I../libmo815-3dvis/include

all:
	gcc $(FLAGS) demoEx05.c ex05.c ../ex01/ex01.c ../ex02/ex02.c ../ex03/ex03.c ../myLib/*.c -o ex05.o $(MO815-3DVIS-FLAGS) $(MO815-3DVIS-LIB) ;

clean:
	rm -rf ex03.o out

run:
	mkdir -p out && rm -f out/slice_* && clear && make && ./ex05.o s ../libmo815-3dvis/data/skull.scn

gif:
	convert -delay 0 -loop 0 out/slice_*.pgm ~/Desktop/slice.gif
