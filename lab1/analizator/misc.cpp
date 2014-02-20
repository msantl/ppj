#include "misc.h"

int string2int(string a) {
  int ret = 0, sign = 1;
  if (a[0] == '-') {
    sign = -1;
  } else {
    ret = a[0] - '0';
  }

  for (int i = 1; i < (int)a.length(); ++i) {
    ret = ret * 10 + a[i] - '0';
  }
  return ret * sign;
}

bool jeOperator(string izraz, int i) {
  int backslashCounter = 0;

  while (i - 1 >= 0 && izraz[i-1] == '\\') {
    backslashCounter ++;
    i --;
  }

  return (backslashCounter % 2) == 0;
}

vector< string > split (string expr, string delimiter) {
  vector< string > ret;
  string curr = "";

  for (int i = 0; i < (int)expr.length(); ++i) {
    string match = "";
    for (int j = 0; j + i < (int)expr.length() && j < (int)delimiter.length(); ++j) {
      match += expr[i + j];
    }

    if (match == delimiter && jeOperator(expr, i) && curr != "") {
      ret.push_back(curr);

      curr = "";
      i += delimiter.length() - 1;
    } else {
      curr += expr[i];
    }

  }

  if (curr.length() > 0) {
    ret.push_back(curr);
  }

  return ret;
}

vector< string > splitLine(bool readNewLine) {
  static vector< string > prev;
  char *buff = (char *)malloc(65535);

  if (readNewLine) {
    memset(buff, 0, sizeof buff);
    buff = gets(buff);

    if (buff == NULL) {
      return vector< string >();
    }

    string izraz(buff);
    prev = split(izraz, " ");
  }

  free(buff);

  return prev;
}

void append(set< int > &a, set< int > b) {
  for (set< int >::iterator sit = b.begin(); sit != b.end(); ++sit) {
    a.insert(*sit);
  }
  return;
}

string escapeSpecialChars(string s) {
  string ret;
  for (int i = 0; i < (int)s.length(); ++i) {
    if (s[i] == '"' && jeOperator(s, i)) {
      ret += '\\';
    } else if (s[i] == '\\') {
      ret += '\\';
    }

    if (s[i] == '\n') {
      ret += "\\n";
    } else if (s[i] == '\t') {
      ret += "\\t";
    } else {
      ret += s[i];
    }
  }
  return ret;
}

int findLast(string s, char c, int start) {
  int ret = -1;
  for (int i = start; i < (int)s.length(); ++i) {
    if (s[i] == c && jeOperator(s, i)) ret = i;
  }
  return ret;
}
int findFirst(string s, char c, int start) {
  for (int i = start; i < (int)s.length(); ++i) {
    if (s[i] == c && jeOperator(s, i)) return i;
  }
  return -1;
}

