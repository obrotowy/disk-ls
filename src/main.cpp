#include "Disk.hpp"
#include "Partition.hpp"
#include <iostream>
#include <cstring>

int main(int argc, const char** argv) {
  if (argc < 2) {
    std::cout << "Usage: ./disk-ls <disk_image_file>" << std::endl;
    exit(-1);
  }
  Disk d = Disk(argv[1]);
  std::vector<Partition> partitions = enumerate_partitions(d);
  for (const Partition& p: partitions) {
    std::cout << p;
  }
}