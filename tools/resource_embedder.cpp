#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[]) {
  std::cout << "#pragma once" << std::endl << std::endl;
  std::cout << "#include <iostream>" << std::endl;
  std::cout << "#include <sstream>" << std::endl;

  for (int fileIndex = 1; fileIndex < argc; ++fileIndex) {
    std::ifstream file(argv[fileIndex]);
    if (file.is_open()) {
      std::cout << "static const unsigned char v" << fileIndex << "[] = {";
      long long int charIndex = 0;
      char character;
      while (file.get(character)) {
        if ((charIndex % 12) == 0) {
          std::cout << std::endl << "  ";
        }
        std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex
                  << (int)character << ", ";
        ++charIndex;
      }
      // Make null-terminated
      std::cout << "0x00" << std::endl << " };" << std::endl;
      file.close();
    } else {
      throw std::runtime_error("file not found.");
    }
  }

  std::cout << std::endl << std::endl;

  for (int fileIndex = 1; fileIndex < argc; ++fileIndex) {
    std::cout << "inline bool getEmbeddedFile(const std::string& filename,\n"
                 "                            std::stringstream& stream) {"
              << std::endl;
    std::cout << "  if (filename == \"" << argv[fileIndex] << "\") {"
              << std::endl;
    std::cout << "    stream << v" << fileIndex << ";" << std::endl;
    std::cout << "    return true;" << std::endl;
    std::cout << "  }" << std::endl;
    std::cout << "  return false;" << std::endl;
    std::cout << "}" << std::endl;
  }
  return 0;
}
