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
  xor ax, ax
  mov es, ax
  mov si, [TINYBOOT_CONFIG_PATH]
  mov di, TINYBOOT_CONFIG
  mov bx, [TINYBOOT_READ]
  call bx

  xor ax, ax
  mov es, ax
  mov si, [TINYBOOT_HELP_PATH]
  mov di, TINYBOOT_HELP
  mov bx, [TINYBOOT_READ]
  call bx

  call menu_draw
  jmp $

%include "src/print.inc"
%include "src/funcs.inc"
%include "src/drive.inc"

%include "src/fs/tinyfs.inc"
%include "src/fs/echfs.inc"
%include "src/fs/fat32.inc"

%include "src/menu.inc"

detect_str:
  db "tinyboot: Detecting filesystems", 0x0A, 0x00
detect_fail_str:
  db "tinyboot: No filesystem found!", 0x0A, 0x00

times 0x0800 - ($ - $$) db 0xCC
