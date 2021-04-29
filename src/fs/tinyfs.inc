%define TINYFS_START     0x7DA6
%define TINYFS_SIZE      0x7DAE
%define TINYFS_SIGNATURE 0x7DB6

tinyfs_detect:
  cmp word [TINYFS_SIGNATURE], 0xF007
  jne .skip_set
  mov word [TINYBOOT_READ], tinyfs_read
.skip_set:
  ret

tinyfs_read:
  push es
  push di
  mov ebx, [TINYFS_START]
  xor ecx, ecx
  xor ax, ax
  mov es, ax
  mov di, TINYBOOT_BUFFER
.header_loop:
  inc cx
  push es
  call drive_read
  pop es
  cmp byte [es:di], 0x80
  jne .header_next
  add di, 0x0002
  jc .tinyfs_error
  call func_str_cmp
  je .header_end
.header_next:
  sub di, 0x0002
  add ebx, [es:di + 0x01F0]
  jmp .header_loop
.header_end:
  sub di, 0x0002
.block_loop:
  mov ecx, [es:di + 0x01F0]
  mov ax, es
  mov fs, ax
  pop di
  pop es
  call drive_read
  push es
  push di
  mov edx, [fs:di + 0x01F8]
  test edx, edx
  jz .block_end
  mov ebx, edx
  inc cx
  call drive_read
  jmp .block_loop
.block_end:
  pop di
  pop es
  ret
.tinyfs_error:
  mov si, tinyfs_error_str
  call print_str
  jmp $

tinyfs_error_str:
  db "tinyboot: tinyfs error", 0x0A, 0x00
