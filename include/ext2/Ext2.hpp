#pragma once
#include "ext2/inode.hpp"
#include "ext2/superblock.hpp"
#include "ext2/block_group.hpp"
#include "Partition.hpp"

class Ext2 {
  public:
    Ext2(Partition& _p);
    ~Ext2();
    uint32_t total_blocks;
    uint32_t total_inodes;
    uint32_t block_size;
    uint32_t blocks_per_group;
    void print_block_info(uint32_t block_n);
    void read_block(uint32_t offset, void* buffer) const;
    void write_block(uint32_t offset, const void* data);
    void read_blocks(uint32_t offset, size_t count, void* buffer) const;
    void write_blocks(uint32_t offset, size_t count, const void* data);
    block_group_descriptor* bgdt;
    size_t inode_size;
  private:
    Partition& p;
    const LBA block_to_LBA(const uint32_t block) const;
    friend std::ostream& operator<<(std::ostream& os, const Ext2& fs);
};