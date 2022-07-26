#pragma once
#include <videoconsole.h>
#include <stdarg.h>

namespace Log {
  void Initialize();
  void SetVideoConsole(VideoConsole* con);
  void EnableBuffer();

  void WriteF(const char* __restrict format, va_list args);

  void Write(const char* str, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
  void Write(unsigned long long num, bool hex = true, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

  void Warning(unsigned long long num);
  void Warning(unsigned char* __restrict fmt, ...);

  void Error(unsigned long long num, bool hex = true);
  void Error(const char* __restrict fmt, ...);

  void Info(unsigned long long num, bool hex = true);
  void Info(const char* __restrict fmt, ...);
}
