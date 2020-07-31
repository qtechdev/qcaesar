#include <cstdio>
#include <iostream>
#include <optional>
#include <string>

#include <qfio/qfio.hpp>
#include <qarg/qarg.hpp>

#include "standard.hpp"
#include "running.hpp"

void show_usage(const std::string name);
char caesar(const int c, int count);
char caesar(const int c, int count, bool &changed);
std::string caesar(const std::string &plain, int count);
std::string runningkey(
  const std::string &plain, const std::string &key, const bool reverse
);

int main(int argc, const char *argv[]) {
  qarg::parser args;
  args.add<bool>('h', "display this text");
  args.add<int>('s', "shift value/key", true);
  args.add<bool>('u', "unshift");
  args.add<std::string>('f', "file path");
  args.add<std::string>('t', "text");
  args.add<std::string>('c', "cipher type");

  try {
    args.parse(argc, argv);
  } catch (std::invalid_argument &e) {
    std::cerr << e.what() << "\n";
    show_usage(argv[0]);
    return 1;
  }

  const bool help = *args.get<bool>('h');
  if (help) {
    show_usage(argv[0]);
    return 0;
  }

  // get text
  std::string plain_text;
  const std::optional<std::string> file = args.get<std::string>('f');
  const std::optional<std::string> text = args.get<std::string>('t');
  if (file) {
    const auto file_text = fio::read(*file);
    if (!file_text) {
      std::cerr << "could not read file\n";
      return 3;
    } else {
      plain_text = *file_text;
    }
  } else if (text) {
    plain_text = *text;
  } else {
    show_usage(argv[0]);
    return 2;
  }

  std::string cipher_text;
  auto cipher_type = args.get<std::string>('c');
  const bool reverse = *args.get<bool>('u');

  if (cipher_type && *cipher_type == "running") {
    const std::string key = *args.get<std::string>('s');
    cipher_text = runningkey(plain_text, key, reverse);
  } else { /* Standad Caesar Shift*/
    // get shift value
    int count = 0;
    const int shift = *args.get<int>('s');
    count = shift;

    if (reverse) {
      count *= -1;
    }

    // encode
    cipher_text = caesar(plain_text, count);
  }

  std::cout << "Plain Text  : " << plain_text << "\n";
  std::cout << "Cipher Text : " << cipher_text << "\n";

  return 0;
}

void show_usage(const std::string name) {
  char buf[512];
  int n = std::snprintf(
    buf, 512,
    (
      "usage:\n"
      "  %1$s -h\n"
      "  %1$s -c caesar -f <file path> -s <shift value> [-u]\n"
      "  %1$s -c caesar -t <plain text> -s <shift value> [-u]\n"
      "  %1$s -c running -f <file path> -s <shift key> [-u]\n"
      "  %1$s -c running -t <plain text> -s <shift key> [-u]\n"
      "\n"
      "ciphers:\n"
      "  caesar   : standard caesar shift\n"
      "  running  : caesar shift with running key\n"
    ),
    name.c_str()
  );
  std::snprintf(buf+n, 512-n, "\n");
  std::cerr << buf;
}
