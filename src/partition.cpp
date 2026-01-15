#include "partition.hpp"

Partition::Partition(const void* entry) {

  bootable = *(uint8_t*) entry == 0x80;
  begin_lba = *(uint32_t*)(entry + 0x8);
  size = *(uint32_t*)(entry + 0xC);
  type = (Type) *(uint8_t*)(entry + 0x4);
}