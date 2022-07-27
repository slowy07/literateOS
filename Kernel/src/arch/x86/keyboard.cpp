#include <scheduler.h>
#include <system.h>
#include <idt.h>
#include <logging.h>

uint8_t key = 0;
bool caps = false;

char keymap_us[128] = {
  0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
  '9', '0', '-', '=', '\b', /* backspace */
  '\t', /* tab */
  'q', 'w', 'e', 'r', /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
  0, /* 29 - control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
  '\'', '`',   0,  /* left shift */
  '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
  'm', ',', '.', '/',   0, /* right shift */
  '*',
  0,  /* alt */
  ' ', /* space bar */
  0, /* caps lock */
  0, /* 59 - F1 key */
  0,   0,   0,   0,   0,   0,   0,   0,
  0, /* ... F10 */
  0, /* 69 - num lock */
  0, /* scroll lock */
  0, /* home key */
  0, /* up arrow */
  0, /* pag up */
  '-',
  0, /* left arrow */
  0,
  0, /* right arrow */
  '+',
  0, /* 79 end key */
  0, /* down arrow */
  0, /* page down */
  0, /* insert key */
  0, /* delete key */
  0,   0,   0,
  0, /* F11 key */
  0, /* F12 key */
  0, /* all other keys are undefined */
};

namespace Keyboard {
  message_t wmKeyMessage;
  char test[2];

  // interrupt handler
  void Hanlder(regs32_t* r) {
    asm("cli");
    // read from the keyboard's data buffer
    key = inportb(0x60);
    if (key == 0x3A) {
      caps = !caps;
    } else {
      test[0] = keymap_us[(key >> 7) ? key - 128 :key];
      test[1] = 0;
      Log::Write(test);
      wmKeyMessage.senderPID = 0;
      // should be the window manager
      wmKeyMessage.receiverPID = 1;
      // desktop event key press
      wmKeyMessage.id = 0x1BEEF;
      // the key that was pressed
      wmKeyMessage.data = key;
      Scheduler::SendMessage(wmKeyMessage);
    }
    asm("sti");
  }

  void Install() {
    IDT::RegisterInterruptHandler(33, Handler);
    
    wmKeyMessage.senderPID = 0;
    wmKeyMessage.receiverPID = 1;
    wmKeyMessage.id = 0x1BEEF;
  }
}
