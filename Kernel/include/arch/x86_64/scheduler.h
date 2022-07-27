#pragma once

#include <stdint.h>
#include <paging.h>
#include <system.h>
#include <list.h>
#include <filesystem.h>
#include <memory.h>

#define PROCESS_STATE_SUSPENDED 0
#define PROCESS_STATE_ACTIVE 1
struct process;

typedef void* handle_t;
typedef uint64_t pid_t;

typedef struct HandleIndex {
  uint32_t owner_pid;
  process* owner;
  handle_t handle;
} handle_index_t;


typedef struct {
  // parent process
  process* parent;
  // pointer to the initial stack
  void* stack;
  // the limi of the stack
  void* kernelStack;
  // thread priority
  uint8_t priority;
  // thread state
  uint8_t state;
  // regiters
  regs64_t registers;
} __attribute((packed)) thread_t;

typedef struct {
  // PID of sender
  uint64_t senderPID;
  // PID of receiver
  uint64_t receiverPID;
  // ID of message
  uint64_t msg;
  // message data
  uin64_t data;
  uint64_t data2;
} __attribute__((packed)) message_t;

typedef struct process {
  // PID
  pid_t pid;
  // Process priority
  uint8_t priority;
  // pointer to page directory and tables
  address_space_t* addressSpace;
  // used to enssure these memory regionss don'tget freed
  // when process terminated
  List<mem_region_t> sharedMemory;
  // process stable
  uint8_t state;
  thread_t threads[1];
  // amount of threads
  uint32_t thread_count;
  uint32_t timeSlice;
  uint32_t timeSliceDefault;
  process* next;

  processs* parent;
  List<process*> childern;

  char workingDir[PATH_MAX];
  
  // state of the extended register
  void* fxState;
  
  List<fs_fd_t*> fileDescriptors;
  List<message_t> messageQueue;
} process_t;

namespace Scheduler {
  extern process_t* currentProcess;
  extern int lock;

  process_t* CreateProcess(void* entry);
  process_t* CreateELFProcess(void* elft, int argc = 0; char** argv = nullptr, int envc = 0, char** envp = nullptr);
  
  process_t* GetCurrentProcess();
  void Yield();
  handle_t RegisterHandle(void* handle);
  void* FindHandle(handle_t handle);
  
  int SendMessage(message_t msg);
  int SendMessage(process_t* proc, message_t msg);

  message_t ReceiverMessage(process_t* proc);
  process_t* FindProcessByPID(uint64_t pid);

  void Initialize();
  void Tick(regs64_t*);

  void EndProcess(process_t* process);
}
