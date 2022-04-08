DIROBJ := obj/
DIREXE := exec/
DIRSRC := src/
DIRBOOKS := books/
DIRHEA := include/

INC := include/color.h include/Resultado.h include/Datos.h

CFLAGS :=  -I$(DIRHEA) -c -pthread -std=c++11
CC := g++

all : dirs resultado SSOOIIGLE main

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

resultado: 
	$(CC) -o $(DIROBJ)resultado.o $(DIRSRC)resultado.cpp $(CFLAGS) 

SSOOIIGLE: 
	$(CC) -o $(DIROBJ)SSOOIIGLE.o $(DIRSRC)SSOOIIGLE.cpp $(CFLAGS) 

main:
	$(CC) -o $(DIREXE)SSOOIIGLE $(DIROBJ)SSOOIIGLE.o $(DIROBJ)resultado.o -pthread -std=c++11

test:
	./$(DIREXE)SSOOIIGLE books/El-oro-y-la-ceniza.txt ceniza 10

clean:
	rm -r $(DIREXE) $(DIROBJ)
