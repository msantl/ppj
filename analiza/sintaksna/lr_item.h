#ifndef LRITEM
#define LRITEM

#include "misc.h"

class LRItem {
  private:
    int dot_position;
    int extended_part;

    std::multimap<int, std::pair< int, std::vector< int > > >::iterator production;

  public:
    LRItem();

    LRItem(
      std::multimap<int, std::pair< int, std::vector< int > > >::iterator
    );

    LRItem(
      std::multimap<int, std::pair< int, std::vector< int > > >::iterator,
      int
    );

    void moveDotRight(void);

    int getDotPosition(void) const;

    int getProductionLength(void) const;

    int getProductionID(void) const;

    int isCompleteItem(void) const;

    int getTokenAfterDot(void) const;

    int getLHS(void) const;

    int getExtendedPart(void) const;

    std::vector< int > getRHS(void) const;

    std::vector< int > getTokensAfterDot(void) const;

    bool operator == (const LRItem &) const;

    bool operator < (const LRItem &) const;
};

#endif
