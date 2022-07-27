global memcpy_sse2

memcpy_sse2:
  ; save the prior EBP value
  push ebp
  mov ebp, esp

  mov eax, [ebp + 8]
  mov ebx, [ebp + 12]
  mov ecx, [ebp + 16]

.loop:
  movdqa xmm0, [ebx]
  movdqa [eax], xm0

  add eax, 0x10
  add ebx, 0x10
  loop .loop

  mov esp, ebp
  pop ebp
  ret

global memcpy_sse2_unaligned


memcpy_sse_unaligned:
  push ebp
  mov ebp, esp
  
  mov eax, [ebp + 8]
  mov ebx, [ebp + 12]
  mov ecx, [ebp + 16]

.loop:

  movdqu xmm0, [ebx]
  movdqu [eax], xmm0

  add eax, 0x10
  add ebx, 0x10
  loop .loop

  mov esp, ebp
  pop ebp
  ret

