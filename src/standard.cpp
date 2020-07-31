#include <string>

#include "standard.hpp"

int txmod(int n, const int c, const int min, const int max) {
  int dist = (max - min) + 1;

  n += c;
  while (n < min) { n += dist; }
  while (n > max) { n -= dist; }

  return n;
}

char caesar(const int c, int count) {
  bool x;
  return caesar(c, count, x);
}

char caesar(const int c, int count, bool &changed) {
  int new_c;
  changed = true;

  if (c >= 'A' && c <= 'Z') {
    new_c = txmod(c, count, 'A', 'Z');
  } else  if (c >= 'a' && c <= 'z') {
    new_c = txmod(c, count, 'a', 'z');
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
