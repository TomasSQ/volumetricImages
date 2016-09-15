FLAGS = -O3 -Wall -Wno-unused-result -std=gnu99 -pedantic
MO815-3DVIS-LIB = -L../libmo815-3dvis/lib -lmo815-3dvis -lm
MO815-3DVIS-FLAGS = -I../libmo815-3dvis/include

all:
	gcc $(FLAGS) demoEx02.c ex02.c ../ex01/ex01.c ../ex01/bitmap.c ../myLib/*.c -o ex02.o $(MO815-3DVIS-FLAGS) $(MO815-3DVIS-LIB) ;

clean:
	rm -rf ex02.o out

run:
	mkdir -p out && rm -f out/afterColored_0* && clear && make && ./ex02.o ../libmo815-3dvis/data/brain.scn 80 20 ../libmo815-3dvis/data/brain-label.scn

gif:
	convert -delay 0 -loop 0 out/afterColored_*.pgm ~/Desktop/afterColored.gif
