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
  std::cout << "\tStart\tEnd\tSectors\tSize\tType" << std::endl;
  for (int i = 0; i<partitions.size(); ++i) {
    std::cout << i << '\t' << partitions[i] << std::endl;
  }
  
  Ext2 fs(partitions[0]);
  inode root_dir = fs.get_inode(2);
  std::vector<directory_entry> files_in_root = fs.list_directory(root_dir.direct_block_pointer[0]);
  for (const directory_entry& e: files_in_root) {
    std::cout << e.name << std::endl;
  }
}