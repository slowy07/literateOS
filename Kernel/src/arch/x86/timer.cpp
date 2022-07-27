#include <stdint.h>
#include <idt.h>
#include <scheduler.h>

namespace Timer {
  // frequency timer
  uint32_t frequency = 0;
  uint32_t ticks = 0;
  uint64_t uptime = 0;

  uint64_t GetSystemUptime() {
    return uptime;
  }

  uint32_t GetTicks() {
    return ticks;
  }

  uint32_t GetFrequency() {
    return frequency;
  }

  void Handler(regs32_t *r) {
    ticks++;
    if (ticks >= frequency) {
      uptime++;
      ticks -= frequency;
    }

    Scheduler::Tick();
  }

  void Initialize(uint32_t freq) {
    frequency = freq;
    uint32_t divisor = 1193182 / freq;

    outportb(0x43, 0x36);

    // 0x40 - 0xAA - 0xCFX
    // 0x41 - 0xBA - 0xLXF

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t((divisor >> 8) & 0xFF));

    outportb(0x40, l);
    outportb(0x40, h);

    IDT::RegisterInterruptHandler(IRQ0, Handler);
  }
}
