#ifndef PPJ_AUTOMATAFACTORY_H
#define PPJ_AUTOMATAFACTORY_H

#include <string>

#include "analizator/enka.h"

using namespace std;

class AutomataFactory {
  public:
    Enka create(string regex);
};

#endif
