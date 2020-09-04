#include <string>
#include <cctype>

#include "standard.hpp"
#include "running.hpp"

std::string runningkey(
  const std::string &plain, const std::string &key, const bool reverse
) {
  std::string cipher;
  std::size_t index = 0;

  for (char c : plain) {
    if (index >= key.size()) {
      index = 0;
    }

    char new_c;
    int count = std::tolower(key[index]) - 'a';

    if (reverse) {
      count *= -1;
    }

    bool changed = false;
    new_c = caesar(c, count, changed);
    cipher += new_c;

    if (changed) {
      index++;
    }
  }

  return cipher;
}
