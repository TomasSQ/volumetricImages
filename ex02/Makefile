FLAGS = -O3 -Wall -Wno-unused-result -std=gnu99 -pedantic
MO815-3DVIS-LIB = -L../libmo815-3dvis/lib -lmo815-3dvis -lm
MO815-3DVIS-FLAGS = -I../libmo815-3dvis/include

all:
	gcc $(FLAGS) demoEx02.c ex02.c ../ex01/ex01.c ../ex01/bitmap.c -o ex02.o $(MO815-3DVIS-FLAGS) $(MO815-3DVIS-LIB) ;

clean:
	rm ex02.o
