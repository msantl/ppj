#include "lr_item.h"

using namespace std;

LRItem::LRItem(): dot_position(0) {}

LRItem::LRItem(
  multimap< int, pair< int, vector< int >  > >::iterator prod
): dot_position(0), production(prod) {}

LRItem::LRItem(
  multimap< int, pair< int, vector< int > > >::iterator prod,
  int ext
): dot_position(0), extended_part(ext), production(prod) {}

LRItem LRItem::moveDotRight(void) const {
  LRItem ret(production, extended_part);
  ret.setDotPosition(dot_position + 1);
  return ret;
}

void LRItem::setDotPosition(int pos) {
  dot_position = min(pos, getProductionLength());
}

int LRItem::getDotPosition(void) const {
  return dot_position;
}

int LRItem::getProductionLength(void) const {
  return (int)production->second.second.size();
}

int LRItem::getProductionID(void) const {
  return production->second.first;
}

int LRItem::isCompleteItem(void) const {
  return (getDotPosition() == getProductionLength());
}

int LRItem::getTokenAfterDot(void) const {
  if (getDotPosition() < getProductionLength()) {
    return production->second.second[getDotPosition()];
  } else {
    return -1;
  }
}

int LRItem::getLHS(void) const {
  return production->first;
}

int LRItem::getExtendedPart(void) const {
  return extended_part;
}

vector< int > LRItem::getRHS(void) const {
  return production->second.second;
}

vector< int > LRItem::getTokensAfterDot(void) const {
  if (isCompleteItem()) {
    return vector< int >();
  } else {
    return vector< int >(production->second.second.begin() + dot_position + 1, production->second.second.end());
  }
}

bool LRItem::operator == (const LRItem &a) const {
  if (this->getDotPosition() != a.getDotPosition()) {
    return 0;
  } else if (this->getProductionID() != a.getProductionID()) {
    return 0;
  } else {
    return this->getExtendedPart() == a.getExtendedPart();
  }
}

bool LRItem::operator < (const LRItem &a) const {
  if (this->getDotPosition() != a.getDotPosition()) {
    return this->getDotPosition() < a.getDotPosition();
  } else if (this->getProductionID() != a.getProductionID()) {
    return this->getProductionID() < a.getProductionID();
  } else {
    return this->getExtendedPart() < a.getExtendedPart();
  }
}

