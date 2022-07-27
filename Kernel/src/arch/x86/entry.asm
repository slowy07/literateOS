BITS 32

global entry
extern kmain

MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
VIDINFO equ 1 << 2
MAGIC equ 0x1BADB002
FLAGS equ MBALIGN | MEMINFO | VIDINFO
CHECKSUM equ -(MAGIC + FLAGS)

KERNEL_VIRTUAL_BASE equ 0xC0000000
KERNEL_PAGE_DIR_NUMBER equ (KERNEL_VIRTUAL_BASE >> 22) ; 768

section .multiboot.hdr
align 4 ; multiboot header

multiboot_header:
dd MAGIC
dd FLAGS
dd CHECKSUM
dd 0
dd 0
dd 0
dd 0
dd 0

dd 0
dd 0
dd 0
dd 32

section .multiboot.data

align 0x1000

boot_page_directory:
  times 1024 dd 0
boot_page_table:
  times 1024 dd 0
boot_page_table2:
  times 1024 dd 0

section .multiboot.text
entry:
  mov edi, boot_page_table1
  mov ecx, 2048
  mov esi, 0

next_page:
  mov edx, esi
  or edx, 0x003
  mov [edi], edx
  
  add esi, 4096
  add edi, 4

  loop next_page
  
  mov dword [boot_page_directory], (boot_page_table1 + 0x003)
  mov dword [boot_page_directory + 4], (boot_page_table2 + 0x003)
  mov dword [boot_page_directory + (KERNEL_PAGE_DIR_NUMBER * 4)], (boot_page_table1 + 0x003)
  mov dword [boot_page_directory + (KERNEL_PAGE_DIR_NUMBER * 4) + 4], (boot_page_table2 + 0x003)

  ; load page directory
  mov ecx, boot_page_directory
  mov cr3, ecx
  
  mov ecx, cr0
  ; enable paging
  or ecx, 0x80000000
  mov cr0, ecx

  lea ecx, [entry_hh]
  jmp ecx

section .text
entry_hh:
  mov esp, stack_top
  push eax

  mov eax, cr0
  ; clear coprocessor emulation
  and ax, 0xFFFB
  ; set coprocessor monitoring
  or ax, 0x2
  mov cr0, eax

  ; enable SSE
  mov eax, cr4
  ; set flags SSE
  or ax, 3 << 9
  mov cr4, eax

  add ebx, KERNEL_VIRTUAL_BASE

  push ebx

  ; call kernel main
  call kmain

  cli
  hlt

section .bss
align 32
stack_bottom:
resb 16384
stack_top:
