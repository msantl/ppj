#include "misc.h"
#include "lr_item.h"
#include "akcija.h"
#include "serijalizacija.h"

using namespace std;

typedef vector< string >::iterator vsit;
typedef vector< int >::iterator viit;
typedef set< int >::iterator siit;
typedef multimap< int, pair< int, vector< int > > >::iterator git;

map< int, string > i2s_uniqueID;
map< string, int > s2i_uniqueID;

int startingNonTerminal, newNonTerminal;
int markerTerminal;
set< int > terminals, nonTerminals, syncron;
multimap< int, pair< int, vector< int > >  > grammar;
set< int > emptyNonTerminals;
map< int, set< int > > FIRST;

set< set< LRItem > > lrItems;

map< pair< int, string >, akcija > actionTable;
map< pair< int, string >, int >    gotoTable;

int isNonTerminal(int a) { return nonTerminals.count(a); }
int isTerminal(int a) { return terminals.count(a); }


// we are using integers instead of strings
int getIndex(string a) {
  int static uniqueID = 0;
  int ret;
  if (s2i_uniqueID.find(a) != s2i_uniqueID.end()) {
    ret = s2i_uniqueID[a];
  } else {
    ret = (s2i_uniqueID[a] = uniqueID++);
  }

  i2s_uniqueID[ret] = a;

  return ret;
}

string getString(int a) {
  if (i2s_uniqueID.find(a) != i2s_uniqueID.end()) {
    return i2s_uniqueID[a];
  } else {
    return "@";
  }
}

void initTerminals(void) {
  markerTerminal = getIndex(krajNiza);   terminals.insert(markerTerminal);

  string a; getline(cin, a);
  vector< string > b = split(a, " ");
  vsit it = b.begin();
  if (*it != "%T") {
    cerr << "No terminals in input" << endl;
  } else {
    for (++it; it != b.end(); ++it) {
      terminals.insert(getIndex(*it));
    }
  }
}

void initNonTerminals(void) {
  startingNonTerminal = -1;
  newNonTerminal = getIndex("<S'>");  nonTerminals.insert(newNonTerminal);

  string a; getline(cin, a);
  vector< string > b = split(a, " ");
  vsit it = b.begin();
  if (*it != "%V") {
    cerr << "No non-terminals in input" << endl;
  } else {
    for (++it; it != b.end(); ++it) {
      if (startingNonTerminal == -1) {
        startingNonTerminal = getIndex(*it);
      }
      nonTerminals.insert(getIndex(*it));
    }
  }
}

void initSyncronise(void) {
  string a; getline(cin, a);
  vector< string > b = split(a, " ");
  vsit it = b.begin();
  if (*it != "%Syn") {
    cerr << "No syncron tokens in input" << endl;
  } else {
    for (++it; it != b.end(); ++it) {
      syncron.insert(getIndex(*it));
    }
  }
}

void printVector(vector< int > v) {
  for (viit it = v.begin(); it != v.end(); ++it) {
    cerr << getString(*it) << " ";
  }
}

void printSet(set< int > s) {
  cerr << "-----------" << endl;
  for (siit it = s.begin(); it != s.end(); ++it) {
    cerr << getString(*it) << " ";
  }
  cerr << endl << "+++++++++++" << endl;
}

void printLRSet(set< LRItem > s) {
  cerr << "-----------" << endl;
  for (set< LRItem>::iterator it = s.begin(); it != s.end(); ++it) {
    cerr << getString(it->getLHS()) << " => ";
    printVector(it->getRHS());
    cerr << "{ " << getString(it->getExtendedPart()) << "} ";
  }
  cerr << endl << "+++++++++++" << endl;
}

void initGrammar(void) {
  static int counter = 0;
  int produkcija;
  string a;
  vector< string > b;
  vector< int > tempProduction;
  vsit it;

  // we add one additional production
  tempProduction.push_back(startingNonTerminal);
  grammar.insert(make_pair(newNonTerminal, make_pair(counter++, tempProduction)));

  // read the rest of the input
  while (getline(cin, a)) {
    if (startsWith(a, " ") == 0) {
      produkcija = getIndex(a);
    } else {
      b = split(a, " ");
      tempProduction.clear();
      for (it = b.begin(); it != b.end(); ++it) {
        if (*it != "$") tempProduction.push_back(getIndex(*it));
      }
      grammar.insert(make_pair(produkcija, make_pair(counter++, tempProduction)));
    }
  }
}

void initEmptyNonTerminals(void) {
  int run_forrest_run = 1;
  while (run_forrest_run--) {
    for (git it = grammar.begin(); it != grammar.end(); ++it) {
      if (emptyNonTerminals.count(it->first)) continue;
      int isEmpty = 1;
      for (viit vit = it->second.second.begin(); vit != it->second.second.end(); ++vit ) {
        if (emptyNonTerminals.count(*vit) == 0) {
          // we can't be sure about this one
          isEmpty = 0;
          break;
        }
      }

      if (isEmpty == 1) {
        emptyNonTerminals.insert(it->first);
        run_forrest_run = 1;
        // added some changes, make one more iteration
      }
    }
  }
}

void initFirstClosure(void) {
  // first closure for terminals is the terminal itself
  for (siit it = terminals.begin(); it != terminals.end(); ++it) {
    FIRST[*it].insert(*it);
  }

  int run_forrest_run = 1;
  while (run_forrest_run--) {
    // first closure for non-terminals is a bit more complicated
    for (siit it = nonTerminals.begin(); it != nonTerminals.end(); ++it) {
      pair< git, git > pmmit = grammar.equal_range(*it);
      set< int > tempFirst;

      for (git mmit = pmmit.first; mmit != pmmit.second; ++mmit) {
        // for every production
        for (viit pit = mmit->second.second.begin(); pit != mmit->second.second.end(); ++pit) {
          // for every token
          append(tempFirst, FIRST[*pit]);
          // if it's not empty, break
          if (emptyNonTerminals.count(*pit) == 0) {
            break;
          }
        }
      }

      if (updated(FIRST[*it], tempFirst)) {
        run_forrest_run = 1;
      }
    }
  }
}

set< int > getFirstClosure(vector< int > X, int extended) {
  set< int > ret;

  X.push_back(extended);

  for (viit it = X.begin(); it != X.end(); ++it) {
    append(ret, FIRST[*it]);
    if (emptyNonTerminals.count(*it) == 0) {
      break;
    }
  }

  return ret;
}

set< LRItem > CLOSURE(set< LRItem > I) {
  queue<LRItem> q;
  for (
      set< LRItem >::iterator
      sit = I.begin();
      sit != I.end();
      ++sit
      ) {
    q.push(*sit);
  }

  while(!q.empty()) {
    LRItem sit = q.front();
    q.pop();

    int B = sit.getTokenAfterDot();
    if (isTerminal(B)) continue;

    vector< int > beta = sit.getTokensAfterDot();

    pair< git, git > pmmit = grammar.equal_range(B);

    for (git mmit = pmmit.first; mmit != pmmit.second; ++mmit) {
      set< int > firstClosure = getFirstClosure(beta, sit.getExtendedPart());

      for (siit fsit = firstClosure.begin(); fsit != firstClosure.end(); ++fsit) {
        LRItem novo(mmit, *fsit);

        if (I.count(novo) == 0) {
          I.insert(novo);
          q.push(novo);
        }
      }
    }
  }

  return I;
}

set< LRItem > GOTO(set< LRItem > I, int X) {
  set< LRItem > J;

  for (set< LRItem >::iterator it = I.begin(); it != I.end(); ++it) {
    if (it->getTokenAfterDot() != X) continue;
    LRItem temp = *it;
    temp.moveDotRight();
    J.insert(temp);
  }

  return CLOSURE(J);
}

void initLrItems(void) {
  set< LRItem > tempGOTO;

  LRItem init(grammar.find(newNonTerminal), markerTerminal);
  tempGOTO.insert(init);

  lrItems.insert(CLOSURE(tempGOTO));

  queue< set< LRItem > > q;
  for (
      set< set< LRItem > >::iterator
      lrit = lrItems.begin();
      lrit != lrItems.end();
      ++lrit
      ) {
    q.push(*lrit);
  }

  while(!q.empty()) {
    set<LRItem> lrit = q.front();
    q.pop();

    for (siit vit = nonTerminals.begin(); vit != nonTerminals.end(); ++vit) {
      tempGOTO = GOTO(lrit, *vit);
      if (tempGOTO.size() && lrItems.count(tempGOTO) == 0) {
        lrItems.insert(tempGOTO);
        q.push(tempGOTO);
      }
    }

    for (siit vit = terminals.begin(); vit != terminals.end(); ++vit) {
      tempGOTO = GOTO(lrit, *vit);
      if (tempGOTO.size() && lrItems.count(tempGOTO) == 0) {
        lrItems.insert(tempGOTO);
        q.push(tempGOTO);
      }
    }
  }

}

int findItemSet(const set< LRItem > &needle) {
  int j = 0;
  set < set < LRItem > >::iterator lrit;

  for (lrit = lrItems.begin(); lrit != lrItems.end(); ++lrit, ++j) {
    if (*lrit == needle) {
      return j;
    }
  }

  return -1;
}

void initActionTable(void) {
  map< pair< int, string >, akcija >::iterator bio;
  int i = 0;
  int j;
  set< set< LRItem > >::iterator lrit;
  set < LRItem >::iterator sit;
  for (lrit = lrItems.begin(); lrit != lrItems.end(); ++lrit, ++i) {
    for (sit = lrit->begin(); sit != lrit->end(); ++sit) {
      int a = sit->getTokenAfterDot();
      int A = sit->getLHS();

      if (isTerminal(a) && (j = findItemSet(GOTO(*lrit, a))) != -1) {
        akcija nova;
        nova.setType(POMAK);
        nova.setState(j);
        string B = getString(a);

        // pomakni reduciraj nejednoznacnost

        if ((bio = actionTable.find(make_pair(i, B))) == actionTable.end()) {
          actionTable.insert(make_pair(make_pair(i, B), nova));
        } else if (bio->second.getType() == REDUCIRAJ) {
          cerr << "Razrjesavanje S/R nejednoznacnosti" << endl;
          cerr << "u korist akcije pomakni" << endl;
          bio->second = nova;
        }

      } else if (A != newNonTerminal && sit->isCompleteItem()) {
        akcija nova;
        nova.setType(REDUCIRAJ);
        nova.setValue(sit->getProductionLength());
        nova.setLHS(getString(A));
        nova.setState(sit->getProductionID());
        string B = getString(sit->getExtendedPart());

        // reduciraj reduciraj nejednoznacnost

        if ((bio = actionTable.find(make_pair(i, B))) == actionTable.end()) {
          actionTable.insert(make_pair(make_pair(i, B), nova));
        } else if (nova.getState() < bio->second.getState()) {
          cerr << "Razrjesavanje R/R nejednoznacnosti" << endl;
          cerr << "u korist produkcije" << nova.getState() << endl;
          cerr << "(maknuli smo produkciju " << bio->second.getState() << " )" << endl;
          bio->second = nova;
        }


      } else if (A == newNonTerminal && sit->isCompleteItem()) {
        akcija nova; nova.setType(PRIHVATI);

        actionTable.insert(make_pair(make_pair(i, krajNiza), nova));
      }
    }
  }
}

void initGotoTable(void) {
  int i = 0;
  int j;
  set< set < LRItem > >::iterator lrit, jlrit;
  set< LRItem >::iterator jsit;
  siit vit;

  for (lrit = lrItems.begin(); lrit != lrItems.end(); ++lrit, ++i) {
    for (vit = nonTerminals.begin(); vit != nonTerminals.end(); ++vit) {
      if ((j = findItemSet(GOTO(*lrit, *vit))) != -1) {
        gotoTable[make_pair(i, getString(*vit))] = j;
      }
    }
  }
}


int initParserInitialState(void) {
  int i = 0;

  set< set< LRItem > >::iterator lrit;
  set< LRItem >::iterator jsit;

  for (lrit = lrItems.begin(); lrit != lrItems.end(); ++lrit, ++i) {
    for (jsit = lrit->begin(); jsit != lrit->end(); ++jsit) {
      if (jsit->getLHS() == newNonTerminal && jsit->getDotPosition() == 0) {
        return i;
      }
    }
  }

  return -1;
}


void saveSynTokens(void) {
  string tokens = "%Syn";
  for (siit sit = syncron.begin(); sit != syncron.end(); ++sit) {
    tokens += " " + getString(*sit);
  }
  saveTokens(tokens);
}

int main(void) {
  // empty_file(lr_parser_table);
  perform_housekeeping(lr_parser_table);

  cerr << "Starting generator" << endl;
  initNonTerminals();
  cerr << "done reading non terminals" << endl;
   initTerminals();
  cerr << "done reading terminals" << endl;
  initSyncronise();
  cerr << "done reading sync tokens" << endl;
  initGrammar();
  cerr << "done reading grammar" << endl;
  initEmptyNonTerminals();
  cerr << "done initializing empty non terminals" << endl;

  // printSet(terminals);
  // printSet(nonTerminals);
  // printSet(syncron);
  // printSet(emptyNonTerminals);

  initFirstClosure();
  cerr << "done initializing first() closure" << endl;

  initLrItems();
  cerr << "done finding LR items" << endl;

  initActionTable();
  initGotoTable();
  cerr << "done creating tables" << endl;

  saveStartState(initParserInitialState());
  saveActionTable(actionTable);
  saveGotoTable(gotoTable);

  cerr << "done saving tables" << endl;

  saveSynTokens();
  cerr << "done saving syn tokens" << endl;
  cerr << "Done!" << endl;

  return 0;
}
