.global timer
.global kb1
.global isr
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
.extern kb
.extern isr0
timer:
	push
	callq timer_handler
	pop
	iretq
kb1:
	push
	callq kb
	pop
	iretq
isr:
	push
	callq isr0
	pop
	iretq