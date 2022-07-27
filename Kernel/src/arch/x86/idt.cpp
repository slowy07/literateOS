#include <idt.h>
#include <system.h>
#include <vga.h>
#include <string.h>
#include <logging.h>

idt_entry_t idt[256];
idt_ptr_t idt_ptr;

isr_t interrupt_hndlers[256];

extern "C"
void isr0();
extern "C"
void isr1();
extern "C"
void isr2();
extern "C"
void isr3();
extern "C"
void isr4();
extern "C"
void isr5();
extern "C"
void isr6();
extern "C"
void isr7();
extern "C"
void isr8();
extern "C"
void isr9();
extern "C"
void isr10();
extern "C"
void isr11();
extern "C"
void isr12();
extern "C"
void isr13();
extern "C"
void isr14();
extern "C"
void isr15();
extern "C"
void isr16();
extern "C"
void isr17();
extern "C"
void isr18();
extern "C"
void isr19();
extern "C"
void isr20();
extern "C"
void isr21();
extern "C"
void isr22();
extern "C"
void isr23();
extern "C"
void isr24();
extern "C"
void isr25();
extern "C"
void isr26();
extern "C"
void isr27();
extern "C"
void isr28();
extern "C"
void isr29();
extern "C"
void isr30();
extern "C"
void isr31();

extern "C"
void irq0();
extern "C"
void irq1();
extern "C"
void irq2();
extern "C"
void irq3();
extern "C"
void irq4();
extern "C"
void irq5();
extern "C"
void irq6();
extern "C"
void irq7();
extern "C"
void irq8();
extern "C"
void irq9();
extern "C"
void irq10();
extern "C"
void irq11();
extern "C"
void irq12();
extern "C"
void irq13();
extern "C"
void irq14();
extern "C"
void irq15();

extern "C"
void isr0x69();

extern "C"
void idt_flush();

namespace IDT {
    static void SetGate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
        idt[num].base_high = (base >> 16) & 0xFFFF;
        idt[num].base_low = base & 0xFFFF;

        idt[num].sel = sel;
        idt[num].null = 0;

        idt[num].flags = flags;
    }

    void Initialize() {
        idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
        idt_ptr.base = (uint32_t)&idt;
        
        memset((uint8_t*)&idt, 0, sizeof(id_entry_t) * 256);
        SetGate(0, (uint32_t)isr0,0x08,0x8E);
        SetGate(1, (uint32_t)isr1,0x08,0x8E);
		    SetGate(2, (uint32_t)isr2,0x08,0x8E);
		    SetGate(3, (uint32_t)isr3,0x08,0x8E);
		    SetGate(4, (uint32_t)isr4,0x08,0x8E);
		    SetGate(5, (uint32_t)isr5,0x08,0x8E);
		    SetGate(6, (uint32_t)isr6,0x08,0x8E);
		    SetGate(7, (uint32_t)isr7,0x08,0x8E);
		    SetGate(8, (uint32_t)isr8,0x08,0x8E);
		    SetGate(9, (uint32_t)isr9,0x08,0x8E);
		    SetGate(10, (uint32_t)isr10,0x08,0x8E);
		    SetGate(11, (uint32_t)isr11,0x08,0x8E);
		    SetGate(12, (uint32_t)isr12,0x08,0x8E);
		    SetGate(13, (uint32_t)isr13,0x08,0x8E);
		    SetGate(14, (uint32_t)isr14,0x08,0x8E);
		    SetGate(15, (uint32_t)isr15,0x08,0x8E);
		    SetGate(16, (uint32_t)isr16,0x08,0x8E);
		    SetGate(17, (uint32_t)isr17,0x08,0x8E);
		    SetGate(18, (uint32_t)isr18,0x08,0x8E);
		    SetGate(19, (uint32_t)isr19,0x08,0x8E);
		    SetGate(20, (uint32_t)isr20,0x08,0x8E);
		    SetGate(21, (uint32_t)isr21,0x08,0x8E);
		    SetGate(22, (uint32_t)isr22,0x08,0x8E);
		    SetGate(23, (uint32_t)isr23,0x08,0x8E);
		    SetGate(24, (uint32_t)isr24,0x08,0x8E);
		    SetGate(25, (uint32_t)isr25,0x08,0x8E);
		    SetGate(26, (uint32_t)isr26,0x08,0x8E);
		    SetGate(27, (uint32_t)isr27,0x08,0x8E);
		    SetGate(28, (uint32_t)isr28,0x08,0x8E);
		    SetGate(29, (uint32_t)isr29,0x08,0x8E);
		    SetGate(30, (uint32_t)isr30,0x08,0x8E);
		    SetGate(31, (uint32_t)isr31,0x08,0x8E);
		    SetGate(0x69, (uint32_t)isr0x69, 0x08, 0x8E);

		    idt_flush();

		    outportb(0x20, 0x11);
		    outportb(0xA0, 0x11);
		    outportb(0x21, 0x20);
		    outportb(0xA1, 0x28);
		    outportb(0x21, 0x04);
		    outportb(0xA1, 0x02);
		    outportb(0x21, 0x01);
		    outportb(0xA1, 0x01);
		    outportb(0x21, 0x0);
		    outportb(0xA1, 0x0);

		    SetGate(32, (uint32_t)irq0, 0x08, 0x8E);
		    SetGate(33, (uint32_t)irq1, 0x08, 0x8E);
		    SetGate(34, (uint32_t)irq2, 0x08, 0x8E);
		    SetGate(35, (uint32_t)irq3, 0x08, 0x8E);
		    SetGate(36, (uint32_t)irq4, 0x08, 0x8E);
		    SetGate(37, (uint32_t)irq5, 0x08, 0x8E);
		    SetGate(38, (uint32_t)irq6, 0x08, 0x8E);
		    SetGate(39, (uint32_t)irq7, 0x08, 0x8E);
		    SetGate(40, (uint32_t)irq8, 0x08, 0x8E);
		    SetGate(41, (uint32_t)irq9, 0x08, 0x8E);
		    SetGate(42, (uint32_t)irq10, 0x08, 0x8E);
		    SetGate(43, (uint32_t)irq11, 0x08, 0x8E);
		    SetGate(44, (uint32_t)irq12, 0x08, 0x8E);
		    SetGate(45, (uint32_t)irq13, 0x08, 0x8E);
		    SetGate(46, (uint32_t)irq14, 0x08, 0x8E);
		    SetGate(47, (uint32_t)irq15, 0x08, 0x8E);

        memset((uint8_t*)&interrupt_hndlers, 0, sizeof(isr_t) * 256);
      
        __asm__ __volatile__("sti");
    }
    
    void RegisterInterruptHandler(uint8_t interrupt, isr_t handler) {
      interrupt_handlers[interrupt] = handler;
    }
}

extern "C"

void isr_handler(regs32_t* regs) {
  if (interrupt_handlers[regs -> int_num] != 0) {
    interrupt_handlers[regs->int_num](regs);
  } else if (regs -> int_num == 0x69) {
    Log::Warning("\r\nEarly syscal");
  }
  else {
    Log::Error("Fatal exception: ");
    Log::Info("\r\nEIP: ");
    Log::Error(itoa(regs->eip, 0, 16));
    Log::Error("\r\nError Code: ");
    Log::Error(itoa(regs->err_code,0, 16));
    for (;;);
  }
}

extern "C"
void irq_handler(regs32_t* regs) {
  if (regs->int_num >= 40) {
    outportb(0x0A, 0x20);
  }

  outportb(0x20, 0x20);

  if (interrupt_handlers[regs->int_num] != 0) {
    isr_t handler;
    handler = interrupt_handlers[regs->int_num];
    handler(regs);
  }
}
