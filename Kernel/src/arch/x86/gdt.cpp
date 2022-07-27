#include <gdt.h>
#include <stdint.h>

extern "C" void gdt_flush(uint32_t ptr);

namespace GDT {
  gdt_entry_t gdt[5];
  gdt_ptr_t gdt_ptr;
  tss_entry_t tss;
  uint16_t tss_sel;

  static void SetGate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_low = (base & 0xFFFF);

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= granularity & 0xF0;
    gdt[num].access = access;

    // null seg
    SetGate(0, 0, 0, 0, 0);
    // code seg
    SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // data seg
    SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    // user code seg
    SetGate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    // user data seg
    SetGate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    // flush / reload the GDT
    gdt_flush((uint32_t)&gdt_ptr);
  }
}
