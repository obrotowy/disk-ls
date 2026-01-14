#pragma once
#include <cstdint>

#define MBR_MAX_PARTITIONS 4
#define MBR_MAGIC_BYTES 0x55AA

enum partition_type : uint8_t {
  FAT32_LBA = 0xC
};

typedef uint32_t CHS;   // TODO: Implement CHS reading
typedef uint32_t LBA;   // LBA może być dłuższe

typedef struct partition{
  bool bootable;
  CHS first_sector_chs;
  partition_type type;
  CHS last_sector_chs;
  LBA first_sector_lba;
  uint32_t sector_count;
} partition;

uint16_t get_signature(const uint8_t* data);
partition get_partition_info(const void* data);