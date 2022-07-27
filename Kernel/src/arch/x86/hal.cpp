#include <hal.h>

#include <gdt.h>
#include <idt.h>
#include <string.h>
#include <serial.h>
#include <logging.h>
#include <video.h>
#include <paging.h>
#include <physicalallocator.h>
#include <pci.h>
#include <ahci.h>
#include <timer.h>

namespace HAL {
  memory_info_t mem_info;
  video_mode_t videoMode;

  multiboot_info_t multibootInfo;
  void InitCore(multiboot_info_t mb_info) {
    multibootInfo = mb_info;

    // check if debugging mode is enabled and if so initialize
    // serial port
    initialize_serial();
    Log::Info("Initializing literate...\r\n");

    // initialize GDT and IDT
    GDT::Initialize();
    IDT::Initialize();

    // initialize paging / virtual memory manager
    Memory::InitializeVirtualMemory();

    // Allocate virtual memory for memory map
    uint32_t mmap_virt = Memory::KernelAllocateVirtualPages(mb_info.mmapLength / PAGE_SIZE + 1);

    // get memory map
    Memory::MapVirtualPages(mb_info.mmapAddr, mmap_virt, mb_info.mmapLength / PAGE_SIZE + 1);
    multiboot_memory_map_t* memory_map = (multiboot_memory_map_t*)mb_info.mmapAddr;

    // initialize memory info structure to pass pyhsical memory allocator
    mem_info.memory_high = mb_info.memoryHi;
    mem_info.memory_low = mb_info.memoryLo;
    mem_info.mem_mp = memory_map;
    mem_info.memory_map_len = mb_info.mmapLength;

    uint32_t mbModsVirt = Memory::KernelAllocateVirtualPages(1);
    Memory::MapVirtualPages(multibootInfo.modsAddr, mbModsVirt, 1);

    // initialize physical memory allocator
    Memory::InitializePhysicalAllocator(&mem_info);

    Log::Info("Initializing system timer ...");
    Timer::Initialize(1200);
    Log::Write("OK");

    Log::Info("Initializing PCI Driver ...");
    PCI::Init();
    Log::Info("OK");
    Log::Info("Initializing AHCI Driver ...");

    if (AHCI::Init())
      Log::Warning("ERROR: No AHCI controller found");
    else
      Log::info("OK");
  }

  void InitVideo() {
    // map video memory
    uint32_t vidMemSize = multibootInfo.framebufferHeight*multibootInfo.framebufferPitch;
    uint32_t vidMemVirt = Memory::KernelAllocateVritualPages(vidMemSize / PAGE_SIZE + 1);
    Memory::MapVirtualPages(multibootInfo.framebufferAddr, vidMemVirt, vidMemSize / PAGE_SIZE + 1);

    videoMode.width = multibootInfo.framebufferWidth;
    videoMode.height = multibootInfo.framebufferHeight;
    videoMode.bpp = multibootInfo.framebufferBpp;
    videoMode.pitch = multibootInfo.framebufferPitch;
    videoMode.address = vidMemVirt;

    Video::Initialize(videoMode);
    Video::DrawRect(0, 0, videoMode.width, videoMode.height, 48, 48, 48);
  }

  void InitExtra() {
    // keyboard mouse
  }
  
  void Init(multiboot_info_t mb_info) {
    InitCore(mb_info);
    InitVideo();
    InitExtra();
  }
}
