#include <physicalallocator.h>

#include <paging.h>
#include <serial.h>
#include <string.h>

extern void* kernel_end;

namespace Memory {
  uint32_t physicalMemoryBitmap[PHYSALLOC_BITMAP_SIZE_DWORDS];

  uint32_t usedPhysicalBlocks = 0;
  uint32_t maxPhysicalBlocks = 0;

  // initialize to physical allocator
  void InitializePhysicalAllocator(memory_info_t* mem_info) {
    memset(physicalMemoryBitmap, 0xFFFFFFFF, PHYSALLOC_BITMAP_SIZE_DWORDS * 4);

    multiboot_memory_map_t* mem_map = mem_info -> mem_map;
    multiboot_memory_map_t* mem_map_end = (multiboot_memory_map_t*)(mem_info ->  mem_map + mem_info -> memory_map_len);

    while (mem_map < mem_map_end) {
      if (mem_map -> type == 1)
        MarkMemoryRegionFree(mem_map -> base, mem_map -> length);
      mem_map = (multiboot_memory_map_t*)((uint32_t)mem_map + mem_map -> size + sizeof(mem_map -> size));
    }
    maxPhysicalBlocks = mem_info -> memory_high * 1024 / PHYSALLOC_BLOCK_SIZE;
    MarkMemoryRegionUsed(0, 6144 * 4096);
  }

  // sets a bit in the physical memory bitmap
  inline void bit_set(uint32_t bit) {
    physicalMemoryBitmap[bit / 32] |= (1 << (bit % 32));
  }

  // clears a bit in the physical memory bitmap
  inline void bit_clear(uint32_t bit) {
    physicalMemoryBitmap[bit / 32] &= ~ (1 << (bit % 32));
  }

  uint32_t GetFirstFreeMemoryBlock() {
    for (uint32_t i = 0; i < PHYSALLOC_BITMAP_SIZE_DWORDS; i++)
      if (physicalMemoryBitmap[i] != 0xffffffff)
        for (uint32_t j = 0; j < 32; j++)
          if (!(physicalMemoryBitmap[i] & (1 << j)))
            return i * 32 + j;
    // the first block is always reserved
    return 0;
  }

  // marks a region in physical memory as being used
  void MarkMemoryRegionUsed(uint32_t base, size_t size) {
    for (uint32_t blocks = size / PHYSALLOC_BLOCK_SIZE + 1, align = base / PHYSALLOC_BLOCK_SIZE; blocks > 0; blocks--, usedPhysicalBlocks++)
      bit_clear(align++);
  }

  // allocates a block of physical memory
  uint32_t AllocatePhysicalMemoryBlock() {
    uint32_t index = GetFirstFreeMemoryBlock();
    if (index == 0)
      return 0;

    bit_set(index);
    usedPhysicalBlocks++;

    return index * PHYSALLOC_BLOCK_SIZE;
  }

  // free a block of physical memory
  void FreePhysicalMemoryBlock(uint32_t addr) {
    uint32_t index = addr / PHYSALLOC_BLOCK_SIZE;
    bit_clear(index);
    usedPhysicalBlocks--;
  }
}
