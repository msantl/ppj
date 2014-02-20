#include "lexem.h"

using namespace std;

lexem_t::lexem_t() {}

lexem_t::lexem_t(string _oznaka) : oznaka(_oznaka) {}

void lexem_t::addChild(lexem_t d) {
  djeca.push_back(d);
}

string lexem_t::getName(void) {
  return oznaka;
}

void dfs(lexem_t root, int spaces) {
  vector< lexem_t >::iterator it;
  reverse(root.djeca.begin(), root.djeca.end());

  for (int i=0; i<spaces; ++i ) printf(" ");
  printf("%s\n", root.getName().c_str());
  for (it=root.djeca.begin(); it!=root.djeca.end(); ++it) {
    dfs(*it, spaces + 1);
  }
}
