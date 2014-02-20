ll: analizator

analizator: analizator.o enka.o misc.o Pravila.o
	g++ -W -Wall -O2 analizator.o enka.o misc.o Pravila.o -o analizator

analizator.o: leksicki_analizator.cpp Pravila.cpp
	g++ -W -Wall -O2 -c leksicki_analizator.cpp -o analizator.o

Pravila.o: Pravila.cpp Pravila.h
	g++ -W -Wall -O2 -c Pravila.cpp

enka.o: enka.cpp enka.h
	g++ -W -Wall -O2 -c enka.cpp
	
misc.o: misc.cpp misc.h
	g++ -W -Wall -O2 -c misc.cpp

clean:
	rm -rf *o analizator
