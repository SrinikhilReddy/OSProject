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

.extern timer_handler
.extern kb
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
isr_0:
	push
	call isr0
	pop
	iretq
isr_1:
	push
	call isr1
	pop
	iretq
isr_2:
	push
	call isr2
	pop
	iretq
isr_3:
	push
	call isr3
	pop
	iretq
isr_4:
	push
	call isr4
	pop
	iretq
isr_5:
	push
	call isr5
	pop
	iretq
isr_6:
	push
	call isr6
	pop
	iretq
isr_7:
	push
	call isr7
	pop
	iretq
isr_8:
	push
	call isr8
	pop
	iretq
isr_9:
	push
	call isr9
	pop
	iretq
isr_10:
	push
	call isr10
	pop
	iretq
isr_11:
	push
	call isr11
	pop
	iretq
isr_12:
	push
	call isr12
	pop
	iretq
isr_13:
	push
	call isr13
	pop
	iretq
isr_14:
	push
	call isr14
	pop
	iretq
isr_15:
	push
	call isr15
	pop
	iretq
isr_16:
	push
	call isr16
	pop
	iretq
isr_17:
	push
	call isr17
	pop
	iretq
isr_18:
	push
	call isr18
	pop
	iretq
isr_19:
	push
	call isr19
	pop
	iretq
isr_20:
	push
	call isr20
	pop
	iretq
isr_21:
	push
	call isr21
	pop
	iretq
isr_22:
	push
	call isr22
	pop
	iretq
isr_23:
	push
	call isr23
	pop
	iretq
isr_24:
	push
	call isr24
	pop
	iretq
isr_25:
	push
	call isr25
	pop
	iretq
isr_26:
	push
	call isr26
	pop
	iretq
isr_27:
	push
	call isr27
	pop
	iretq
isr_28:
	push
	call isr28
	pop
	iretq
isr_29:
	push
	call isr29
	pop
	iretq

isr_30:
	push
	call isr30
	pop
	iretq
isr_31:
	push
	call isr31
	pop
	iretq

