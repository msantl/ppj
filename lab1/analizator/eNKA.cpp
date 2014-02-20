#include "misc.h"
#include "eNKA.h"

int Automat::createState(void) {
  static int id = 0;
  int novo_stanje = id ++;

  addState(novo_stanje);

  return novo_stanje;
}

void Automat::addPraviloID(int a) {praviloID = a;}
void Automat::addState(int a) {Q.insert(a);}
void Automat::addStartState(int a) {Q0.insert(a);}
void Automat::addAcceptableState(int a) {F.insert(a);}

void Automat::addTransition(int a, string ulaz, int b) {
  delta[make_pair(a, ulaz)].insert(b);
}

set< int > Automat::epsilon(int state) {
  queue< int > q;
  set < int > flag;

  flag.insert(state);
  q.push(state);

  for(;!q.empty(); q.pop()) {
    int curr = q.front();

    pair< int, string > P = make_pair(curr, "<kurcina>");

    for (
        set< int >::iterator
        sit = delta[P].begin();
        sit != delta[P].end();
        ++sit
        ) {
      if (flag.count(*sit) == 0) {
        flag.insert(*sit);
        q.push(*sit);
      }
    }

  }

  return flag;
}

void Automat::expandAutomata(void) {
  for (set< int >::iterator sit = Q.begin(); sit != Q.end(); ++sit) {
    set< int > eps = epsilon(*sit);

    // dopuni pocetna stanja sa epsilon prijelazima
    if (Q0.count(*sit) > 0) {
      append(Q0, eps);
    }

    // dopuni prijelaze sa epsilon prijelazima
    for (
        map< pair<int, string>, set< int > >::iterator
        mit = delta.begin();
        mit != delta.end();
        ++mit
        ) {
      if (mit->second.count(*sit) > 0) {
        append(mit->second, eps);
      }
    }
  }
  return;
}

void Automat::fileDump(const char *filename) {
  FILE *f = fopen(filename, "a+");

  fprintf(f, "PRAVILO %d\n", praviloID);

  for (set< int >::iterator sit = Q.begin(); sit != Q.end(); ++sit) {
    fprintf(f, "STANJE %d\n", *sit);
  }

  for (set< int >::iterator sit = F.begin(); sit != F.end(); ++sit) {
    fprintf(f, "PRIHVAT %d\n", *sit);
  }

  for (set< int >::iterator sit = Q0.begin(); sit != Q0.end(); ++sit) {
    fprintf(f, "POCETNO %d\n", *sit);
  }

  for (
      map< pair<int, string>, set< int > >::iterator
      mit = delta.begin();
      mit != delta.end();
      ++mit
      ) {
    int stanje = mit->first.first;
    string ulaz = escapeSpecialChars(mit->first.second);

    if (ulaz == "<kurcina>")continue;

    for (
        set< int >::iterator
        sit = mit->second.begin();
        sit != mit->second.end();
        ++sit
        ) {
      fprintf(
          f,
          "PRIJELAZ %d %s %d\n",
          stanje,
          ulaz.c_str(),
          *sit
          );
    }
  }

  fprintf(f, "KRAJ_AUTOMATA\n");

  fclose(f);
  return;
}

