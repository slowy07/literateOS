#pragma once

#include <stdint.h>
#include <stddef.h>

#include <list.h>

#define PATH_MAX 4096

#define FS_NODE_FILE 0x1
#define FS_NODE_DIRECTORY 0x2
#define FS_NODE_MOUNTPOINT 0x8
#define FS_NODE_BLKDEVICE 0x10
#define FS_NODE_SYMLINK 0x20
#define FS_NODE_CHARDEVICE 0x40

#define S_IFMT 0x0F000
#define S_IFBLK 0x06000
#define S_IFCHR 0x02000
#define S_IFIFO 0x01000
#define S_IFREG 0x08000
#define S_IFDIR 0x04000
#define S_IFLNK 0x0A000
#define S_IFSOCK 0x0C000

typedef int64_t ino_t;
typedef uint64_t dev_t;
typedef int32_t uid_t;
typedef int64_t off_t;
typedef int32_t mode_t;
typedef int32_t nlink_t;

typedef struct {
  dev_t st_dev;
  ino_t st_ino;
  mode_t st_mode;
  nlink_t st_nlik;
  uid_t st_uid;
  uid_t st_guid;
  dev_t st_rdev;
  off_t st_size;
  int64_t st_blksize;
  int64_t st_blocks;
} stat_t;

struct fs_node;

typedef size_t (*read_type_t) (struct fs_node *, size_t, size_t, uint8_t *);
typedef size_t (*write_type_t) (struct fs_node *, size_t, size_t, uint8_t *);
typedef void (*open_type_t) (struct fs_node*, uint32_t flags);
typedef void (*close_type_t) (struct fs_node*);
typedef int (*readdir_type_t) (struct fs_node*, struct fs_dirent*, uint32_t);
typedef struct fs_node*(*finddir_type_t) (struct fs_node*, char *name);
typedef int (*ioctl_type_t) (struct fs_node*, uint64_t cmd, uint64_t arg);

typedef struct fs_node {
  // filename
  char name[128];
  // flags
  uint32_t flags;
  // permission mask
  uint32_t pmask;
  // user id
  uid_t uid;
  // inode number
  ino_t inode;
  // node size
  size_t size;

  // read callback
  read_type_t read;
  // write callback
  write_type_t write;
  // open callback
  open_type_t open;
  // close callback
  close_type_t close;
  // read callback
  readdir_type_t readDir;
  // find callback
  finddir_type_t findDir;
  // ioctl callback
  ioctl_type_t ioctl;

  // some fs drive using
  void* vol;
  
  fs_node* link;
} fs_node_t;

typedef struct fs_dirent {
  // inode number
  uint32_t inode;
  // filename
  char name[128];
} fs_dirent_t;

typedef struct fs_fd {
  fs_node_t* node;
  off_t pos;
  mode_t mode;
} fs_fd_t;

namespace fs {
  class FsVolume;
  extern List<FsVolume*>* volumes;

  void Initialize();
  fs_node_t* GetRoot();
  void RegisterDevice(fs_node_t* device);

  fs_node_t* ResolvePath(char* path, char* workingDir = nullptr);
  char* CanocalizePath(char* path, char* workingDir);

  size_t Read(fs_node_t* node, size_t offset, size_t size, uint8_t *buffer);
  size_t Write(fs_node_t* node, size_t offset, size_t size, uint8_t *buffer);
  fs_fd_t* Open(fs_node_t* node, uint32_t flags = 0);
  void Close(fs_node_t* node);
  void Close(fs_fd_t* handle);
  int ReadDir(fs_node_t* node, fs_dirent_t* dirent, uint32_t index);
  fs_node_t* FindDir(fs_node_t* noe, char* name);

  size_t Read(fs_fd_t* handle, size_t size, uint8_t *buffer);
  size_t Write(fs_fd_t* handle, size_t size, uint8_t *buffer);
  int ReadDir(fs_fd_t* handle, fs_dirent_t* dirent, uint32_t index);
  fs_node_t* FindDir(fs_fd_t* handle, char* name);

  int Ioctl(fs_fd_t* handle, uint64_t cmd, uint64_t arg);
}
