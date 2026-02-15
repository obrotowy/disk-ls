#pragma once
#include "ext2/inode.hpp"
#include "ext2/superblock.hpp"
#include "ext2/block_group.hpp"
#include "ext2/directory.hpp"
#include "Partition.hpp"
#include <vector>

class Ext2 {
  public:
    Ext2(Partition& _p);
    ~Ext2();
    uint32_t TOTAL_BLOCKS;
    uint32_t TOTAL_INODES;
    uint32_t BLOCK_SIZE;
    uint32_t BLOCKS_PER_GROUP;
    uint32_t INODES_PER_GROUP;
    size_t INODE_SIZE;
    void print_block_info(uint32_t block_n);
    void read_block(uint32_t offset, void* buffer) const;
    void write_block(uint32_t offset, const void* data);
    void read_blocks(uint32_t offset, size_t count, void* buffer) const;
    void write_blocks(uint32_t offset, size_t count, const void* data);
    inode get_inode(const uint32_t& inode);
    std::vector<directory_entry> list_root_directory();
    std::vector<directory_entry> list_directory(uint32_t block_n);
    block_group_descriptor* bgdt;
  private:
    Partition& p;
    const LBA block_to_LBA(const uint32_t block) const;
    friend std::ostream& operator<<(std::ostream& os, const Ext2& fs);
};