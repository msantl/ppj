#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <map>

#include "misc.h"
#include "enka.h"
#include "Pravila.h"

using namespace std;

#define izlazAutomata "Automat.txt"

vector< string > LAStates;
vector< string > LJNames;
vector< pair< string, string > > LARules;
map< int, vector< pair< string, string > > > LAAction;
vector< Enka > automatas;

string loadInputFile(void) {
  string ret;
  char c;
  while((c = getchar()) != EOF) {ret += c;}
  return ret;
}

int main(int argc, char **argv) {
  // load LA rules, states and stuff

  LAStates = dohvatiStanja();
  LJNames = dohvatiImena();
  LARules = dohvatiPravila();
  LAAction = dohvatiAkcije();

  string stanjeLA = LAStates.front();

  // load the #LARules automatas
  FILE *f = fopen(izlazAutomata, "r");
  for (int i = 0; i < (int)LARules.size(); ++i) {
    // reads the next automata from file
    Enka temp(f);
    automatas.push_back( temp );
  }
  fclose(f);

  // load input file for LA
  string INPUT_BUFFER = loadInputFile();;

  // current position in input file
  int current = 0;
  // marks the end of file
  int endOfFile = INPUT_BUFFER.length();

  // line number counter
  int lineNumberCounter = 1;

  while (current < endOfFile) {
    string restOfTheFile = INPUT_BUFFER.substr(current);

    // give this to every automata and get the largest match (lowest id if
    // there is a tie)

    int longestMatch = -1, ruleID = -1;

    int ruleCounter = 0;
    for (
      vector< Enka >::iterator
      it = automatas.begin();
      it != automatas.end();
      ++it
    ) {
      // we only look on automatas that have rules on states we are currently
      // on
      if (stanjeLA == LARules[ruleCounter].first) {

        int match = it->largestMatch(restOfTheFile.c_str());

        if (match > 0 && match > longestMatch) {
          longestMatch = match;
          ruleID = ruleCounter;
        }
      }

      ++ruleCounter;
    }

    // if we have something in ruleID that is not -1 we do the ruleID
    // stuff, else we have the error detection thing

    if (ruleID != -1) {
      bool outputLUName = false;
      string identity;

      // the name of the lexic unit starts on (included)
      int startLUName = current;
      // the name of the lexic unit ends on (excluded)
      int endLUName = current + longestMatch;

      vector< pair <string, string > > akcije = LAAction[ruleID];

      for (
        vector< pair< string, string > >::iterator
        vit = akcije.begin();
        vit != akcije.end();
        ++vit
      ) {
        string actionName = vit->first;
        string argument = vit->second;

        if (actionName == "UDJI_U_STANJE") {
          stanjeLA = argument;
        } else if (actionName == "NOVI_REDAK") {
          ++lineNumberCounter;
        } else if (actionName == "VRATI_SE") {
          endLUName = current + string2int(argument);
        } else if (actionName == "-") {
          // do nothing
        } else {
          // lexic unit needs some outputing
          outputLUName = true;
          identity = actionName;
        }

      }

      if (outputLUName) {
        string lexicUnit;
        for (int i = startLUName; i < endLUName; ++i)
          lexicUnit += INPUT_BUFFER[i];

        // output in given format
        printf(
          "%s %d %s\n",
          identity.c_str(),
          lineNumberCounter,
          lexicUnit.c_str()
        );
      }

      current = endLUName;

    } else {
      // recover from error by discarding the first char
      current = current + 1;
    }

  }

  return 0;
}
