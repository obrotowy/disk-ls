#pragma once
#include <cstdint>
#include <string>
#include "File.hpp"


typedef struct directory_entry{
  uint32_t inode;
  uint8_t type;
  uint16_t entry_size;
  std::string name;
} directory_entry;

class Ext2File : public File {
  public:
    uint32_t inode;
};