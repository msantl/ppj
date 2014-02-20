CC=g++
CFLAGS=-O2 -W -Wall -c

SOURCE=sintaksni_analizator.cpp deserijalizacija.cpp akcija.cpp lexem.cpp misc.cpp
OBJS=$(SOURCE:.cpp=.o)
EXE=analizator

all: $(SOURCE) $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE) $(OBJS)

