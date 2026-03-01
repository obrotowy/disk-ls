#pragma once

#include <string>

enum FILE_TYPE {
  REGULAR_FILE,
  DIRECTORY
};

class File {
  public:
    std::string name;
    std::string path;
    size_t size;
    FILE_TYPE type;
    virtual ~File() = default;
};
