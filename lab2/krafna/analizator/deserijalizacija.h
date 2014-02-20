#ifndef DESERIJALIZACIJA
#define DESERIJALIZACIJA

#include <string>
#include <map>
#include <vector>
#include "akcija.h"
#include "misc.h"

void loadActionTable (std::map< std::pair<int, std::string>, akcija > &, std::ifstream &);

void loadGotoTable (std::map< std::pair<int, std::string>, int > &, std::ifstream &);

void loadStartState (int &, std::ifstream &);

void loadSynTokens (std::set< std::string > &, std::ifstream &);

#endif
