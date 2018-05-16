section .text

GLOBAL overflowTest

overflowTest:
	mov rax,0FFFFFFFFFFFFFFFFh
	add rax,qword 0Fh
	ret


