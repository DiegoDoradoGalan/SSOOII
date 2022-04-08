DIROBJ := obj/
DIREXE := exec/
DIRSRC := src/
DIRBOOKS := books/
DIRHEA := include/

INC := include/color.h include/searchResult.h

CFLAGS :=  -I$(DIRHEA) -c  -pthread -std=c++11
CC := g++

all : dirs searchResult SSOOIIGLE main

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

searchResult: 
	$(CC) -o $(DIROBJ)resultado.o $(DIRSRC)resultado.cpp $(CFLAGS) 

SSOOIIGLE: 
	$(CC) -o $(DIROBJ)SSOOIIGLE.o $(DIRSRC)SSOOIIGLE.cpp $(CFLAGS) 

main:
	$(CC) -o $(DIREXE)SSOOIIGLE $(DIROBJ)SSOOIIGLE.o $(DIROBJ)resultado.o -pthread -std=c++11

test:
	./$(DIREXE)SSOOIIGLE books/21-LEYES-DEL-LIDERAZGO.txt sucedió 5

clean:
	rm -r $(DIREXE) $(DIROBJ)
