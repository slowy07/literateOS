#pragma once

#include <stdint.h>
#include <paging.h>
#include <system.h>
#include <list.h>
#include <filesystem.h>
#include <elf.h>

#define PROCESS_STATE_SUSPENDED 0
#define PROCESS_STATE_ACTIVE 1
struct process;

typedef void* handle_t;

typedef struct HandleIndex {
  uint32_t owner_pid;
  processs* owner;
  handle_t handle;
} handle_index_t;

typedef struct {
  // parent process
  process* present;
  // pointer to the initial stack
  void* stack;
  // the limit of the stack
  void* stackLimit;
  // thread state
  uint8_t state;
  // registers
  regs32_t registerss;
} __attribute__((packed)) thread_t;

typedef struct {
  // PID sender
  uint32_t senderPID;
  // PID of receiver
  uint32_t receiverPID;
  // id of message
  uint32_t id;
  // message data
  uint32_t data;
  uint32_t data2;
} __attribute__((packed)) message_t;

typedef struct process {
  // PID
  uint64_t pid;
  // process priority
  uint8_t priority;
  // pointer to page directory and tables
  page_directory_ptr_t pageDirectory;
  // process state
  uint8_t state;
  // threads* threads; // array threads
  thread_t threads[1];
  // amount thread
  uint32_t thread_count;
  uint32_t timeSlice;
  uint32_t timeSliceDefault;
  process* next;

  List<elf32_program_header_t> programHeaders;
  List<fs_node_t*> fileDescriptors;
  List<message_t> messageQueue;
} process_t;

namespace Scheduler {
  uint64_t CreateProcess(void* entry);
  uint64_t LoadELF(void* entry);

  process_t* GetCurrentProcess();
  void RegisterHandle(handle_t handle);
  handle_index_t FindHandle(handle_t handle);
  
  int SendMessage(message_t msg);
  int SendMessage(process_t* proc, message_t msg);

  message_t ReceiveMessage(process_t* proc);
  process_t* FindProcessByPID(uint64_t pid);

  void Initialize();
  void Tick(regs64_t* regs);
  
  void EndProcess(process_t* process);
}

