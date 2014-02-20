#ifndef PRAVILA
#define PRAVILA

#include <map>
#include <vector>
#include <string>

using namespace std;

vector< string > dohvatiStanja(void);
vector< string > dohvatiImena(void);
vector< pair< string, string > > dohvatiPravila(void);
map< int, vector< pair< string, string > > > dohvatiAkcije(void);

#endif
