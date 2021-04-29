menu_init:
  mov ah, 0x01
  mov ch, 0x3F
  int 0x10
  ret

menu_draw:
  push es
  mov ax, 0xB800
  mov es, ax

  mov ah, 0x70
  mov cx, 0x0050
  xor di, di
  rep stosw
  mov cl, 0x50
  mov di, 0x0F00
  rep stosw

  mov dx, 0x0001
  call goto_xy
  mov si, menu_top_str
  call print_str
  mov dx, 0x1801
  call goto_xy
  mov si, menu_btm_str
  call print_str
  pop es

.menu_loop:
  push es
  mov ax, 0xB800
  mov es, ax

  mov di, 0x00A0
  mov ah, 0x1F
  mov cx, 0x0730
  rep stosw

  xor cx, cx
  mov di, 0x0144
.entry_loop:
  cmp cl, [menu_entry_cnt]
  jge .entry_end
  cmp cl, [menu_entry_pos]
  jne .skip_curr
  pusha
  mov ax, 0x7000
  mov cx, 0x4C
  rep stosw
  popa
.skip_curr:
  add di, 0x00A0
  mov dx, 0x0203
  add dh, cl
  call goto_xy
  mov si, menu_top_str
  call print_str
  inc cl
  jmp .entry_loop
.entry_end:
  pop es

.read_key:
  xor ax, ax
  int 0x16

  cmp ah, 0x1C
  je .menu_end
  cmp ah, 0x3B
  je .menu_help
  cmp ah, 0x3D
  je .menu_quit
  cmp ah, 0x48
  je .menu_up
  cmp ah, 0x50
  je .menu_down
  jmp .read_key
.menu_quit:
  mov ax, 0x1000
  mov ax, ss
  mov sp, 0xf000
  mov ax, 0x5307
  mov bx, 0x0001
  mov cx, 0x0003
  int 0x15
  jmp $
.menu_help:
  push es
  mov ax, 0xB800
  mov es, ax
  mov ah, 0x70
  mov cx, 0x0050
  xor di, di
  rep stosw
  mov cl, 0x50
  mov di, 0x0F00
  rep stosw
  mov di, 0x00A0
  mov ah, 0x1F
  mov cx, 0x0730
  rep stosw
  pop es

  mov dx, 0x0001
  call goto_xy
  mov si, menu_help_top_str
  call print_str
  mov dx, 0x1801
  call goto_xy
  mov si, menu_help_btm_str
  call print_str
  pop es

  jmp $
  jmp .menu_help
.menu_up:
  cmp byte [menu_entry_pos], 0x00
  je .read_key
  dec byte [menu_entry_pos]
  jmp .menu_loop
.menu_down:
  mov cl, [menu_entry_cnt]
  test cl, cl
  jz .read_key
  dec cl
  cmp [menu_entry_pos], cl
  jge .read_key
  inc byte [menu_entry_pos]
  jmp .menu_loop
.menu_end:
  ret

menu_entry_cnt:
  db 0x06
menu_entry_pos:
  db 0x00

menu_top_str:
  db "tinyboot - Select an entry", 0x00
menu_btm_str:
  db "ENTER=Boot  ", 0x18, "/", 0x19, "=Move  F1=Help  F3=Quit", 0x00
menu_help_top_str:
  db "tinyboot - Help", 0x00
menu_help_btm_str:
  db "F3=Back", 0x00
