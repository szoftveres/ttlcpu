        .global _start

        .text
_start:
        call main
        call _exit

_exit:
        # exit(0)
        mov     $60, %rax               # system call 60 is exit
        xor     %rdi, %rdi              # we want return code 0
        syscall                         # invoke operating system to exit

putc:
         # write(1, message, 13)
         mov     %rsp, %rax
         add     $0x8, %rax
         mov     %rax, %rsi
         mov     $1, %rdx                # number of bytes
         mov     $1, %rdi                # file handle 1 is stdout
         mov     $1, %rax                # system call 1 is write
         syscall                         # invoke os
         ret


