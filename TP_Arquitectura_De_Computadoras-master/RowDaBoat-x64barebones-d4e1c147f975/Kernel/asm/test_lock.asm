GLOBAL _unlocked

section .text
_unlocked:
  mov rax, 0
  xchg rax, [rdi]
	ret
