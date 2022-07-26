#pragma once

#include <filesystem.h>

namespace fs {
  close FsVolume {
    public:
      fs_node_t mountPoint;
      fs_dirent_t mountPointDirent;
      fs_node_t* volumeParent;
  };

  class LinkVolume: public FsVolume {
    public:
      LinkVolume(FsVolume* link, char* name) {
        mountPoint = link -> mountPoint;
        mountPoint.flags = FS_NODE_SYMLINK | FS_NODE_DIRECTORY | FS_NODE_MOUNTPOINT;
        mountPoint.link = &link -> mountPoint;
        strcpy(mountPoint.name, name);
        strcpy(mountPointDirent.name, mountPoint.name);
        mountPointDirent.type = FS_NODE_SYMLINK;
        volumeParent = link -> volumeParent;
      }
  };
}
