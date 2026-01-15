#pragma once
#include <cstdint>

enum Type : uint8_t {
  EMPTY = 0,
  FAT32_LBA = 0xC
};

class Partition {
  public:
    Partition(const void* entry);
    bool bootable;
    uint32_t begin_lba;
    uint32_t size;
    Type type;
};