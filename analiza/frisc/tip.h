#ifndef _TIPH
#define _TIPH

#include <iostream>

#include <map>
#include <vector>
#include <cstdio>
#include <cstdlib>


// moguca vrsta tipa podataka u ppjC
class tip{};

// brojevni tip T
class tip_T : virtual public tip {};

class tip_int : public tip_T {
  private:
    tip_int() {}
  public:
    static const tip* instance(void);
};

class tip_const_int : public tip_T {
  private:
    tip_const_int() {}
  public:
    static const tip* instance(void);
};

class tip_char : public tip_T {
  private:
    tip_char() {}
  public:
    static const tip* instance(void);
};

class tip_const_char : public tip_T {
  private:
    tip_const_char() {}
  public:
    static const tip* instance(void);
};

class tip_niz : virtual public tip {
  private:
    tip_niz() {}
  public:
    static const tip* instance(const tip*);
};

class tip_funkcija : virtual public tip {
  private:
    tip_funkcija() {}
  public:
    static const tip* instance(const tip*, std::vector< const tip* >);
};

class tip_void : virtual public tip {
  private:
    tip_void() {}
  public:
    static const tip* instance(void);
};

void print_type_info(void);

#endif
