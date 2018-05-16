section .text
GLOBAL breakpointTest

breakpointTest:
	int 3h
	ret