#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <cstdio>
#include <cstdlib>

#include "tip.h"
#include "misc.h"
#include "produkcije.h"

using namespace std;

stablo* create_tree(void) {
  map< int, stablo*  > level;
  pair< string, int > ulaz;

  // handle the first node which will be <prijevodna_jedinica>
  ulaz = read_line();
  level[ulaz.second] = new stablo(ulaz.first);
  int root_level = ulaz.second;

  while(1) {
    ulaz = read_line();
    if (ulaz.second == -1) break;

    stablo* nova = new stablo(ulaz.first);

    level[ulaz.second - 1]->children.push_back(nova);

    level[ulaz.second] = nova;
  }

  return level[root_level];
}

g_tree* dfs(stablo* curr, int depth = 0) {
  g_tree* ret;

  if (is_terminal(curr->lhs) == 0) {
    // remove the < and > from left-hand-side non-terminal
    string prod = strip_signs(curr->lhs) + "_";

    vector< string > IDN;
    string uniformni_znak;

    for (
      vector< stablo* >::iterator
      it = curr->children.begin();
      it != curr->children.end();
      ++it
    ) {
      // construct the production string
      string alfa = (*it)->lhs;

      if (is_terminal(alfa)) {
        // use only the uniform sign from the terminals
        IDN = split(alfa, " ");
        uniformni_znak = IDN[0];

        prod += "_" + uniformni_znak;
      } else {
        // remove the < and > from non-terminals
        prod += "_" + strip_signs(alfa);
      }
    }

    // save the production to the node
    ret = produkcija(prod);
    ret->prod = prod;

    // do the same for others non-terminals
    for (
      vector< stablo* >::iterator
      it = curr->children.begin();
      it != curr->children.end();
      ++it
    ) {
      g_tree* child = dfs(*it, depth + 1);
      child->parent = ret;
      ret->children.push_back(child);
    }
  } else {
    // dealing with terminals here
    ret = new list_stabla;
    ret->prod = curr->lhs;
  }

  return ret;
}

int main(int argc, char **argv) {
  // create the helper tree
  stablo *korijen = create_tree();

  // create the man tree
  g_tree* root = dfs(korijen);

  // delete the helper tree
  delete korijen;

  // prepare the main tree
  root->parent = NULL;
  root->oznaka_globalnog_djelokruga = true;

  // root->DEBUG();
  // print_type_info();

  // common parts
  FILE *konstante = fopen("a_const.frisc", "w");

  // memorijska lokacija za spremanje raznih stvari
  fprintf(konstante, "KAKAC\t\t\t`DS 4\n");

  fclose(konstante);

  FILE *frisc = fopen("a.frisc", "w");

  fprintf(frisc, "PPJ\t\t\t`BASE D\n");
  fprintf(frisc, "\t\t\tMOVE %%H 40000, R7\n");
  fprintf(frisc, "\t\t\tCALL F_MAIN\n");
  fprintf(frisc, "\t\t\tHALT\n");

  fprintf(frisc, "MODULO\t\t\tPUSH R0\n");
  fprintf(frisc, "\t\t\tPUSH R1\n");
  fprintf(frisc, "\t\t\tLOAD R1, (R7+12)\n");
  fprintf(frisc, "\t\t\tLOAD R0, (R7+16)\n");
  fprintf(frisc, "M_LOOP\t\t\tCMP R0, R1\n");
  fprintf(frisc, "\t\t\tJR_SLT M_RET\n");
  fprintf(frisc, "\t\t\tSUB R0, R1, R0\n");
  fprintf(frisc, "\t\t\tJR M_LOOP\n");
  fprintf(frisc, "M_RET\t\t\tMOVE R0, R6\n");
  fprintf(frisc, "\t\t\tPOP R1\n");
  fprintf(frisc, "\t\t\tPOP R0\n");
  fprintf(frisc, "\t\t\tRET\n");

  fprintf(frisc, "MNOZI\t\t\tPUSH R0\n");
  fprintf(frisc, "\t\t\tPUSH R1\n");
  fprintf(frisc, "\t\t\tPUSH R2\n");
  fprintf(frisc, "\t\t\tLOAD R1, (R7+16)\n");
  fprintf(frisc, "\t\t\tLOAD R0, (R7+20)\n");
  fprintf(frisc, "\t\t\tMOVE 0, R6\n");
  fprintf(frisc, "\t\t\tCMP R1, 0\n");
  fprintf(frisc, "\t\t\tJR_SLT P_NEG\n");
  fprintf(frisc, "P_POZ\t\t\tMOVE 1, R2\n");
  fprintf(frisc, "\t\t\tJR P_LOOP\n");
  fprintf(frisc, "P_NEG\t\t\tMOVE -1, R2\n");
  fprintf(frisc, "\t\t\tXOR R2, R0, R0\n");
  fprintf(frisc, "\t\t\tADD R0, 1, R0\n");
  fprintf(frisc, "P_LOOP\t\t\tCMP R1, 0\n");
  fprintf(frisc, "\t\t\tJR_EQ P_RET\n");
  fprintf(frisc, "\t\t\tADD R6, R0, R6\n");
  fprintf(frisc, "\t\t\tSUB R1, R2, R1\n");
  fprintf(frisc, "\t\t\tJR P_LOOP\n");
  fprintf(frisc, "P_RET\t\t\tPOP R2\n");
  fprintf(frisc, "\t\t\tPOP R1\n");
  fprintf(frisc, "\t\t\tPOP R0\n");
  fprintf(frisc, "\t\t\tRET\n");

  fprintf(frisc, "DIJELI\t\t\tPUSH R0\n");
  fprintf(frisc, "\t\t\tPUSH R1\n");
  fprintf(frisc, "\t\t\tMOVE 0, R6\n");
  fprintf(frisc, "\t\t\tLOAD R1, (R7+12)\n");
  fprintf(frisc, "\t\t\tLOAD R0, (R7+16)\n");
  fprintf(frisc, "D_LOOP\t\t\tCMP R0, R1\n");
  fprintf(frisc, "\t\t\tJR_SLT D_RET\n");
  fprintf(frisc, "\t\t\tSUB R0, R1, R0\n");
  fprintf(frisc, "\t\t\tADD R6, 1, R6\n");
  fprintf(frisc, "\t\t\tJR D_LOOP\n");
  fprintf(frisc, "D_RET\t\t\tPOP R1\n");
  fprintf(frisc, "\t\t\tPOP R0\n");
  fprintf(frisc, "\t\t\tRET\n");
  fclose(frisc);

  // start analyzing from the root
  root->provjeri();

  // append constants on the end
  konstante = fopen("a_const.frisc", "r");
  frisc = fopen("a.frisc", "a");

  char c;
  while ((c = fgetc(konstante)) != EOF) {
    fputc(c, frisc);
  }

  fclose(frisc);
  fclose(konstante);

  return 0;
}
