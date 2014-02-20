#include "tip.h"

using namespace std;

const tip* tip_int::instance(void) {
  static const tip* type = NULL;
  if (type == NULL) {
    type = new tip_int;
  }
  return type;
}

const tip* tip_const_int::instance(void) {
  static const tip* type = NULL;
  if (type == NULL) {
    type = new tip_const_int;
  }
  return type;
}

const tip* tip_char::instance(void) {
  static const tip* type = NULL;
  if (type == NULL) {
    type = new tip_char;
  }
  return type;
}

const tip* tip_const_char::instance(void) {
  static const tip* type = NULL;
  if (type == NULL) {
    type = new tip_const_char;
  }
  return type;
}

const tip* tip_void::instance(void) {
  static const tip* type = NULL;
  if (type == NULL) {
    type = new tip_void;
  }
  return type;
}

const tip* tip_niz::instance(const tip* param) {
  static map< const tip*, const tip* > dict;
  if (dict.find(param) == dict.end()) {
    dict[param] = new tip_niz;
  }
  return dict[param];
}


map< const tip*, pair< const tip*, vector< const tip* > > > lookup;

const tip* tip_funkcija::instance(const tip* ret, std::vector< const tip* > params) {
  static map< pair< const tip*, vector< const tip* > >, const tip* > dict;
  if (dict.find(make_pair(ret, params)) == dict.end()) {
    dict[make_pair(ret, params)] = new tip_funkcija;

    lookup[dict[make_pair(ret, params)]] = make_pair(ret, params);
  }

  return dict[make_pair(ret, params)];
}


void print_type_info(void) {
  cout << "NIZ INT " << tip_niz::instance(tip_int::instance()) << endl;
  cout << "NIZ CHAR" << tip_niz::instance(tip_char::instance()) << endl;
  cout << "NIZ CONST INT " << tip_niz::instance(tip_const_int::instance()) << endl;
  cout << "NIZ CONST CHAR " << tip_niz::instance(tip_const_char::instance()) << endl;
  cout << "INT " << tip_int::instance() << endl;
  cout << "CHAR " << tip_char::instance() << endl;
  cout << "CONST INT " << tip_const_int::instance() << endl;
  cout << "CONST CHAR " << tip_const_char::instance() << endl;
}
