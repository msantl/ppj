#ifndef _MISCH
#define _MISCH

#include <iostream>
#include <sstream>

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

std::vector< std::string > split(std::string, std::string);
std::string strip_signs(std::string);

bool is_terminal(std::string);

bool findInVector(std::string, std::vector< std::string >);

std::pair< std::string, int > read_line(bool = true);

int char2int(char);
long long int string2int(std::string);

class stablo {
  public:
    std::string lhs;
    std::vector< stablo* > children;

    stablo ();
    stablo (std::string a);

    ~stablo ();
};
#endif
