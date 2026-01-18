#pragma once
#include <cstdint>
#include <string>

typedef struct directory_entry{
  uint32_t inode;
  uint8_t type;
  uint16_t entry_size;
  std::string name;
} directory_entry;