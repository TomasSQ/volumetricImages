FLAGS = -O3 -Wall -Wno-unused-result -std=gnu99 -pedantic
MO815-3DVIS-LIB = -L../libmo815-3dvis/lib -lmo815-3dvis -lm
MO815-3DVIS-FLAGS = -I../libmo815-3dvis/include

all:
	gcc $(FLAGS) demoEx01.c ex01.c ../myLib/*.c -o ex01.o $(MO815-3DVIS-FLAGS) $(MO815-3DVIS-LIB) ;

clean:
	rm -rf ex01.o out

run:
	mkdir -p out && rm -f out/z_* && clear && make && ./ex01.o ../libmo815-3dvis/data/skull.scn out/z 25

gif:
	convert -delay 0 -loop 0 out/z_R_A_*.pgm ~/Desktop/axial01.gif
	convert -delay 0 -loop 0 out/z_N_A_*.pgm ~/Desktop/axial02.gif
	convert -delay 0 -loop 0 out/z_R_S_*.pgm ~/Desktop/sagital01.gif
	convert -delay 0 -loop 0 out/z_N_S_*.pgm ~/Desktop/sagital02.gif
	convert -delay 0 -loop 0 out/z_R_C_*.pgm ~/Desktop/coronal01.gif
	convert -delay 0 -loop 0 out/z_N_C_*.pgm ~/Desktop/coronal02.gif
