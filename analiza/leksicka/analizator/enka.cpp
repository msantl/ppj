#include <queue>
#include <cassert>
#include <cstdio>
#include <sstream>

#include "enka.h"

using namespace std;


Enka::Enka() {
  startState = new State();
  endState = new State();
}

Enka::Enka(char letter) {
  startState = new State();
  endState = new State();
  startState->addTrans(letter, endState);
}

Enka::Enka(FILE* input) {
  const int BUFF_SZ = 256;
  char buff[BUFF_SZ];
  int numTrans, startId, endId;

  ostringstream sEnka;
  fscanf(input, "%d\n", &numTrans);
  fgets(buff, BUFF_SZ, input);
  sEnka << numTrans << endl << buff;
  for(int i = 0; i < numTrans; i++) {
    fgets(buff, BUFF_SZ, input);
    sEnka << buff;
  }
  initFromString(sEnka.str());
}

Enka::Enka(string serializedEnka) {
  initFromString(serializedEnka);
}

void Enka::makeTransition(char letter) {
  set <State*> next;
  for(set<State*>::iterator it = currStates.begin();
      it != currStates.end();
      it++) {
    set <State*> nextStates = (*it)->getNext(letter);
    next.insert(nextStates.begin(), nextStates.end());
  }
  currStates = next;
}

void Enka::addEpsClosure() {
  queue <State*> q;
  set <State*> visited;

  for(set<State*>::iterator it = currStates.begin();
      it != currStates.end();
      it++) {
    q.push(*it);
  }

  while(!q.empty()) {
    State* curr = q.front();
    q.pop();

    if (visited.find(curr) != visited.end()) {
      continue;
    }
    visited.insert(curr);
    currStates.insert(curr);

    set <State*> epsClosure = curr->getNext(EPS);
    for(set <State*>::iterator it = epsClosure.begin();
        it != epsClosure.end();
        it++) {
      q.push(*it);
    }
  }
}


bool Enka::startEval() {
  currStates.clear();
  currStates.insert(startState);
  addEpsClosure();
  return currStates.find(endState) != currStates.end();
}

bool Enka::evaluate(string word) {
  startEval();
  for(string::iterator it = word.begin();
      it != word.end();
      it++) {
    makeTransition(*it);
    addEpsClosure();
  }
  return currStates.find(endState) != currStates.end();
}

int Enka::largestMatch(const char* word) {
  int largest = -1; // means no match
  if (startEval()) {
    largest = 0; // empty string is matched
  }
  for (int i = 0; word[i] != '\0'; i++) {
    if (currStates.empty()) {
      // nothing can get accepted after this point
      break;
    }
    makeTransition(word[i]);
    addEpsClosure();
    if (currStates.find(endState) != currStates.end()) {
      largest = i+1;
    }
  }
  return largest;
}

void Enka::fillIds(
    vector <pair <int, pair <char, int> > >& allTrans,
    map <const State*, int>& ID,
    int& nextId,
    const State* curr) {
  if (ID.find(curr) != ID.end()) {
    return;
  }
  ID[curr] = nextId++;

  const map <char, set<State*> >& trans = curr->getAll();
  for(map <char, set<State*> >::const_iterator transIt = trans.begin();
      transIt != trans.end();
      transIt++) {
    for(set <State*>::const_iterator stateIt = transIt->second.begin();
        stateIt != transIt->second.end();
        stateIt++) {
      fillIds(allTrans, ID, nextId, *stateIt);
    }
  }

  // now all curr's neighbours have id's and we can store static transitions
  vector <pair <int, pair <char, int> > > transList = curr->getStatic(ID);
  for(vector <pair <int, pair <char, int> > >::iterator it = transList.begin();
      it != transList.end();
      it++) {
    allTrans.push_back(*it);
  }
}

void Enka::initFromString(string serializedEnka) {
  istringstream sEnka(serializedEnka);
  int numTrans, startId, endId;
  sEnka >> numTrans;
  sEnka >> startId >> endId;

  int sState, eState, letter;
  State *sReal, *eReal;
  map <int, State*> ID;
  for(int i = 0; i < numTrans; i++) {
    sEnka >> sState >> letter >> eState;
    if (ID.find(sState) == ID.end()) {
      sReal = new State();
      ID[sState] = sReal;
    } else {
      sReal = ID[sState];
    }
    if (ID.find(eState) == ID.end()) {
      eReal = new State();
      ID[eState] = eReal;
    } else {
      eReal = ID[eState];
    }

    sReal->addTrans((char)letter, eReal);
  }
  startState = ID[startId];
  endState = ID[endId];
}

string Enka::serialize() {
  map <const State*, int> ID;
  int nextId = 0;
  vector <pair <int, pair <char, int> > > allTrans;
  fillIds(allTrans, ID, nextId, startState);

  ostringstream serialized;
  serialized << allTrans.size() << endl;
  serialized << ID[startState] << " " << ID[endState] << endl;
  for(vector <pair <int, pair <char, int> > >::iterator it = allTrans.begin();
      it != allTrans.end();
      it++) {

    serialized << it->first << " " << (int)(it->second.first)
               << " " << it->second.second << endl;
  }
  return serialized.str();
}

Enka Enka::operator +(const Enka& right) {
  this->startState->addTrans(EPS, right.startState);
  right.endState->addTrans(EPS, this->endState);
  return Enka(this->startState, this->endState);
}

Enka Enka::operator -(const Enka& right) {
  this->endState->addTrans(EPS, right.startState);
  return Enka(this->startState, right.endState);
}

Enka Enka::operator ~() {
  startState->addTrans(EPS, endState);
  endState->addTrans(EPS, startState);
  return *this;
}

/*
void log(const string& message) {
  printf("%s", message.c_str());
  fflush(stdout);
}

void log(const char *message) {
  printf("%s", message);
  fflush(stdout);
}


int main() {
  log("Starting testing\n");

  Enka A('a');
  Enka B('b');
  Enka C;
  log("Constructors passed\n");

  assert( A.evaluate("a"));
  assert(!A.evaluate("aa"));
  assert(!A.evaluate("aaa"));
  assert(!A.evaluate("b"));
  assert(!A.evaluate(""));
  log("Basic evaluation for simple enka passed\n");

  //  operator izbora (a|b)
  assert( (A+B).evaluate("a") );
  assert( (A+B).evaluate("b") );
  assert(!(A+B).evaluate("c") );
  assert(!(A+B).evaluate("aa") );
  assert(!(A+B).evaluate("ba") );
  assert(!(A+B).evaluate("") );
  log("Basic evaluation for A+B enka passed\n");

  //  operator nadovezivanja (ab)
  assert( (A-B).evaluate("ab"));
  assert(!(A-B).evaluate("b"));
  assert(!(A-B).evaluate("c"));
  assert(!(A-B).evaluate("aa"));
  assert(!(A-B).evaluate("ba"));
  assert(!(A-B).evaluate(""));
  log("Basic evaluation for A-B enka passed\n");

  //  kleenov operator a*
  assert( (~A).evaluate(""));
  assert( (~A).evaluate("a"));
  assert( (~A).evaluate("aa"));
  assert( (~A).evaluate("aaa"));
  assert(!(~A).evaluate("aba"));
  assert(!(~A).evaluate("ba"));
  assert(!(~A).evaluate("bbb"));
  assert( (~(A+B)).evaluate("ababbbababbb"));
  assert(!(~(A+B)).evaluate("ababcbbababbb"));
  log("Basic evaluation for kleen operator passed\n");

  A = Enka('a');
  B = Enka('b');

  Enka reg = ~(A+B);
  Enka sameReg = Enka(reg.serialize());
  assert(sameReg.largestMatch("ababbbababbb") == 12);
  assert(sameReg.evaluate("ababbbababbb"));
  assert(!sameReg.evaluate("ababcbbababbb"));
  log("Serialization tests passed\n");

  log("All tests passed!\n");
  return 0;
}
*/
