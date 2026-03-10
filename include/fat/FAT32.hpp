#pragma once
#include "FileSystem.hpp"
#include "Partition.hpp"
#include "File.hpp"
#include "fat/bpb.hpp"
#include <memory>
#include <vector>

class FAT32 : public FileSystem {
  public:
    FAT32(Partition& p);
    const std::vector<char> read_file(const std::filesystem::path& path);
    const std::vector<std::unique_ptr<File>> list_directory(const std::filesystem::path& path);
    std::unique_ptr<File> get_file_descriptor(const std::filesystem::path& path);
    ~FAT32();
  private:
    uint16_t BYTES_PER_SECTOR;
    uint8_t SECTORS_PER_CLUSTER;
    uint16_t RESERVED_SECTORS;
    uint8_t FAT_COUNT;
    uint16_t TOTAL_SECTORS;
    uint32_t SECTORS_PER_FAT;
};
