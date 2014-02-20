#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <map>

using namespace std;

#include "analizator/eNKA.h"
#include "analizator/misc.h"

#define izlazAutomat "analizator/Automat.txt"
#define izlazPravila "analizator/Pravila.h"

map< string, string > regexLookup;
vector< string > LAStates;
vector< string > LJNames;
vector< pair< string, string > > LARules;
map< int, vector< pair< string, string > > > LAAction;

void loadRegularExpressions() {
  vector< string > input;
  map< string, string > regex;
  vector< string > expand;

  while(1) {
    input = splitLine();
    if (input.front() == "%X") break;

    regex[ input[0] ] = input[1];
    expand.push_back(input[0]);
  }

  for (vector< string >::iterator it = expand.begin(); it != expand.end(); ++it) {
    string regularExpression = regex[*it];

    while (1) {
      int left = findFirst(regularExpression, '{');
      if (left == -1) break;
      int right = findFirst(regularExpression, '}', left);
      if (right == -1) break;

      string referenca = regularExpression.substr(left, right - left + 1);
      string expandedRegex =
        regularExpression.substr(0, left) + "(" +
        regex[referenca]
        + ")" + regularExpression.substr(right + 1);

#ifdef DEBUG
  fprintf(
    stderr,
    "REGEX: %s\t REF: %s\t SUB: %s\n",
    regularExpression.c_str(),
    referenca.c_str(),
    regex[referenca].c_str()
  );
#endif

      regularExpression = expandedRegex;
   }

    regex[*it] = regularExpression;
  }

  regexLookup = regex;

  return;
}

void loadLAStates() {
  vector< string > input = splitLine(false);
  input.erase(input.begin());

#ifdef DEBUG
  for (vector< string >::iterator it = input.begin(); it != input.end(); ++it) {
    fprintf(stderr, "%s ", it->c_str());
  } fprintf(stderr, "\n");
#endif

  LAStates = input;
}

void loadLJNames() {
  vector< string > input = splitLine();
  input.erase(input.begin());

#ifdef DEBUG
  for (vector< string >::iterator it = input.begin(); it != input.end(); ++it) {
    fprintf(stderr, "%s ", it->c_str());
  } fprintf(stderr, "\n");
#endif

  LJNames = input;
}

void loadLARules() {
  vector< string > input;
  while (1) {
    input = splitLine();
    if (input.size() == 0) break; // traljav nacin prepoznavanja EOF

    vector< string > stanje_regex = split(input.front(), ">");
    string imeStanje = stanje_regex[0].substr(1);
    string regularniIzraz = stanje_regex[1];

    while (1) {
      int left = findFirst(regularniIzraz, '{');
      if (left == -1) break;
      int right = findFirst(regularniIzraz, '}', left);
      if (right == -1) break;

      string referenca = regularniIzraz.substr(left, right - left + 1);
      string expandedRegex = regularniIzraz.substr(0, left) + "(" +
        regexLookup[referenca] +
        ")" + regularniIzraz.substr(right + 1);

      regularniIzraz = expandedRegex;
    }

    LARules.push_back(make_pair(imeStanje, regularniIzraz));
#ifdef DEBUG
  fprintf(
    stderr,
    "STANJE: %s REGEX: %s\n",
    imeStanje.c_str(),
    regularniIzraz.c_str()
  );
#endif

    vector< pair< string, string > > akcijeLA;

    while (1) {
      input = splitLine();
      if (input.front() == "{") continue;
      if (input.front() == "}") break;

      input.push_back(""); // osiguram se da imam 2 clana

      akcijeLA.push_back(make_pair(input[0], input[1]));

#ifdef DEBUG
  fprintf(stderr, "\tAKCIJA: %s ARG: %s\n", input[0].c_str(), input[1].c_str());
#endif

    }

    LAAction[ LARules.size() - 1 ] = akcijeLA;
  }
  return;
}

// pretvorba regex -> automat prema uputama

pair< int, int > pretvori(string izraz, Automat &automat) {
#ifdef DEBUG
  fprintf(stderr, "AUTOMAT: %s\n", izraz.c_str());
#endif

  vector< string > izbor;
  int br_zagrada = 0;
  int zadnji_negrupirani = 0;

  for (int i = 0; i < (int)izraz.length(); ++i) {
    if (izraz[i] == '(' && jeOperator(izraz, i)) {
      ++br_zagrada;
    } else if (izraz[i] == ')' && jeOperator(izraz, i)) {
      --br_zagrada;
    } else if (br_zagrada == 0 && izraz[i] == '|' && jeOperator(izraz, i)) {
      izbor.push_back(
        izraz.substr(zadnji_negrupirani, i - zadnji_negrupirani)
      );
      zadnji_negrupirani = i + 1;
    }
  }

  if (izbor.size() > 0) {
    izbor.push_back(
      izraz.substr(zadnji_negrupirani)
    );
  }

  int lijevoStanje = automat.createState();
  int desnoStanje = automat.createState();

  if (izbor.size() > 0) {
    for (
      vector< string >::iterator
      it = izbor.begin();
      it != izbor.end();
      ++it
    ) {
      pair< int, int > temp = pretvori(*it, automat);
      automat.addTransition(lijevoStanje, "<kurcina>", temp.first);
      automat.addTransition(temp.second, "<kurcina>", desnoStanje);
    }
  } else {
    bool prefiksirano = false;
    int zadnje_stanje = lijevoStanje;
    for (int i = 0; i < (int)izraz.length(); ++i) {
      int a, b;
      if (prefiksirano == true) {
        // slucaj 1
        prefiksirano = false;
        string prijelazni_znak;
        if (izraz[i] == 't') {
          prijelazni_znak += '\t';
        } else if (izraz[i] == 'n') {
          prijelazni_znak += '\n';
        } else if (izraz[i] == '_') {
          prijelazni_znak += ' ';
        } else {
          prijelazni_znak += izraz[i];
        }

        a = automat.createState();
        b = automat.createState();

        automat.addTransition(a, prijelazni_znak, b);

      } else {
        // slucaj 2
        if (izraz[i] == '\\') {
          prefiksirano = true;
          continue;
        }

        if (izraz[i] != '(') {
          //slucaj 2a
          a = automat.createState();
          b = automat.createState();
          if (izraz[i] == '$') {
            automat.addTransition(a, "<kurcina>", b);
          } else {
            string smece; smece += izraz[i];
            automat.addTransition(a, smece, b);
          }
        } else {
          // slucaj 2b
          int j = i + 1, privremeniBrojacZagrada = 0;
          for (; j < (int)izraz.length(); ++j) {
            if (izraz[j] == '(' && jeOperator(izraz, j)) {
              privremeniBrojacZagrada ++;
            } else if (izraz[j] == ')' && jeOperator(izraz, j)) {
              if (privremeniBrojacZagrada == 0) {
                break;
              } else {
                privremeniBrojacZagrada --;
              }
            }
          }

          pair< int, int > temp =
            pretvori(izraz.substr(i + 1, j - i - 1), automat);

          a = temp.first;
          b = temp.second;
          i = j;
        }
      }

      if (i + 1 < (int)izraz.length() && izraz[i+1] == '*') {
        int x = a;
        int y = b;

        a = automat.createState();
        b = automat.createState();

        automat.addTransition(a, "<kurcina>", x);
        automat.addTransition(y, "<kurcina>", b);
        automat.addTransition(a, "<kurcina>", b);
        automat.addTransition(y, "<kurcina>", x);

        i ++;
      }

      automat.addTransition(zadnje_stanje, "<kurcina>", a);
      zadnje_stanje = b;
    }

    automat.addTransition(zadnje_stanje, "<kurcina>", desnoStanje);
  }

  return make_pair(lijevoStanje, desnoStanje);
}

void cleanFiles(const char *filename) {
  FILE *f = fopen(filename, "w");
  fclose(f);
}

int main(int argc, char **argv) {
  cleanFiles(izlazAutomat);
  cleanFiles(izlazPravila);

  loadRegularExpressions();
  loadLAStates();
  loadLJNames();
  loadLARules();

  int ruleCounter = 0;

  for (
    vector< pair< string, string > >::iterator
    it = LARules.begin();
    it != LARules.end();
    ++it
  ) {
    Automat temp;
    pair< int, int > FQ = pretvori(it->second, temp);

    temp.addPraviloID(ruleCounter);
    temp.addStartState(FQ.first);
    temp.addAcceptableState(FQ.second);

    // expand Automata
    temp.expandAutomata();

    // ispis automata
    temp.fileDump(izlazAutomat);

    ++ruleCounter;
  }

  // ispis pravila

  FILE *f = fopen(izlazPravila, "w");

  fprintf(f, "vector< string > dohvatiStanja() {\n");
  fprintf(f, "\tvector< string > ret;\n");
  for (
    vector< string >::iterator
    it = LAStates.begin();
    it != LAStates.end();
    ++it
  ) {
    fprintf(
      f,
      "\tret.push_back(\"%s\");\n",
      it->c_str()
    );
  }
  fprintf(f, "\treturn ret;\n");
  fprintf(f, "}\n");

  fprintf(f, "vector< string > dohvatiImena() {\n");
  fprintf(f, "\tvector< string > ret;\n");
  for (
    vector< string >::iterator
    it = LJNames.begin();
    it != LJNames.end();
    ++it
  ) {
    fprintf(
      f,
      "\tret.push_back(\"%s\");\n",
      it->c_str()
    );
  }
  fprintf(f, "\treturn ret;\n");
  fprintf(f, "}\n");

  fprintf(f, "vector< pair< string, string > > dohvatiPravila() {\n");
  fprintf(f, "\tvector< pair< string, string > > ret;\n");
  for (
    vector< pair<string, string> >::iterator
    it = LARules.begin();
    it != LARules.end();
    ++it
  ) {
    fprintf(
      f,
      "\tret.push_back(make_pair(\"%s\", \"%s\"));\n",
      it->first.c_str(),
      escapeSpecialChars(it->second).c_str()
    );
  }
  fprintf(f, "\treturn ret;\n");
  fprintf(f, "}\n");

  fprintf(
    f,
    "map< int, vector< pair< string, string > > > dohvatiAkcije() {\n"
  );
  fprintf(
    f,
    "\tmap< int, vector< pair< string, string > > >ret;\n"
  );
  for (
    map< int, vector< pair< string, string > > >::iterator
    mit = LAAction.begin();
    mit != LAAction.end();
    ++mit
  ) {
    int praviloID = mit->first;
    for (
      vector< pair<string, string> >::iterator
      it = mit->second.begin();
      it != mit->second.end();
      ++it
      ) {
        fprintf(
          f,
          "\tret[%d].push_back(make_pair(\"%s\", \"%s\"));\n",
          praviloID,
          it->first.c_str(),
          it->second.c_str()
        );
      }
  }
  fprintf(f, "\treturn ret;\n");
  fprintf(f, "}\n");

  fclose(f);

  return 0;
}
