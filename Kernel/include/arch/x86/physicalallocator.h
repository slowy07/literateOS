#pragma once

#include <paging.h>
#include <serial.h>
#include <string.h>
#include <mischdr.h>

// the size of aa block in physical memory
#define PHYSICALLOC_BLOCK_SIZE 4096
// the amount of block in a byte
#define PHYSICALLOC_BLOCKS_PER_BYTE 8
// the size of the memory bitmap in dwords
#define PHYSICALLOC_BITMAP_SIZE_DWORDS 32768

extern void* kernel_end;

namespace Memory {
  // initialize the physical page allocator
  void InitializePhysicalAllocator(memory_info_t* mem_info);
  // find the first free block in physical memory
  uint32_t GetFirstMemoryBlock();
  // marks a region in physical memory as being used
  void MarkMemoryRegionFree(uint32_t base, size_t size);
  // allocate a block of pyhsical memory
  uint32_t AlloctePyhsicalMemoryBlock();
  // frees a block of pyhsical memory
  void FreePyhsicalMemoryBlock(uint32_t addr);
  // used blocks of memory
  extern uint32_t usedPyhsicalBlocks;
}
