#pragma once
#include <fstream>
#include <string>
#include <streambuf>
#include "test_config.hpp"

class FileComparator : public Catch::MatcherBase<std::string> {
 public:
  FileComparator(const std::string& filename,
                 const std::string& extension = "txt")
      : filename_(filename), extension_(extension) {}

  virtual bool match(const std::string& received) const override {
    bool check_successful = false;
    std::ifstream acceptedFile(
        data_file(filename_ + ".accepted." + extension_));
    if (acceptedFile.is_open()) {
      std::string acceptedStr((std::istreambuf_iterator<char>(acceptedFile)),
                              std::istreambuf_iterator<char>());
      check_successful = (acceptedStr == received);
      acceptedFile.close();
    }
    // write to file if not successful
    if (!check_successful) {
      std::ofstream receivedFile(filename_ + ".received." + extension_);
      if (receivedFile.is_open()) {
        receivedFile << received;
        receivedFile.close();
      }
    }
    return check_successful;
  }

  virtual std::string describe() const override {
    return "String should match file " + filename_ + ".accepted." + extension_ +
           ".";
  }

 private:
  std::string filename_;
  std::string extension_;
};

inline FileComparator EqualsFile(const std::string& filename) {
  return FileComparator(filename);
}

inline FileComparator EqualsXmlFile(const std::string& filename) {
  return FileComparator(filename, "xml");
}
