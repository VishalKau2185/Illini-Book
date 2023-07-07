#include "illini_book.hpp"
#include "utilities.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " [uin_file] [rel_file]"
              << std::endl;
    return 1;
  }

  std::string uin_file = argv[1];
  std::string rel_file = argv[2];
  IlliniBook i(uin_file, rel_file);
  std::cout << i.GetRelated(1, 2, "128") << std::endl;
}
