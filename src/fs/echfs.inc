%define ECHFS_SIGNATURE 0x7C04

echfs_detect:
  mov cx, 0x0008
  mov si, echfs_signature
  mov di, ECHFS_SIGNATURE
.cmp_loop:
  test cx, cx
  jz .cmp_end
  dec cx
  cmpsb
  je .cmp_loop
.cmp_end:
  jne .skip_set
  mov word [TINYBOOT_READ], echfs_read
.skip_set:
  ret

echfs_read:
  ; TODO
  ret

echfs_signature:
  db "_ECH_FS_"
