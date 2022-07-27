#pragma once

#include <stdint.h>

#define LOCAL_APIC_ID 0x20 // APIC ID Register
#define LOCAL_APIC_VERSION 0x30 // APIC Version Register
#define LOCAL_APIC_TPR 0x80 // Task Priority Register 
#define LOCAL_APIC_APR 0x90 // Arbitration Priority Register
#define LOCAL_APIC_PPR 0xA0 // Processor Priority Register
#define LOCAL_APIC_EOI 0xB0 // End of Interrupt Register
#define LOCAL_APIC_RRD 0xC0 // Remote Read Register
#define LOCAL_APIC_LDR 0xD0 // Logical Destination Register
#define LOCAL_APIC_DFR 0xE0 // Destination Format Register
#define LOCAL_APIC_SIVR 0xF0 // Spurious Interrupt Vector Register
#define LOCAL_APIC_ISR 0x100 // In-service Register
#define LOCAL_APIC_TMR 0x180 // Trigger Mode Register
#define LOCAL_APIC_IRR 0x200 // Interrupt Request Register
#define LOCAL_APIC_ERROR_STATUS 0x280 // Error Status Register
#define LOCAL_APIC_ICR_LOW 0x300 // Interrupt Command Register Low
#define LOCAL_APIC_ICR_HIGH 0x310 // Interrupt Command Register High
#define LOCAL_APIC_LVT_TIMER 0x320 // Timer Local Vector Table Entry
#define LOCAL_APIC_LVT_THERMAL 0x330 // Thermal Local Vector Table Entry
#define LOCAL_APIC_LVT_PERF_MONITORING 0x340 // Performance Local Vector Table Entry
#define LOCAL_APIC_LVT_LINT0 0x350 // Local Interrupt 0 Local Vector Table Entry
#define LOCAL_APIC_LVT_LINT1 0x360 // Local Interrupt 1 Local Vector Table Entry
#define LOCAL_APIC_LVT_ERROR 0x370 // Error Local Vector Table Entry
#define LOCAL_APIC_TIMER_INITIAL_COUNT 0x380 // Timer Initial Count Register
#define LOCAL_APIC_TIMER_CURRENT_COUNT 0x390 // Timer Current Count Register
#define LOCAL_APIC_TIMER_DIVIDE 0x3E0 // Timer Divide Configuration Register

#define LOCAL_APIC_BASE 0xFFFFFFFFFF000

#define ICR_VECTOR(x) (x & 0xFF)
#define ICR_MESSAGE_TYPE_FIXED 0
#define ICR_MESSAGE_TYPE_LOW_PRIORITY (1 << 8)
#define ICR_MESSAGE_TYPE_SMI (2 << 8)
#define ICR_MESSAGE_TYPE_REMOTE_READ (3 << 8)
#define ICR_MESSAGE_TYPE_NMI (4 << 8)
#define ICR_MESSAGE_TYPE_INIT (5 << 8)
#define ICR_MESSAGE_TYPE_STARTUP (6 << 8)
#define ICR_MESSAGE_TYPE_EXTERNAL (7 << 8)

namespace APIC {
  namespace Local {
    int Initialize();
    
    volatile uint32_t Read(uint32_t off);
    void Write(uint32_t off, uint32_t val);
    
    void SendIPI(uint8_t apic, uint32_t type, uint8_t vector);
  }

  namespace IO {
    // TODO: implemented io
  }

  int Initialize();
}
