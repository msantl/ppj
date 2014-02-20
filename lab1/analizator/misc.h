#ifndef MISC
#define MISC

#include <map>
#include <queue>
#include <set>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

int string2int(string);

bool jeOperator(string, int);

vector< string > split (string, string);

vector< string > splitLine(bool readNewLine = true);

void append(set< int >&, set< int >);

string escapeSpecialChars(string);

int findLast(string, char, int start = 0);

int findFirst(string, char, int start = 0);

#endif
