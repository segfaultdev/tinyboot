config_entry:
.entry_loop:
  mov al, [si]
  cmp al, '['
  jne .entry_next
.entry_next:
  call func_next_line
  jmp .entry_loop
  ret

config_property:
  ret
