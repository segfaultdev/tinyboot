[org 0x7C00]
[bits 16]

%define TINYBOOT_STACK       0x7000 ; Stack
%define TINYBOOT_DAP         0x7000 ; Extended INT 0x13 DAP
%define TINYBOOT_DRIVE       0x7010 ; Current INT 0x13 drive
%define TINYBOOT_READ        0x7012 ; Pointer to read function
%define TINYBOOT_CONFIG_PATH 0x7014 ; Pointer to config file path
%define TINYBOOT_HELP_PATH   0x7016 ; Pointer to help file path
%define TINYBOOT_LBA         0x7018 ; LBA address of partition
%define TINYBOOT_BUFFER      0x5000 ; FS temp. buffer
%define TINYBOOT_CONFIG      0x1000 ; Config file buffer
%define TINYBOOT_HELP        0x3000 ; Help file buffer

tinyboot_bpb:
  jmp tinyboot_stage_1
  nop

times 0x0080 - ($ - $$) db 0xCC

tinyboot_stage_1:
  cli					; interrupts should be disabled when setting stack
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
  jl short drv_error_1
.check_lba:
  mov ah, 0x41
  mov bx, 0x55AA
  int 0x13
  jc short ext_error_1
  cmp bx, 0xAA55
  jne short ext_error_1
.setup_dap:
  mov si, TINYBOOT_DAP
  mov byte  [si + 0x00], 0x10
  mov byte  [si + 0x02], 0x03
  mov byte  [si + 0x05], 0x7E

  ; check all 4 mbr pair of qwords
  mov di, mbr_table
  mov cx, 4				; we have 4 pairs
.check_mbr_atr:
  mov eax,   [di + 0x08]		; lba
  test dword [di + 0x00], 0x80		; attr check 8th bit flag
  jnz short .load_stage_2		; if match then load stage 2
  add di, 16				; go to next pair
  loop .check_mbr_atr

  xor eax, eax
.load_stage_2:
  mov [TINYBOOT_DRIVE], dl
  mov [TINYBOOT_LBA], eax
  inc eax
  mov dword [si + 0x08], eax
  mov ah, 0x42
  int 0x13
  jc short ext_error_1
  jmp 0x0000:0x7E00

drv_error_1:
  mov si, drv_error_1_str
  jmp short error_1
ext_error_1:
  mov si, ext_error_1_str

; implicit fallback ;)
; jmp short error_1

error_1:
  mov ah, 0x0E
  xor bh, bh
.str_loop:
  lodsb
  test al, al
  jz short .str_end
  int 0x10
  jmp short .str_loop
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

mbr_table:
  dq 0 ; atr
  dq 0 ; lba

  dq 0
  dq 0

  dq 0
  dq 0

  dq 0
  dq 0

mbr_signature:
  dw 0xAA55

%include "src/stage_2.inc"
