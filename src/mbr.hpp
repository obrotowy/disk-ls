#pragma once
#include <cstdint>
#include <vector>
#include "Disk.hpp"

#define PARTITION_BOOTABLE 0x80

enum PartitionType : uint8_t {
  EMPTY     = 0,
  FAT32_LBA = 0xC,
  EXT2      = 0x83
};

typedef struct PartitionEntry {
  bool bootable;
  PartitionType type;
  LBA begin;
  size_t size;
} Partition;

std::vector<Partition> enumerate_partitions(const Disk d);
void describe_partition(const Partition& p);