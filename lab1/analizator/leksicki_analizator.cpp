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

#include "eNKA.h"
#include "misc.h"

#include "Pravila.h"

#define izlazAutomata "Automat.txt"

vector< string > LAStates;
vector< string > LJNames;
vector< pair< string, string > > LARules;
map< int, vector< pair< string, string > > > LAAction;

vector< Automat > automati;

vector< set < int > > pocetnaStanja(void) {
  vector< set< int > > ret;
  for (
    vector< Automat >::iterator
    it = automati.begin();
    it != automati.end();
    ++it
  ) {
    ret.push_back(it->Q0);
  }
  return ret;
}

void ucitaj_automate(const char *filename, int BROJ_AUTOMATA) {
  char buff[65535];

  FILE *f = fopen(filename, "r");
  Automat curr;

  while (BROJ_AUTOMATA) {
    memset(buff, 0, sizeof buff);
    fgets(buff, sizeof(buff), f);

    buff[strlen(buff) - 1] = '\0';
    string ulaz = string(buff);

    vector< string > words = split(ulaz, " ");

    if (words.front() == "KRAJ_AUTOMATA") {
      automati.push_back(curr);
      BROJ_AUTOMATA --;
      Automat novi; curr = novi;
    } else if (words.front() == "PRIJELAZ") {
      int a = string2int(words[1]);
      int b = string2int(words[3]);
      curr.addTransition(a, words[2], b);
#ifdef DEBUG
      fprintf(stderr, "PRIJELAZ: %d %s %d\n", a, words[2].c_str(), b);
#endif
    } else if (words.front() == "PRAVILO") {
      int a = string2int(words[1]);
      curr.addPraviloID(a);
#ifdef DEBUG
      fprintf(stderr, "PRAVILO: %d\n", a);
#endif
    } else if (words.front() == "STANJE") {
      int a = string2int(words[1]);
      curr.addState(a);
#ifdef DEBUG
      fprintf(stderr, "STANJE: %d\n", a);
#endif
    } else if (words.front() == "POCETNO") {
      int a = string2int(words[1]);
      curr.addStartState(a);
#ifdef DEBUG
      fprintf(stderr, "POCETNO: %d\n", a);
#endif
    } else if(words.front() == "PRIHVAT") {
      int a = string2int(words[1]);
      curr.addAcceptableState(a);
#ifdef DEBUG
      fprintf(stderr, "PRIHVAT: %d\n", a);
#endif
    }

  }
  fclose(f);
}

int main(int argc, char **argv) {
  // ucitaj pravila leksickog analizatora
  LAStates = dohvatiStanja();
  LJNames = dohvatiImena();
  LARules = dohvatiPravila();
  LAAction = dohvatiAkcije();

  // ucitaj automate
  int BROJ_AUTOMATA = LARules.size();
  ucitaj_automate(izlazAutomata, BROJ_AUTOMATA);

  // rad leksickog analizatora
  int trenutno = 0, procitano = 0;
  vector< set< int > > stanja = pocetnaStanja();

  int zadnjiDobarAutomat = -1;

  int pozicijaZadnjegDobrogStanja = 0, pocetakParsiranja = 0;
  int brojacRedaka = 1;

  string stanjeLA = LAStates.front();

  vector< char > ulazniBuffer;

  while (1) {
    if (trenutno == procitano) {
      char c;
      if ((c = getchar()) == EOF) {
        break;
      }

      ulazniBuffer.push_back(c);  ++procitano;
    }

    string ulaz;  ulaz += ulazniBuffer[trenutno];
    ulaz = escapeSpecialChars(ulaz);

    vector< set< int > > next(BROJ_AUTOMATA);

    for (int i = 0; i < BROJ_AUTOMATA; ++i) {
      for (
        set< int >::iterator
        sit = stanja[i].begin();
        sit != stanja[i].end();
        ++sit
      ) {
        int pravilo = automati[i].praviloID;
        if (stanjeLA != LARules[pravilo].first) continue;
        append(next[i], automati[i].delta[make_pair(*sit, ulaz)]);
      }
    }

#ifdef DEBUG
    fprintf(stderr, "Stanja automata %s\tUlaz: %s:\n", stanjeLA.c_str(), ulaz.c_str());
    for (int i = 0; i < BROJ_AUTOMATA; ++i) {
      int pravilo = automati[i].praviloID;
      if (stanjeLA != LARules[pravilo].first) continue;
      fprintf(stderr, "\tAutomat %d: ", pravilo);

      for (
        set< int >::iterator
        it = next[i].begin();
        it != next[i].end();
        ++it
      ) {
        fprintf(stderr, "%d ", *it);
      }
      fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");

#endif

    // provjeri koji je automat u prihvatljivom stanju a ujedno i u stanju LA
    // zapamti samo prvi jer je on i najprioritetniji
    bool ziviAutomati = false;
    int trenutnaIteracija = -1;
    for (int i = 0; i < BROJ_AUTOMATA; ++i) {
      int pravilo = automati[i].praviloID;
      if (stanjeLA != LARules[pravilo].first) continue;

      if ((int)next[i].size() > 0) ziviAutomati = true;

      for (
        set< int >::iterator
        it = next[i].begin();
        it != next[i].end();
        ++it
      ) {
        // ako jos nisam nasao automat koji je u prihvatljivom stanju
        if (trenutnaIteracija == -1 && automati[i].F.count(*it) > 0) {
          trenutnaIteracija = i;
          pozicijaZadnjegDobrogStanja = trenutno;
        }
      }
    }

    if (trenutnaIteracija != -1) zadnjiDobarAutomat = trenutnaIteracija;

    if (!ziviAutomati) {
      // crni oporavak od pogreske
      if (zadnjiDobarAutomat == -1) {
        fprintf(stderr, "Something went terribly wrong!!!\n");
        trenutno = pocetakParsiranja + 1;
        pocetakParsiranja = pocetakParsiranja + 1;
        stanja = pocetnaStanja();
        continue;
      }

      pair< string, string > pravilo = LARules[zadnjiDobarAutomat];
      vector< pair< string, string > > akcije = LAAction[zadnjiDobarAutomat];

      bool vrati_se = false, ispisiSmece = false;

      int pocetakLJ = pocetakParsiranja;
      int krajLJ = pozicijaZadnjegDobrogStanja + 1;

      string imeLeksickeJedinke;

      for (
        vector< pair <string, string > >::iterator
        vit = akcije.begin();
        vit != akcije.end();
        ++vit
      ) {
        string imeAkcije = vit->first;
        string argument = vit->second;

        if (imeAkcije == "UDJI_U_STANJE") {
          stanjeLA = argument;
        } else if (imeAkcije == "NOVI_REDAK") {
          ++brojacRedaka;
        } else if (imeAkcije == "VRATI_SE") {
          vrati_se = true;
          krajLJ = pocetakParsiranja + string2int(argument);
        } else if (imeAkcije == "-") {
          // do nothing
        } else {
          // uniformni znak
          ispisiSmece = true;
          imeLeksickeJedinke = imeAkcije;
        }

      }

      if (ispisiSmece) {
        string leksickaJedinka;

        for (int i = pocetakLJ; i < krajLJ; ++i)
          leksickaJedinka += ulazniBuffer[i];

        printf(
          "%s %d %s\n",
          imeLeksickeJedinke.c_str(),
          brojacRedaka,
          leksickaJedinka.c_str()
        );
      }

      trenutno = krajLJ;
      pocetakParsiranja = krajLJ;

      zadnjiDobarAutomat = -1;

      stanja = pocetnaStanja();
    } else {
      stanja = next;
      trenutno ++;
    }

  }


  return 0;
}
