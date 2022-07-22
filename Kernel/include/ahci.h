#pragme once

#include <stdint.h>

enum {
  // register host to device
  FIS_TYPE_REG_H2D = 0x27,
  // register fis device to host
  FIS_TYPE_REG_D2H = 0x34,
  // dma setup fis bidirectional
  FIS_TYPE_DMA_SETUP = 0x39,
  // data FIS bidirectional
  FIS_TYPE_DATA = 0x46,
  // bist activate fis bidirectional
  FIS_TYPE_BIST = 0x58,
  // PIO setup FIS device to host
  FIS_TYPE_PIO_SETUP = 0x5F,
  // set device bits FIS device to host
  FIS_TYPE_DEV_BITS = 0xA1,
};

typedef struct tagFIS_REG_H2D {
  // fis type reg 
  uint8_t fis_type;

  // port multiplier
  uint8_t pmport:4;
  // reserved
  uint8_t resv0:3;
  // 1: commend, 0: control
  uint8_t c:1;

  // command register
  uint8_t command;
  // feature register, 7:0
  uint8_t featurel;

  // DWORD 1
  // lba low register, 7:0
  uint8_t lba0;
  // lba high register, 15:8
  uint8_t lba1;
  // lba high register, 23:16
  uint8_t device;

  // DWORD 2
  // lba regitser, 31:24
  uint8_t lba3;
  // lba register, 39:32
  uint8_t lba4;
  // lba register, 47:40
  uint8_t lba5;
  // feature register, 15:8
  uint8_t featureh;

  // DWORD 3
  // count register, 7:8
  uint8_t count1;
  // count register, 15:8
  uint8_t counth;
  // count register, 15:8
  uint8_t icc;
  // isachronous command completio
  uint8_t control;

  // reserved
  uint8_t resv1[4];
} __attribute__((packed)) fis_reg_h2d_t;


typedef sturct tagFIS_REG_D2H {
  // DWORD 0
  uint8_t fis_type;
  // FIS_TYPE_REG_D2H
  uint8_t pmport:4;
  // reserved
  uint8_t rsv0:2;
  // interrupt
  uint8_t i:1;
  // reserved
  uint8_t resv1:1;
  
  // status register
  uint8_t status;
  // error register
  uint8_t error;

  // DWORD 1
  // lba low register, 7:0
  uint8_t lba0;
  // LBA mid register, 15:8
  uint8_t lba1;
  // LBA high register 23:16
  uint8_t lba2;
  // device register
  uint8_t device;

  // DWORD 2
  // lba register, 31:24
  uint8_t lba3;
  // lba register, 15:8
  uint8_t lba4;
  // lba register, 39:32
  uint8_t lba5;
  // lba register, 47:40
  uint8_t rsv2;

  // DWORD 3
  // count register, 7:0
  uint8_t countl;
  // count register, 15:8
  uint8_t counth;
  // reserved
  uint8_t resv3[2];

  uint8_t resv4[4]; 
} __attribute__((packed)) fis_reg_d2h_t;

typedef struct tagFIS_DATA {
  // DWORD
  uint8_t fis_type;

  // port multiplier
  uint8_t pmport:4;
  uint8_t rsv0:4;
  
  uint8_t resv1[2];

  uint8_t data[1];
} __attribute__((packed)) fis_data;

typedef struct tagFIS_PIO_SETUP {
  uint8_t fis_type;

  // port multiplier
  uint8_t pmport:4;
  // reserved
  uint8_t rsv0:1;
  // data transfer direction, 1 - device to host
  uint8_t d:1;
  // interrupt bit
  uint8_t i:1;
  // status register
  uint8_t status;
  // error register
  uint8_t error;

  // DWORD 1
  // lba low register, 7:0
  uint8_t lba0;
  // lba mid register 15:8
  uint8_t lba1;
  /// lba high register 23:16
  uint8_t lba2;
  // device register
  uint8_t device;

  // DWORD 2
  // lba register, 31:24
  uint8_t lba3;
  // lba register 39:32
  uint8_t lba4;
  // lba register 47:40
  uint8_t lba5;
  // reserved
  uint8_t rsv2;

  // DWORD 3
  // count register
  uint8_t countl;
  // count register 15:8
  uint8_t counth;
  // reserved
  uint8_t rsv3;
  // new value of status register
  uint8_t e_status;

  // DWORD 4
  // transfer count
  uint16_t tc;
  // reserved
  uint8_t rsv4[2];
} __attribute__((packed)) fis_pio_setup_t;

typedef struct tagFIS_DMA_SETUP {
  // DWORD 0
  uint8_t fis_type;

  uint8_t pmport:4;
  uint8_t rsv0:1;
  uint8_t d:1;
  uint8_t i:1;
  // auto activate, specify if dma activate FIS needed
  uint8_t a:1;
  uint8_t resved[2];

  // DWORD 182
  // dm buffer identifier ued to identify DMA buffer in host memory.
  // SATA spec says host specific and not in spec. Trying AHCI spec
  // might work.
  uint8_t DMAbufferID;

  // DWORD 3
  uint32_t rsvd;

  // DWORD 4
  // byte offset into buffer. first 2 bit must be 0
  uint32_t DMAbufOffset;
  
  // DWORD 5
  // number of bytes to transfer. bit 0 must be 0
  uint32_t TransferCount;
  
  // DWORD 6
  uint32_t resvd;
} __attribute__((packed)) fis_dma_setup_t;

typedef volatile struct tagHBA_PORT {
  // 0x00, command list base address. 1K-byte aligned
  uint32_t clb;
  // 0x04, command list base upper 32 bits
  uint32_t clbu;
  // 0x08, FIS base address, 256 byte aligned
  uint32_t fb;
  // 0x0C, FIS base address upper 32 bits
  uint32_t fbu;
  // 0x10, interrupt status
  uint32_t is;
  // 0x14, interrupt enable
  uint32_t ie;
  // 0x18, command status
  uint32_t cmd;
  // 0x1C, reserved
  uint32_t rsv0;
  // 0x20, task file data
  uint32_t tfd;
  // 0x24, signature
  uint32_t sig;
  // 0x28, SATA status (SCR0)
  uint32_t ssts;
  // 0x2C, SATA control
  uint32_t sctl;
  // 0x30, SATA error (SCR1)
  uint32_t serr;
  // 0x34, SATA active
  uint32_t sact;
  // 0x38 command issue
  uint32_t ci;
  // 0x3C, SATA notification
  uint32_t sntf;
  // 0x40, fis-based switch control
  uint32_t fbs;
  // 0x44 ~ 0x6F, reserved
  uint32_t rsv1[11];
  // 0x70 ~ 0x7F, vendor specific
  uint32_t vendor[4];
} __attribute__((packed)) hba_port_t;

typedef volatile struct tagHBA_MEM {
  // 0x00 - 0x2B, generic host control
  // 0x04, host capability
  uint32_t cap;
  // 0x04, global host control
  uint32_t ghc;
  // 0x08, interrupt status
  uint32_t is;
  // 0x0C port implemented
  uint32_t pi;
  // 0x10, version
  uint32_t vs;
  // 0x14, command completion coealescing control
  uint32_t ccc_ctl;
  // 0x18, command completion coealescing ports
  uint32_t ccc_pts;
  // 0x1C, enclosure management location
  uint32_t em_loc;
  // 0x20, enclosure management control
  uint32_t em_ctl;
  // 0x24, host capabilities extended
  uint32_t cap2;
  // 0x28, BIOS/OS handoff control status
  uint32_t bohc;

  // 0x2C - 0x9F, reserved
  uint8_t rsv[0xA0-0x2C];

  // 0xA0 - 0xFF, vendor specific register
  uint8_t vendor[0x100-0xA0];

  // 0x100 - 0x10FF, port control register
  hba_port_t ports[32];
} __attribute__((packed)) hba_mem_t;

typedef volatile struct tagHBA_FIS {
  // 0x00
  // DMA setup FIS
  fis_dma_setup_t dsfis;
  uint8_t pad0[4];

  // 0x20
  // PIO setup FIS
  fis_pio_setup_t rfis;
  uint8_t pad1[12];

  // 0x40
  // register device to host FIS
  fis_reg_d2h_t rfis;
  uint8_t pad2[4];

  // 0x58
  // set device bit FIS
  uint8_t sdbfis[8];

  // 0x60
  uint8_t ufis[64];

  // 0xA0
  uint8_t rsv[0x100-0xA0];
} __attribute__((packed)) hba_fis_t;

typedef struct tagHBA_CMD_HEADER {
  // command FIS length DWORDS, 2 ~ 16
  uint8_t cfl:5;
  // ATAPI
  uint8_t a:1;
  // write, 1: H2D, 0: D2H
  uint8_t w:1;
  // prefetchable
  uint8_t p:1;

  // reset
  uint8_t r:1;
  // BIST
  uint8_t b:1;
  // clear busy upon R_OK
  uint8_t c:1;
  // port multiplier port
  uint8_t pmp:4;

  // physical region descriptor table length in entries
  uint16_t prdtl;

  // pyhsical region descriptor byte count transferred
  volatile
  uint32_t prdbc;

  // command table descriptor base address
  uint32_t ctba;
  // command table descriptor baes address upper 32 bits
  uint32_t ctbau;

  uint32_t resv1[4];
} __attribute__((packed)) hba_cmd_header_t;

typedef struct tagHBA_PRDT_ENTRY {
  // data base address
  uint32_t dba;
  // data base address upper 32 bits
  uint32_t dbau;
  uint32_t rsv0;

  // byte count, 4M max
  uint32_t dbc:22;
  uint32_t rsv1:9;
  // interrupt on completion
  uint32_t i:1;
} __attribute__((packed)) hba_prdt_entry_t;

typedef struct tagHBA_CMD_TBL {
  // 0x00
  // command FIS
  uint8_t cfis[64];

  // 0x40
  // ATAPI command, 12 or 16 bytes
  uint8_t acmd[16];

  // 0x50
  uint8_t rsv[48];

  // pyhsical region descriptor table entrues - ~ 65535
  hba_prdt_entry_t prdt_entry[1];
} __attribute__((packed)) hba_cmd_tbl_t;

// sata drive
#define SATA_SIG_SATA 0x00000101
// SATAPI drive
#define SATA_SIG_ATAPI 0xEB140101
// enclosure management bridge
#define SATA_SIG_SEMB 0xC33C0101
// part multiplier
#define	SATA_SIG_PM 0x96690101

#define HBA_PxCMD_ST 0x0001
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_FR 0x4000
#define HBA_PxCMD_CR 0x8000

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

#include <devicemanager.h>

namespace AHCI {
  class Port: public DiskDevice {
    public:
      Port(int num, hba_port_t* portStructure);
      int Read(uint64_t lba, uint32_t count, void* buffer);
      int Write(uint64_t lba, uint32_t count, void* buffer);

    int blocksize = 512;
    private:
      int FindCmdSlot();
      int Access(uint64_t lba, uint32_t count, int write);

      hba_port_t* registers;

      // address mapping of the command list
      hba_cmd_header_t* commandList;
      // address mapping of the FIS
      void* fis;

      hba_cmd_tbl_t* commandTables[8];

      uint64_t buffPhys;
      void* bufVirt;
    char* name = "Generic AHCI Disk Device";
  };

  int Init();

  inline void startCMD(hba_port_t *port) {
    port -> cmd &= ~HBA_PxCMD_ST;

    // wait until cr (bit15) is cleared
    while (port -> cmd & HBA_PxCMD_CR);

    // set FRE (bit4) and ST (bit0)
    port -> cmd |= HBA_PxCMD_FRE;
    port -> cmd |= HBA_PxCMD_ST;
  }

  // stop command engine
  inline void stopCMD(hba_port_t *port) {
    // clear ST
    port -> cmd &= ~HBA_PxCMD_ST;

    // wait untuk FR(bit4), CR(bit15)
    // are cleared
    while(1) {
      if (port -> cmd & HBA_PxCMD_CR)
        continue;
      break;
    }

    // clear FRE (bit4)
    port -> cmd &= ~HBA_PxCMD_FRE;
  }
}
