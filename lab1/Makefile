CC=g++
CFLAGS=-O2 -W -Wall -c

SOURCE=generator_leksickog_analizatora.cpp analizator/misc.cpp analizator/eNKA.cpp
OBJS=$(SOURCE:.cpp=.o)
EXE=generator

all: $(SOURCE) $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXE) $(OBJS)
