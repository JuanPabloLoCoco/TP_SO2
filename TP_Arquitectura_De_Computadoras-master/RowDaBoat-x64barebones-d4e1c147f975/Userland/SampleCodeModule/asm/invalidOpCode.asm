section .text

GLOBAL invalidOpCodeTest

invalidOpCodeTest:
	UD2
	ret