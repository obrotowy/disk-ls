#include "partition.hpp"

Partition::Partition(const void* entry, FILE* _f) {
  f = _f;
  bootable = *(uint8_t*) entry == 0x80;
  begin_lba = *(uint32_t*)(entry + 0x8);
  size = *(uint32_t*)(entry + 0xC);
  type = (Type) *(uint8_t*)(entry + 0x4);
}

const uint8_t* Partition::read_sector(int i) {
  uint8_t* sector = new uint8_t[SECTOR_SIZE];
  fseek(f, (begin_lba + i) * SECTOR_SIZE, SEEK_SET);
  fread(sector, SECTOR_SIZE, 1, f);
  return sector;
}