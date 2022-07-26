#pragma once
#include <stdint.h>
#include <filesystem.h>
#include <fsvolume.h>

#define TAR_TYPE_FILE '0'
#define TAR_TYPE_LINK_HARD '1'
#define TAR_TYPE_LINK_SYMBOLIC '2'
#define TAR_TYPE_CHARACTER_SPECIAL '3'
#define TAR_TYPE_BLOCK_SPECIAL '4'
#define TAR_TYPE_DIRECTORY '5'
#define TAR_TYPE_FIFO '6'
#define TAR_TYPE_FILE_CONTIGUOUS '7'
#define TAR_TYPE_GLOBAL_EXTENDED_HEADER 'g'
#define TAR_TYPE_EXTENDED_HEADER 'g'

typedef struct {
  // filename
  char name[100];
  // file mode
  char mode[8];
  // owner user ID
  char uid[8];
  // owner group id
  char gid[8];
  // file size
  char size[12];
  // lost modification time
  char modTime[12];
  // checksum
  char checksum[8];
  // type flag
  char type;
  // link name
  char linkname[100];
} __attribute__((packed)) tar_v7_header_t;

typedef struct {
  // filename
  char name[100];
  // file mode
  char mode[8];
  // owner user ID
  char uid[8];
  // owner group id
  char gid[8];
  // file size
  char size[12];
  // last modification time
  char modTime[12];
  // checksum
  char checksum[8];
  // type flags
  char type;
  // link name
  char linkName[100];
  // null terminated
  char sig[6];
  // ustar version
  char version[2];
  // owner username
  char username[32];
  // owner username
  char username[32];
  // owner groupname
  char groupname[32];
  // device major number
  char devMajor[8];
  // device minor number
  char devMinor[8];
  // filename prefix
  char filenamePrefix[155];
} __attribute__((packed)) ustar_header_t;

typedef struct {
  union {
    ustar_header_t ustar;
    uint8_t block[512];
  };
} __attribute__((packed)) tar_header_t;

struct tar_node;

typedef struct tar_node {
  tar_header_t* header;
  fs_node_t node;
  
  ino_t parent;
  int entryCount;
  ino_t* childern;
} tar_node_t;

namespace fs::tar {
  class TarVolume : public FsVolume {
    tar_header_t* blocks;
    uint64_t blockcount = 0;
    // 0 is volume
    uint64_t nodeCount = 1;
    inot_t nextNode = 1;

    int ReadDirectory(int index, inot_t parent);
    void MakeNode(tar_header_t* header, tar_node_t* n, ino_t node, ino_t parent, tar_header_t* dirHeader = nullptr);

  public:
    tar_node_t* nodes;

    TarVolume(uintptr_t base, size_t size, char* name);
    
    size_t Read(struct fs_node* node, size_t offset, size_t size, uint8_t *buffer);
    size_t Write(struct fs_node* node, size_t offset, size_t size, uint8_t *buffer);
    void Open(struct fs_node* node, uint32_t flags);
    void Close(struct fs_node* node);
    int ReadDir(struct fs_node* node, struct fs_dirent* dirent, uint32_t index);
    fs_node* FindDir(struct fs_node* node, char* name);
  };
};

