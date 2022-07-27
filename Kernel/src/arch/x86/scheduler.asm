extern processStack
extern processBase
extern processEntryPoint
extern processPageDirectory

global TaskSwitch
global ReadEIP
global IdleProc

extern IdleProcess

ReadEIP:
  mov eax, [esp]
  ret

TaskSwitch:
  cli
  mov ebx, [processEntryPoint]
  mov esp, [processStack]
  mov ebp, [processBase]
  mov eax, [processPageDirectory]
  mov cr3, eax
  ; let the scheduler kno the task has been
  ; switches
  mov eax, 0xC0000123
  sti
  jmp ebx

IdleProc:
  call IdleProcess
  jmp IdleProc
