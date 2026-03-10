#include <fat/FAT32.hpp>

FAT32::FAT32(Partition& p) {
  uint8_t buffer[SECTOR_SIZE];
  p.read_sectors(0, 1, &buffer);
  FAT32_BootRecord boot_record = *(FAT32_BootRecord*)(buffer);
  BYTES_PER_SECTOR = boot_record.BIOS_Parameter_block.BYTES_PER_SECTOR;
  SECTORS_PER_CLUSTER = boot_record.BIOS_Parameter_block.SECTORS_PER_CLUSTER;
  RESERVED_SECTORS = boot_record.BIOS_Parameter_block.RESERVED_SECTORS;
  FAT_COUNT = boot_record.BIOS_Parameter_block.FAT_COUNT;
  SECTORS_PER_FAT = boot_record.SECTORS_PER_FAT;
}

const std::vector<char> FAT32::read_file(const std::filesystem::path& path) {};
const std::vector<std::unique_ptr<File>> FAT32::list_directory(const std::filesystem::path& path) {};
std::unique_ptr<File> FAT32::get_file_descriptor(const std::filesystem::path& path) {};
FAT32::~FAT32() {};