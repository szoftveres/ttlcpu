.global _start


.bss
bss_start:

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


# http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/

.macro SYSCALL_FUNC_0 callnr
    mov     \callnr, %rax
    syscall
    ret
.endm

.macro SYSCALL_FUNC_1 callnr
    mov     0x08(%rsp), %rdi
    mov     \callnr, %rax
    syscall
    ret
.endm

.macro SYSCALL_FUNC_2 callnr
    mov     0x08(%rsp), %rsi
    mov     0x10(%rsp), %rdi
    mov     \callnr, %rax
    syscall
    ret
.endm

.macro SYSCALL_FUNC_3 callnr
    mov     0x08(%rsp), %rdx
    mov     0x10(%rsp), %rsi
    mov     0x18(%rsp), %rdi
    mov     \callnr, %rax
    syscall
    ret
.endm

.macro SYSCALL_FUNC_4 callnr
    mov     0x08(%rsp), %r10
    mov     0x10(%rsp), %rdx
    mov     0x18(%rsp), %rsi
    mov     0x20(%rsp), %rdi
    mov     \callnr, %rax
    syscall
    ret
.endm


# read (fd, buf, count)
read:
    SYSCALL_FUNC_3 $0

# write (fd, buf, count)
write:
    SYSCALL_FUNC_3 $1

# open (pathname, flags, mode)
open:
    SYSCALL_FUNC_3 $2

# close (fd)
close:
    SYSCALL_FUNC_1 $3

# dup (fd)
dup:
    SYSCALL_FUNC_1 $32

# getpid ()
getpid:
    SYSCALL_FUNC_0 $39

# fork ()
fork:
    SYSCALL_FUNC_0 $57

# execve (filename, argv, envp)
execve:
    SYSCALL_FUNC_3 $59

# exit (code)
exit:
    SYSCALL_FUNC_1 $60

# wait4 (pid, stat, options, rusage)
wait4:
    SYSCALL_FUNC_4 $61

## --------------------------End of header---------------------------
