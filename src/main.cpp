#include <cstdio>
#include <iostream>
#include <optional>
#include <string>

#include <qfio/qfio.hpp>
#include <qarg/qarg.hpp>

void show_usage(const std::string name);
std::string caesar(const std::string plain, int count);

int main(int argc, const char *argv[]) {
  qarg::parser args;
  args.add<bool>('h', "display this text");
  args.add<int>('s', "shift value", true);
  args.add<bool>('u', "unshift");
  args.add<std::string>('f', "file path");
  args.add<std::string>('t', "text");

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

  // get shift value
  int count = 0;
  const std::optional<int> shift = args.get<int>('s');
  count = *shift;

  const bool unshift = *args.get<bool>('u');
  if (unshift) {
    count *= -1;
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


  // encode
  const std::string cipher_text = caesar(plain_text, count);

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
      "  %1$s -f <file path> -s <shift value> [-u]\n"
      "  %1$s -t <plain text> -s <shift value> [-u]\n"
    ),
    name.c_str()
  );
  std::snprintf(buf+n, 100-n, "\n");
  std::cerr << buf;
}

std::string caesar(const std::string plain, int count) {
  std::string cipher;
  for (char c : plain) {
    char new_c;

    if (c >= 'A' && c <= 'Z') {
      new_c = c + count;

      if (new_c < 'A') {
        new_c += 26;
      } else if (new_c > 'Z') {
         new_c -= 26;
      }

      cipher += new_c;
      continue;
    }

    if (c >= 'a' && c <= 'z') {
      new_c = c + count;

      if (new_c < 'a') {
       new_c += 26;
      } else if (new_c > 'z') {
        new_c -= 26;
      }

      cipher += new_c;
      continue;
    }

    cipher += c;
  }

  return cipher;
}
