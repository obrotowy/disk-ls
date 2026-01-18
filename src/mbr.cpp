#include "mbr.hpp"

Partition parse_partition_entry(void* data) {
  Partition p;
  p.bootable = (*(uint8_t*)(data) == PARTITION_BOOTABLE);
  p.type = static_cast<PartitionType>(*(uint8_t*)(data+0x4));
  p.begin = (*(LBA*)(data + 0x8));
  p.size = (*(uint32_t*)(data + 0xC));
  return p;
}

std::vector<Partition> enumerate_partitions(Disk d) {
  std::vector<Partition> partitions;
  uint8_t mbr[512];
  d.read_sectors(0, 1, &mbr);
  uint16_t signature = *(uint16_t*)(mbr+510);
  if (signature != 0xAA55) {
    printf("ERROR: Invalid MBR Signature: 0x%X\n", signature);
    return partitions;
  }
  for (int i = 0x1BE; i<510; i += 16) {
    Partition p = parse_partition_entry(mbr+i);
    if (p.type != EMPTY)
      partitions.push_back(p);
  }
  return partitions;
}


void describe_partition(const Partition& p) {
  printf("Bootable: %s\n", (p.bootable) ? "Yes" : "No");
  printf("Type: 0x%X\n", p.type);
  printf("Begin LBA: 0x%X\n", p.begin);
  printf("Size: %d sectors\n", p.size);
}