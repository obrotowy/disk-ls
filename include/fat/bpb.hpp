#pragma once
#include <cstdint>

typedef struct __attribute__((packed)) BPB{
  uint8_t reserved[3];
  char oem_identifier[8];
  uint16_t BYTES_PER_SECTOR;
  uint8_t SECTORS_PER_CLUSTER;
  uint16_t RESERVED_SECTORS;
  uint8_t FAT_COUNT;
  uint16_t root_dir_entries;
  uint16_t TOTAL_SECTORS;
  uint8_t MEDIA_DESCRIPTOR_TYPE;
  uint16_t SECTORS_PER_FAT_16ONLY;
  uint16_t SECTORS_PER_TRACK;
  uint16_t HEADS;
  uint32_t HIDDEN_SECTORS;
  uint32_t LARGE_SECTOR_COUNT;
} BPB;

typedef struct __attribute__((packed)) FAT16_BootRecord {
  BPB BIOS_Parameter_block;
  uint8_t DRIVE_NUMBER;
  uint8_t NT_FLAGS;
  uint8_t SIGNATURE;
  uint32_t VOLUME_SERIAL_NUMBER;
  char VOLUME_LABEL[11];
  char SYSTEM_IDENTIFIER[8];
  uint8_t BOOTCODE[448];
  uint16_t MBR_SIGNATURE;
} FAT16_BootRecord;

typedef struct __attribute__((packed)) FAT32_BootRecord {
  BPB BIOS_Parameter_block;
  uint32_t SECTORS_PER_FAT;
  uint16_t flags;
  uint16_t FAT_VERSION;
  uint32_t ROOT_DIRECTORY_CLUSTER;
  uint16_t FSINFO_STRUCT_SECTOR;
  uint16_t BACKUP_BOOT_SECTOR;
  uint8_t DRIVE_NUMBER;
  uint32_t VOLUME_SERIAL_NUMBER;
  char VOLUME_LABEL[11];
  char SYSTEM_IDENTIFIER[8];  // Should always be equal to "FAT32   "
  uint8_t BOOTCODE[420];
  uint16_t MBR_SIGNATURE;
} FAT32_BootRecord;