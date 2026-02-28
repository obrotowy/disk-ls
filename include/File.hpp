#pragma once

#include <string>
#include "ext2/directory.hpp"

enum FILE_TYPE {
  REGULAR_FILE,
  DIRECTORY
};

class File {
  public:
    File(const directory_entry& ext2_entry);
    std::string name;
    std::string path;
    size_t size;
    FILE_TYPE type;  
};
