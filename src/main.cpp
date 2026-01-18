#include "Disk.hpp"
#include "Partition.hpp"
#include "Ext2.hpp"
#include <iostream>
#include <cstring>

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cout << "Usage: ./disk-ls <disk_image_file>" << std::endl;
    exit(-1);
  }
  Disk d = Disk(argv[1]);
  std::vector<Partition> partitions = enumerate_partitions(d);
  for (Partition& p: partitions) {
    std::cout << p;
    if (p.type == EXT2) {
      Ext2 fs(p);
      std::cout << "Ext2 Block size: " << fs.block_size << std::endl;
    }
  }
}