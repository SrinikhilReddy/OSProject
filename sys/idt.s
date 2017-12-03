.global timer
.global kb1

.global isr_0
.global isr_1
.global isr_2
.global isr_3
.global isr_4
.global isr_5
.global isr_6
.global isr_7
.global isr_8
.global isr_9
.global isr_10
.global isr_11
.global isr_12
.global isr_13
.global isr_14
.global isr_15
.global isr_16
.global isr_17
.global isr_18
.global isr_19
.global isr_20
.global isr_21
.global isr_22
.global isr_23
.global isr_24
.global isr_25
.global isr_26
.global isr_27
.global isr_28
.global isr_29
.global isr_30
.global isr_31
.global isr_128

.extern timer_handler
.extern write_terminal
.extern isr0
.extern isr1
.extern isr2
.extern isr3
.extern isr4
.extern isr5
.extern isr6
.extern isr7
.extern isr8
.extern isr9
.extern isr10
.extern isr11
.extern isr12
.extern isr13
.extern isr14
.extern isr15
.extern isr16
.extern isr17
.extern isr18
.extern isr19
.extern isr20
.extern isr21
.extern isr22
.extern isr23
.extern isr24
.extern isr25
.extern isr26
.extern isr27
.extern isr28
.extern isr29
.extern isr30
.extern isr31


.macro pop
popq %r15
popq %r14
popq %r13
popq %r12
popq %r11
popq %r10
popq %r9
popq %r8
popq %rbp
popq %rdi
popq %rsi
popq %rdx
popq %rcx 
popq %rbx 
popq %rax 
.endm

.macro push
pushq %rax 
pushq %rbx 
pushq %rcx 
pushq %rdx 
pushq %rsi 
pushq %rdi
pushq %rbp
pushq %r8
pushq %r9
pushq %r10
pushq %r11
pushq %r12
pushq %r13
pushq %r14
pushq %r15
.endm

timer:
	push
	callq timer_handler
	pop
	iretq
kb1:
	push
	callq write_terminal
	pop
	iretq
isr_0:
	push
	callq isr0
	pop
	iretq
isr_1:
	push
	callq isr1
	pop
	iretq
isr_2:
	push
	callq isr2
	pop
	iretq
isr_3:
	push
	callq isr3
	pop
	iretq
isr_4:
	push
	callq isr4
	pop
	iretq
isr_5:
	push
	callq isr5
	pop
	iretq
isr_6:
	push
	callq isr6
	pop
	iretq
isr_7:
	push
	callq isr7
	pop
	iretq
isr_8:
	push
	callq isr8
	pop
	iretq
isr_9:
	push
	callq isr9
	pop
	iretq
isr_10:
	push
	callq isr10
	pop
	iretq
isr_11:
	push
	callq isr11
	pop
	iretq
isr_12:
	push
	callq isr12
	pop
	iretq
isr_13:
	push
	callq isr13
	pop
	iretq
isr_14:
	push
	callq isr14
	pop
	add $8, %rsp
	iretq
isr_15:
	push
	callq isr15
	pop
	iretq
isr_16:
	push
	callq isr16
	pop
	iretq
isr_17:
	push
	callq isr17
	pop
	iretq
isr_18:
	push
	callq isr18
	pop
	iretq
isr_19:
	push
	callq isr19
	pop
	iretq
isr_20:
	push
	callq isr20
	pop
	iretq
isr_21:
	push
	callq isr21
	pop
	iretq
isr_22:
	push
	callq isr22
	pop
	iretq
isr_23:
	push
	callq isr23
	pop
	iretq
isr_24:
	push
	callq isr24
	pop
	iretq
isr_25:
	push
	callq isr25
	pop
	iretq
isr_26:
	push
	callq isr26
	pop
	iretq
isr_27:
	push
	callq isr27
	pop
	iretq
isr_28:
	push
	callq isr28
	pop
	iretq
isr_29:
	push
	callq isr29
	pop
	iretq

isr_30:
	push
	callq isr30
	pop
	iretq
isr_31:
	push
	callq isr31
	pop
	iretq
isr_128:
	cli
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq %rsp,%rdi
	callq isr128
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9 
	popq %r8 
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rdx
	popq %rcx 
	popq %rbx 
	sti	
	iretq
