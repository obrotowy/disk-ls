#include "Disk.hpp"
#include "Partition.hpp"
#include "ext2/Ext2.hpp"
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
      std::cout << fs;
      const std::vector<directory_entry> root_dir = fs.list_root_directory();
      for (const directory_entry& f: root_dir) {
        std::cout << "Filename: " << f.name << std::endl;
        std::cout << "Inode: " << std::hex << f.inode << std::endl;
      }
    }
  }
}