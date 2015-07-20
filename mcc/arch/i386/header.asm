section .data               ;section declaration

section .text               ;section declaration
    global _start

_start:
    call    main
    push    eax         
    call    exit
    pop     ebx         
    ret

exit:
    push    ebp
    mov     ebp, esp
    mov     ebx, [ebp + 0x8]; exit code
    mov     eax, 0x1        ; system call number (sys_exit)
    int     0x80            ; call kernel
    pop     ebp
    ret

putc:
    push    ebp
    mov     ebp, esp
    mov     edx, 0x1        ; length (1 byte)
    mov     ecx, ebp        ; pointer to message to write
    add     ecx, 0x8        ; ebp + 0x8
    mov     ebx, 0x1        ; file handle (stdout)
    mov     eax, 0x4        ; system call number (sys_write)
    int     0x80            ; call kernel
    pop     ebp
    ret

getc:
    push    ebp
    sub     esp, 0x4        ; lcl variable
    mov     ebp, esp
    mov     [ebp + 0], DWORD 0x0
    mov     edx, 0x1        ; length (1 byte)
    mov     ecx, ebp        ; pointer to message to write
    mov     ebx, 0x0        ; file handle (stdin)
    mov     eax, 0x3        ; system call number (sys_read)
    int     0x80            ; call kernel
    mov     eax, [ebp + 0]  ; ret value
    add     esp, 0x4
    pop     ebp
    ret

