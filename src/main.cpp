#include "mbr.h"
#include <cstdio>
#include <iostream>

int main(int argc, const char** argv) {
  if (argc < 2){
    std::cerr << "Usage: ./mbr disk.bin";
    exit(-1);
  }
  std::FILE* f = std::fopen(argv[1], "r");
  uint8_t mbr[512];
  fread(mbr, 512, 1, f);
  uint16_t magic = get_signature(mbr);
  if (magic != MBR_MAGIC_BYTES) {
    std::cerr << "MBR signature not found. Got 0x" << std::hex << magic << " instead.";
    exit(-1);
  }
  for (int i = 0; i<MBR_MAX_PARTITIONS; ++i) {
    partition part = get_partition_info(mbr+446+i*16);
    if (part.type == 0)
      continue;
    printf("Partition %d:\n", i);
    printf("\tBootable: %s\n", (part.bootable) ? "Yes" : "No");
    printf("\tType: 0x%x\n", part.type);
    printf("\tFirst sector LBA: 0x%x\n", part.first_sector_lba);
  }
}