#include "ext2/Ext2.hpp"
#include <cstring>

Ext2::Ext2(Partition& _p) : p(_p) {
  uint8_t super_sector[SECTOR_SIZE];
  p.read_sectors(2, 1, &super_sector);
  struct Superblock super_block = *(struct Superblock*)(super_sector);
  if (super_block.signature != 0xEF53) {
    throw -1;
  }
  if (super_block.major_version == 0) {
    inode_size = 128;
  } else {
    inode_size = *(uint32_t*)(super_sector + 88);
  }
  total_blocks = super_block.total_blocks;
  total_inodes = super_block.total_inodes;
  block_size = 1024 << super_block.log2_block_size;
  blocks_per_group = super_block.blocks_per_group;
  bgdt = new block_group_descriptor[total_blocks / blocks_per_group];
  const uint32_t bgdt_size_in_blocks = ((total_blocks / blocks_per_group) * sizeof(block_group_descriptor)) / block_size;
  const uint32_t bgdt_offset = (block_size == 1024) ? 2 : 1;
  read_blocks(bgdt_offset, bgdt_size_in_blocks, bgdt);
}

Ext2::~Ext2() {
  delete[] bgdt;
}

void Ext2::print_block_info(uint32_t block_n) {
  printf("Block no. %d:\n", block_n);
  printf("Inode table address: 0x%X\n", bgdt[block_n].inode_table_address);
}

const LBA Ext2::block_to_LBA(const uint32_t block) const {
  return (block_size / SECTOR_SIZE) * block;
}

void Ext2::read_block(uint32_t offset, void* buffer) const {
  return p.read_sectors(block_to_LBA(offset), block_size/SECTOR_SIZE, buffer);
}

void Ext2::write_block(uint32_t offset, const void* buffer) {
  return p.write_sectors(block_to_LBA(offset), block_size/SECTOR_SIZE, buffer);
}

void Ext2::read_blocks(uint32_t offset, size_t count, void* buffer) const {
  return p.read_sectors(block_to_LBA(offset), (block_size/SECTOR_SIZE) * count, buffer);
}

void Ext2::write_blocks(uint32_t offset, size_t count, const void* buffer) {
  return p.write_sectors(block_to_LBA(offset), (block_size/SECTOR_SIZE) * count, buffer);
}

std::ostream& operator<<(std::ostream& os, const Ext2& fs) {
  os << "Ext2 partition:" << std::endl;
  os << "\tBlock Size: " << fs.block_size << std::endl;
  os << "\tBlocks per group: " << fs.blocks_per_group << std::endl;
  os << "\tTotal blocks: " << fs.total_blocks << std::endl;
  os << "\tInode size: " << fs.inode_size << std::endl;
  return os;
}

std::vector<directory_entry> Ext2::list_root_directory() {
  inode inode_table[block_size / inode_size];
  read_block(bgdt[0].inode_table_address, inode_table);
  return list_directory(inode_table[1].direct_block_pointer[0]);
}

std::vector<directory_entry> Ext2::list_directory(uint32_t block_n) {
  uint8_t dir_block[block_size];
  read_block(block_n, dir_block);
  uint8_t* p = dir_block;
  std::vector<directory_entry> files;
  while (p - dir_block < block_size) {
    uint32_t inode = *(uint32_t*)(p);
    uint16_t entry_size = *(uint16_t*)(p+4);
    uint8_t name_length = p[6];
    uint8_t type = p[7];
    char* name = new char[name_length];
    memcpy(name, p+8, name_length);
    directory_entry file = {.inode = inode, .type = type, .entry_size = entry_size, .name = std::string(name)};
    files.push_back(file);
    delete[] name;
    p += entry_size;
  }
  return files;
}