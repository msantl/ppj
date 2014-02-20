#include "frisc.h"

using namespace std;

void appendToFile(string a) {
  FILE *frisc = fopen("a.frisc", "a");
  fprintf(frisc, "%s", a.c_str());
  fclose(frisc);
}

void appendToConstFile(string a) {
  FILE *frisc = fopen("a_const.frisc", "a");
  fprintf(frisc, "%s", a.c_str());
  fclose(frisc);
}

bool isGlobalVar(string a) {
  return a.substr(0, 2) == "G_";
}
