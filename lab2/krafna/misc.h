#ifndef MISC
#define MISC

#include <cstdio>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>

#define lr_parser_table "analizator/ParseTable.txt"

#define krajNiza "?"
#define prazanStog "#"

int fReadLine(std::ifstream &, std::string &, int = 0);

int readLine(std::string &, int = 0);

std::string collapseSet(std::set< std::string >, std::string = "");

int startsWith(std::string, std::string);

std::vector< std::string > split(std::string, std::string);

void append(std::set< int > &, std::set< int >);

int updated(std::set< int > &, std::set< int >);

int string2int(std::string);

void perform_housekeeping(const char *filename);

#endif
