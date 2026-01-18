#include "Ext2.hpp"

Ext2::Ext2(Partition& _p) : p(_p) {
  uint8_t super_sector[SECTOR_SIZE];
  p.read_sectors(2, 1, &super_sector);
  struct Superblock super_block = *(struct Superblock*)(super_sector);
  if (super_block.signature != 0xEF53) {
    throw -1;
  }
  total_blocks = super_block.total_blocks;
  total_inodes = super_block.total_inodes;
  block_size = 1024 << super_block.log2_block_size;
}