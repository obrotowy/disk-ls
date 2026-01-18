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

class Ext2 {
  public:
    Ext2(Partition& _p);
    uint32_t total_blocks;
    uint32_t total_inodes;
    uint32_t block_size;

  private:
    Partition& p;
};