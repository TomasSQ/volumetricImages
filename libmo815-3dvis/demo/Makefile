BIN=../bin

#FLAGS= -g -O0 -Wall -Wno-unused-result -std=gnu99 -pedantic
FLAGS= -O3 -Wall -Wno-unused-result -std=gnu99 -pedantic
MO815-3DVIS-LIB    = -L../lib -lmo815-3dvis -lm 
MO815-3DVIS-FLAGS  = -I../include 

libmo815-3dvis:
	cd ../; $(MAKE) ; cd -\


$@.c: libmo815-3dvis $@.c 
	make
	gcc $(FLAGS) $@.c -o $(BIN)/$@ $(MO815-3DVIS-FLAGS) $(MO815-3DVIS-LIB) ; \

clean:	
	rm $(BIN)/*








