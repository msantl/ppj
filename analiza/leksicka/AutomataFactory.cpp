#include <vector>

#include "AutomataFactory.h"
#include "analizator/misc.h"

using namespace std;

Enka AutomataFactory::create(string regex) {
#ifdef DEBUG
  fprintf(stderr, "AUTOMAT: %s\n", regex.c_str());
#endif

  vector< string > izbor;
  int br_zagrada = 0;
  int zadnji_negrupirani = 0;

  for (int i = 0; i < (int)regex.length(); ++i) {
    if (regex[i] == '(' && jeOperator(regex, i)) {
      ++br_zagrada;
    } else if (regex[i] == ')' && jeOperator(regex, i)) {
      --br_zagrada;
    } else if (br_zagrada == 0 && regex[i] == '|' && jeOperator(regex, i)) {
      izbor.push_back(
        regex.substr(zadnji_negrupirani, i - zadnji_negrupirani)
      );
      zadnji_negrupirani = i + 1;
    }
  }

  if (izbor.size() > 0) {
    izbor.push_back(
      regex.substr(zadnji_negrupirani)
    );
  }

  if (izbor.size() > 0) {
    Enka sviOr;
    for (
      vector< string >::iterator
      it = izbor.begin();
      it != izbor.end();
      ++it
    ) {
      sviOr = sviOr + create(*it);
    }
    return sviOr;
  } else {
    Enka sviSpojeni(Enka::EPS);
    bool prefiksirano = false;
    for (int i = 0; i < (int)regex.length(); ++i) {
      Enka zadnji;
      if (prefiksirano == true) {
        // slucaj 1
        prefiksirano = false;
        char prijelazni_znak;
        if (regex[i] == 't') {
          prijelazni_znak = '\t';
        } else if (regex[i] == 'n') {
          prijelazni_znak = '\n';
        } else if (regex[i] == '_') {
          prijelazni_znak = ' ';
        } else {
          prijelazni_znak = regex[i];
        }
        zadnji = Enka(prijelazni_znak);
      } else {
        // slucaj 2
        if (regex[i] == '\\') {
          prefiksirano = true;
          continue;
        }

        if (regex[i] != '(') {
          //slucaj 2a
          if (regex[i] == '$') {
            zadnji = Enka(Enka::EPS);
          } else {
            zadnji = Enka(regex[i]);
          }
        } else {
          // slucaj 2b
          int j = i + 1, privremeniBrojacZagrada = 0;
          for (; j < (int)regex.length(); ++j) {
            if (regex[j] == '(' && jeOperator(regex, j)) {
              privremeniBrojacZagrada ++;
            } else if (regex[j] == ')' && jeOperator(regex, j)) {
              if (privremeniBrojacZagrada == 0) {
                break;
              } else {
                privremeniBrojacZagrada --;
              }
            }
          }

          zadnji = create(regex.substr(i + 1, j - i - 1));

          i = j;
        }
      }

      if (i + 1 < (int)regex.length() && regex[i+1] == '*') {
        zadnji = ~zadnji;
        i ++;
      }
      sviSpojeni = sviSpojeni - zadnji;
    }
    return sviSpojeni;
  }
}
