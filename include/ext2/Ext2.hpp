#pragma once
#include "FileSystem.hpp"
#include "File.hpp"
#include "ext2/inode.hpp"
#include "ext2/superblock.hpp"
#include "ext2/block_group.hpp"
#include "ext2/directory.hpp"
#include "Partition.hpp"
#include <vector>

class Ext2 : public FileSystem {
  public:
    Ext2(Partition& _p);
    const char* read_file(const std::string& path);
    const std::vector<File> list_directory(const std::string& path);
    std::vector<directory_entry> list_directory(uint32_t block_n);
    ~Ext2();
  private:
    uint32_t TOTAL_BLOCKS;
    uint32_t TOTAL_INODES;
    uint32_t BLOCK_SIZE;
    uint32_t BLOCKS_PER_GROUP;
    uint32_t INODES_PER_GROUP;
    block_group_descriptor* bgdt;
    size_t INODE_SIZE;
    Partition& p;
    uint32_t traverse_path(const std::string& path_elements, const int& starting_inode);
    inode_t get_inode(const uint32_t& inode);
    const char* read_file(const inode_t& fd);
    void print_block_info(uint32_t block_n);
    void read_block(uint32_t offset, void* buffer) const;
    void write_block(uint32_t offset, const void* data);
    void read_blocks(uint32_t offset, size_t count, void* buffer) const;
    void write_blocks(uint32_t offset, size_t count, const void* data);
    const LBA block_to_LBA(const uint32_t block) const;
    friend std::ostream& operator<<(std::ostream& os, const Ext2& fs);
};
