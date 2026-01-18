#include "Disk.hpp"
#include <iostream>
#include <vector>

#define PARTITION_BOOTABLE 0x80
enum PartitionType : uint8_t {
  EMPTY     = 0,
  FAT32_LBA = 0xC,
  EXT2      = 0x83
};

struct __attribute__((packed)) PartitionEntry{
  uint8_t bootable;
  uint8_t start_head;
  uint8_t start_sector;
  uint8_t start_cylinder;
  PartitionType type;
  uint8_t end_head;
  uint8_t end_sector;
  uint8_t end_cylinder;
  LBA begin;
  uint32_t size;
};

class Partition {
  public:
    Partition(Disk& parent_disk, struct PartitionEntry* p);
    void read_sectors(LBA offset, size_t count, void* buffer) const;
    void write_sectors(LBA offset, size_t count, const void* data);
    friend std::ostream& operator<<(std::ostream& os, const Partition& p);
    bool bootable;
    PartitionType type;
    LBA begin;
    uint32_t size;
  private:
    Disk& parent;
};

std::vector<Partition> enumerate_partitions(Disk& d);