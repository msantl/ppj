CC=g++
CFLAGS=-W -Wall -c -g

SOURCE=semanticki_analizator.cpp tip.cpp produkcije.cpp misc.cpp
OBJS=$(SOURCE:.cpp=.o)
EXE=analizator

all: $(SOURCE) $(EXE)

$(EXE): $(OBJS)
			$(CC) $(OBJS) -o $@

.cpp.o:
			$(CC) $(CFLAGS) $< -o $@

clean:
			rm $(EXE) $(OBJS)
			
