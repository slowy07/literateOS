#include <system.h>
#include <serial.h>

#define PORT 0x3F8

void initialize_serial() {
  // disable all interrupts
  outportb(PORT + 1, 0x00);
  // enable DLAB (set boud rate divisor)
  outportb(PORT + 3, 0x80);
  // set divisor to 3 (IO byte) 38400 boud
  outportb(PORT + 0, 0x03);
  outportb(PORT + 1, 0x00);
  // 8 bits, no parity, one stop bit
  outportb(PORT + 3, 0x03);
  // enable FIFO, clear them, 14-byte threshold
  outportb(PORT + 2, 0xC7);
  // irq enable DSR set
  outportb(PORT + 4, 0x0B);
}

int is_transmit_empty() {
  return inportb(PORT + 5) & 0x20;
}

void write_serial(const char c) {
  while (is_transmit_empty() == 0);
  outportb(PORT, c);
}

void write_serial(const char* s) {
  while (*s != '\0') {
    while (is_transmit_empty() == 0);
    outportb(PORT, *s++);
  }
}
