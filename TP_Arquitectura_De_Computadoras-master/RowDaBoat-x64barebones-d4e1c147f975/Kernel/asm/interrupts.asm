
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _lidt
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _change_process
GLOBAL _yield_process
GLOBAL _yield_interrupt

GLOBAL _systemCallsHandler

GLOBAL _exception0Handler
GLOBAL _exception3Handler
GLOBAL _exception4Handler
GLOBAL _exception6Handler
GLOBAL _exception13Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN systemCallDispatcher
EXTERN main
EXTERN next_process
EXTERN timer_handler

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push fs
	push gs
%endmacro

%macro popState 0
	pop gs
	pop fs
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro



;Fuente:http://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/
%macro systemCallHandler 0
	call systemCallDispatcher
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp
	call exceptionDispatcher
	popState
	mov qword [rsp], main
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp
  mov rbp, rsp
  mov ax, di
  out	21h,al
  pop rbp
  retn

picSlaveMask:
	push rbp
  mov rbp, rsp
  mov ax, di  ; ax = mascara de 16 bits
  out	0A1h,al
  pop rbp
  retn

_lidt:				; Carga el IDTR
  push    rbp
  mov     rbp, rsp
  push    rbx
  mov     rbx, rdi ; puntero a IDTR
  lidt    [rbx]          ; carga IDTR
  pop     rbx
  pop     rbp
  retn

;8254 Timer (Timer Tick)
_irq00Handler:
	pushState

	call timer_handler

	mov rdi, rsp
	call next_process

	mov rsp, rax

	mov al, 0x20
	out 0x20, al

	popState

	iretq

_change_process:
	mov rsp, rdi
	popState
	iretq

_yield_process:
	int 70h
	ret

_yield_interrupt:
	pushState

	mov rdi, rsp
	call next_process

	mov rsp,rax
	popState

	iretq

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

;Systems calls handler
_systemCallsHandler:
	systemCallHandler


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Breakpoint Exception
_exception3Handler:
	exceptionHandler 3

;Overflow Exception
_exception4Handler:
	exceptionHandler 4

;Invalid Opcode Exception
_exception6Handler:
	exceptionHandler 6

;General Protection Fault
_exception13Handler:
	exceptionHandler 13

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1
