section .text

GLOBAL generalProtectionFaultTest

generalProtectionFaultTest:
	mov [variable],byte rax
	ret

section .rodata
variable db 0xA