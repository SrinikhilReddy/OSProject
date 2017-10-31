.text
.globl loadPageDirectory
loadPageDirectory:
push %rbp
mov %rsp, %rbp
mov 8(%rsp), %rax
mov %rax, %cr3
mov %rbp, %rsp
pop %rbp
ret

.text
.globl enablePaging
enablePaging:
push %rbp
mov %rsp, %rbp
mov %cr0, %rax
or $0x80000000, %rax
mov %rax, %cr0
mov %rbp, %rsp
pop %rbp
ret
