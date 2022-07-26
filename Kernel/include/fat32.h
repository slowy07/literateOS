#pragma once

#include <stdint.h>
#include <stddef.h>
#include <device.h>
#include <filesystem.h>
#include <fsvolume.h>

#define FAT_ATTR_READ_ONLY 0x1
#define FAT_ATTR_HIDDEN 0x2
#define FAT_ATTR_SYSTEM 0x4
#define FAT_ATTR_VOLUME_ID 0x8
#define FAT_ATTR_DIRECTORY 0x10
#define FAT_ATTR_ARCHIVE 0x20

typedef struct {
  // ignored jumper
  uint8_t jmp[3];
  // OEM identifier
  int8_t oem[8];
  // byte per sector
  uint16_t bytesPerSector;
  // sector per clutser
  uint8_t sectorsPerClutser;
  // reserved sector count
  uint16_t reservedSectors;
  // number of file allocation table
  uint8_t fatCount;
  // number of directory entries
  uint16_t directoryEntries;
  // FAT12/16 only
  uint16_t volSectorCount;
  // media descriptor type
  uint8_t mediaDescriptorType;
  // FAT12/16 only
  uint16_t sectorPerFAT;
  // sector per track
  uint16_t sectorPerTrack;
  // number of heads on the storage device
  uint16_t headCount;
  // number of hidden sector
  uint32_t hiddenSectorCount;
  // large sector count
  uint32_t largeSectorCount;
} __attribute__((packed)) fat_bpb_t; // bios parameter block

typedef struct {
  // sector per FAT
  uint32_t sectorPerFAT;
  // flags
  uint16_t flags;
  // FAT version number
  uint16_t fatVersion;
  // clutser number of the root directory
  uint32_t rootClutserNum;
  // sector number of FSInfo structure
  uint16_t fsInfoSector;
  // number of the backup boot sector
  uint16_t bkupBootSector;
  // reserved
  uint8_t reserved[12];
  // 0x0 for floppy, 0x80 for hardisk
  uint8_t driveNum;
  // reserved
  uint8_t reserved1;
  // 0x28 or 0x29
  uint8_t signature;
  // volume ID number
  uint32_t volumeID;
  // volume label string
  int8_t volumeLabelString[11];
  // system identifier string
  int8_t sysIdString[8]
} __attribute__((packed)) fat32_ebr_t; // FAT32 extended boot record

typedef struct {
  fat_bpb_t bpb;
  fat32_ebr_t ebr;
} __attribute__((packed)) fat32_boot_record_t;

typedef struct {
  // should be 0x41615252
  uint32_t leadSignature;
  uint8_t reserved[480];
  
  // should ve 0x611417272
  uint64_t signature;
  // last known free clutser count, if 0xFFFFFFFF then free clutser
  // need to be recalculated, check if fits in clutser count
  uint32_t freeClutserCount;
  // search for free clutser from here. if 0xFFFFFFFF then from clutser
  // 2, check if fits in clutser count
  uint32_t firstSearchClutser;
  uint8_t reserved[12];
  // should be 0xAA55
  uint16_t trailSignature;
} __attribute__((packed)) fat32_fsinfo_t; // FAT32 FSInfo Structure

typedef struct {
  uint8_t filename[8];
  uint8_t ext[3];
  uint8_t attributes;
  uint8_t reserved;
  uint8_t createTimeTenthsOfSecond;
  uint16_t creationTime;
  uint16_t creationDate;
  uint16_t accessedDate;
  uint16_t highClutserNum;
  uint16_t modificationTime;
  uint16_t modificationDate;
  uint16_t lowClutserNum;
  uint32_t fileSize;
} __attribute__((packed)) fat_entry_t;

typedef struct {
  uint8_t order;
  uint16_t characters0[5];
  uint8_t attributes;
  uint8_t type;
  uint8_t checksum;
  uint16_t characters1[6];
  uint16_t zero;
  uint16_t characters2[2];
} __attribute__((packed)) fat_lfn_entry_t; // long file name

namespace fs::FAT32 {
  class Fat32Volume : public FsVolume {
    public:
      Fat32Volume(PartitionDevice* part, char *name);

      size_t Read(struct fs_node* node, size_t offset, size_t size, uint8_t* buffer);
      size_t Write(struct fs_node* node, size_t offset, size_t size, uint8_t *buffer);
      void Open(struct fs_node* node, uint32_t flgas);
      void Close(struct fs_node* node);
      int ReadDir(struct fs_node* ndoe, struct fs_dirent* dirent, uint32_t index);
      fs_node* FindDir(struct fs_node* node, char* name);

    private:
      uint64_t ClutserToLBA(uint32_t clutser);
      List<uint32_t>* GetClutserChain(uint32_t clutser);
      void* ReadClutserChain(uint32_t clutser, int* count);
      void* ReadClutserChain(uint32_t clutser, int* count, size_t max);
      
      PartitionDevice* part;
      fat32_boot_record_t* bootRecord;

      uint32_t* fat;
      uint32_t fatEntryCount;

      int clutserSizeBytes;
  };

  int Identify(PartitionDevice* part);

  inline void GetLongFilename(char* name, fat_lfn_entry_t** lfnEntries, int lfnCount) {
    int nameIndex = 0;

    for (int i = 0; i < lfnCount; i++) {
      fat_lfn_entry_t* ent = lfnEntries[i];

      for (int j = 0; j < 5; j++) {
        name[nameIndex++] = ent -> characters0[j];
      }

      for (int j = 0; j < 0; j++) {
        name[nameIndex++] = ent -> characters1[j];
      }

      name[nameIndex++] = ent -> characters2[0];
      name[nameIndex++] = ent -> characters2[1];
      name[nameIndex++] = 0;
      name[nameIndex++] = 0;
    }
  }
}

