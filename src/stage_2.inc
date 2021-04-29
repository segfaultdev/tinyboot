tinyboot_stage_2:
  call menu_init

  mov si, detect_str
  call print_str

  call tinyfs_detect
  je .load_config

  call echfs_detect
  je .load_config

  call fat32_detect
  je .load_config

  mov si, detect_fail_str
  call print_str

  jmp $

.load_config:
  mov si, test_filename_str
  mov di, 0x1000
  call fat32_read

  mov si, 0x1000
  call print_str

  ; call menu_draw
  jmp $

%include "print.inc"
%include "funcs.inc"
%include "drive.inc"

%include "fs/tinyfs.inc"
%include "fs/echfs.inc"
%include "fs/fat32.inc"

%include "menu.inc"

test_filename_str:
  db "TEST    TXT", 0x00

detect_str:
  db "tinyboot: Detecting filesystems", 0x0A, 0x0D, 0x00
detect_fail_str:
  db "tinyboot: No filesystem found!", 0x0A, 0x0D, 0x00

times 0x0800 - ($ - $$) db 0xCC
