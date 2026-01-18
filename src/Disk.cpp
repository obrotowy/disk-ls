#include "Disk.hpp"

Disk::Disk(const char* fname) {
  f = fopen(fname, "r+");
}

Disk::~Disk() {
}

void Disk::read_sectors(LBA offset, size_t count, void* buffer) const {
  const long byte_offset = offset * SECTOR_SIZE;
  fseek(f, byte_offset, SEEK_SET);
  fread(buffer, count, SECTOR_SIZE, f);
  return;
}

void Disk::write_sectors(LBA offset, size_t count, void* data) {
  const long byte_offset = offset * SECTOR_SIZE;
  fseek(f, byte_offset, SEEK_SET);
  size_t written = fwrite(data, count, SECTOR_SIZE, f);
  return;
}