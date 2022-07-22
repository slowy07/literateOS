#pragma once

#include <stdint.h>
#include <ata.h>
#include <device.h>

namespace ATA {
  class ATADiskDevice: public DiskDevice {
    public:
      // constructor
      ATADiskDevice(int port, int device);
      // read byte
      int Read(uint64_t lba, uint32_t count, void* buffer);

      uint32_t* prdBufferPhys;
      uint8_t* prdBuffer;
      
      uint32_t prdtPhys;
      uint64_t* prdt;
      uint64_t prd;

      int port;
      // 0 master, 1 slave
      int drive;

      int blocksize = 512;
    private:
      char* name = "Generic ATA Disk Device";
  };
}
