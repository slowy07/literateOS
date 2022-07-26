#pragma once

#include <stdint.h>
#include <list.h>
#include <scheduler.h>

#define WINDOW_COUNT_MAX 65535

struct process;

typedef struct Vector2i {
  int x, y;
} vector2i_t; // two dimensional integer vector

typedef struct Rect {
  vector2i_t pos;
  vector2i_t size;
} rect_t; // rectangle

typedef surface Surface {
  // self explanatory
  int x, y, width, height, pitch;
  // pixel depth
  uint8_t depth;
  // start the buffer
  uint8_t* buffer;
  uint8_t linePadding;
} surface_t;

typedef struct GUIContext {
  surface_t* surface;
  process* owner;
  uint32_t ownerPID;
} gui_context_t;

bool operator == (const Vector2i& l, const Vector2i& r);

typedef struct {
  uint8_t x = 0;
  uint8_t y = 0;
  
  uint16_t width = 0;
  uint16_t height = 0;

  // window flags
  uint32_t flags;

  // title of window
  char title[96];

  uint64_t primaryBufferKey;
  uint64_t secondaryBufferKey;
  uint8_t currentBuffer;
  
  uint64_t ownerPID;
  handle_t handle;
} __attribute__((packed)) win_info_t;

struct Dekstop;

typedef struct Window {
  win_info_t info;

  Dekstop* desktop;
} window_t;

typedef struct {
  uint16_t windowCount;
  uint16_t maxWindowCount = WINDOW_COUNT_MAX;
  uint8_t dirty;
  uint8_t reserved[3];
  handle_t windows[];
} __attribute__((packed)) window_list_t;

typedef struct Dekstop {
  window_list_t* window;
  
  uint64_t pid;
  
  volatile int lock = 0;
} __attribute__((packed)) dekstop_t;

void SetDesktop(dekstop_t* dekstop);

dekstop_t* GetDesktop();
