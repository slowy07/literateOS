#pragma once

#include <stdint.h>
#include <filesystem.h>
#include <fsvolume.h>

#define LITERATEINITFS_FILE 0x01
#define LITERATEINITFS_DIRECTORY 0x02
#define LITERATEINITFS_SYMLINK 0x03
#define LITERATEINITFS_MOUNTPOINT 0x04

typedef struct {
  // amount of file present
  uint32_t fileCount;
  // version string
  char versionString[16];
} __attribute__((packed)) literateinitfs_header_t;

typedef struct {
  // check for corruption
  uint16_t magic;
  // filename
  char filename[32];
  // offset in file
  uint32_t offset;
  // file size
  uint32_t size;
  uint8_t type
} __attribute__((packed)) literateinitfs_node_t;


namespace Initrd {
  class InitrdVolume: public fs::FsVolume {
    public:
      InitrdVolume();

      size_t Read(struct fs_node* node, size_t offset, size_t size, uint8_t *buffer);
      size_t Write(struct fs_node* node, size_t offset, size_t size, uint8_t *buffer);
      void Open(struct fs_node* node, uint32_t flags);
      void Close(struct fs_node* node);
      int ReadDir(struct fs_node* node, struct fs_dirent* dirent, uint32_t index);
      fs_node* FindDir(struct fs_node* node, char* name);
  };

  void Initialize(uintptr_t address, uint32_t size);
  
  literateinitfs_node_t* List();
  literateinitfs_header_t* GetHeader();
  void* Read(int node);
  void* Read(char* filename);

  literateinitfs_node_t GetNode(char* filename);
  uint8_t* Read(literateinitfs_node_t node, uint8_t* buffer);
}

