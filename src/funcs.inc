func_str_cmp:
  pusha
.str_loop:
  cmp byte [si], 0x00
  je .str_end
  cmpsb
  je .str_loop
  dec si
  dec di
.str_end:
  cmpsb
  popa
  ret

func_mem_cmp:
  pusha
  push si
  repe cmpsb
  pop di
  sub si, di
  cmp si, cx
  popa
  ret

func_next_line:
.char_loop:
  cmp byte [si], 0x20
  jl .char_end
  inc si
  jmp .char_loop
.char_end:
  inc si
  ret
