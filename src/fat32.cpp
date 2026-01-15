#include "fat32.hpp"
#include <cstring>

FAT32::FAT32(const uint8_t* vbr) {
  BytesPerSector = *(uint16_t*) (vbr + 0xB);
  SectorsPerCluster = *(uint8_t*) (vbr + 0xD);
  ReservedSectors = *(uint16_t*) (vbr + 0xE);
  FatCopies = *(uint8_t*) (vbr + 0x10);
  RootDirEntries = *(uint16_t*) (vbr + 0x11);
  NumSectors = *(uint16_t*) (vbr + 0x13);
  MediaType = *(uint8_t*) (vbr + 0x15);
  SectorsPerFAT = *(uint32_t*) (vbr + 0x24); // For FAT32 we're reading it from extended BPB
  SectorsPerTrack = *(uint16_t*) (vbr + 0x18);
  NumberOfHeads = *(uint16_t*) (vbr + 0x1A);
  HiddenSectors = *(uint32_t*) (vbr + 0x1C);
  SectorsBig = *(uint32_t*) (vbr + 0x20);
  flags = *(uint16_t*) (vbr+0x28);
  FatVersion = *(uint16_t*) (vbr + 0x2A);
  ClustersInRoot = *(uint32_t*) (vbr + 0x2C);
  FSInfoSectorNumber = *(uint16_t*) (vbr + 0x30);
  DriveNumber = *(uint8_t*) (vbr+40);
  memcpy(Label, (vbr + 0x47), 11);
}

BasicFileRecord FAT32::readFileRecord(const uint8_t* sector, int index) {
  const void* entry = (const void*)(sector + index * FILE_ENTRY_SIZE);
  BasicFileRecord record;
  memcpy(record.filename, entry, 11);
  record.filename[11] = 0;
  record.attributes = *(uint8_t*) (entry + 11);
  record.creation_time = *(uint16_t*) (entry+14);
  record.creation_time = *(uint16_t*) (entry+16);
  record.position_in_fat = (uint32_t) (*(uint16_t*)(entry+20) << 16) 
                          | (uint32_t) (*(uint16_t*) (entry+26));
  record.size = *(uint32_t*)(entry+28);
  return record;
}