#pragma once
#include <cstdint>
#include <cstdio>

typedef uint32_t LBA;
constexpr size_t SECTOR_SIZE = 512;

class Disk {
  public:
    Disk(const char* fname);
    ~Disk();
    void read_sectors(LBA offset, size_t count, void* buffer) const;
    void write_sectors(LBA offset, size_t count, void* data);
  private:
    std::FILE* f;
};