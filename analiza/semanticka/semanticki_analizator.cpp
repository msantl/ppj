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

bool provjeriFunkciju(g_tree* root, const tip* funkcija, string name) {
  bool ret = false;

  if (root->tablica.find(name) != root->tablica.end()) {
    if (root->tablica[name].type == funkcija &&
        root->tablica[name].status == 0) {
      ret = true;
    }
  }

  for (
    vector< g_tree* >::iterator
    it = root->children.begin();
    it != root->children.end();
    ++it
  ) {
    ret |= provjeriFunkciju(*it, funkcija, name);
  }

  return ret;
}

bool checkRecursive(g_tree* root) {
  bool ret = true;

  for (
    map< string, lex >::iterator
    it = root->tablica.begin();
    it != root->tablica.end();
    ++it
  ) {
    if (it->second.status == 1) {
      g_tree* curr = root;
      bool temp = false;
      // trazimo name = it->first, tip = it->second.type
      while (curr != NULL) {
        if (curr->tablica.find(it->first) != curr->tablica.end()) {
          if (curr->tablica[it->first].status == 0 &&
              curr->tablica[it->first].type == it->second.type
          ) {
            temp = true;
            break;
          }
        }
        curr = curr->parent;
      }
      ret &= temp;
    }
  }

  for (
    vector< g_tree* >::iterator
    it = root->children.begin();
    it != root->children.end();
    ++it
  ) {
    ret &= checkRecursive(*it);
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

  // start analyzing from the root
  root->provjeri();

  const tip* ret = tip_int::instance();
  vector< const tip* > params;  params.push_back(tip_void::instance());
  const tip* funkcijaMain = tip_funkcija::instance(ret, params);

  if (provjeriFunkciju(root, funkcijaMain, "main") == 0) {
    printf("main\n");
  } else if (checkRecursive(root) == 0){
    printf("funkcija\n");
  }

  return 0;
}
