#include <stack>

#include "misc.h"
#include "lexem.h"
#include "deserijalizacija.h"
#include "akcija.h"

using namespace std;

map< pair< int, string >, akcija > Akcija;
map< pair< int, string >, int > Goto;
int startState;
set< string > Syn;

set< string > getValidValues(int state) {
 set< string > ret;
  for (
    map< pair< int, string >, akcija >::iterator
    it = Akcija.begin();
    it != Akcija.end();
    ++it
  ) {
    if (it->second.getType() != ODBACI && it->first.first == state) {
      ret.insert(it->first.second);
    }
  }
  return ret;
}

int main(void) {
  map< pair< int, string >, akcija >::iterator rec;
  set< string > oporavak;
  vector< string > ulaz;
  string value;

  lexem_t root, unutrasnji;

  stack< int > stog_s;
  stack< lexem_t > lex;

  // get needed tables
  ifstream fajl;
  fajl.open(lr_parser_table, ifstream::in);
  loadStartState(startState, fajl);
  loadActionTable(Akcija, fajl);
  loadGotoTable(Goto, fajl);
  loadSynTokens(Syn, fajl);
  fajl.close();
  // done

  stog_s.push(startState);
  lex.push(lexem_t("dummy"));

  int lineNumber = 0;
  int run_forrest_run = 1;
  int read_next = 1;
  int oporavi;


  while(run_forrest_run == 1) {
    if (read_next) {
      if (!readLine(value)) {
        value = krajNiza;
      } else {
        lineNumber++;
      }
      ulaz = split(value, " ");
    }

    string curr_lex = ulaz[0];
    int top_state = stog_s.top();

    int stanje, iteracija;
    string A;

    akcija curr_akcija =
      Akcija[make_pair(top_state, curr_lex)];

    switch(curr_akcija.getType()) {
      case POMAK:
        stanje = curr_akcija.getState();

        stog_s.push(stanje);
        lex.push(lexem_t(value));

        read_next = 1;
        break;
      case REDUCIRAJ:
        iteracija = curr_akcija.getValue();
        A = curr_akcija.getLHS();

        unutrasnji = lexem_t(A);

        if (iteracija == 0) {
          unutrasnji.addChild(lexem_t("$"));
        }

        while (iteracija--) {
          stog_s.pop();
          unutrasnji.addChild(lex.top());
          lex.pop();
        }

        top_state = stog_s.top();
        stog_s.push(Goto[make_pair(top_state, A)]);
        lex.push(unutrasnji);

        root = unutrasnji;

        read_next = 0;
        break;
      case PRIHVATI:
        run_forrest_run = 0;
        break;
      default:
        cerr << "Dogodila se greska!" << endl;
        cerr << "1. Broj retka: " << lineNumber << endl;
        // nadji uniformne znakove koji ne bi uzrokovali pogresku
        // zapisi posotje u Akcija[top_state, X]
        oporavak = getValidValues(top_state);
        cerr << "2. Ocekivani znak: " << collapseSet(oporavak, ", ") << endl;
        cerr << "3. Procitani uniformi znak: " << value << endl;

        run_forrest_run = 0;
        read_next = 0;
        oporavi = 0;
        // try to recover by reading tokens until the next syn token

        while (1) {
          if (oporavi) {
            if (!readLine(value)) { break; }
            ulaz = split(value, " ");
            ++lineNumber;
          } else {
            oporavi = 1;
          }

          if (Syn.count(ulaz[0]) > 0) {
            while (!stog_s.empty()) {
              top_state = stog_s.top();
              if ((rec = Akcija.find(make_pair(top_state, ulaz[0]))) != Akcija.end() && rec->second.getType() != ODBACI) {
                run_forrest_run = 1;
                root = lex.top();
                break;
              }
              stog_s.pop();
              lex.pop();
            }

            break;
          }
        }

        break;
    }
  }

  dfs(root);

  return 0;
}
