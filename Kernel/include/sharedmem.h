#pragma once

#include <scheduler.h>

#define SMEM_FLAGS_PRIVATE 1

typedef struct {
  // physical pages
  uintptr_t* pages;
  // size in pages
  unsigned pgCount;
  
  uint64_t flags;
  uint64_t key;
  
  pid_t owner;
  pid_t receipent;
} shared_mem_t;

namespace Memory {
  void InitializeSharedMemory();

  int CanModifySharedMemory(pid_t pid, uint64_t key);
  shared_mem_t* GetSharedMemory(uint64_t key);

  uint64_t CreateSharedMemory(uint64_t size, uint64_t flags, pid_t owner, pid_t receipent);
  void* MapSharedMemory(uint64_t key, process_t* proc, uint64_t hint);
  void DestroySharedMemory(uint64_t key);
}
