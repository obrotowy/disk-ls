#pragma once

#include <cstdint>

typedef struct __attribute__((packed)) block_group_descriptor {
  uint32_t block_usage_bitmap;
  uint32_t inode_usage_bitmap;
  uint32_t inode_table_address;
  uint16_t unallocated_blocks;
  uint16_t unallocated_inodes;
  uint16_t directories;
} block_group_descriptor;
