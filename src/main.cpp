#include "disk.hpp"
#include "partition.hpp"
#include <cstdio>

int main(int argc, const char** argv) {
  if (argc < 2) {
    puts("Usage: ./disk-ls <img_file>\n");
    exit(-1);
  }
  Disk disk(argv[1]);
  for (const Partition &part: disk.enumerate_partitions()) {
    printf("Partition found:\n");
    printf("\tBootable: %s\n", (part.bootable ? "Yes" : "No"));
    printf("\tType: 0x%x\n", part.type);
    printf("\tSize: %d sectors\n", part.size);
    printf("\tBegin LBA: 0x%x\n", part.begin_lba);
  }
}