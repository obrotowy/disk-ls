#pragma once
#include <cstdint>

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