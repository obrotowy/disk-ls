#include "ext2/Ext2.hpp"
#include <cstring>
#include <algorithm>

Ext2::Ext2(Partition& _p) : p(_p) {
  uint8_t super_sector[SECTOR_SIZE];
  p.read_sectors(2, 1, &super_sector);
  struct Superblock super_block = *(struct Superblock*)(super_sector);
  if (super_block.signature != 0xEF53) {
    throw -1;
  }
  if (super_block.major_version == 0) {
    INODE_SIZE = 128;
  } else {
    INODE_SIZE = *(uint32_t*)(super_sector + 88);
  }
  TOTAL_BLOCKS = super_block.total_blocks;
  TOTAL_INODES = super_block.total_inodes;
  BLOCK_SIZE = 1024 << super_block.log2_block_size;
  BLOCKS_PER_GROUP = super_block.blocks_per_group;
  INODES_PER_GROUP = super_block.inodes_per_group;
  bgdt = new block_group_descriptor[TOTAL_BLOCKS / BLOCKS_PER_GROUP];
  const uint32_t bgdt_size_in_blocks = ((TOTAL_BLOCKS / BLOCKS_PER_GROUP) * sizeof(block_group_descriptor)) / BLOCK_SIZE;
  const uint32_t bgdt_offset = (BLOCK_SIZE == 1024) ? 2 : 1;
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
  return (BLOCK_SIZE / SECTOR_SIZE) * block;
}

void Ext2::read_block(uint32_t offset, void* buffer) const {
  return p.read_sectors(block_to_LBA(offset), BLOCK_SIZE/SECTOR_SIZE, buffer);
}

void Ext2::write_block(uint32_t offset, const void* buffer) {
  return p.write_sectors(block_to_LBA(offset), BLOCK_SIZE/SECTOR_SIZE, buffer);
}

void Ext2::read_blocks(uint32_t offset, size_t count, void* buffer) const {
  return p.read_sectors(block_to_LBA(offset), (BLOCK_SIZE/SECTOR_SIZE) * count, buffer);
}

void Ext2::write_blocks(uint32_t offset, size_t count, const void* buffer) {
  return p.write_sectors(block_to_LBA(offset), (BLOCK_SIZE/SECTOR_SIZE) * count, buffer);
}

std::ostream& operator<<(std::ostream& os, const Ext2& fs) {
  os << std::dec;
  os << "Ext2 partition:" << std::endl;
  os << "\tBlock Size: " << fs.BLOCK_SIZE << std::endl;
  os << "\tBlocks per group: " << fs.BLOCKS_PER_GROUP << std::endl;
  os << "\tTotal blocks: " << fs.TOTAL_BLOCKS << std::endl;
  os << "\tInode size: " << fs.INODE_SIZE << std::endl;
  os << "\tInodes per group: " << fs.INODES_PER_GROUP << std::endl;
  return os;
}

std::vector<directory_entry> Ext2::list_root_directory() {
  inode_t inode_table[BLOCK_SIZE / INODE_SIZE];
  read_block(bgdt[0].inode_table_address, inode_table);
  return list_directory(inode_table[1].direct_block_pointer[0]);
}

std::vector<directory_entry> Ext2::list_directory(uint32_t inode_n) {
  inode_t inode = get_inode(inode_n);
  const uint32_t& block_n = inode.direct_block_pointer[0];
  uint8_t dir_block[BLOCK_SIZE];
  read_block(block_n, dir_block);
  uint8_t* p = dir_block;
  std::vector<directory_entry> files;
  while (p - dir_block < BLOCK_SIZE) {
    uint32_t inode = *(uint32_t*)(p);
    uint16_t entry_size = *(uint16_t*)(p+4);
    uint8_t name_length = p[6];
    uint8_t type = p[7];
    char* name = new char[name_length+1];
    memcpy(name, p+8, name_length);
    name[name_length] = 0;
    directory_entry file = {.inode = inode, .type = type, .entry_size = entry_size, .name = std::string(name)};
    files.push_back(file);
    delete[] name;
    p += entry_size;
  }
  return files;
}

inode_t Ext2::get_inode(const uint32_t& inode_n) {
  const unsigned block_group = (inode_n - 1) / INODES_PER_GROUP;
  const unsigned index = (inode_n - 1) % INODES_PER_GROUP;
  const unsigned inode_table_block = (index * INODE_SIZE) / BLOCK_SIZE;
  const unsigned index_in_block = (inode_n - 1) % (BLOCK_SIZE / INODE_SIZE);
  inode_t inode_table[BLOCK_SIZE / INODE_SIZE];
  read_block(bgdt[block_group].inode_table_address + inode_table_block, inode_table);
  return inode_table[index_in_block];
}

uint32_t Ext2::traverse_path(const std::vector<std::string>& path_elements, const int& starting_inode = 2) {
  int current_inode = starting_inode;
  for (const auto& e: path_elements) {
    std::vector<directory_entry> curr_dir_listing = list_directory(current_inode);
    auto target = std::find_if(curr_dir_listing.begin(), curr_dir_listing.end(), [&](directory_entry& c){return c.name == e;});
    if (target == curr_dir_listing.end())
      throw std::exception();
    current_inode = target->inode;
  }
  return current_inode;
}

const char* Ext2::readfile(const inode_t& fd) {
  const size_t& fsize = fd.size_lower;
  if (fsize > BLOCK_SIZE * 12)
    // File don't fit in direct block pointers
    throw std::exception();

  char* output_buf = new char[fsize];
  int whole_blocks = fsize / BLOCK_SIZE;
  int remainder = fsize % BLOCK_SIZE;

  // Read whole blocks
  for (int i = 0; i<whole_blocks; ++i) {
    read_block(fd.direct_block_pointer[i], output_buf+i*BLOCK_SIZE);
  }

  // Read the remainder from last block of file
  char tmp_buf[BLOCK_SIZE];
  read_block(fd.direct_block_pointer[whole_blocks], tmp_buf);
  memcpy(output_buf + whole_blocks*BLOCK_SIZE, tmp_buf, remainder);
  
  return output_buf;
}