section .text
GLOBAL _int80
%macro pushRegisters 0
	push rbx
	push rcx
	push rdx
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
%endmacro;


%macro popRegisters 0
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
	pop rdx
	pop rcx
	pop rbx
%endmacro

_int80:
    
    push rbp
    mov rbp,rsp
	
    int 80h
	
	mov rsp,rbp
	pop rbp
	ret


