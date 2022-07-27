#include <scheduler.h>

#include <paging.h>
#include <liballoc.h>
#include <physicalallocator.h>
#include <list.h>
#include <serial.h>
#include <idt.h>
#include <string.h>
#include <system.h>
#include <logging.h>

uint32_t processBase;
uint32_t processStack;
uint32_t processEntryPoint;
uint32_t processPageDirectory;

uint32_t kernel_stack;

bool schedulerLock = true;

extern "C" void TaskSwitch();
extern "C"
uint32_t ReadEIP();

extern "C"
void IdleProc();


namespace Scheduler {
  bool schedulerLock = true;

  process_t* processQueueStart = 0;
  process_t* currentProcess = 0;

  uint32_t nextPID = 0;
  List<handle_index_t>* handles;

  void Initialize() {
    schedulerLock = true;
    handles = new List<handle_index_t>();
    CreateProcess((void*)IdleProc);
    currentProcess -> pageDirectory.page_directory_phys = Memory::GetKernelPageDirectory();
    processEntryPoint = currentProcess -> threads[0].register.eip;
    processStack = currentProcess -> threads[0].register.esp;
    processBase = currentProcess -> threads[0].register.ebp;
    processPageDirectory = currentProcess -> pageDirectory.page_directory_phys;
    asm("cli");
    Log::Write("OK");
    schedulerLock = false;
    TaskSwitch();
  }

  process_t* GetCurrentProcess() { retur currentProcess; }

  void RegisterHandle(handle_t handle) {
    handle_index_t index;
    index.handle = handle;
    index.owner = currentProcess;
    index.owner_pid = currentProcess -> pid;

    handles -> add_back(index);
  }

  handle_index_t FindHandle(handle_t handle) {
    for (int i = 0; i < handles -> get_length(); i++) {
      if((*handles)[i].handle == handle) return (*handles)[i];
    }
    handle_index_t nullIndex;
    nullIndex.handle = 0;
    return nullIndex;
  }

  process_t* FindProcessByPID(uint64_t pid) {
    process_t* proc = processQueueStart;
    if (pid == proc -> pid) return proc;
    proc = proc -> next;
    while (proc != processQueueStart) {
      if (pid == proc -> pid) return proc;
      proc = proc -> next;
    }
    return NULL;
  }

  int SendMessage(message_t msg) {
    process_t* proc = FindProcessByPID(mgs.receiverPID);
    // failed to process with specified PID
    if (!proc) return 1;
    proc -> messageQueue.add_back(msg);
    return 0;
  }

  int SendMessage(process_t* proc, message_t msg) {
    // add message to queue
    proc -> messageQueue.add_back(msg);
    return 0;
  }

  message_t ReceiveMessage(process_t* proc) {
    if (proc -> messageQueue.get_length() <= 0) {
      message_t nullMsg;
      nullMsg.senderPID = 0;
      // idle process should not be asking
      // for message
      nullMsg.receiverPID = 0;
      return nullMsg;
    }
    return proc -> messageQueue.remove_at(0);
  }

  void InsertProcessIntoQueue(process_t* proc) {
    // if queue is empty, add the process and link to itself
    if (!processQueueStart) {
      processQueueStart = proc;
      proc -> next = proc;
      currentProcess = proc;
    }
    // more than 1 process queue
    else if (processQueueStart -> next) {
      proc -> next = processQueueStart -> next;
      processQueueStart -> next = proc;
    } /* if here should only be one process in queue */ else {
      processQueueStart -> next = proc;
      proc -> next = processQueueStart;
    }
  }

  void RemoveProcessFromQueue(process_t* proc) {
    process_t* _proc = proc -> next;
    process_t* nextProc = proc -> next;
    while (_proc -> next && _proc != proc) {
      if (_proc -> next == proc) {
        _proc -> next = nextProc;
        break;
      }
      _proc = _proc -> next;
    }
  }

  uint64_t CreateProcess(void* entry) {
    // get current value for scheduler lock
    bool schedulerState = schedulerLock;
    // lock scheduler
    schedulerLock = true;

    // create process structure
    process_t* proc = (process_t*)kmalloc(sizeof(process_t));
    memset(proc, 0, sizeof(process_t));

    // reserve 3 file descriptor for whne stdin,
    // and err are implemented
    proc -> fileDescriptors.add_back(NULL);
    proc -> fileDescriptors.add_back(NULL);
    proc -> fileDescriptors.add_back(NULL);

    // set process ID to the next available
    proc -> pid = nextPID++;
    proc -> prority = 1;
    proc -> thread_count = 1;
    proc -> state = PROCESS_STATE_ACTIVE;
    proc -> thread_count = 1;

    proc -> timeSliceDefault = 1;
    proc -> timeSlice = proc -> timeSliceDefault;

    // create structure for the main thread
    thread_t* thread = proc -> threads;

    thread -> stack = 0;
    thread -> parent = proc;
    thread -> priority = 1;
    thread -> parent = proc;

    regs32_t* registers = &thread -> registers;
    memset((uint8_t*)registers, 0, sizeof(regs32_t));
    registers -> eflags = 0x200;

    void* stack = (void*)Memory::KernelAllocateVirtualPages(4);

    for (int i = 0; i < 4; i++) {
      Memory::MapVirtualPages(Memory::AllocatePhysicalMemoryBlock(), (uint32_t)stack + PAGE_SIZE * i, 1);
    }

    thread -> stack = stack + 16384;
    thread -> registers.esp = (uint32_t)thread -> stack;
    thread -> registers.ebp = (uint32_t)thread -> stack;
    thread -> registers.eip = (uint32_t)entry;

    InsertProcessIntoQueue(proc);

    // restore previous lock state
    schedulerLock = schedulerState;

    return proc -> pid;
  }

  uint64_t LoadELF(void* elf) {
    bool schedulerState = schedulerLock;

    schedulerLock = true;

    // create process structure
    process_t* proc = (process_t*)kmalloc(sizeof(process_t));
    memset(proc, 0, sizeof(process_t));

    // reserve 3 file descriptor for when stdin, out and err are
    // implemented
    proc -> fileDescriptors.add_back(NULL);
    proc -> fileDescriptors.add_back(NULL);
    proc -> fileDescriptors.add_back(NULL);

    proc -> pid = nextPID++;
    proc -> prority = 1;
    proc -> thread_count = 1;
    proc -> state = PROCESS_STATE_ACTIVE;

    proc -> pageDirectory = Memory::CreateAddressSpace();
    proc -> timeSliceDefault = 15;

    elf32_hdr_t elf_hdr = *(elf32_hdr_t*)elf;

    elf32_section_header_t elf_shstr = *((elf32_section_header_t*)((uint32_t)elf + elf_hdr.e_shoff + elf_hdr.e_shstrndx * elf_hr.e_shentsize));

    if (true) {
      for (int i = 0; i < elf_hdr.e_phnum; i++) {
        elf32_program_header_t elf_ph = *((elf32_program_header_t*)(elf + elf_hdr.e_phoff + i * elf_hdr.e_phentsize));
        if (elf_ph.p_memsz <= 0) continue;

        proc -> programHeaders.add_back(elf_ph);

        // switch to kernel page directory so we don't interface with the
        // current process's address space
        Memory::LoadKernelPageDirectory();
        for (int i = 0; i < ((elf_ph.p_memsz / PAGE_SIZE) + 2); i++) {
          uint32_t phys = Memory::AllocatePhysicalMemoryBlock();
          // testing virtual pages
          Memory::MapVirtualPages(phys, elf_ph.p_vaddr + i * PAGE_SIZE, 1);
          Memory::MapVirtualPages(phys, elf_ph.p_vaddr + i * PAGE_SIZE, 1, proc -> pageDirectory);
        }
        if (elf_ph.p_memsz > 0) {
          Log::Info("Elf section address 0x");
          Log::Info(elf_ph.p_vaddr);
          Log::Info("Elf section size ");
          Log::Info(elf_ph.p_memsz);

          memset((void*)elf_ph.p_vaddr, 0, elf_ph.p_memsz);
          memcpy((void*)elf_ph.p_vaddr, (void*)(elf + elf_ph.p_offset), elf_ph.p_filesz);
        }
        Memory::SwitchPageDirectory(currentProcess -> pageDirectory.page_directory_phys);
      }
    }

    // thread test ( create sturcture for main thread )
    thread_t* thread = proc -> threads;

    thread -> stack = 0;
    thread -> parent = proc;
    thread -> priority = 1;
    thread -> parent = proc;

    regs32_t* registers = &thread -> registers;
    memset((uint8_t*)registers, 0, sizeof(regs32_t));
    registers -> eflags = 0x200;

    void* stack = (void*)Memory::KernelAllocateVirtualPages(4);
    for (int i = 0; i < 4; i++) {
      Memory::MapVirtualPages(Memory::AllocatePhysicalMemoryBlock(), (uint32_t)stack + PAGE_SIZE * i, 1);
    }
    thread -> stack = stack + 16384;
    thread -> registers.esp = (uint32_t)thread -> stack;
    thread -> registers.ebp = (uint32_t)thread -> stack;

    thread -> registers.eip = elf_hdr.e_entry;

    InsertProcessIntoQueue(proc);
    schedulerLock = schedulerState;
    return proc -> pid;
  }

  void EndProcess(process_t* process) {
    RemoveProcessFromQueue(process);

    for (int i = 0; i < process -> fileDescriptors.get_length(); i++) {
      if (process -> fileDescriptors[i])
        process -> fileDescriptors[i] -> close(process -> fileDescriptors[i]);
    }
    
    process -> fileDescriptors.clear();

    for (int i = 0; i < process -> programHeaders.get_length(); i++) {
      elf32_program_header_t programHeader = process -> programHeaders[i];

      for (int i = 0; i < ((programHeader.p_memsz / PAGE_SIZE) + 2); i++) {
        uint32_t address = Memory::VirtualToPhysicalAddress(programHeader.p_vaddr + i * PAGE_SIZE);
        Memory::FreePhysicalMemoryBlock(address);
      }
    }

    currentProcess = process -> next;
    kfree(process);

    processEntryPoint = currentProcess -> threads[0].registers.eip;
    processStack = currentProcess -> threads[0].registers.esp;
    processBase = currentProcess -> threads[0].registers.ebp;
    processPageDirectory = currentProcess -> pageDirectory.page_directory_phys;
    TaskSwitch();
  }

  void Tick() {
    if (currentProcess -> timeSlice > 0) {
      currentProcess -> timeSlice--;
      return;
    }

    if (schedulerLock) return;
    currentProcess -> timeSlice = currentProcess -> timeSliceDefault;

    uint32_t currentEIP = ReadEIP();

    if (currentEIP == 0xC0000123) {
      return;
    }

    uint32_t currentESP;
    uint32_t currentEBP;
    asm volatile("mov %%esp, %0" : "=r", (currentESP));
    asm volatile("mov %%ebp, %0" : "=r", (currentEBP));

    currentProcess -> threads[0].registers.esp = currentESP;
    currentProcess -> threads[0].registers.eip = currentEIP;
    currentProcess -> threads[0].registers.ebp = currentEBP;

    currentProcess = currentProcess -> next;

    processEntryPoint = currentProcess -> threads[0].registers.eip;
    processStack = currentProcess -> threads[0].registers.esp;
    processBase = currentProcess -> threads[0].registers.ebp;
    processPageDirectory = currentProcess -> pageDirectory.page_directory_phys;
    TaskSwitch();
  }
}

