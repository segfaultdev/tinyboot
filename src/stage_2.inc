tinyboot_stage_2:
  call menu_init

  mov si, detect_str
  call print_str

  call tinyfs_detect
  je short .load_config

  call echfs_detect
  je short .load_config

  call fat32_detect
  je short .load_config

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

%include "src/print.inc"
%include "src/funcs.inc"
%include "src/drive.inc"

%include "src/fs/tinyfs.inc"
%include "src/fs/echfs.inc"
%include "src/fs/fat32.inc"

%include "src/menu.inc"

test_filename_str:
  db "TEST    TXT", 0x00

detect_str:
  db "tinyboot: Detecting filesystems", 0x0A, 0x0D, 0x00
detect_fail_str:
  db "tinyboot: No filesystem found!", 0x0A, 0x0D, 0x00

times 0x0800 - ($ - $$) db 0xCC
