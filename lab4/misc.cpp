#include "misc.h"

using namespace std;

string int2string(long long int a) {
  string ret;
  stringstream ss;
  ss << a;
  ss >> ret;
  return ret;
}

string toUpper(string a) {
  string b;
  for (int i=0; i<(int)a.length(); ++i) {
    b += toupper(a[i]);
  }
  return b;
}

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

int char2int(char a) {
  if ('0' <= a && a <= '9') return a - '0';
  if ('a' <= a && a <= 'z') return 10 + a - 'a';
  if ('A' <= a && a <= 'Z') return 10 + a - 'A';
  return 0;
}

long long int string2int(string b) {
  string a;
  int base = 0;
  int minus = 1;
  if (b[0] == '-') { b = b.substr(1); minus = -1;; }
  if (b.substr(0, 2) == "0x") {
    b = b.substr(2);
    base = 16;
  } else if (b.substr(0, 1) == "0") {
    b = b.substr();
    base = 8;
  } else {
    base = 10;
  }

  while(b.size() > 2 && b[0] == '0') b = b.substr(1);

  a += b;
  if (a.length() > 12) return 999999999999999LL;
  long long int val = 0;
  long long int curr = 1;

  for (int i = (int)a.length() - 1; i >= 0; --i) {
    val += curr * char2int(a[i]);
    curr *= base;
  }

  return val * minus;
}
