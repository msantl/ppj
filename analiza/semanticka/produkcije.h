#ifndef _PRODUKCIJEH
#define _PRODUKCIJEH

#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>

#include "tip.h"

extern std::map< const tip*, std::pair<const tip*, std::vector< const tip* > > > lookup;

class lex {
  public:
    const tip* type;
    bool l_value;
    int status; // definicija = 0, deklaracije = 1, varijable = 2

    lex() {}
    lex(const tip*, bool, int);
};

// generativno stablo
class g_tree {
  public:
    std::string prod;
    std::vector< g_tree* > children;
    g_tree* parent;

    // svaki cvor ima informacije o necem
    std::map<std::string, lex> tablica;
    bool inside_loop;
    bool oznaka_lokalnog_djelokruga;
    bool oznaka_globalnog_djelokruga;
    bool inside_function;

    // ako je cvor funkcija ima postavljene ove vrijednosti
    std::vector< const tip* > fun_arg;
    const tip* fun_ret;

    virtual void provjeri(void) {}
    virtual void provjeri(const tip*) {}

    virtual void ispis_pogreske(void) { exit(0); }

    virtual const tip* get_type(void) { return NULL; }
    virtual bool get_l_value(void) { return false; }
    virtual int get_count(void) { return 0; }
    virtual std::string get_name(void) { return ""; }
    virtual std::vector< const tip* > get_types(void) { return std::vector< const tip* >(); }
    virtual const tip* get_types(int) { return NULL; }
    virtual std::vector< std::string > get_names(void) { return std::vector< std::string >(); }

    g_tree() {
      inside_loop = false;
      inside_function = false;
      oznaka_lokalnog_djelokruga = false;
      oznaka_globalnog_djelokruga = false;
    }

    void DEBUG(int = 0);
};

// oblijezja
class have_type : virtual public g_tree {
  public:
    const tip* type;
    const tip* get_type(void);
};
class have_l_value : virtual public g_tree {
  public:
    bool l_value;
    bool get_l_value(void);
};
class have_name : virtual public g_tree {
  public:
    std::string name;
    std::string get_name(void);
};
class have_count : virtual public g_tree {
  public:
    int count;
    int get_count(void);
};
class have_types : virtual public g_tree {
  public:
    std::vector< const tip* > types;
    std::vector< const tip* > get_types(void);
    const tip* get_types(int);
};
class have_names : virtual public g_tree {
  public:
    std::vector< std::string > names;
    std::vector< std::string > get_names(void);
};

// list stabla
class list_stabla : public g_tree {};

// izrazi
class primarni_izraz : public have_type, public have_l_value {};
class postfiks_izraz : public have_type, public have_l_value {};
class lista_argumenata : public have_types {};
class unarni_izraz : public have_type, public have_l_value {};
class unarni_operator : public g_tree {};
class cast_izraz : public have_type, public have_l_value {};
class ime_tipa : public have_type {};
class specifikator_tipa : public have_type {};
class multiplikativni_izraz : public have_type, public have_l_value {};
class aditivni_izraz : public have_type, public have_l_value {};
class odnosni_izraz : public have_type, public have_l_value {};
class jednakosni_izraz : public have_type, public have_l_value {};
class bin_i_izraz : public have_type, public have_l_value {};
class bin_xili_izraz : public have_type, public have_l_value {};
class bin_ili_izraz : public have_type, public have_l_value {};
class log_i_izraz : public have_type, public have_l_value {};
class log_ili_izraz : public have_type, public have_l_value {};
class izraz_pridruzivanja : public have_type, public have_l_value {};
class izraz : public have_type, public have_l_value {};

// naredbena struktura
class slozena_naredba : public g_tree {
  public:
    slozena_naredba() {
      oznaka_lokalnog_djelokruga = true;
    }
};
class lista_naredbi : public g_tree {};
class naredba : public g_tree {};
class izraz_naredba : public have_type {};
class naredba_grananja : public g_tree {};
class naredba_petlje : public g_tree {
  public:
    naredba_petlje() {
      inside_loop = true;
    }
};
class naredba_skoka : public g_tree {};
class prijevodna_jedinica : public g_tree {};
class vanjska_deklaracija : public g_tree {};

// deklaracije i definicije
class definicija_funkcije : public g_tree {
  public:
    definicija_funkcije() {
      inside_function = true;
    }
};
class lista_parametara : public have_types, public have_names {};
class deklaracija_parametra : public have_type, public have_name {};
class lista_deklaracija : public g_tree {};
class deklaracija : public g_tree{};
class lista_init_deklaratora : public g_tree {};
class init_deklarator : public g_tree {};
class izravni_deklarator : public have_type, public have_count {};
class inicijalizator : public have_type, public have_types, public have_count {};
class lista_izraza_pridruzivanja : public have_types, public have_count {};

// SEMANTICKA PRAVILA
// primarni izraz
class primarni_izraz__IDN : public primarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class primarni_izraz__BROJ : public primarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class primarni_izraz__ZNAK : public primarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class primarni_izraz__NIZ_ZNAKOVA : public primarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class primarni_izraz__L_ZAGRADA_izraz_D_ZAGRADA : public primarni_izraz {
	public:
		void provjeri(void);
};

// postfiks izraz
class postfiks_izraz__primarni_izraz : public postfiks_izraz {
	public:
		void provjeri(void);
};
class postfiks_izraz__postfiks_izraz_L_UGL_ZAGRADA_izraz_D_UGL_ZAGRADA : public postfiks_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class postfiks_izraz__postfiks_izraz_L_ZAGRADA_D_ZAGRADA : public postfiks_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class postfiks_izraz__postfiks_izraz_L_ZAGRADA_lista_argumenata_D_ZAGRADA : public postfiks_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class postfiks_izraz__postfiks_izraz_OP_INC : public postfiks_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class postfiks_izraz__postfiks_izraz_OP_DEC : public postfiks_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// lista argumenata
class lista_argumenata__izraz_pridruzivanja : public lista_argumenata {
	public:
		void provjeri(void);
};
class lista_argumenata__lista_argumenata_ZAREZ_izraz_pridruzivanja : public lista_argumenata {
	public:
		void provjeri(void);
};

// unarni izraz
class unarni_izraz__postfiks_izraz : public unarni_izraz {
	public:
		void provjeri(void);
};
class unarni_izraz__OP_INC_unarni_izraz : public unarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class unarni_izraz__OP_DEC_unarni_izraz : public unarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class unarni_izraz__unarni_operator_cast_izraz : public unarni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// unarni operator
class unarni_operator__PLUS : public unarni_operator {
	public:
		void provjeri(void);
};
class unarni_operator__MINUS : public unarni_operator {
	public:
		void provjeri(void);
};
class unarni_operator__OP_TILDA : public unarni_operator {
	public:
		void provjeri(void);
};
class unarni_operator__OP_NEG : public unarni_operator {
	public:
		void provjeri(void);
};

// cast izraz
class cast_izraz__unarni_izraz : public cast_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class cast_izraz__L_ZAGRADA_ime_tipa_D_ZAGRADA_cast_izraz : public cast_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// ime tipa
class ime_tipa__specifikator_tipa : public ime_tipa {
	public:
		void provjeri(void);
};
class ime_tipa__KR_CONST_specifikator_tipa : public ime_tipa {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// specifikator tipa
class specifikator_tipa__KR_VOID : public specifikator_tipa {
	public:
		void provjeri(void);
};
class specifikator_tipa__KR_INT : public specifikator_tipa {
	public:
		void provjeri(void);
};
class specifikator_tipa__KR_CHAR : public specifikator_tipa {
	public:
		void provjeri(void);
};

// multiplikativni izraz
class multiplikativni_izraz__cast_izraz : public multiplikativni_izraz {
	public:
		void provjeri(void);
};
class multiplikativni_izraz__multiplikativni_izraz_OP_PUTA_cast_izraz : public multiplikativni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class multiplikativni_izraz__multiplikativni_izraz_OP_DIJELI_cast_izraz : public multiplikativni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class multiplikativni_izraz__multiplikativni_izraz_OP_MOD_cast_izraz : public multiplikativni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// aditivni izraz
class aditivni_izraz__multiplikativni_izraz : public aditivni_izraz {
	public:
		void provjeri(void);
};
class aditivni_izraz__aditivni_izraz_PLUS_multiplikativni_izraz : public aditivni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class aditivni_izraz__aditivni_izraz_MINUS_multiplikativni_izraz : public aditivni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// odnosni izraz
class odnosni_izraz__aditivni_izraz : public odnosni_izraz {
	public:
		void provjeri(void);
};
class odnosni_izraz__odnosni_izraz_OP_LT_aditivni_izraz : public odnosni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class odnosni_izraz__odnosni_izraz_OP_GT_aditivni_izraz : public odnosni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class odnosni_izraz__odnosni_izraz_OP_LTE_aditivni_izraz : public odnosni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class odnosni_izraz__odnosni_izraz_OP_GTE_aditivni_izraz : public odnosni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// jednakosni izraz
class jednakosni_izraz__odnosni_izraz : public jednakosni_izraz {
	public:
		void provjeri(void);
};
class jednakosni_izraz__jednakosni_izraz_OP_EQ_odnosni_izraz : public jednakosni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class jednakosni_izraz__jednakosni_izraz_OP_NEQ_odnosni_izraz : public jednakosni_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// bin i izraz
class bin_i_izraz__jednakosni_izraz : public bin_i_izraz {
	public:
		void provjeri(void);
};
class bin_i_izraz__bin_i_izraz_OP_BIN_I_jednakosni_izraz : public bin_i_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// bin xili izraz
class bin_xili_izraz__bin_i_izraz : public bin_xili_izraz {
	public:
		void provjeri(void);
};
class bin_xili_izraz__bin_xili_izraz_OP_BIN_XILI_bin_i_izraz : public bin_xili_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// bin ili izraz
class bin_ili_izraz__bin_xili_izraz : public bin_ili_izraz {
	public:
		void provjeri(void);
};
class bin_ili_izraz__bin_ili_izraz_OP_BIN_ILI_bin_xili_izraz : public bin_ili_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// log i izraz
class log_i_izraz__bin_ili_izraz : public log_i_izraz {
	public:
		void provjeri(void);
};
class log_i_izraz__log_i_izraz_OP_I_bin_ili_izraz : public log_i_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// log ili izraz
class log_ili_izraz__log_i_izraz : public log_ili_izraz {
	public:
		void provjeri(void);
};
class log_ili_izraz__log_ili_izraz_OP_ILI_log_i_izraz : public log_ili_izraz {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// izraz_pridruzivanja
class izraz_pridruzivanja__log_ili_izraz : public izraz_pridruzivanja {
	public:
		void provjeri(void);
};
class izraz_pridruzivanja__postfiks_izraz_OP_PRIDRUZI_izraz_pridruzivanja : public izraz_pridruzivanja {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// izraz
class izraz__izraz_pridruzivanja : public izraz {
	public:
		void provjeri(void);
};
class izraz__izraz_ZAREZ_izraz_pridruzivanja : public izraz {
	public:
		void provjeri(void);
};

// slozena naredba
class slozena_naredba__L_VIT_ZAGRADA_lista_naredbi_D_VIT_ZAGRADA : public slozena_naredba {
	public:
		void provjeri(void);
};
class slozena_naredba__L_VIT_ZAGRADA_lista_deklaracija_lista_naredbi_D_VIT_ZAGRADA : public slozena_naredba {
	public:
		void provjeri(void);
};

// lista naredbni
class lista_naredbi__naredba : public lista_naredbi {
	public:
		void provjeri(void);
};
class lista_naredbi__lista_naredbi_naredba : public lista_naredbi {
	public:
		void provjeri(void);
};

// naredba
class naredba__slozena_naredba : public naredba {
	public:
		void provjeri(void);
};
class naredba__izraz_naredba : public naredba {
	public:
		void provjeri(void);
};
class naredba__naredba_grananja : public naredba {
	public:
		void provjeri(void);
};
class naredba__naredba_petlje : public naredba {
	public:
		void provjeri(void);
};
class naredba__naredba_skoka : public naredba {
	public:
		void provjeri(void);
};

// izraz_naredba
class izraz_naredba__TOCKAZAREZ : public izraz_naredba {
	public:
		void provjeri(void);
};
class izraz_naredba__izraz_TOCKAZAREZ : public izraz_naredba {
	public:
		void provjeri(void);
};

// naredba grananja
class naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba : public naredba_grananja {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class naredba_grananja__KR_IF_L_ZAGRADA_izraz_D_ZAGRADA_naredba_KR_ELSE_naredba : public naredba_grananja {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// naredba petlja
class naredba_petlje__KR_WHILE_L_ZAGRADA_izraz_D_ZAGRADA_naredba : public naredba_petlje {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_D_ZAGRADA_naredba : public naredba_petlje {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class naredba_petlje__KR_FOR_L_ZAGRADA_izraz_naredba_izraz_naredba_izraz_D_ZAGRADA_naredba : public naredba_petlje {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// naredba skoka
class naredba_skoka__KR_CONTINUE_TOCKAZAREZ : public naredba_skoka {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class naredba_skoka__KR_BREAK_TOCKAZAREZ : public naredba_skoka {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class naredba_skoka__KR_RETURN_TOCKAZAREZ : public naredba_skoka {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class naredba_skoka__KR_RETURN_izraz_TOCKAZAREZ : public naredba_skoka {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// prijevodna jedinica
class prijevodna_jedinica__vanjska_deklaracija : public prijevodna_jedinica {
	public:
		void provjeri(void);
};
class prijevodna_jedinica__prijevodna_jedinica_vanjska_deklaracija : public prijevodna_jedinica {
	public:
		void provjeri(void);
};

// vanjska deklaracija
class vanjska_deklaracija__definicija_funkcije : public vanjska_deklaracija {
	public:
		void provjeri(void);
};
class vanjska_deklaracija__deklaracija : public vanjska_deklaracija {
	public:
		void provjeri(void);
};

// definicija funkcije
class definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA_slozena_naredba : public definicija_funkcije {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class definicija_funkcije__ime_tipa_IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA_slozena_naredba : public definicija_funkcije {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// lista parametara
class lista_parametara__deklaracija_parametra : public lista_parametara {
	public:
		void provjeri(void);
};
class lista_parametara__lista_parametara_ZAREZ_deklaracija_parametra : public lista_parametara {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// lista deklaracija
class lista_deklaracija__deklaracija : public lista_deklaracija {
	public:
		void provjeri(void);
};
class lista_deklaracija__lista_deklaracija_deklaracija : public lista_deklaracija {
	public:
		void provjeri(void);
};

// deklaracija parametara
class deklaracija_parametra__ime_tipa_IDN : public deklaracija_parametra {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};
class deklaracija_parametra__ime_tipa_IDN_L_UGL_ZAGRADA_D_UGL_ZAGRADA : public deklaracija_parametra {
	public:
		void provjeri(void);
		void ispis_pogreske(void);
};

// deklaracija
class deklaracija__ime_tipa_lista_init_deklaratora_TOCKAZAREZ : public deklaracija {
	public:
		void provjeri(void);
};

// lista init deklaratora
class lista_init_deklaratora__init_deklarator : public lista_init_deklaratora {
	public:
		void provjeri(const tip*);
};
class lista_init_deklaratora__lista_init_deklaratora_ZAREZ_init_deklarator : public lista_init_deklaratora {
	public:
		void provjeri(const tip*);
};

// init deklarator
class init_deklarator__izravni_deklarator : public init_deklarator {
	public:
		void provjeri(const tip*);
		void ispis_pogreske(void);
};
class init_deklarator__izravni_deklarator_OP_PRIDRUZI_inicijalizator : public init_deklarator {
	public:
		void provjeri(const tip*);
		void ispis_pogreske(void);
};

// izravni deklarator
class izravni_deklarator__IDN : public izravni_deklarator {
	public:
		void provjeri(const tip*);
		void ispis_pogreske(void);
};
class izravni_deklarator__IDN_L_UGL_ZAGRADA_BROJ_D_UGL_ZAGRADA : public izravni_deklarator {
	public:
		void provjeri(const tip*);
		void ispis_pogreske(void);
};
class izravni_deklarator__IDN_L_ZAGRADA_KR_VOID_D_ZAGRADA : public izravni_deklarator {
	public:
		void provjeri(const tip*);
		void ispis_pogreske(void);
};
class izravni_deklarator__IDN_L_ZAGRADA_lista_parametara_D_ZAGRADA : public izravni_deklarator {
	public:
		void provjeri(const tip*);
		void ispis_pogreske(void);
};

// inicijalizator
class inicijalizator__izraz_pridruzivanja : public inicijalizator {
	public:
		void provjeri(void);
};
class inicijalizator__L_VIT_ZAGRADA_lista_izraza_pridruzivanja_D_VIT_ZAGRADA : public inicijalizator {
	public:
		void provjeri(void);
};

// lista izraza pridruzivanja
class lista_izraza_pridruzivanja__izraz_pridruzivanja : public lista_izraza_pridruzivanja {
	public:
		void provjeri(void);
};
class lista_izraza_pridruzivanja__lista_izraza_pridruzivanja_ZAREZ_izraz_pridruzivanja : public lista_izraza_pridruzivanja {
	public:
		void provjeri(void);
};

g_tree* produkcija(std::string);

#endif
