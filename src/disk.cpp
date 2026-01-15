#include "disk.hpp"

Disk::Disk(const char* fname) {
  f = fopen(fname, "r");
}

Disk::~Disk() {
  fclose(f);
}

const uint8_t* Disk::read_mbr() {
  fseek(f, 0, SEEK_SET);
  uint8_t* mbr = new uint8_t[512];
  fread(mbr, 512, 1, f);
  return mbr;
}

std::vector<Partition> Disk::enumerate_partitions() {
  const uint8_t* mbr = read_mbr();
  std::vector<Partition> partitions;
  for (int i = 0; i<4; ++i) {
    Partition part = Partition(mbr+446+16*i, f);
    if (part.type != EMPTY)
      partitions.push_back(part);
  }
  return partitions;
}