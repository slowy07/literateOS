#pragma once

#include <stdint.h>

namespace timer {
  uint64_t GetSystemUptime();
  uint32_t GetTicks();
  uint32_t GetFrequency();

  void Initialize(uint32_t freq);
}
