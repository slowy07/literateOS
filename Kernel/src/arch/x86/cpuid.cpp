#include <cpuid.h>

namespace CPU {
  cpuid_info_t GetCPUInfo() {
    cpuid_info_t cpuid_info_s;

    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;

    // get vendor string
    asm volatile("cpuid": "=b"(ebx), "=d"(edx), "=c"(ecx): "a"(0));
    // copy string to buffer
    for (int i = 0; i < 4; i++) cpuid_info_s.vendor_string[i] = ebx >> (i * 8) & 0xFF;
    for (int i = 0; i < 4; i++) cpuid_info_s.vendor_string[i+4] = edx >> (i * 8) & 0xFF;
    for (int i = 0; i < 4; i++) cpuid_info_s.vendor_string[i+8] = ecx >> (i * 8) & 0xFF;

    // get features
    asm volatile("cpuid": "=d"(edx), "=c"(ecx) : "a"(1));
    cpuid_info_s.features_ecx = ecx;
    cpuid_info_s.features_edx = edx;
    
    return cpuid_info_s;
  }
}
