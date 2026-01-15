#pragma once
#include "partition.hpp"

#define FILE_ENTRY_SIZE 32
typedef struct BasicFileRecord{
  char filename[12];
  uint8_t attributes;
  uint16_t creation_time;
  uint16_t creation_date;
  uint16_t last_accessed_date;
  uint32_t position_in_fat;
  uint32_t size;
} BasicFileRecord;

class FAT32 {
  public:
  FAT32(const uint8_t* vbr);
  BasicFileRecord readFileRecord(const uint8_t* sector, int index);
  uint16_t BytesPerSector;
  uint8_t SectorsPerCluster;
  uint16_t ReservedSectors;
  uint8_t FatCopies;
  uint16_t RootDirEntries;
  uint16_t NumSectors;
  uint8_t MediaType;
  uint32_t SectorsPerFAT;
  uint16_t SectorsPerTrack;
  uint16_t NumberOfHeads;
  uint32_t HiddenSectors;
  uint32_t SectorsBig;
  uint16_t flags;
  uint16_t FatVersion;
  uint32_t ClustersInRoot;
  uint16_t FSInfoSectorNumber;
  uint8_t DriveNumber;
  char Label[11];
};

