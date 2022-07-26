#pragma once
#include <stdint.h>

typedef struct {
  uint32_t flags;
  uint32_t memoryLo;
  uint32_t memoryHi;
  uint32_t bootDevice;
  uint32_t cmdLine;
  uint32_t modsCount;
  uint32_t modsAddr;
  uint32_t num;
  uint32_t size;
  uint32_t addr;
  uint32_t shndx;
  uint32_t mmapLength;
  uint32_t mmapAddr;
  uint32_t drivesLength;
  uint32_t configTable;
  uint32_t bootloaderName;
  uint32_t apmTable;

  uint32_t vbeControlInfo;
  uint32_t vbeModeInfo;
  uint16_t vbeMode;
  uint16_t vbeInterfaceSeg;
  uint16_t vbeInterfaceLen;

  uint64_t frameBufferAddr;
  uint32_t frameBufferPitch;
  uint32_t frameBufferWidth;
  uint32_t frameBufferHeight;
  uint8_t framBufferBpp;
  uint8_t frameBufferType;
} __attribute__((packed)) multiboot_info_t;

typedef struct {
  uint32_t size;
  uint64_t base;
  uint64_t length;
  uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

typedef struct {
  // resolution
  uint32_t width;
  uint32_t height;
  uint16_t bpp;
  
  uint32_t pitch;
  void* address;
} video_mode_t;

typedef struct {
  uintptr_t memory_loww;
  uintptr_t memory_high;
  
  multiboot_memory_map_t* mem_map;
  uintptr_t memory_map_len;
} memory_info_t;

typedef struct {
  uintptr_t base;
  uintptr_t size;
} boot_module_t;

