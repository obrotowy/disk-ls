#pragma once
#include "File.hpp"
#include <string>
#include <vector>
#include <sstream>

class FileSystem {
  public:
    virtual const char* read_file(const std::string& path) = 0;
    virtual const std::vector<File> list_directory(const std::string& path) = 0;
};

static const std::vector<std::string> split_path(const std::string& path) {
  std::vector<std::string> path_elements;
  std::stringstream ss(path);
  std::string item;

  while (std::getline(ss, item, '/')) {
      path_elements.push_back(item);
  }
  return path_elements;
}
