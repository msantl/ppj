#ifndef ENKA
#define ENKA

#include <map>
#include <queue>
#include <set>
#include <string>
#include <cstring>
#include <cstdio>

using namespace std;

class Automat {
  public:
    int praviloID;
    set < int > Q, Q0, F;

    map< pair< int, string >, set< int > > delta;

    int createState(void);
    void addPraviloID(int);
    void addState(int);
    void addStartState(int);
    void addAcceptableState(int);
    void addTransition(int, string, int);
    set< int >epsilon(int);
    void expandAutomata(void);
    void fileDump(const char* );

};

#endif
