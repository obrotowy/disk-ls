#include "disk.hpp"
#include "partition.hpp"
#include "fat32.hpp"
#include <cstdio>

int main(int argc, const char** argv) {
  if (argc < 2) {
    puts("Usage: ./disk-ls <img_file>\n");
    exit(-1);
  }
  Disk disk(argv[1]);
  for (Partition part: disk.enumerate_partitions()) {
    printf("Partition found:\n");
    printf("\tBootable: %s\n", (part.bootable ? "Yes" : "No"));
    printf("\tType: 0x%x\n", part.type);
    printf("\tSize: %d sectors\n", part.size);
    printf("\tBegin LBA: 0x%x\n", part.begin_lba);
    FAT32 fs(part.read_sector(0));
    printf("FAT32 Information:\n");
    printf("\tVolume Label: %s\n", fs.Label);
    printf("\tCluster size: %d sectors\n", fs.SectorsPerCluster);
    printf("\tReserved Sectors: %d\n", fs.ReservedSectors);
    printf("\tFAT copies: %d\n", fs.FatCopies);
    printf("\tFAT Size: %d sectors\n", fs.SectorsPerFAT);
    const uint32_t fs_data_offset = fs.ReservedSectors + fs.FatCopies * fs.SectorsPerFAT;
    const uint8_t* root_dir_sector = part.read_sector(fs_data_offset);
    BasicFileRecord file = fs.readFileRecord(root_dir_sector, 1);
    printf("Short file name: %s\n", file.filename);
    printf("Position in FAT: %d\n", file.position_in_fat);
    printf("File size: %d B\n", file.size);
    const uint8_t* file_data = part.read_sector(fs_data_offset + (file.position_in_fat - 2) * fs.SectorsPerCluster);
    printf("File data: %s\n", file_data);
  }
}