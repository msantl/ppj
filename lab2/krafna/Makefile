CC=g++
CFLAGS=-O2 -W -Wall -c 

SOURCE=generator_sintaksnog_analizatora.cpp misc.cpp lr_item.cpp akcija.cpp serijalizacija.cpp
OBJS=$(SOURCE:.cpp=.o)
EXE=generator

all: $(SOURCE) $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE) $(OBJS)

