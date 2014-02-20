#include "deserijalizacija.h"

using namespace std;

void loadActionTable (map< pair<int, string>, akcija > &a, ifstream &f) {
  vector< string > ulaz;
  string value;
  fReadLine(f, value);

  if (value != "ACTION") {
    cerr << "No action table!" << endl;
    return;
  }

  akcija novo;

  while(1) {
    if(fReadLine(f, value) == 0 || value == "KRAJ") break;
    ulaz = split(value, " ");

    if (ulaz[0] == "POSITION") {
      int state = string2int(ulaz[1]);
      string znak = ulaz[2];

      a[make_pair(state, znak)] = novo;
      novo.clear();

    } else if (ulaz[0] == "POMAK") {
      novo.setType(POMAK);
    } else if (ulaz[0] == "STATE") {
      int state = string2int(ulaz[1]);

      novo.setState(state);
    } else if (ulaz[0] == "REDUCIRAJ") {
      novo.setType(REDUCIRAJ);
    } else if (ulaz[0] == "VALUE") {
      int value_k = string2int(ulaz[1]);

      novo.setValue(value_k);
    } else if (ulaz[0] == "LHS") {
      novo.setLHS(ulaz[1]);
    } else if (ulaz[0] == "PRIHVATI") {
      novo.setType(PRIHVATI);
    }

  }
}

void loadGotoTable (map< pair<int, string>, int > &a, ifstream &f) {
  vector< string > ulaz;
  string value;
  fReadLine(f, value);

  if (value != "GOTO") {
    cerr << "No goto table!" << endl;
    return;
  }

  while(1) {
    if(fReadLine(f, value) == 0 || value == "KRAJ") break;
    ulaz = split(value, " ");

    if (ulaz[0] == "POSITION") {
      int state = string2int(ulaz[1]);
      string znak = ulaz[2];
      int novo = string2int(ulaz[3]);

      a[make_pair(state, znak)] = novo;
    }
  }

}

void loadStartState (int &a, ifstream &f) {
  vector< string > ulaz;
  string value;
  fReadLine(f, value);

  ulaz = split(value, " ");
  if (ulaz[0] != "START") {
    cerr << "No start state!" << endl;
  } else {
    a = string2int(ulaz[1]);
  }
}

void loadSynTokens (set< string > &a, ifstream &f) {
  vector< string > ulaz;
  string value;
  fReadLine(f, value);

  ulaz = split(value, " ");
  if (ulaz[0] != "%Syn") {
    cerr << "No Syn tokens!" << endl;
  } else {
    for (
      vector< string >::iterator
      it = ulaz.begin();
      it != ulaz.end();
      ++it
    ) {
      if (*it == "%Syn") { continue; }
      a.insert(*it);
    }
  }
}
