#include "serijalizacija.h"

using namespace std;

void saveActionTable(map< pair< int, string >, akcija > a) {
  FILE *f = fopen(lr_parser_table, "a+");
  fprintf(f, "ACTION\n");

  for (
    map< pair< int, string >, akcija >::iterator
    mit = a.begin();
    mit != a.end();
    ++mit
  ) {
    akcija temp = mit->second;

    switch(temp.getType()) {
      case POMAK:
        fprintf(f, "POMAK\n");
        break;
      case REDUCIRAJ:
      fprintf(f, "REDUCIRAJ\n");
        break;
      case PRIHVATI:
        fprintf(f, "PRIHVATI\n");
        break;
    }

    if (temp.getType() == POMAK)
      fprintf(f, "STATE %d\n", temp.getState());
    if (temp.getType() == REDUCIRAJ)
      fprintf(f, "VALUE %d\n", temp.getValue());
    if (temp.getType() == REDUCIRAJ)
      fprintf(f, "LHS %s\n", temp.getLHS().c_str());

    fprintf(
      f,
      "POSITION %d %s\n",
      mit->first.first,
      mit->first.second.c_str()
    );

  }
  fprintf(f, "KRAJ\n");
  fclose(f);
}

void saveGotoTable(map< pair< int, string >, int > g) {
  FILE *f = fopen(lr_parser_table, "a+");
  fprintf(f, "GOTO\n");

  for (
    map< pair< int, string >, int >::iterator
    mit = g.begin();
    mit != g.end();
    ++mit
  ) {
    fprintf(
      f,
      "POSITION %d %s %d\n",
      mit->first.first,
      mit->first.second.c_str(),
      mit->second
    );
  }
  fprintf(f, "KRAJ\n");
  fclose(f);
}

void saveStartState(int s) {
  FILE *f = fopen(lr_parser_table, "a+");
  fprintf(f, "START %d\n", s);
  fclose(f);
}

void saveTokens(string tokens) {
  FILE *f = fopen(lr_parser_table, "a+");
  string znakovi;
  fprintf(f, "%s\n", tokens.c_str());
  fclose(f);
}

