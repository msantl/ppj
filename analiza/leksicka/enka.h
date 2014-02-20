#ifndef PPJ_ENKA_H
#define PPJ_ENKA_H

#include <set>
#include <map>
#include <string>

using namespace std;

class State {
  public:
    void addTrans(char letter, State* next) {
      trans[letter].insert(next);
    }

    set <State*> getNext (char letter) {
      return trans[letter];
    }

    const map <char, set <State*> >& getAll() const {
      return trans;
    }

    /**
     * Assumes all states have been numerated in ID map.
     */
    vector <pair <int, pair <char, int> > > getStatic(
        const map <const State*, int>& ID) const {
      vector <pair <int, pair <char, int> > > ret;
      int thisId = ID.find(this)->second;

      for(map <char, set<State*> >::const_iterator transIt = trans.begin();
          transIt != trans.end();
          transIt++) {
        for(set <State*>::const_iterator stateIt = transIt->second.begin();
            stateIt != transIt->second.end();
            stateIt++) {
          ret.push_back(make_pair(
                thisId,
                make_pair(transIt->first,ID.find(*stateIt)->second)));
        }
      }
      return ret;
    }

  private:
    map <char, set <State*> > trans;
};

/**
 * This class only keeps pointers to start state and to end state.
 * It provides convinient ways to build Enka, through operators.
 *
 */
class Enka {
  public:
    const static char EPS = 0; //  value used for eps transitions

    /**
     * Constructs an automata that doesn't accepts anything.
     */
    Enka();

    /**
     * Constructs an automata that only accepts the letter 'letter'.
     */
    Enka(char letter);

    /**
     * Constructs an automata from a previously serialized Enka.
     */
    Enka(string serializedEnka);

    /**
     * Reads serialized enka from that file decriptor.
     */
    Enka(FILE* input);
    /**
     * Evaluates if a string is accepted by this automata.
     * @return  True if word is accepted
     *          False otherwise
     */
    bool evaluate(string word);

    /**
     * Returns the length of a largest prefix of word that matches this
     * automata.
     * -1 if nothing is matched.
     */
    int largestMatch(const char* word);

    /**
     * Dumps the whole automata to a string.
     * new Enka(A.serialize()) is equivalent to A.
     */
    string serialize();

    /**
     * Creates automata that accepts left | right (or).
     */
    Enka operator +(const Enka& right);

    /**
     * Concatenation operator.
     * Creates automata that accepts left.right
     * (concatenation of left and right).
     */
    Enka operator -(const Enka& right);

    /**
     * Kleen operator A*.
     */
    Enka operator ~();

    State* startState;
    State* endState; //  endState is always considered acceptable


  private:
    set <State*> currStates;

    Enka(State* s, State* v) {
      startState = s;
      endState = v;
    }

    void makeTransition(char letter);
    void addEpsClosure();
    bool startEval();
    void fillIds(
        vector <pair <int, pair <char, int> > >& allTrans,
        map <const State*, int>& ID,
        int& nextId,
        const State* curr);
    void initFromString(string serializedEnka);
};

#endif
