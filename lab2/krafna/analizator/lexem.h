#ifndef LEXEM
#define LEXEM

#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <map>

#include "misc.h"

class lexem_t {
  private:
    std::string oznaka;
  public:
    std::vector< lexem_t > djeca;

    lexem_t ();
    lexem_t (std::string);

    void addChild(lexem_t);
    std::string getName(void);
};

void dfs(lexem_t, int = 0);

#endif
