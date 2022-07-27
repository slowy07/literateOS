#include <stddef.h>
#include <string.h>

#pragma GCC push_options
#pragma GCC optimize ("O3")
extern "C" void memcpy_sse2(void* dest, void* src, size_t count);
extern "C" void memcpy_sse_unaligned(void* dest, void* src, size_t count);

void memcpy_optimized(void* dest, void* src, size_t count) {
  // amount of overflow bytes
  size_t overflow = (count % 0x10);
  // rounded down lowest multiple 128 bits
  size_t size_aligned = (count - overflow);

  if (((size_t)dest % 0x10) || ((size_t)src % 0x10))
    memcpy_sse_unaligned(dest, src, size_aligned/0x10);
  else
    memcpy_sse2(dest, src, size_aligned / 0x10);

  if (overflow > 0)
    memcpy(dest + size_aligned, src + size_aligned, overflow);
}

#pragma GCC pop_options
