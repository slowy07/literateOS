#pragma once

#include <stdint.h>
#include <acpispec/tables.h>

typedef struct MADT {
  acpi_header_t header;
  uint32_t localAPICAddress;
  uint32_t flags;
} __attribute__((packed)) acpi_mode_t;

typedef struct MADTEntry {
  uint8_t entry;
  uint8_t length;
} __attribute__((packed)) acpi_madt_entry_t;

typedef struct LocalAPIC {
  // madt entry structure
  acpi_madt_entry_t entry;
  uint8_t processorID;
  uint8_t apicID;
  uint32_t flags;
} __attribute__((packed)) apic_local_t;

typedef struct IOAPIC {
  acpi_madt_entry_t entry;
  uint8_t apicID;
  uint8_t reserved;
  uint32_t address;
  uint32_t gSIB;
} __attribute__((packed)) apic_io_t;

typedef struct ISO {
  acpi_madt_entry_t entry;
  uint8_t busSource;
  uint8_t irqSource;
  uint32_t gSI;
  uint16_t flags;
} __attribute__((packed)) apic_iso_t;

typedef struct NMI {
  acpi_madt_entry_t entry;
  uint8_t processorID;
  uint64_t flags;
  uint8_t lINT;
} __attribute__((packed)) apic_nmi_t;

typedef struct LocalAPICAddressOverr {
  // madt entry structure
  acpi_madt_entry_t entry;
  // reserved
  uint16_t reserved;
  // 64-bit address of local APIC
  uint64_t address;
} __attribute__((packed)) apic_local_address_override_t;

namespace ACPI {
  extern uint8_t processors[];
  extern int processCount;

  extern acpi_resdp_t* desc;
  extern acpi_resdt_t* resdtHeader;

  void Init();
  void Reset();
}
