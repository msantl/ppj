#include "misc.h"

using namespace std;

int startsWith(string a, string b) {
  return a.substr(0, b.length()) == b;
}

int readLine(string &value, int offset) {
  if (!getline(cin, value)) return 0;
  value = value.substr(offset);
  return 1;
}

string collapseSet(set< string > X, string delimiter) {
  string ret;

  for (set< string >::iterator it=X.begin(); it!=X.end(); ++it) {
    ret += *it + delimiter;
  }

  return ret;
}
int string2int(string a) {
  int ret = 0, minus = 1;
  if (a.length() == 0) return 0;

  if (a[0] == '-') {minus = -1;}
  else {ret = a[0] - '0';}

  for (int i=1; i<(int)a.length(); ++i) {
    ret = 10 * ret + a[i] - '0';
  }

  return minus * ret;
}

void perform_housekeeping(const char *filename) {
  FILE *f = fopen(filename, "w");
  fclose(f);
}

int fReadLine(ifstream &fajl, string &value, int offset) {
  if (!getline(fajl, value)) return 0;
  value = value.substr(offset);
  return 1;
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

void append(set< int > &a, set< int > b) {
  for (set< int >::iterator it = b.begin(); it != b.end(); ++it) {
    a.insert(*it);
  }
}

int updated(set< int > &a, set< int > b) {
  int ret = 0;
  for (set< int >::iterator it = b.begin(); it != b.end(); ++it) {
    ret += (a.count(*it) == 0);
    a.insert(*it);
  }
  return ret;
}

