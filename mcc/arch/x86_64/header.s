.global _start

.text

_start:                         # call main function then exit

    mov     %rsp, %rax
    mov     (%rax),%rax         # argc
    push    %rax
    mov     %rsp, %rax
    add     $0x10, %rax         # argv
    push    %rax
    call main
    pop     %rdi
    pop     %rdi
    mov     %rax, %rdi          # rdi contains exit code
    mov     $60, %rax           # system call 60 is exit
    syscall                     # invoke operating system to exit

putc:
    # write(1, message, 13)     # write 1 char to stdout
    mov     %rsp, %rax
    add     $0x8, %rax
    mov     %rax, %rsi
    mov     $1, %rdx            # number of bytes
    mov     $1, %rdi            # file handle 1 is stdout
    mov     $1, %rax            # system call 1 is write
    syscall                     # invoke os
    ret

## --------------------------End of header---------------------------
