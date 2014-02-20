#include "misc.h"

using namespace std;

vector< string > split(string needle, string haystack) {
  vector< string > hajduk;
  int n = needle.length();
  int m = haystack.length();

  string buff;
  for (int i = 0; i < n; ++i) {
    if (needle.substr(i, m) == haystack) {
      if (buff.length()) {
        hajduk.push_back(buff);
      }
      buff = "";
      i += m - 1;
    } else {
      buff += needle[i];
    }
  }

  if (buff.length()) {
    hajduk.push_back(buff);
  }

  return hajduk;
}

string strip_signs(string a) {
  return a.substr(1, a.length() - 2);
}

bool is_terminal(string a) {
  // zavrsni znakovi nisu oblika <ime_znaka>
  return a[0] != '<';
}

bool findInVector(string needle, vector< string > haystack) {
    if (find(haystack.begin(), haystack.end(), needle) == haystack.end()) {
      return false;
    } else {
      return true;
    }
}

pair< string, int > read_line(bool should_read) {
  static pair< string, int > prev;
  string value;
  int depth;

  if (should_read) {

    if (getline(cin, value)) {
      for (depth = 0; value[depth] == ' '; ++depth) ;
      value = value.substr(depth);
    } else {
      depth = -1;
      value = "oznaka kraja niza";
    }

    prev = make_pair(value, depth);
  }

  return prev;
}

stablo::stablo () {}
stablo::stablo (string a) : lhs(a) {}

stablo::~stablo () {
  for (
    vector< stablo* >::iterator
    it = children.begin();
    it != children.end();
    ++it
  ) {
    delete (*it);
  }
}

long long int string2int(string a) {
  if (a.length() > 11) return 999999999999999;
  if (a.substr(0, 2) == "0x") {a = a.substr(2);}
  if (a.substr(0, 3) == "-0x") {a = "-" + a.substr(3);}
  long long int val;
  stringstream ss;
  ss << a;
  ss >> val;
  return val;
}
