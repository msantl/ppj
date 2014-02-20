ll: generator

generator: generator.o AutomataFactory.o analizator/enka.o analizator/misc.o
	g++ -W -Wall -O2 generator.o AutomataFactory.o analizator/enka.o analizator/misc.o -o generator

generator.o: generator_leksickog_analizatora.cpp
	g++ -W -Wall -O2 -c generator_leksickog_analizatora.cpp -o generator.o

AutomataFactory.o: AutomataFactory.cpp
	g++ -W -Wall -O2 -c AutomataFactory.cpp

enka.o: analizator/enka.cpp
	g++ -W -Wall -O2 -c analizator/enka.cpp

misc.o: analizator/misc.cpp
	g++ -W -Wall -O2 -c analizator/misc.cpp

clean:
	rm -rf *o generator analizator/*o
