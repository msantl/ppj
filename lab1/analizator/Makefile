CC=g++
CFLAGS=-O2 -W -Wall -c

SOURCE=leksicki_analizator.cpp eNKA.cpp misc.cpp
OBJS=$(SOURCE:.cpp=.o)
EXE=analizator

all: $(SOURCE) $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXE) $(OBJS)
