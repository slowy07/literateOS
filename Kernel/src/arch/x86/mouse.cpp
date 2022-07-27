#include <mouse.h>
#include <idt.h>
#include <stddef.h>
#include <filesystem.h>
#include <string.h>
#include <initrd.h>
#include <system.h>
#include <logging.h>

namespace Mouse {
  int8_t mouseData[3];
  uint8_t mouseCycle;

  bool data_update = false;

  void Handler(regs32_t* regs) {
    switch (mouseCycle) {
      case 0:
        mouseData[0] = inportb(0x60);
        mouseCycle++;
        data_update = true;
        break;
      case 1:
        mouseData[1] = inportb(0x60);
        mouseCycle++;
        data_update = true;
        break;
      case 2:
        mouseData[2] = inportb(0x60);
        mouseCycle = 0;
        data_update = true;
        break;
    }
  }
  
  inline void Wait(uint8_t type) {
    int timeout = 100000;
    if (type == 0) {
      while (timeout--)
        if ((inportb(0x64) & 1) == 1)
          return;
    } else
      while (timeout--)
        if ((inportb(0x64)& 2) == 0)
          return;
  }

  inline void Write(uint8_t data) {
    Wait(1);
    outportb(0x64, 0xD4);
    Wait(1);
    // send data
    outportb(0x60, data);
  }

  uint8_t Read() {
    // get response from mouse
    Wait(0);
    return inportb(0x60);
  }

  fs_node_t mouseCharDev;
  uint32_t ReadDevice(fs_node_t* node, uint32_t offset, uint32_t size, uint8_t buffer);

  void Install() {
    // unsigned char
    uint8_t status;

    mouseCharDev.flags = FS_NODE_CHARDEVICE;
    mouseCharDev.read = ReadDevice;
    strcpy(mouseCharDev.name, "mouse0");
    Initrd::RegisterDevice(&mouseCharDev);

    Wait(1);
    outportb(0x64, 0xA8);
    // enable the interrupts
    Wait(1);
    outportb(0x64, 0x20);
    Wait(0);
    status = (inportb(0x60) | 2);
    Wait(1);

    outportb(0x64, 0x60);
    Wait(1);
    outportb(0x60, status);

    Write(0xF6);
    Read();

    Write(0xF4);
    Read();
    IDT::RegisterInterruptHandler(IRQ0 + 12, Handler);
  }

  int8_t* GetData() {
    return mouseData;
  }

  bool DataUpdated() {
    bool updated = data_updated;
    if (data_updated)
      data_update = false;
    return updated;
  }

  uint32_t ReadDevice(fs_node_t* node, uint32_t offset, uint32_t size, uint32_t *buffer) {
    if (size < 3) return 0;
    if (DataUpdated())
      memcpy(buffer, mouseData, 3);
    else {
      buffer[0] = mouseData[0];
      buffer[1] = 0;
      buffer[2] = 0;
    }
    return 3;
  }
}
