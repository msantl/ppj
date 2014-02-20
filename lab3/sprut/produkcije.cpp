#include <iostream>
#include <sstream>
#include <string>

#include "produkcije.h"
#include "misc.h"

using namespace std;

// leksicka jedinka
lex::lex(const tip* _type, bool _l_value, int _status) {
  type = _type;
  l_value = _l_value;
  status = _status;
}

// additional checks
bool eksplicitnaPromjena(const tip* U, const tip *V) {
  if (U == V) { return true; }
  // int->char, jedina eksplicitna dozvoljena promjena
  if (U == tip_int::instance() && V == tip_char::instance()) { return true; }
  if (U == tip_int::instance() && V == tip_const_char::instance()) { return true; }

  if (U == tip_const_int::instance() && V == tip_const_char::instance()) { return true; }

  // implicitne promjene, prosirene tranzitivnoscu
  if (U == tip_const_int::instance() && V == tip_int::instance()) { return true; }
  if (U == tip_const_char::instance() && V == tip_char::instance()) { return true; }
  if (U == tip_int::instance() && V == tip_const_int::instance()) { return true; }
  // tranzitivno int->char->const_char
  if (U == tip_int::instance() && V == tip_const_char::instance()) { return true; }

  if (U == tip_char::instance() && V == tip_const_char::instance()) { return true; }
  if (U == tip_char::instance() && V == tip_int::instance()) { return true; }

  // tranzitivno char->int->const int
  if (U == tip_char::instance() && V == tip_const_int::instance()) { return true; }
  if (U == tip_const_char::instance() && V == tip_const_int::instance()) { return true; }

  if (U == tip_niz::instance(tip_int::instance()) &&
      V == tip_niz::instance(tip_const_int::instance())) { return true; }

  if (U == tip_niz::instance(tip_char::instance()) &&
      V == tip_niz::instance(tip_const_char::instance())) { return true; }

  return false;
}
bool implicitnaPromjena(const tip* U, const tip *V) {
  if (U == V) { return true; }
  if (U == tip_const_int::instance() && V == tip_int::instance()) { return true; }
  if (U == tip_const_char::instance() && V == tip_char::instance()) { return true; }
  if (U == tip_int::instance() && V == tip_const_int::instance()) { return true; }
  if (U == tip_char::instance() && V == tip_const_char::instance()) { return true; }
  if (U == tip_char::instance() && V == tip_int::instance()) { return true; }

  // tranzitivno char->int->const int
  if (U == tip_char::instance() && V == tip_const_int::instance()) { return true; }
  if (U == tip_const_char::instance() && V == tip_const_int::instance()) { return true; }

  if (U == tip_niz::instance(tip_int::instance()) &&
      V == tip_niz::instance(tip_const_int::instance())) { return true; }

  if (U == tip_niz::instance(tip_char::instance()) &&
      V == tip_niz::instance(tip_const_char::instance())) { return true; }

  return false;
}
// izvdvoji uniformni znak
string uniformni_znak(string a) {
  vector< string > v = split(a, " ");
  return v[0];
}
// izdvoji redak
string redak(string a) {
  vector< string > v = split(a, " ");
  return v[1];
}
// izdvoji leksicku jedinku
string vrijednost(string a) {
  vector< string > v = split(a, " ");
  return v[2];
}
bool checkInt(string leksicka_jedinka) {
  string a = vrijednost(leksicka_jedinka);
  long long int val = string2int(a);
  if (val < -2147483648L || val > 2147483647L) return 0;
  return 1;
}
bool checkChar(string leksicka_jedinka) {
  string a = vrijednost(leksicka_jedinka);
  // pocinje i zavrsava sa jednostrukim
  if (a[0] != '\'' || a[a.length() - 1] != '\'') return 0 ;
  a = strip_signs(a);

  if (a.length() == 1) return 1;
  if (a == "\\t") return 1;
  if (a == "\\n") return 1;
  if (a == "\\0") return 1;
  if (a == "\\\'") return 1;
  if (a == "\\\"") return 1;
  if (a == "\\\\") return 1;
  return false;
}
bool checkCharArray(string leksicka_jedinka) {
  string a = vrijednost(leksicka_jedinka);
  // pocinje i zavrsava sa dvostrukim
  if (a[0] != '\"' || a[a.length() - 1] != '\"') return 0 ;
  a = strip_signs(a);

  for (int i = 0; i < (int)a.length(); ++i) {
    if (a[i] == '\\' && i + 1 < (int)a.length()) {
      i += 1;
      if (a[i] == 't') continue;
      if (a[i] == 'n') continue;
      if (a[i] == '0') continue;
      if (a[i] == '\'') continue;
      if (a[i] == '\"') continue;
      if (a[i] == '\\') continue;
      return 0;
    } else if (a[i] == '\\') {
      // nedozvoljeni znak \ ,
      return 0;
    } else {
      if (!isprint(a[i])) return 0;
    }
  }
  return 1;
}

// production stuff
g_tree::~g_tree() {
  for (
    vector< g_tree* >::iterator
    it = children.begin();
    it != children.end();
    ++it
  ) {
    delete (*it);
  }
}

void g_tree::DEBUG(int depth) {
  for (int i = 0; i < depth; ++i) cout << " ";
  cout << prod << endl;

  for (
    vector< g_tree* >::iterator
    it = children.begin();
    it != children.end();
    ++it
  ) {
    (*it)->DEBUG(depth + 1);
  }
}

const tip* have_type::get_type(void) {
  return type;
}

bool have_l_value::get_l_value(void) {
  return l_value;
}

string have_name::get_name(void) {
  return name;
}

int have_count::get_count(void) {
  return count;
}

vector< const tip* > have_types::get_types(void) {
  return types;
}

vector< string > have_names::get_names(void) {
  return names;
}

// SEMANTICKA PRAVILA
// primarni izraz
void primarni_izraz__IDN::provjeri(void) {
  // tip <- IDN.tip
  // l-izraz <- IDN.l-izraz
  // 1. IDN.ime je deklarirano
  g_tree* curr = this;
  string name = vrijednost(children[0]->prod);
  bool nasao = false;

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      type = curr->tablica[name].type;
      l_value = curr->tablica[name].l_value;
      nasao = true;
      break;
    }
    curr = curr->parent;
  }

  if (nasao == false) {
    printf(
      "<primarni_izraz> ::= IDN(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }

}
void primarni_izraz__BROJ::provjeri(void) {
  // tip <- int
  // l-izraz <- 0
  // 1. vrijednost je u rasponu tipa int
  if (!checkInt(children[0]->prod)) {
    printf(
      "<primarni_izraz> ::= BROJ(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }
  type = tip_int::instance();
  l_value = false;
}
void primarni_izraz__ZNAK::provjeri(void) {
  // tip <- char
  // l-izraz <- 0
  // 1. znak je ispravan (ASCII, \t, \n, \0, \' \" \\ ")
  if (!checkChar(children[0]->prod)) {
    printf(
      "<primarni_izraz> ::= ZNAK(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }
  type = tip_char::instance();
  l_value = false;
}
void primarni_izraz__NIZ_ZNAKOVA::provjeri(void) {
  // tip <- niz(const(char))
  // l_izraz <- 0
  // 1. niz je ispravan (ASCII, \t, \n, \0, \' \" \\ ")
  if (!checkCharArray(children[0]->prod)) {
    printf(
      "<primarni_izraz> ::= NIZ_ZNAKOVA(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);

  }
  type = tip_niz::instance(tip_const_char::instance());
  l_value = false;
}
void primarni_izraz__L_ZAGRADA_izraz_D_ZAGRADA::provjeri(void) {
  // tip <- izraz.tip
  // l_izraz <- izraz.l_izraz
  // 1. provjeri(<izraz>)
  children[1]->provjeri();

  type = children[1]->get_type();
  l_value = children[1]->get_l_value();
}

// postfiks izraz
void postfiks_izraz__primarni_izraz::provjeri(void) {
  // tip <- primarni_izraz.tip
  // l_izraz <- primarni_izraz.l_izraz
  // 1. provjeri(<primarni_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void postfiks_izraz__postfiks_izraz_L_UGL_ZAGRADA_izraz_D_UGL_ZAGRADA::provjeri(void) {
  // tip <- X
  // l_izraz <- (X != const(T))
  // 1. provjeri(<postfiks_izraz>)
  // 2. postfiks_izraz.tip = niz(X)
  // 3. provjeri(<izraz>)
  // 4. izraz.tip ~ int
  children[0]->provjeri();
  if (children[0]->get_type() != tip_niz::instance(tip_int::instance()) &&
      children[0]->get_type() != tip_niz::instance(tip_char::instance()) &&
      children[0]->get_type() != tip_niz::instance(tip_const_int::instance()) &&
      children[0]->get_type() != tip_niz::instance(tip_const_char::instance())
  ) {
    printf(
      "<postfiks_izraz> ::= <postfiks_izraz> L_UGL_ZAGRADA(%s,%s) <izraz> D_UGL_ZAGRADA(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<postfiks_izraz> ::= <postfiks_izraz> L_UGL_ZAGRADA(%s,%s) <izraz> D_UGL_ZAGRADA(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  if (children[0]->get_type() == tip_niz::instance(tip_int::instance())) {
    type = tip_int::instance();
    l_value = true;
  } else if (children[0]->get_type() == tip_niz::instance(tip_char::instance())) {
    type = tip_char::instance();
    l_value = true;
  } else if (children[0]->get_type() == tip_niz::instance(tip_const_int::instance())) {
    type = tip_const_int::instance();
    l_value = false;
  } else if (children[0]->get_type() == tip_niz::instance(tip_const_char::instance())) {
    type = tip_const_char::instance();
    l_value = false;
  }
}
void postfiks_izraz__postfiks_izraz_L_ZAGRADA_D_ZAGRADA::provjeri(void) {
  // tip <- povratna
  // l_izraz = 0
  // 1. provjeri(<postfiks_izraz>)
  // 2. postfiks_izraz.tip = funkcija(void -> povratna)
  children[0]->provjeri();

  pair< const tip*, vector< const tip* > > funkcija = lookup[children[0]->get_type()];


  if (funkcija.second.size() != 1 || funkcija.second[0] != tip_void::instance()  ) {
    printf(
      "<postfiks_izraz> ::= <postfiks_izraz> L_ZAGRADA(%s,%s) D_ZAGRADA(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str()
    );
    exit(0);
  }

  type = funkcija.first;
  l_value = false;
}
void postfiks_izraz__postfiks_izraz_L_ZAGRADA_lista_argumenata_D_ZAGRADA::provjeri(void) {
  // tip <- povratna
  // l_izraz <- 0
  // 1. provjeri(<postfiks_izraz>)
  // 2. provjeri(<lista_argumenata>)
  // 3. postfiks_izraz.tip = funkcija(parametri -> povratna)
  //    i redom po elementima lista_argumenata.tipovi vrijedi
  //    arg-tip ~ param~tip
  children[0]->provjeri();
  children[2]->provjeri();

  bool greska = false;
  vector< const tip* > dobiveni = children[2]->get_types();

  pair< const tip*, vector< const tip* > > funkcija = lookup[children[0]->get_type()];

  // children[0]->get_type je tip_funkcija::instance(ret, params)

  for (int i = 0; i < (int)dobiveni.size(); ++i) {
    if (implicitnaPromjena(dobiveni[i], funkcija.second[i]) == 0) {
      greska = true;
      break;
    }
  }

  if (greska == true) {
    printf(
      "<postfiks_izraz> ::= <postfiks_izraz> L_ZAGRADA(%s,%s) <lista_argumenata> D_ZAGRADA(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  type = funkcija.first;
  l_value = false;
}
void postfiks_izraz__postfiks_izraz_OP_INC::provjeri(void) {
  // tip <- int
  // l_izraz <- 0
  // 1. provjeri(<postfiks_izraz>)
  // 2. postfiks_izraz.l_value = 1
  //    i postfiks_izraz.tip ~ int
  children[0]->provjeri();
  if (children[0]->get_l_value() == false ||
      implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<postfiks_izraz> ::= <postfiks_izraz> OP_INC(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void postfiks_izraz__postfiks_izraz_OP_DEC::provjeri(void) {
  // tip <- int
  // l_izraz <- 0
  // 1. provjeri(<postfiks_izraz>)
  // 2. postfiks_izraz.l_value = 1
  //    i postfiks_izraz.tip ~ int
  children[0]->provjeri();
  if (children[0]->get_l_value() == false ||
      implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<postfiks_izraz> ::= <postfiks_izraz> OP_DEC(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// lista argumenata
void lista_argumenata__izraz_pridruzivanja::provjeri(void) {
  // tipovi <- [izraz_pridruzivanja.tip]
  // 1. provjeri(<izraz_pridruzivanja>)
  children[0]->provjeri();

  types.push_back(children[0]->get_type());
}
void lista_argumenata__lista_argumenata_ZAREZ_izraz_pridruzivanja::provjeri(void) {
  // tipovi <- lista_argumenata.tipovi + [izraz_pridruzivanja.tip]
  // 1. provjeri(<lista_argumenata>)
  // 2. provjeri(<izraz_pridruzivanja>)
  children[0]->provjeri();
  children[2]->provjeri();

  types = children[0]->get_types(); types.push_back(children[2]->get_type());
}

// unarni izraz
void unarni_izraz__postfiks_izraz::provjeri(void) {
  // tip <- postfiks_izraz.tip
  // l_value <- postfiks_izraz.l_value
  // 1. provjeri(<postfiks_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void unarni_izraz__OP_INC_unarni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<unarni_izraz>)
  // 2. unarni_izraz.l_vaue = 1
  //    unarni_izraz.tip ~ int
  children[1]->provjeri();
  if (children[1]->get_l_value() == false ||
      implicitnaPromjena(children[1]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<unarni_izraz> ::= OP_INC(%s,%s) <unarni_izraz>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void unarni_izraz__OP_DEC_unarni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<unarni_izraz>)
  // 2. unarni_izraz.l_vaue = 1
  //    unarni_izraz.tip ~ int
  children[1]->provjeri();
  if (children[1]->get_l_value() == false ||
      implicitnaPromjena(children[1]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<unarni_izraz> ::= OP_DEC(%s,%s) <unarni_izraz>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void unarni_izraz__unarni_operator_cast_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<cast_izraz>)
  // 2. cast_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance())) {
    printf(
      "<unarni_izraz> ::= <unarni_izraz> <cast_izraz>\n"
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// unarni operator
void unarni_operator__PLUS::provjeri(void) {
  // do nothing
}
void unarni_operator__MINUS::provjeri(void) {
  // do nothing
}
void unarni_operator__OP_TILDA::provjeri(void) {
  // do nothing
}
void unarni_operator__OP_NEG::provjeri(void) {
  // do nothing
}

// cast izraz
void cast_izraz__unarni_izraz::provjeri(void) {
  // tip <- unarni_izraz.tip
  // l_value <- unarni_izraz.l_value
  // 1. provjeri(<uanrni_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void cast_izraz__L_ZAGRADA_ime_tipa_D_ZAGRADA_cast_izraz::provjeri(void) {
  // tip <- ime_tipa.tip
  // l_value = 0
  // 1. provjeri(ime_tipa)
  // 2. provjeri(cast_izraz>)
  // 3. cast_izraz.tip se moze pretvoriti u ime_tipa.tip
  children[1]->provjeri();
  children[3]->provjeri();
  if (eksplicitnaPromjena(children[3]->get_type(), children[1]->get_type()) == 0) {

    printf(
      "<cast_izraz> ::= L_ZAGRADA(%s,%s) <ime_tipa> D_ZAGRADA(%s,%s) <cast_izraz>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str()
    );
    exit(0);
  }

  type = children[1]->get_type();
  l_value = false;
}

// ime tipa
void ime_tipa__specifikator_tipa::provjeri(void) {
  // tip <- specifikator_tipa.tip
  // 1. provjeri(<specifikator_tipa>)
  children[0]->provjeri();

  type = children[0]->get_type();
}
void ime_tipa__KR_CONST_specifikator_tipa::provjeri(void) {
  // tip <- const(specifikator_tipa.tip)
  // 1. provjeri(<specifikator_tipa>)
  // 2. specifikator_tipa.tip != void
  children[1]->provjeri();
  if (children[1]->get_type() == tip_void::instance()) {
    printf(
      "<ime_tipa> ::= KR_CONST(%s,%s) <specifikator_tipa>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }

  if (children[1]->get_type() == tip_int::instance()) {
    type = tip_const_int::instance();
  } else if (children[1]->get_type() == tip_char::instance()) {
    type = tip_const_char::instance();
  }
}

// specifikator tipa
void specifikator_tipa__KR_VOID::provjeri(void) {
  // tip <- void
  type = tip_void::instance();
}
void specifikator_tipa__KR_INT::provjeri(void) {
  // tip <- int
  type = tip_int::instance();
}
void specifikator_tipa__KR_CHAR::provjeri(void) {
  // tip <- char
  type = tip_char::instance();
}

// multiplikativni izraz
void multiplikativni_izraz__cast_izraz::provjeri(void) {
  // tip <- cast_izraz.tip
  // l_value <- cast_izraz.l_value
  // 1. provjeri(cast_izraz)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void multiplikativni_izraz__multiplikativni_izraz_OP_PUTA_cast_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<multiplikativni_izraz>)
  // 2. multiplikativni_izraz.tip ~ int
  // 3. provjeri(<cast_izraz>)
  // 4. cast_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<multiplikativni_izraz> ::= <multiplikativni_izraz> OP_PUTA(%s,%s) <cast_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<multiplikativni_izraz> ::= <multiplikativni_izraz> OP_PUTA(%s,%s) <cast_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void multiplikativni_izraz__multiplikativni_izraz_OP_DIJELI_cast_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<multiplikativni_izraz>)
  // 2. multiplikativni_izraz.tip ~ int
  // 3. provjeri(<cast_izraz>)
  // 4. cast_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<multiplikativni_izraz> ::= <multiplikativni_izraz> OP_DIJELI(%s,%s) <cast_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<multiplikativni_izraz> ::= <multiplikativni_izraz> OP_DIJELI(%s,%s) <cast_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void multiplikativni_izraz__multiplikativni_izraz_OP_MOD_cast_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<multiplikativni_izraz>)
  // 2. multiplikativni_izraz.tip ~ int
  // 3. provjeri(<cast_izraz>)
  // 4. cast_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<multiplikativni_izraz> ::= <multiplikativni_izraz> OP_MOD(%s,%s) <cast_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<multiplikativni_izraz> ::= <multiplikativni_izraz> OP_MOD(%s,%s) <cast_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// aditivni izraz
void aditivni_izraz__multiplikativni_izraz::provjeri(void) {
  // tip <- multiplikativni_izraz.tip
  // l_value <- multiplikativni_izraz.l_value
  // 1. provjeri(<multiplikativni_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void aditivni_izraz__aditivni_izraz_PLUS_multiplikativni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<aditivni_izraz>)
  // 2. aditivni_izraz.tip ~ int
  // 3. provjeri(<multiplikativni_izraz>)
  // 4. multiplikativni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<aditivni_izraz> ::= <aditivni_izraz> PLUS(%s,%s) <multiplikativni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<aditivni_izraz> ::= <aditivni_izraz> PLUS(%s,%s) <multiplikativni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void aditivni_izraz__aditivni_izraz_MINUS_multiplikativni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<aditivni_izraz>)
  // 2. aditivni_izraz.tip ~ int
  // 3. provjeri(<multiplikativni_izraz>)
  // 4. multiplikativni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<aditivni_izraz> ::= <aditivni_izraz> MINUS(%s,%s) <multiplikativni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<aditivni_izraz> ::= <aditivni_izraz> MINUS(%s,%s) <multiplikativni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// odnosni izraz
void odnosni_izraz__aditivni_izraz::provjeri(void) {
  // tip <- aditivni_izraz.tip
  // l_value <- aditivni_izraz.l_value
  // 1. provjeri(<aditivni_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void odnosni_izraz__odnosni_izraz_OP_LT_aditivni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<odnosni_izraz>)
  // 2. odnosni_izraz.tip ~ int
  // 3. provjeri(<aditivni_izraz>)
  // 4. aditivni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_LT(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_LT(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void odnosni_izraz__odnosni_izraz_OP_GT_aditivni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<odnosni_izraz>)
  // 2. odnosni_izraz.tip ~ int
  // 3. provjeri(<aditivni_izraz>)
  // 4. aditivni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_GT(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_GT(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void odnosni_izraz__odnosni_izraz_OP_LTE_aditivni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<odnosni_izraz>)
  // 2. odnosni_izraz.tip ~ int
  // 3. provjeri(<aditivni_izraz>)
  // 4. aditivni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_LTE(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_LTE(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void odnosni_izraz__odnosni_izraz_OP_GTE_aditivni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<odnosni_izraz>)
  // 2. odnosni_izraz.tip ~ int
  // 3. provjeri(<aditivni_izraz>)
  // 4. aditivni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_GTE(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<odnosni_izraz> ::= <odnosni_izraz> OP_GTE(%s,%s) <aditivni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// jednakosni izraz
void jednakosni_izraz__odnosni_izraz::provjeri(void) {
  // tip <- odnosni_izraz.tip
  // l_value <- odnosni_izraz.l_value
  // 1. provjeri(<odnosni_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void jednakosni_izraz__jednakosni_izraz_OP_EQ_odnosni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<jednakosni_izraz>)
  // 2. jednakosni_izraz.tip ~ int
  // 3. provjeri(<odnosni_izraz>)
  // 4. odnosni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<jednakosni_izraz> ::= <jednakosni_izraz> OP_EQ(%s,%s) <odnosni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<jednakosni_izraz> ::= <jednakosni_izraz> OP_EQ(%s,%s) <odnosni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}
void jednakosni_izraz__jednakosni_izraz_OP_NEQ_odnosni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<jednakosni_izraz>)
  // 2. jednakosni_izraz.tip ~ int
  // 3. provjeri(<odnosni_izraz>)
  // 4. odnosni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<jednakosni_izraz> ::= <jednakosni_izraz> OP_NEQ(%s,%s) <odnosni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<jednakosni_izraz> ::= <jednakosni_izraz> OP_NEQ(%s,%s) <odnosni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// bin i izraz
void bin_i_izraz__jednakosni_izraz::provjeri(void) {
  // tip <- jednakosni_izraz.tip
  // l_izraz <- jednakosni_izraz.l_value
  // 1. provjeri(<jednakosni_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void bin_i_izraz__bin_i_izraz_OP_BIN_I_jednakosni_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<bin_i_izraz>)
  // 2. bin_i_izraz.tip ~ int
  // 3. provjeri(<jednakosni_izraz>)
  // 4. jednakosni_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<bin_i_izraz> ::= <bin_i_izraz> OP_BIN_I(%s,%s) <jednakosni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<bin_i_izraz> ::= <bin_i_izraz> OP_BIN_I(%s,%s) <jednakosni_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// bin xili izraz
void bin_xili_izraz__bin_i_izraz::provjeri(void) {
  // tip <- bin_i_izraz.tip
  // l_value <- bin_i_izraz.l_value
  // 1. provjeri(<bin_i_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void bin_xili_izraz__bin_xili_izraz_OP_BIN_XILI_bin_i_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<bin_xili_izraz>)
  // 2. bin_xili_izraz.tip ~ int
  // 3. provjeri(<bin_i_izraz>)
  // 4. bin_i_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<bin_xili_izraz> ::= <bin_xili_izraz> OP_BIN_XILI(%s,%s) <bin_i_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<bin_xili_izraz> ::= <bin_xili_izraz> OP_BIN_XILI(%s,%s) <bin_i_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// bin ili izraz
void bin_ili_izraz__bin_xili_izraz::provjeri(void) {
  // tip <- bin_xili_izraz.tip
  // l_value <- bin_xili_izraz.l_value
  // 1. provjeri(<bin_xili_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void bin_ili_izraz__bin_ili_izraz_OP_BIN_ILI_bin_xili_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<bin_ili_izraz>)
  // 2. bin_ili_izraz.tip ~ int
  // 3. provjeri(<bili_xili_izraz>)
  // 4. bin_xili_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<bin_ili_izraz> ::= <bin_ili_izraz> OP_BIN_ILI(%s,%s) <bin_xili_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<bin_ili_izraz> ::= <bin_ili_izraz> OP_BIN_ILI(%s,%s) <bin_xili_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// log i izraz
void log_i_izraz__bin_ili_izraz::provjeri(void) {
  // tip <- bin_ili_izraz.tip
  // l_value <- bin_ili_izraz.l_izraz
  // 1. provjeri(<bin_ili_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void log_i_izraz__log_i_izraz_OP_I_bin_ili_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<log_i_izraz>)
  // 2. log_i_izraz.tip ~ int
  // 3. provjeri(<bin_ili_izraz>)
  // 4. bin_ili_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<log_i_izraz> ::= <log_i_izraz> OP_I(%s,%s) <bin_ili_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<log_i_izraz> ::= <log_i_izraz> OP_I(%s,%s) <bin_ili_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// log ili izraz
void log_ili_izraz__log_i_izraz::provjeri(void) {
  // tip <- log_i_izraz.tip
  // l_value <- log_i_izraz.l_value
  // 1. provjeri(<log_i_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void log_ili_izraz__log_ili_izraz_OP_ILI_log_i_izraz::provjeri(void) {
  // tip <- int
  // l_value <- 0
  // 1. provjeri(<log_ili_izraz>)
  // 2. log_ili_izraz.tip ~ int
  // 3. provjeri(<log_i_izraz>)
  // 4. log_i_izraz.tip ~ int
  children[0]->provjeri();
  if (implicitnaPromjena(children[0]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<log_ili_izraz> ::= <log_ili_izraz> OP_ILI(%s,%s) <log_i_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<log_ili_izraz> ::= <log_ili_izraz> OP_ILI(%s,%s) <log_i_izraz>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = tip_int::instance();
  l_value = false;
}

// izraz_pridruzivanja
void izraz_pridruzivanja__log_ili_izraz::provjeri(void) {
  // tip <- log_ili_izraz.tip
  // l_value <- log_ili_izraz.l_value
  // 1. provjeri(<log_ili_izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void izraz_pridruzivanja__postfiks_izraz_OP_PRIDRUZI_izraz_pridruzivanja::provjeri(void) {
  // tip <- postfiks_izraz.tip
  // l_value <- 0
  // 1. provjeri(<postfiks_izraz>)
  // 2. postfiks_izraz.l_value = 1
  // 3. provjeri(<izraz_pridruzivanja>)
  // 4. izraz_pridruzivanja.tip ~ postfiks_izraz.tip
  children[0]->provjeri();
  if (children[0]->get_l_value() != 1) {
    printf(
      "<izraz_pridruzivanja> ::= <postfiks_izraz> OP_PRIDRUZI(%s,%s) <izraz_pridruzivanja>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), children[0]->get_type()) == 0) {
    printf(
      "<izraz_pridruzivanja> ::= <postfiks_izraz> OP_PRIDRUZI(%s,%s) <izraz_pridruzivanja>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = children[0]->get_type();
  l_value = false;
}

// izraz
void izraz__izraz_pridruzivanja::provjeri(void) {
  // tip <- izraz_pridruzivanja.tip
  // l_value <- izraz_pridruzivanja.l_value
  // 1. provjeri(<izraz_pridruzivanja>)
  children[0]->provjeri();

  type = children[0]->get_type();
  l_value = children[0]->get_l_value();
}
void izraz__izraz_ZAREZ_izraz_pridruzivanja::provjeri(void) {
  // tip <- izraz_pridruzivanja.tip
  // l_value <- 0
  // 1. provjeri(<izraz>)
  // 2 .provjeri(<izraz_pridruzivanja>)
  children[0]->provjeri();
  children[2]->provjeri();

  type = children[2]->get_type();
  l_value = false;
}

// slozena naredba
void slozena_naredba__L_VIT_ZAGRADA_lista_naredbi_D_VIT_ZAGRADA::provjeri(void) {
  // 1.provjer(<lista_naredbi>)
  children[1]->provjeri();
}
void slozena_naredba__L_VIT_ZAGRADA_lista_deklaracija_lista_naredbi_D_VIT_ZAGRADA::provjeri(void) {
  // 1.provjeri(<lista_deklaracija>)
  // 2.provjeri(<lista_naredbi>)
  children[1]->provjeri();
  children[2]->provjeri();
}

// lista naredbni
void lista_naredbi__naredba::provjeri(void) {
  // 1.provjeri(<naredba>)
  children[0]->provjeri();
}
void lista_naredbi__lista_naredbi_naredba::provjeri(void) {
  // 1.provjeri(<lista_naredbi>)
  // 2.provjeri(<naredba>)
  children[0]->provjeri();
  children[1]->provjeri();
}

// naredba
void naredba__slozena_naredba::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}
void naredba__izraz_naredba::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}
void naredba__naredba_grananja::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}
void naredba__naredba_petlje::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}
void naredba__naredba_skoka::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}

// izraz_naredba
void izraz_naredba__TOCKAZAREZ::provjeri(void) {
  // tip <- int
  type = tip_int::instance();
}
void izraz_naredba__izraz_TOCKAZAREZ::provjeri(void) {
  // tip <- izraz.tip
  // 1. provjeri(<izraz>)
  children[0]->provjeri();

  type = children[0]->get_type();
}

// naredba grananja
void naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba::provjeri(void) {
  // 1.provjeri(<izraz>)
  // 2.izraz.tip ~ int
  // 3.provjeri(<naredba>)
  children[2]->provjeri();

  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<naredba_grananja> ::= KR_IF(%s,%s) L_ZAGRADA(%s,%s) <izraz> D_ZAGRADA(%s,%s) <naredba>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }
  children[4]->provjeri();
}
void naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba_KR_ELSE_naredba::provjeri(void) {
  // 1.provjeri(<izraz>)
  // 2.izraz.tip ~ int
  // 3.provjeri(<naredba1>)
  // 4.provjeri(<naredba2>)
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
     printf(
      "<naredba_grananja> ::= KR_IF(%s,%s) L_ZAGRADA(%s,%s) <izraz> D_ZAGRADA(%s,%s) <naredba> KR_ELSE(%s,%s) <naredba>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str(),
      redak(children[5]->prod).c_str(),
      vrijednost(children[5]->prod).c_str()
    );
    exit(0);
  }
  children[4]->provjeri();
  children[6]->provjeri();
}

// naredba petlja
void naredba_petlje__KR_WHILE_L_ZAGRADA_izraz_D_ZAGRADA_naredba::provjeri(void) {
  // 1.provjeri(<izraz>)
  // 2.izraz.tip ~ int
  // 3.provjeri(<naredba>)
  children[2]->provjeri();
  if (implicitnaPromjena(children[2]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<naredba_petlje> ::= KR_WHILE(%s,%s) L_ZAGRADA(%s,%s) <izraz> D_ZAGRADA(%s,%s) <naredba>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }
  children[4]->provjeri();
}
void naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_D_ZAGRADA_naredba::provjeri(void) {
  //1.provjeri(<izraz_naredba1>)
  //2.provjeri(<izraz_naredba2>)
  //3.izraz_naredba2.tip ~ int
  //4.provjeri(<naredba>)
  children[2]->provjeri();
  children[3]->provjeri();
  if (implicitnaPromjena(children[3]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<naredba_petlje> ::= KR_FOR(%s,%s) L_ZAGRADA(%s,%s) <izraz_naredba> <izraz_naredba> D_ZAGRADA(%s,%s) <naredba>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[4]->prod).c_str(),
      vrijednost(children[4]->prod).c_str()
    );
    exit(0);
  }
  children[5]->provjeri();
}
void naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_izraz_D_ZAGRADA_naredba::provjeri(void) {
  //1.provjeri(<izraz_naredba1>)
  //2.provjeri(<izraz_naredba2>)
  //3.izraz_naredba2.tip ~ int
  //4.provjeri(<izraz>)
  //5.provjeri(<naredba>)
  children[2]->provjeri();
  children[3]->provjeri();
  if (implicitnaPromjena(children[3]->get_type(), tip_int::instance()) == 0) {
    printf(
      "<naredba_petlje> ::= KR_FOR(%s,%s) L_ZAGRADA(%s,%s) <izraz_naredba> <izraz_naredba> <izraz> D_ZAGRADA(%s,%s) <naredba>\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[5]->prod).c_str(),
      vrijednost(children[5]->prod).c_str()
    );
    exit(0);
  }
  children[4]->provjeri();
  children[6]->provjeri();
}

// naredba skoka
void naredba_skoka__KR_CONTINUE_TOCKAZAREZ::provjeri(void) {
  // unutar petlje
  g_tree* curr = this;
  bool nasao = false;

  while (curr != NULL) {
    if (curr->inside_loop == true) {
      nasao = true;
      break;
    }
    curr = curr->parent;
  }

  if (nasao == false) {
    printf(
      "<naredba_skoka> ::= KR_CONTINUE(%s,%s) TOCKAZAREZ(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
}
void naredba_skoka__KR_BREAK_TOCKAZAREZ::provjeri(void) {
  // unutar petlje
  g_tree* curr = this;
  bool nasao = false;

  while (curr != NULL) {
    if (curr->inside_loop == true) {
      nasao = true;
      break;
    }
    curr = curr->parent;
  }

  if (nasao == false) {
    printf(
      "<naredba_skoka> ::= KR_BREAK(%s,%s) TOCKAZAREZ(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
}
void naredba_skoka__KR_RETURN_TOCKAZAREZ::provjeri(void) {
  // unutar funkcije tipa funkcija(parametri -> void)
  g_tree* curr = this;

  while (curr != NULL) {
    if (curr->inside_function == true) {
      break;
    }
    curr = curr->parent;
  }

  if (curr == NULL || curr->fun_ret != tip_void::instance()) {
    printf(
      "<naredba_skoka> ::= KR_RETURN(%s,%s) TOCKAZAREZ(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
}
void naredba_skoka__KR_RETURN_izraz_TOCKAZAREZ::provjeri(void) {
  // 1. provjeri(<izraz>)
  // 2. unutar funkcije tipa funkcija(parametri -> povratna)
  //    i vrijedi izraz.tip ~ povratna
  children[1]->provjeri();

  g_tree* curr = this;

  while (curr != NULL) {
    if (curr->inside_function == true) {
      break;
    }
    curr = curr->parent;
  }

  if (curr == NULL || implicitnaPromjena(children[1]->get_type(), curr->fun_ret) == 0) {
    printf(
      "<naredba_skoka> ::= KR_RETURN(%s,%s) <izraz> TOCKAZAREZ(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str()
    );
    exit(0);
  }
}

// prijevodna jedinica
void prijevodna_jedinica__vanjska_deklaracija::provjeri(void) {
  // 1. provjeri(<vanjska_deklaracija>)
  children[0]->provjeri();
}
void prijevodna_jedinica__prijevodna_jedinica_vanjska_deklaracija::provjeri(void) {
  // 1. provjeri(<prijevodna_jedinica>)
  // 2. provjeri(<vanjska_deklaracija>)
  children[0]->provjeri();
  children[1]->provjeri();
}

// vanjska deklaracija
void vanjska_deklaracija__definicija_funkcije::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}
void vanjska_deklaracija__deklaracija::provjeri(void) {
  // do nothing
  children[0]->provjeri();
}

// definicija funkcije
void definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA_slozena_naredba::provjeri(void) {
  // 1. provjeri(<ime_tipa>)
  // 2. ime_tipa.tip != const(T)
  // 3. ne postoji prije definirana funkcija imena IDN.ime
  // 4. ako postoji deklaracija imena IDN.ime u globalnom djelokrugu onda je
  //    pripadni tip te deklaracije funkcija(void -> ime_tipa.tip)
  // 5. zabiljezi definiciju i deklaraciju funkcije
  // 6. provjeri(<slozena_naredba>)
  children[0]->provjeri();
  if (children[0]->get_type() == tip_const_int::instance() ||
      children[0]->get_type() == tip_const_char::instance()) {
    printf(
      "<definicija_funkcije> ::= <ime_tipa> IDN(%s,%s) L_ZAGRADA(%s,%s) KR_VOID(%s,%s) D_ZAGRADA(%s,%s) <slozena_naredba>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str(),
      redak(children[4]->prod).c_str(),
      vrijednost(children[4]->prod).c_str()
    );
    exit(0);
  }

  g_tree* curr = this;
  bool nasao = false;

  vector< const tip* > params; params.push_back(tip_void::instance());
  const tip* funkcija = tip_funkcija::instance(children[0]->get_type(), params);

  fun_arg = params;
  fun_ret = children[0]->get_type();

  string name = vrijednost(children[1]->prod);

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      if (curr->tablica[name].status == 0) {
        nasao = true;
        break;
      } else if (curr->tablica[name].status == 1 && curr->oznaka_globalnog_djelokruga == true){
        if (curr->tablica[name].type != funkcija) {
          nasao = true;
          break;
        }
      }
    }
    curr = curr->parent;
  }

  if (nasao == true) {
    printf(
      "<definicija_funkcije> ::= <ime_tipa> IDN(%s,%s) L_ZAGRADA(%s,%s) KR_VOID(%s,%s) D_ZAGRADA(%s,%s) <slozena_naredba>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str(),
      redak(children[4]->prod).c_str(),
      vrijednost(children[4]->prod).c_str()
    );
    exit(0);
  }

  curr = this;
  while (curr != NULL) {
    if (curr->oznaka_globalnog_djelokruga == true) {
      curr->tablica[name] = lex(funkcija, 0, 0);
      break;
    }
    curr = curr->parent;
  }

  children[5]->provjeri();
}
void definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA_slozena_naredba::provjeri(void) {
  // 1. provjeri(<ime_tipa>)
  // 2. ime_tipa.tip != const(T)
  // 3. ne postoji prije definirana funckija imena IDN.ime
  // 4. provjeri(<lista_parametara>)
  // 5. ako postoji deklaracija imena IDN.ime u globalnom djelokrugu onda je
  //    pripadni tip te deklaracije funkcija(<lista_parametara>.tipovi -> ime_tipa.tip)
  // 6. zabiljezi definicju i deklaraciju funkcije
  // 7. provjeri(<slozena_naredba>) uz parametre funkcije koristeri
  //    lista_parametara.tipovi i lista_parametara.imena
  children[0]->provjeri();
  if (children[0]->get_type() == tip_const_int::instance() ||
      children[0]->get_type() == tip_const_char::instance()) {
    printf(
      "<definicija_funkcije> ::= <ime_tipa> IDN(%s,%s) L_ZAGRADA(%s,%s) <lista_parametara> D_ZAGRADA(%s,%s) <slozena_naredba>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[4]->prod).c_str(),
      vrijednost(children[4]->prod).c_str()
    );
    exit(0);
  }

  children[3]->provjeri();

  g_tree* curr = this;
  bool nasao = false;

  const tip* funkcija = tip_funkcija::instance(children[0]->get_type(), children[3]->get_types());

  fun_arg = children[3]->get_types();
  fun_ret = children[0]->get_type();

  string name = vrijednost(children[1]->prod);

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      if (curr->tablica[name].status == 0) {
        nasao = true;
        break;
      } else if (curr->tablica[name].status == 1 && curr->oznaka_globalnog_djelokruga == true){
        if (curr->tablica[name].type != funkcija) {
          nasao = true;
          break;
        }
      }
    }
    curr = curr->parent;
  }

  if (nasao == true) {
    printf(
      "<definicija_funkcije> ::= <ime_tipa> IDN(%s,%s) L_ZAGRADA(%s,%s) KR_VOID(%s,%s) D_ZAGRADA(%s,%s) <slozena_naredba>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str(),
      redak(children[4]->prod).c_str(),
      vrijednost(children[4]->prod).c_str()
    );
    exit(0);
  }

  curr = this;
  while (curr != NULL) {
    if (curr->oznaka_globalnog_djelokruga == true) {
      curr->tablica[name] = lex(funkcija, 0, 0);
      break;
    }
    curr = curr->parent;
  }

  vector< string > imena = children[3]->get_names();
  vector< const tip* > tipovi = children[3]->get_types();

  for (int i = 0; i < (int)imena.size(); ++i) {
    children[5]->tablica[imena[i]] = lex(tipovi[i], 1, 2);
  }

  children[5]->provjeri();
}

// lista parametara
void lista_parametara__deklaracija_parametra::provjeri(void) {
  // tipovi <- [deklaracija_parametra.tip]
  // imena <- [deklaracija_parametra.ime]
  // 1. provjeri(<deklaracija_parametra>)
  children[0]->provjeri();

  types.push_back(children[0]->get_type());
  names.push_back(children[0]->get_name());
}
void lista_parametara__lista_parametara_ZAREZ_deklaracija_parametra::provjeri(void) {
  // tipovi <- lista_parametara.tipovi + [deklaracija_parametra.tip]
  // imena <- lista_parametara.imena + [deklaracija_parametra.ime]
  // 1. provjeri(<lista_parametara>)
  // 2. provjeri(<deklaracija_parametra>)
  // 3. deklaracija_parametra.ime ne postoji u lista_parametara.imena
  children[0]->provjeri();
  children[2]->provjeri();

  if (findInVector(children[2]->get_name(), children[0]->get_names()) == 1) {
    printf(
      "<lista_parametara> ::= <lista_parametara> ZAREZ(%s,%s) <deklaracija_parametra>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  types = children[0]->get_types(); types.push_back(children[2]->get_type());
  names = children[0]->get_names(); names.push_back(children[2]->get_name());
}

// deklaracija parametara
void deklaracija_parametra__ime_tipa_IDN::provjeri(void) {
  // tip <- ime_tipa.tip
  // ime <- IDN.ime
  // 1. provjeri(<ime_tipa>)
  // 2. ime_tipa.tip != void
  children[0]->provjeri();
  if (children[0]->get_type() == tip_void::instance()) {
    printf(
      "<deklaracija_parametra> ::= <ime_tipa> IDN(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }

  type = children[0]->get_type();
  name = vrijednost(children[1]->prod);

}
void deklaracija_parametra__ime_tipa_IDN_L_UGL_ZAGRADA_D_UGL_ZAGRADA::provjeri(void) {
  // tip <- niz(ime_tipa.tip)
  // ime <- IDN.ime
  // 1. provjeri(<ime_tipa>)
  // 2. ime_tipa.tip != void
  children[0]->provjeri();
  if (children[0]->get_type() == tip_void::instance()) {
    printf(
      "<deklaracija_parametra> ::= <ime_tipa> IDN(%s,%s) L_UGL_ZAGRADA(%s,%s) D_UGL_ZAGRADA(%s,%s)\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  type = tip_niz::instance(children[0]->get_type());
  name = vrijednost(children[1]->prod);
}

// lista deklaracija
void lista_deklaracija__deklaracija::provjeri(void) {
  // 1. provjeri(<deklaracija>)
  children[0]->provjeri();
}
void lista_deklaracija__lista_deklaracija_deklaracija::provjeri(void) {
  // 1. provjeri(<lista_deklaracija>)
  // 2. provjeri(<deklaracija>)
  children[0]->provjeri();
  children[1]->provjeri();
}


// deklaracija
void deklaracija__ime_tipa_lista_init_deklaratora_TOCKAZAREZ::provjeri(void) {
  // 1. provjeri(<ime_tipa>)
  // 2. provjeri(<lista_init_deklaratora>) uz nasljedno svojstvo
  //    lista_init_deklaratora.ntip <- ime_tipa.tip
  children[0]->provjeri();
  children[1]->provjeri(children[0]->get_type());
}

// lista init deklaratora
void lista_init_deklaratora__init_deklarator::provjeri(const tip* ntip) {
  // 1. provjeri(<init_deklarator>) uz naseljedno svojstvo
  //    init_deklarator.ntip <- lista_init_deklaratora.ntip
  children[0]->provjeri(ntip);
}
void lista_init_deklaratora__lista_init_deklaratora_ZAREZ_init_deklarator::provjeri(const tip* ntip) {
  // 1. provjeri(<lista_init_deklaratora2>) uz nasljedno svojstvo
  //    lista_init_deklaratora2.btip <- lista_init_deklaratora1.ntip
  // 2. provjeri(<init_deklarator>) uz nasljedno svojstvo
  //    init_deklarator.ntip <- lista_init_deklaratora1.ntip
  children[0]->provjeri(ntip);
  children[2]->provjeri(ntip);
}

// init deklarator
void init_deklarator__izravni_deklarator::provjeri(const tip* ntip) {
  // 1. provjeri(<izravni_deklarator>) uz nasljedno_svojstvno
  //    izravni_deklarator.ntip <- init_deklarator.ntip
  // 2. izravni_deklarator.tip != const(T) i
  //    izravni_deklarator.tip != niz(const(T))
  children[0]->provjeri(ntip);
  if (children[0]->get_type() == tip_const_int::instance() ||
      children[0]->get_type() == tip_const_char::instance() ||
      children[0]->get_type() == tip_niz::instance(tip_const_int::instance()) ||
      children[0]->get_type() == tip_niz::instance(tip_const_char::instance())) {
    printf(
      "<init_deklarator> ::= <izravni_deklarator>\n"
    );
    exit(0);
  }

}
void init_deklarator__izravni_deklarator_OP_PRIDRUZI_inicijalizator::provjeri(const tip* ntip) {
  // 1. provjeri(<izravni_deklarator) uz nasljedno svojstvo
  //    izravni_deklarator.ntip <- init_deklarator.ntip
  // 2. provjeri(<inicijalizator>)
  // 3. ako je izravni_deklarator.tip = T ili const(T)
  //      inicijalizator.tip ~ T
  //    inace ako je izravni_deklarator.tip = niz(T) ili niz(const(T))
  //      inicijalizator.br_elem <= izravni_deklarator.br_elem
  //      za svaki U iz inicijalizator.tipvoi vrijedi U ~ T
  //    inace greska
  children[0]->provjeri(ntip);
  children[2]->provjeri();

  bool greska = false;

  if (children[0]->get_type() == tip_int::instance() ||
      children[0]->get_type() == tip_char::instance() ||
      children[0]->get_type() == tip_const_int::instance() ||
      children[0]->get_type() == tip_const_char::instance()
  ) {
    if (implicitnaPromjena(children[2]->get_type(), children[0]->get_type()) == 0) {
      greska = true;
    }
  } else if (children[0]->get_type() == tip_niz::instance(tip_int::instance()) ||
             children[0]->get_type() == tip_niz::instance(tip_char::instance()) ||
             children[0]->get_type() == tip_niz::instance(tip_const_int::instance()) ||
             children[0]->get_type() == tip_niz::instance(tip_const_char::instance())
  ) {
    if (children[2]->get_count() > children[0]->get_count()) {
      greska = true;
    }
    for (
      vector< const tip* >::iterator
      it = children[2]->get_types().begin();
      it != children[2]->get_types().end();
      ++it
    ) {
      if (implicitnaPromjena(*it, tip_int::instance()) == 0 &&
          implicitnaPromjena(*it, tip_char::instance()) == 0) {
        greska = true;
      }
    }
  } else {
    greska = true;
  }

  if (greska == true) {
    printf(
      "<init_deklarator> ::= <izravni_deklarator> OP_PRIDRUZI(%s,%s) <inicijalizator>\n",
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str()
    );
    exit(0);
  }
}

// izravni deklarator
void izravni_deklarator__IDN::provjeri(const tip* ntip) {
  // tip <- ntip
  // 1. ntip != void
  // 2. IDN.ime nije deklarirano u lokalnom djelokrugu
  // 3. zabiljezi deklaraciju IDN.ime s odgovarajucim tipom
  if (ntip == tip_void::instance()) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }

  g_tree* curr = this;
  g_tree* vrh = this;
  bool nasao = false;
  string name = vrijednost(children[0]->prod);

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      nasao = true;
      break;
    }
    vrh = curr;
    if (curr->oznaka_lokalnog_djelokurga == true) break;
    curr = curr->parent;
  }

  if (nasao == true) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str()
    );
    exit(0);
  }

  type = ntip;
  vrh->tablica[name] = lex(ntip, 1, 2);
}
void izravni_deklarator__IDN_L_UGL_ZAGRADA_BROJ_D_UGL_ZAGRADA::provjeri(const tip* ntip) {
  // tip <- niz(ntip)
  // br_elem <- BROJ.vrijednost
  // 1. ntip != void
  // 2. IDN.ime nije deklarirano u lokalnom djelokrugu
  // 3. BROJ.vrijednost je > 0 i <= 1024
  // 4. zabiljezi deklaraciju IDN.ime s odgovorajucim tipom
  if (ntip == tip_void::instance()) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s) L_UGL_ZAGRADA(%s,%s) BROJ(%s,%s) D_UGL_ZAGRADA(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }
  g_tree* curr = this;
  g_tree* vrh = this;
  bool nasao = false;
  string name = vrijednost(children[0]->prod);

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      nasao = true;
      break;
    }
    vrh = curr;
    if (curr->oznaka_lokalnog_djelokurga == true) break;
    curr = curr->parent;
  }

  if (nasao == true) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s) L_UGL_ZAGRADA(%s,%s) BROJ(%s,%s) D_UGL_ZAGRADA(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  long long int broj = string2int(vrijednost(children[2]->prod));
  if (broj < 0 || broj > 1024) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s) L_UGL_ZAGRADA(%s,%s) BROJ(%s,%s) D_UGL_ZAGRADA(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  type = tip_niz::instance(ntip);
  vrh->tablica[name] = lex(type, 1, 2);
}
void izravni_deklarator__IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA::provjeri(const tip* ntip) {
  // tip <- funkcija(void -> ntip)
  // 1. ako je IDN.ime deklarirano u lokalnom djelokrugu, tip prethode
  //    deklaracije je jednak funkcija(void -> ntip)
  // 2. zabiljezi deklaraciju IDN.ime s odgovrajucim tipom ako ista funkcija vec
  //    nije deklariran u lokalnom djelokrugu
  g_tree* curr = this;
  g_tree* vrh = this;

  bool greska = false;
  vector< const tip* > params;  params.push_back(tip_void::instance());
  const tip* funkcija = tip_funkcija::instance(ntip, params);

  string name = vrijednost(children[0]->prod);

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      if (curr->tablica[name].type != funkcija) {
        greska = true;
        break;
      }
    }
    vrh = curr;
    if (curr->oznaka_lokalnog_djelokurga) break;
    curr = curr->parent;
  }

  if (greska == true) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s) L_ZAGRADA(%s,%s) KR_VOID(%s,%s) D_ZAGRADA(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[2]->prod).c_str(),
      vrijednost(children[2]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  vrh->tablica[name] = lex(funkcija, 0, 1);

  type = funkcija;
}
void izravni_deklarator__IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA::provjeri(const tip* ntip) {
  // tip <- funckija(lista_parametara.tipovi -> ntip)
  // 1. provjeri(<lista_parametara>)
  // 2. ako je IDN.ime deklarirano u lokalnom djelogutu, tip prethode
  //    deklaracije je jednak funkcija(lista_parametara.tipovi -> ntip)
  // 3. zabiljezi deklaraciju IDN.ime s odgovrajucim tipom ako ista funkcija vec
  //    nije deklariran u lokalnom djelokrugu
  children[2]->provjeri();
  g_tree* curr = this;
  g_tree* vrh = this;

  bool greska = false;
  const tip* funkcija = tip_funkcija::instance(ntip, children[2]->get_types());

  string name = vrijednost(children[0]->prod);

  while (curr != NULL) {
    if (curr->tablica.find(name) != curr->tablica.end()) {
      if (curr->tablica[name].type != funkcija) {
        greska = true;
        break;
      }
    }
    vrh = curr;
    if (curr->oznaka_lokalnog_djelokurga) break;
    curr = curr->parent;
  }

  if (greska == true) {
    printf(
      "<izravni_deklarator> ::= IDN(%s,%s) L_ZAGRADA(%s,%s) <lista_parametara> D_ZAGRADA(%s,%s)\n",
      redak(children[0]->prod).c_str(),
      vrijednost(children[0]->prod).c_str(),
      redak(children[1]->prod).c_str(),
      vrijednost(children[1]->prod).c_str(),
      redak(children[3]->prod).c_str(),
      vrijednost(children[3]->prod).c_str()
    );
    exit(0);
  }

  vrh->tablica[name] = lex(funkcija, 0, 1);
  type = tip_funkcija::instance(ntip, children[2]->get_types());
}

// inicijalizator
void inicijalizator__izraz_pridruzivanja::provjeri(void) {
  // ako je izraz_pridruzivanja *=> NIZ_ZNAKOVA
  //  br_elem <- duljina niza znakova + 1
  //  tipovi <- lista duljine br_elem, svi elementi su char
  // inace
  //  tip <- izraz_pridruzivanja.tip
  //  1. provjeri(izraz_pridruzvanja)
  children[0]->provjeri();

  if (false) {
    // count =
    // for (int i = 0; i < count; ++i) {
    //   types.push_back(tip_char::instance());
    // }
  } else {
    type = children[0]->get_type();
  }
}
void inicijalizator__L_VIT_ZAGRADA_lista_izraza_pridruzivanja_D_VIT_ZAGRDA::provjeri(void) {
  // br_elem <- lista_izraz_pridruzivanja.br_elem
  // tipovi <- lista_izraz_pridruzivanja.tipovi
  // 1. provjeri(lista_izraz_pridruzivanja)
  children[1]->provjeri();

  count = children[1]->get_count();
  types = children[1]->get_types();
}

// lista izraza pridruzivanja
void lista_izraza_pridruzivanja__izraz_pridruzivanja::provjeri(void) {
  // tipovi <- [izraz_pridruzivanja.tip]
  // br_elem <- 1
  // 1. provjeri(<izraz_pridruzivanja>)
  children[0]->provjeri();

  count = 1;
  types.push_back(children[0]->get_type());
}
void lista_izraza_pridruzivanja__lista_izraza_pridruzivanja_ZAREZ_izraz_pridruzivanja::provjeri(void) {
  // tipovi <- lista_izraza_pridruzivanja.tipovi + [izraz_pridruzivanja.tip]
  // br_elem <- lista_izraz_pridruzivanja.br_elem + 1
  // 1. provjeri(<lista_izraza_pridruzivanja>)
  // 2. provjeri(<izraz_pridruzivanja>)
  children[0]->provjeri();
  children[2]->provjeri();

  types = children[0]->get_types();  types.push_back(children[2]->get_type());
  count = children[0]->get_count() + 1;
}

g_tree* produkcija(string a) {
  if (a == "<primarni_izraz>") {
    return new primarni_izraz;
  } else if (a == "<postfiks_izraz>") {
    return new postfiks_izraz;
  } else if (a == "<lista_argumenata>") {
    return new lista_argumenata;
  } else if (a == "<unarni_izraz>") {
    return new unarni_izraz;
  } else if (a == "<unarni_operator>") {
    return new unarni_operator;
  } else if (a == "<cast_izraz>") {
    return new cast_izraz;
  } else if (a == "<ime_tipa>") {
    return new ime_tipa;
  } else if (a == "<specifikator_tipa>") {
    return new specifikator_tipa;
  } else if (a == "<multiplikativni_izraz>") {
    return new multiplikativni_izraz;
  } else if (a == "<aditivni_izraz>") {
    return new aditivni_izraz;
  } else if (a == "<odnosni_izraz>") {
    return new odnosni_izraz;
  } else if (a == "<jednakosni_izraz>") {
    return new jednakosni_izraz;
  } else if (a == "<bin_i_izraz>") {
    return new bin_ili_izraz;
  } else if (a == "<bin_xili_izraz>") {
    return new bin_xili_izraz;
  } else if (a == "<bin_ili_izraz>") {
    return new bin_ili_izraz;
  } else if (a == "<log_i_izraz>") {
    return new log_i_izraz;
  } else if (a == "<log_ili_izraz>") {
    return new log_ili_izraz;
  } else if (a == "<izraz_pridruzivanja>") {
    return new izraz_pridruzivanja;
  } else if (a == "<izraz>") {
    return new izraz;
  } else if (a == "<slozena_naredba>") {
    return new slozena_naredba;
  } else if (a == "<lista_naredbi>") {
    return new lista_naredbi;
  } else if (a == "<naredba>") {
    return new naredba;
  } else if (a == "<izraz_naredba>") {
    return new izraz_naredba;
  } else if (a == "<naredba_grananja>") {
    return new naredba_grananja;
  } else if (a == "<naredba_petlje>") {
    return new naredba_petlje;
  } else if (a == "<naredba_skoka>") {
    return new naredba_skoka;
  } else if (a == "<prijevodna_jedinica>") {
    return new prijevodna_jedinica;
  } else if (a == "<vanjska_deklaracija>") {
    return new vanjska_deklaracija;
  } else if (a == "<definicija_funkcije>") {
    return new definicija_funkcije;
  } else if (a == "<lista_parametara>") {
    return new lista_parametara;
  } else if (a == "<deklaracija_parametra>") {
    return new deklaracija_parametra;
  } else if (a == "<lista_deklaracija>") {
    return new lista_deklaracija;
  } else if (a == "<deklaracija>") {
    return new deklaracija;
  } else if (a == "<lista_init_deklaratora>") {
    return new lista_init_deklaratora;
  } else if (a == "<init_deklarator>") {
    return new init_deklarator;
  } else if (a == "<izravni_deklarator>") {
    return new izravni_deklarator;
  } else if (a == "<inicijalizator>") {
    return new inicijalizator;
  } else if (a == "<lista_izraza_pridruzivanja>") {
    return new lista_izraza_pridruzivanja;
  } else if (a == "primarni_izraz__IDN") {
    return new primarni_izraz__IDN;
  } else if (a == "primarni_izraz__BROJ") {
    return new primarni_izraz__BROJ;
  } else if (a == "primarni_izraz__ZNAK") {
    return new primarni_izraz__ZNAK;
  } else if (a == "primarni_izraz__NIZ_ZNAKOVA") {
    return new primarni_izraz__NIZ_ZNAKOVA;
  } else if (a == "primarni_izraz__L_ZAGRADA_izraz_D_ZAGRADA") {
    return new primarni_izraz__L_ZAGRADA_izraz_D_ZAGRADA;
  } else if (a == "postfiks_izraz__primarni_izraz") {
    return new postfiks_izraz__primarni_izraz;
  } else if (a == "postfiks_izraz__postfiks_izraz_L_UGL_ZAGRADA_izraz_D_UGL_ZAGRADA") {
    return new postfiks_izraz__postfiks_izraz_L_UGL_ZAGRADA_izraz_D_UGL_ZAGRADA;
  } else if (a == "postfiks_izraz__postfiks_izraz_L_ZAGRADA_D_ZAGRADA") {
    return new postfiks_izraz__postfiks_izraz_L_ZAGRADA_D_ZAGRADA;
  } else if (a == "postfiks_izraz__postfiks_izraz_L_ZAGRADA_lista_argumenata_D_ZAGRADA") {
    return new postfiks_izraz__postfiks_izraz_L_ZAGRADA_lista_argumenata_D_ZAGRADA;
  } else if (a == "postfiks_izraz__postfiks_izraz_OP_INC") {
    return new postfiks_izraz__postfiks_izraz_OP_INC;
  } else if (a == "postfiks_izraz__postfiks_izraz_OP_DEC") {
    return new postfiks_izraz__postfiks_izraz_OP_DEC;
  } else if (a == "lista_argumenata__izraz_pridruzivanja") {
    return new lista_argumenata__izraz_pridruzivanja;
  } else if (a == "lista_argumenata__lista_argumenata_ZAREZ_izraz_pridruzivanja") {
    return new lista_argumenata__lista_argumenata_ZAREZ_izraz_pridruzivanja;
  } else if (a == "unarni_izraz__postfiks_izraz") {
    return new unarni_izraz__postfiks_izraz;
  } else if (a == "unarni_izraz__OP_INC_unarni_izraz") {
    return new unarni_izraz__OP_INC_unarni_izraz;
  } else if (a == "unarni_izraz__OP_DEC_unarni_izraz") {
    return new unarni_izraz__OP_DEC_unarni_izraz;
  } else if (a == "unarni_izraz__unarni_operator_cast_izraz") {
    return new unarni_izraz__unarni_operator_cast_izraz;
  } else if (a == "unarni_operator__PLUS") {
    return new unarni_operator__PLUS;
  } else if (a == "unarni_operator__MINUS") {
    return new unarni_operator__MINUS;
  } else if (a == "unarni_operator__OP_TILDA") {
    return new unarni_operator__OP_TILDA;
  } else if (a == "unarni_operator__OP_NEG") {
    return new unarni_operator__OP_NEG;
  } else if (a == "cast_izraz__unarni_izraz") {
    return new cast_izraz__unarni_izraz;
  } else if (a == "cast_izraz__L_ZAGRADA_ime_tipa_D_ZAGRADA_cast_izraz") {
    return new cast_izraz__L_ZAGRADA_ime_tipa_D_ZAGRADA_cast_izraz;
  } else if (a == "ime_tipa__specifikator_tipa") {
    return new ime_tipa__specifikator_tipa;
  } else if (a == "ime_tipa__KR_CONST_specifikator_tipa") {
    return new ime_tipa__KR_CONST_specifikator_tipa;
  } else if (a == "specifikator_tipa__KR_VOID") {
    return new specifikator_tipa__KR_VOID;
  } else if (a == "specifikator_tipa__KR_INT") {
    return new specifikator_tipa__KR_INT;
  } else if (a == "specifikator_tipa__KR_CHAR") {
    return new specifikator_tipa__KR_CHAR;
  } else if (a == "multiplikativni_izraz__cast_izraz") {
    return new multiplikativni_izraz__cast_izraz;
  } else if (a == "multiplikativni_izraz__multiplikativni_izraz_OP_PUTA_cast_izraz") {
    return new multiplikativni_izraz__multiplikativni_izraz_OP_PUTA_cast_izraz;
  } else if (a == "multiplikativni_izraz__multiplikativni_izraz_OP_DIJELI_cast_izraz") {
    return new multiplikativni_izraz__multiplikativni_izraz_OP_DIJELI_cast_izraz;
  } else if (a == "multiplikativni_izraz__multiplikativni_izraz_OP_MOD_cast_izraz") {
    return new multiplikativni_izraz__multiplikativni_izraz_OP_MOD_cast_izraz;
  } else if (a == "aditivni_izraz__multiplikativni_izraz") {
    return new aditivni_izraz__multiplikativni_izraz;
  } else if (a == "aditivni_izraz__aditivni_izraz_PLUS_multiplikativni_izraz") {
    return new aditivni_izraz__aditivni_izraz_PLUS_multiplikativni_izraz;
  } else if (a == "aditivni_izraz__aditivni_izraz_MINUS_multiplikativni_izraz") {
    return new aditivni_izraz__aditivni_izraz_MINUS_multiplikativni_izraz;
  } else if (a == "odnosni_izraz__aditivni_izraz") {
    return new odnosni_izraz__aditivni_izraz;
  } else if (a == "odnosni_izraz__odnosni_izraz_OP_LT_aditivni_izraz") {
    return new odnosni_izraz__odnosni_izraz_OP_LT_aditivni_izraz;
  } else if (a == "odnosni_izraz__odnosni_izraz_OP_GT_aditivni_izraz") {
    return new odnosni_izraz__odnosni_izraz_OP_GT_aditivni_izraz;
  } else if (a == "odnosni_izraz__odnosni_izraz_OP_LTE_aditivni_izraz") {
    return new odnosni_izraz__odnosni_izraz_OP_LTE_aditivni_izraz;
  } else if (a == "odnosni_izraz__odnosni_izraz_OP_GTE_aditivni_izraz") {
    return new odnosni_izraz__odnosni_izraz_OP_GTE_aditivni_izraz;
  } else if (a == "jednakosni_izraz__odnosni_izraz") {
    return new jednakosni_izraz__odnosni_izraz;
  } else if (a == "jednakosni_izraz__jednakosni_izraz_OP_EQ_odnosni_izraz") {
    return new jednakosni_izraz__jednakosni_izraz_OP_EQ_odnosni_izraz;
  } else if (a == "jednakosni_izraz__jednakosni_izraz_OP_NEQ_odnosni_izraz") {
    return new jednakosni_izraz__jednakosni_izraz_OP_NEQ_odnosni_izraz;
  } else if (a == "bin_i_izraz__jednakosni_izraz") {
    return new bin_i_izraz__jednakosni_izraz;
  } else if (a == "bin_i_izraz__bin_i_izraz_OP_BIN_I_jednakosni_izraz") {
    return new bin_i_izraz__bin_i_izraz_OP_BIN_I_jednakosni_izraz;
  } else if (a == "bin_xili_izraz__bin_i_izraz") {
    return new bin_xili_izraz__bin_i_izraz;
  } else if (a == "bin_xili_izraz__bin_xili_izraz_OP_BIN_XILI_bin_i_izraz") {
    return new bin_xili_izraz__bin_xili_izraz_OP_BIN_XILI_bin_i_izraz;
  } else if (a == "bin_ili_izraz__bin_xili_izraz") {
    return new bin_ili_izraz__bin_xili_izraz;
  } else if (a == "bin_ili_izraz__bin_ili_izraz_OP_BIN_ILI_bin_xili_izraz") {
    return new bin_ili_izraz__bin_ili_izraz_OP_BIN_ILI_bin_xili_izraz;
  } else if (a == "log_i_izraz__bin_ili_izraz") {
    return new log_i_izraz__bin_ili_izraz;
  } else if (a == "log_i_izraz__log_i_izraz_OP_I_bin_ili_izraz") {
    return new log_i_izraz__log_i_izraz_OP_I_bin_ili_izraz;
  } else if (a == "log_ili_izraz__log_i_izraz") {
    return new log_ili_izraz__log_i_izraz;
  } else if (a == "log_ili_izraz__log_ili_izraz_OP_ILI_log_i_izraz") {
    return new log_ili_izraz__log_ili_izraz_OP_ILI_log_i_izraz;
  } else if (a == "izraz_pridruzivanja__log_ili_izraz") {
    return new izraz_pridruzivanja__log_ili_izraz;
  } else if (a == "izraz_pridruzivanja__postfiks_izraz_OP_PRIDRUZI_izraz_pridruzivanja") {
    return new izraz_pridruzivanja__postfiks_izraz_OP_PRIDRUZI_izraz_pridruzivanja;
  } else if (a == "izraz__izraz_pridruzivanja") {
    return new izraz__izraz_pridruzivanja;
  } else if (a == "izraz__izraz_ZAREZ_izraz_pridruzivanja") {
    return new izraz__izraz_ZAREZ_izraz_pridruzivanja;
  } else if (a == "slozena_naredba__L_VIT_ZAGRADA_lista_naredbi_D_VIT_ZAGRADA") {
    return new slozena_naredba__L_VIT_ZAGRADA_lista_naredbi_D_VIT_ZAGRADA;
  } else if (a == "slozena_naredba__L_VIT_ZAGRADA_lista_deklaracija_lista_naredbi_D_VIT_ZAGRADA") {
    return new slozena_naredba__L_VIT_ZAGRADA_lista_deklaracija_lista_naredbi_D_VIT_ZAGRADA;
  } else if (a == "lista_naredbi__naredba") {
    return new lista_naredbi__naredba;
  } else if (a == "lista_naredbi__lista_naredbi_naredba") {
    return new lista_naredbi__lista_naredbi_naredba;
  } else if (a == "naredba__slozena_naredba") {
    return new naredba__slozena_naredba;
  } else if (a == "naredba__izraz_naredba") {
    return new naredba__izraz_naredba;
  } else if (a == "naredba__naredba_grananja") {
    return new naredba__naredba_grananja;
  } else if (a == "naredba__naredba_petlje") {
    return new naredba__naredba_petlje;
  } else if (a == "naredba__naredba_skoka") {
    return new naredba__naredba_skoka;
  } else if (a == "izraz_naredba__TOCKAZAREZ") {
    return new izraz_naredba__TOCKAZAREZ;
  } else if (a == "izraz_naredba__izraz_TOCKAZAREZ") {
    return new izraz_naredba__izraz_TOCKAZAREZ;
  } else if (a == "naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba") {
    return new naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba;
  } else if (a == "naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba_KR_ELSE_naredba") {
    return new naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba_KR_ELSE_naredba;
  } else if (a == "naredba_petlje__KR_WHILE_L_ZAGRADA_izraz_D_ZAGRADA_naredba") {
    return new naredba_petlje__KR_WHILE_L_ZAGRADA_izraz_D_ZAGRADA_naredba;
  } else if (a == "naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_D_ZAGRADA_naredba") {
    return new naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_D_ZAGRADA_naredba;
  } else if (a == "naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_izraz_D_ZAGRADA_naredba") {
    return new naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_izraz_D_ZAGRADA_naredba;
  } else if (a == "naredba_skoka__KR_CONTINUE_TOCKAZAREZ") {
    return new naredba_skoka__KR_CONTINUE_TOCKAZAREZ;
  } else if (a == "naredba_skoka__KR_BREAK_TOCKAZAREZ") {
    return new naredba_skoka__KR_BREAK_TOCKAZAREZ;
  } else if (a == "naredba_skoka__KR_RETURN_TOCKAZAREZ") {
    return new naredba_skoka__KR_RETURN_TOCKAZAREZ;
  } else if (a == "naredba_skoka__KR_RETURN_izraz_TOCKAZAREZ") {
    return new naredba_skoka__KR_RETURN_izraz_TOCKAZAREZ;
  } else if (a == "prijevodna_jedinica__vanjska_deklaracija") {
    return new prijevodna_jedinica__vanjska_deklaracija;
  } else if (a == "prijevodna_jedinica__prijevodna_jedinica_vanjska_deklaracija") {
    return new prijevodna_jedinica__prijevodna_jedinica_vanjska_deklaracija;
  } else if (a == "vanjska_deklaracija__definicija_funkcije") {
    return new vanjska_deklaracija__definicija_funkcije;
  } else if (a == "vanjska_deklaracija__deklaracija") {
    return new vanjska_deklaracija__deklaracija;
  } else if (a == "definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA_slozena_naredba") {
    return new definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA_slozena_naredba;
  } else if (a == "definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA_slozena_naredba") {
    return new definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA_slozena_naredba;
  } else if (a == "lista_parametara__deklaracija_parametra") {
    return new lista_parametara__deklaracija_parametra;
  } else if (a == "lista_parametara__lista_parametara_ZAREZ_deklaracija_parametra") {
    return new lista_parametara__lista_parametara_ZAREZ_deklaracija_parametra;
  } else if (a == "lista_deklaracija__deklaracija") {
    return new lista_deklaracija__deklaracija;
  } else if (a == "lista_deklaracija__lista_deklaracija_deklaracija") {
    return new lista_deklaracija__lista_deklaracija_deklaracija;
  } else if (a == "deklaracija_parametra__ime_tipa_IDN") {
    return new deklaracija_parametra__ime_tipa_IDN;
  } else if (a == "deklaracija_parametra__ime_tipa_IDN_L_UGL_ZAGRADA_D_UGL_ZAGRADA") {
    return new deklaracija_parametra__ime_tipa_IDN_L_UGL_ZAGRADA_D_UGL_ZAGRADA;
  } else if (a == "deklaracija__ime_tipa_lista_init_deklaratora_TOCKAZAREZ") {
    return new deklaracija__ime_tipa_lista_init_deklaratora_TOCKAZAREZ;
  } else if (a == "lista_init_deklaratora__init_deklarator") {
    return new lista_init_deklaratora__init_deklarator;
  } else if (a == "lista_init_deklaratora__lista_init_deklaratora_ZAREZ_init_deklarator") {
    return new lista_init_deklaratora__lista_init_deklaratora_ZAREZ_init_deklarator;
  } else if (a == "init_deklarator__izravni_deklarator") {
    return new init_deklarator__izravni_deklarator;
  } else if (a == "init_deklarator__izravni_deklarator_OP_PRIDRUZI_inicijalizator") {
    return new init_deklarator__izravni_deklarator_OP_PRIDRUZI_inicijalizator;
  } else if (a == "izravni_deklarator__IDN") {
    return new izravni_deklarator__IDN;
  } else if (a == "izravni_deklarator__IDN_L_UGL_ZAGRADA_BROJ_D_UGL_ZAGRADA") {
    return new izravni_deklarator__IDN_L_UGL_ZAGRADA_BROJ_D_UGL_ZAGRADA;
  } else if (a == "izravni_deklarator__IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA") {
    return new izravni_deklarator__IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA;
  } else if (a == "izravni_deklarator__IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA") {
    return new izravni_deklarator__IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA;
  } else if (a == "inicijalizator__izraz_pridruzivanja") {
    return new inicijalizator__izraz_pridruzivanja;
  } else if (a == "inicijalizator__L_VIT_ZAGRADA_lista_izraza_pridruzivanja_D_VIT_ZAGRDA") {
    return new inicijalizator__L_VIT_ZAGRADA_lista_izraza_pridruzivanja_D_VIT_ZAGRDA;
  } else if (a == "lista_izraza_pridruzivanja__izraz_pridruzivanja") {
    return new lista_izraza_pridruzivanja__izraz_pridruzivanja;
  } else if (a == "lista_izraza_pridruzivanja__lista_izraza_pridruzivanja_ZAREZ_izraz_pridruzivanja") {
    return new lista_izraza_pridruzivanja__lista_izraza_pridruzivanja_ZAREZ_izraz_pridruzivanja;
  } else {
    return new list_stabla;
  }
  fprintf(stderr, "Nesto je poslo po zlu! \n%s\n", a.c_str());
  return NULL;
}
