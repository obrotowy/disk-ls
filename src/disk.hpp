#pragma once
#include <string>
#include <vector>
#include "partition.hpp"
class Disk {
  public:
    Disk(const char* name);
    ~Disk();
    const uint8_t* read_mbr();
    std::vector<Partition> enumerate_partitions();
  private:
    std::FILE* f;
    uint8_t* mbr;
};