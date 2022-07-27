#pragma once

#include <mischdr.h>

namespace HAL {
  extern memory_info_t mem_info;
  extern video_mode_t videoMode;
  extern multiboot_info_t multibootInfo;
  extern uintptr_t multibootModulesAddress;
  extern int bootModuleCount;
  extern bool_module_t bootModules[];
  extern bool debugMode;

  void InitCore(multiboot_info_t mb_info);
  void InitVideo();
  void InitExtra();
  
  void Init(multiboot_info_t mb_info);
}
