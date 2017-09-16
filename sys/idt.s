.global timer
.macro pop
popq %r11 
popq %r10 
popq %r9 
popq %r8 
popq %rdi 
popq %rsi 
popq %rdx 
popq %rcx 
popq %rax 
.endm
.macro push
pushq %rax 
pushq %rcx 
pushq %rdx 
pushq %rsi 
pushq %rdi 
pushq %r8 
pushq %r9 
pushq %r10 
pushq %r11
.endm
.extern timer_handler
timer:
cli
push
callq timer_handler
pop
sti
iretq
