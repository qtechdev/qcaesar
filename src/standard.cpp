#include <string>

#include "standard.hpp"

char caesar(const int c, int count) {
  bool x;
  return caesar(c, count, x);
}

char caesar(const int c, int count, bool &changed) {
  int new_c;
  changed = true;

  if (c >= 'A' && c <= 'Z') {
    new_c = c + count;

    if (new_c < 'A') {
      new_c += 26;
    } else if (new_c > 'Z') {
      new_c -= 26;
    }
  } else  if (c >= 'a' && c <= 'z') {
    new_c = c + count;

    if (new_c < 'a') {
      new_c += 26;
    } else if (new_c > 'z') {
      new_c -= 26;
    }
  } else {
    new_c = c;
    changed = false;
  }

  return new_c;
}

std::string caesar(const std::string &plain, int count) {
  std::string cipher;
  for (char c : plain) {
    cipher += caesar(c, count);
  }

  return cipher;
}
