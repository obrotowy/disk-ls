#pragma once

#include <string>

enum FILE_TYPE {
  UNKNOWN,
  REGULAR_FILE,
  DIRECTORY,
  CHARACTER_DEVICE,
  BLOCK_DEVICE,
  FIFO,
  SOCKET,
  SYMLINK
};

class File {
  public:
    std::string name;
    std::string path;
    size_t size;
    FILE_TYPE type;
    virtual ~File() = default;
};
