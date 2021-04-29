print_chr:
  pusha
  mov ah, 0x0E
  xor bh, bh
  int 0x10
  popa
  ret

print_hex:
  pusha
  mov dx, ax
  shr dl, 0x04
  shr dh, 0x04
  and ax, 0x0F0F
  and dx, 0x0F0F
  add ax, 0x3030
  add dx, 0x3030
.test_al:
  cmp al, '9'
  jle short .test_ah
  add al, 0x07
.test_ah:
  cmp ah, '9'
  jle short .test_dl
  add ah, 0x07
.test_dl:
  cmp dl, '9'
  jle short .test_dh
  add dl, 0x07
.test_dh:
  cmp dh, '9'
  jle short .print_all
  add dh, 0x07
.print_all:
  push ax
  mov al, dh
  call print_chr
  mov al, ah
  call print_chr
  mov al, dl
  call print_chr
  pop ax
  call print_chr
  popa
  ret

print_str:
  push ax
  mov ah, 0x0E
.str_loop:
  lodsb
  test al, al
  jz short .str_end
  int 0x10
  jmp short .str_loop
.str_end:
  pop ax
  ret

goto_xy:
  pusha
  xor bx, bx
  mov ah, 0x02
  int 0x10
  popa
  ret
