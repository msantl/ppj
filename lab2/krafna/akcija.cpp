#include "akcija.h"

using namespace std;

void akcija::clear(void) {
  LHS = "";
  value = state = 0;
  tip = ODBACI;
}

akcija::akcija(void) {
  clear();
}

int akcija::getType() {
  return tip;
}

int akcija::getState() {
  return state;
}

int akcija::getValue() {
  return value;
}

string akcija::getLHS() {
  return LHS;
}

void akcija::setType(type _tip) {
  tip = _tip;
}

void akcija::setState(int _state) {
  state = _state;
}

void akcija::setValue(int _value) {
  value = _value;
}

void akcija::setLHS(string lhs) {
  LHS = lhs;
}
