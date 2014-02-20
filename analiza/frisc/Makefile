CC=g++
CFLAGS=-W -Wall -c -g

SOURCE=generator_koda.cpp tip.cpp produkcije.cpp frisc.cpp misc.cpp
OBJS=$(SOURCE:.cpp=.o)
EXE=generator

all: $(SOURCE) $(EXE)

$(EXE): $(OBJS)
			$(CC) $(OBJS) -o $@

.cpp.o:
			$(CC) $(CFLAGS) $< -o $@

clean:
			rm -f $(EXE) $(OBJS)
			
