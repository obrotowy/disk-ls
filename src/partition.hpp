#pragma once
#include "constants.hpp"
#include <cstdint>
#include <cstdio>

enum Type : uint8_t {
  EMPTY = 0,
  FAT32_LBA = 0xC
};

class Partition {
  public:
    Partition(const void* entry, FILE* _f);
    const uint8_t* read_sector(int i);
    bool bootable;
    uint32_t begin_lba;
    uint32_t size;
    Type type;
  private:
    FILE* f;
};