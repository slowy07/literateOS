#pragma once

#include <paging.h>
#include <serial.h>
#include <string.h>
#include <mischdr.h>

// the ssize of a block in a physsical memory
#define PHYSALLOC_BLOCK_SIZE 4096
// the amount of blocks in a byte
#define PHYSALLOC_BLOCK_PER_BYTE 8

// the size of memory bitmap in dwords
#define PHYSALLOC_BITMAP_SIZE_DWORDS 524488

extern void* kernel_end;

namespace Memory {
  // Initialize the physical page allocator
  void InitializePhysicalAllocator(memory_info_t* mem_info);

  // finds the first free block in physical memory
  uint64_t GetFirstMemoryBlock();

  // marks region in physical memory as being used
  void MarkMemoryRegionUsed(uint64_t base, size_t size);

  // Allocates a block of physical memory
  uint64_t AllocatePhysicalMemoryBlock();

  // allocates a block of physical memory
  uint64_t AllocatePhysicalMemoryBlock();

  // frees a block of physical memory
  void FreePhysicalMemoryBlock(uint64_t addr);

  // used block of memory
  extern uint64_t usedPhysicalBlocks;
}
