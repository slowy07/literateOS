#pragma once

#include <stdint.h>

typedef struct {
  // resolution width
  uint32_t width;
  // resolution height
  uint32_t height;
  // resolution depth/bits per pixel
  uint16_t bpp;

  // video mode pitch
  uint32_t pitch;
} __attribute__((packed)) fb_info_t;
