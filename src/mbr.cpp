#include "mbr.h"
#include <iostream>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <iomanip>

uint16_t get_signature(const uint8_t* data) {
  return static_cast<uint16_t>(data[511]) | static_cast<uint16_t>(data[510]) << 8;
}

partition get_partition_info(const void* data) {
  partition ret;
  ret.bootable = (*(uint8_t*)data == 0x80);
  ret.type = (partition_type)*((uint8_t*)data+4);
  ret.first_sector_lba = ((uint32_t*)data)[2];
  return ret;
}
