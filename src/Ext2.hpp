#pragma once
#include "Partition.hpp"

struct __attribute__((packed)) Superblock{
  uint32_t total_inodes;
  uint32_t total_blocks;
  uint32_t reserved_blocks;
  uint32_t unallocated_blocks;
  uint32_t unallocated_inodes;
  uint32_t superblock_begin;
  uint32_t log2_block_size;
  uint32_t log2_fragment_size;
  uint32_t blocks_per_group;
  uint32_t fragments_per_group;
  uint32_t inodes_per_group;
  uint32_t last_mount_time;
  uint32_t last_write_time;
  uint16_t mounts_since_fsck;
  uint16_t mounts_before_fsck;
  uint16_t signature;
  uint16_t fs_state;
  uint16_t error_handling;
  uint16_t minor_version;
  uint32_t last_fsck_time;
  uint32_t interval_between_fsck;
  uint32_t os_id;
  uint32_t major_version;
  uint16_t user_id_reserved_blocks;
  uint16_t group_id_reserved_blocks;
};

typedef struct __attribute__((packed)) block_group_descriptor {
  uint32_t block_usage_bitmap;
  uint32_t inode_usage_bitmap;
  uint32_t inode_table_address;
  uint16_t unallocated_blocks;
  uint16_t unallocated_inodes;
  uint16_t directories;
} block_group_descriptor;

typedef struct __attribute__((packed)) inode {
  uint16_t type_and_permissions;
  uint16_t user_id;
  uint32_t size_lower;
  uint32_t last_access_time;
  uint32_t creation_time;
  uint32_t last_modification_time;
  uint32_t deletion_time;
  uint16_t group_id;
  uint16_t hard_links_count;
  uint32_t disk_sectors_used;
  uint32_t flags;
  uint32_t os_specific;
  uint32_t direct_block_pointer[12];
  uint32_t singly_indirect_block_pointer;
  uint32_t doubly_indirect_block_pointer;
  uint32_t triply_indirect_block_pointer;
  uint32_t generation_number;
  uint32_t reserved[2];
  uint32_t block_address_of_fragment;
  uint32_t os_specific_value[3];
} inode;

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