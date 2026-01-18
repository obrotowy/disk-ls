#include "Partition.hpp"

Partition::Partition(Disk& parent_disk, struct PartitionEntry* p)
  : parent(parent_disk), bootable(p->bootable == PARTITION_BOOTABLE),
    type(p->type), begin(p->begin), size(p->size) {}

void Partition::read_sectors(LBA offset, size_t count, void* buffer) const {
  if (offset + count > size) {
    throw std::out_of_range("Read outside partition");
  }
  return parent.read_sectors(begin + offset, count, buffer);
}

void Partition::write_sectors(LBA offset, size_t count, const void* data) {
  if (offset + count > size) {
    throw std::out_of_range("Write outside partition");
  }
  return parent.write_sectors(begin + offset, count, data);
}

std::ostream& operator<<(std::ostream& os, const Partition& p) {
  os << "Bootable: " << ((p.bootable) ? "Yes" : "No") << std::endl;
  os << "Type: 0x" << std::hex << static_cast<unsigned>(p.type) << std::endl;
  os << "Begin LBA: 0x" << std::hex << p.begin << std::endl;
  os << "Size: " << std::dec << p.size << " sectors\n";
  return os;
}

std::vector<Partition> enumerate_partitions(Disk& d) {
  std::vector<Partition> partitions;
  uint8_t mbr[512];
  d.read_sectors(0, 1, &mbr);
  struct PartitionEntry* partition_table = (struct PartitionEntry*)(mbr + 0x1BE);
  for (int i = 0; i<4; ++i) {
    Partition p(d, (struct PartitionEntry*)(&partition_table[i]));
    if (p.type != EMPTY)
      partitions.push_back(p);
  }
  return partitions;
}