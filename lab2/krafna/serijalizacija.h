#ifndef SERIJALIZACIJA
#define SERIJALIZACIJA

#include "misc.h"
#include "akcija.h"

void saveActionTable(std::map<std::pair<int, std::string>, akcija >);

void saveGotoTable(std::map<std::pair<int, std::string>, int >);

void saveStartState(int);

void saveTokens(std::string);

#endif
