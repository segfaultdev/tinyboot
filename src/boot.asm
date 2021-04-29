[org 0x7C00]
[bits 16]

%define TINYBOOT_STACK  0x7000 ; Stack
%define TINYBOOT_DAP    0x7000 ; Extended INT 0x13 DAP
%define TINYBOOT_DRIVE  0x7010 ; Current INT 0x13 drive
%define TINYBOOT_READ   0x7012 ; Pointer to read function
%define TINYBOOT_LBA    0x7014 ; LBA address of partition
%define TINYBOOT_BUFFER 0x0600 ; FS temp. buffer

tinyboot_bpb:
  jmp tinyboot_stage_1
  nop

times 0x0080 - ($ - $$) db 0xCC

tinyboot_stage_1:
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, TINYBOOT_STACK
  cld
  mov cx, 0x0010
  mov di, TINYBOOT_DAP
  rep stosb
  sti
.check_drv:
  cmp dl, 0x80
  jl drv_error_1
.check_lba:
  mov ah, 0x41
  mov bx, 0x55AA
  int 0x13
  jc ext_error_1
  cmp bx, 0xAA55
  jne ext_error_1
.setup_dap:
  mov si, TINYBOOT_DAP
  mov byte  [si + 0x00], 0x10
  mov byte  [si + 0x02], 0x03
  mov byte  [si + 0x05], 0x7E
  mov eax, [mbr_lba_1]
  test byte [mbr_atr_1], 0x80
  jnz .load_stage_2
  mov eax, [mbr_lba_2]
  test byte [mbr_atr_2], 0x80
  jnz .load_stage_2
  mov eax, [mbr_lba_3]
  test byte [mbr_atr_3], 0x80
  jnz .load_stage_2
  mov eax, [mbr_lba_4]
  test byte [mbr_atr_4], 0x80
  jnz .load_stage_2
  xor eax, eax
.load_stage_2:
  mov [TINYBOOT_DRIVE], dl
  mov [TINYBOOT_LBA], eax
  inc eax
  mov dword [si + 0x08], eax
  mov ah, 0x42
  int 0x13
  jc ext_error_1
  jmp 0x0000:0x7E00

drv_error_1:
  mov si, drv_error_1_str
  jmp error_1
ext_error_1:
  mov si, ext_error_1_str
  jmp error_1

error_1:
.str_loop:
  mov al, [si]
  test al, al
  jz .str_end
  mov ah, 0x0E
  xor bh, bh
  int 0x10
  inc si
  jmp .str_loop
.str_end:
  jmp $


drv_error_1_str:
  db "tinyboot: Invalid drive", 0x00
ext_error_1_str:
  db "tinyboot: Read error", 0x00

times 0x01B8 - ($ - $$) db 0xCC

mbr_uuid:
  dd 0x00000000
mbr_reserved:
  dw 0x0000

mbr_atr_1:
  dq 0x0000000000000000
mbr_lba_1:
  dq 0x0000000000000000

mbr_atr_2:
  dq 0x0000000000000000
mbr_lba_2:
  dq 0x0000000000000000

mbr_atr_3:
  dq 0x0000000000000000
mbr_lba_3:
  dq 0x0000000000000000

mbr_atr_4:
  dq 0x0000000000000000
mbr_lba_4:
  dq 0x0000000000000000

mbr_signature:
  dw 0xAA55

%include "src/stage_2.inc"
