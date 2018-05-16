GLOBAL getSeconds
GLOBAL getMinutes
GLOBAL getHours
GLOBAL getYear
GLOBAL getMonth
GLOBAL getDay

section .text

RTCConfig:
	cli
	mov al,0Bh 
	out 70h,al
	in al,71h
	or al, 0100b
	out 71h,al
	sti
	ret

getSeconds:
	push rbp
	mov rbp,rsp
	mov rax,0h
	call RTCConfig
	mov rax,0h
	mov al,0h
	out 70h,al
	in al,71h
	movzx rax,al
	mov rsp,rbp
	pop rbp
	ret

getMinutes:
	push rbp
	mov rbp,rsp
	mov rax,0h
	call RTCConfig
	mov rax,0h
	mov al,02h
	out 70h,al
	in al,71h
	movzx rax,al
	mov rsp,rbp
	pop rbp
	ret

getHours:
	push rbp
	mov rbp,rsp
	mov rax,0h
	call RTCConfig
	mov rax,0h
	mov al,04h
	out 70h,al
	in al,71h
	sub al,03h
	JNS end
	add al,18h
	end:
	movzx rax,al
	mov rsp,rbp
	pop rbp
	ret


getYear:
	push rbp
	mov rbp,rsp
	mov rax,0h
	call RTCConfig
	mov rax,0h
	mov al,09h
	out 70h,al
	in al,71h
	movzx rax,al
	mov rsp,rbp
	pop rbp
	ret
	

getMonth:
	push rbp
	mov rbp,rsp
	mov rax,0h
	call RTCConfig
	mov rax,0h
	mov al,08h
	out 70h,al
	in al,71h
	movzx rax,al
	mov rsp,rbp
	pop rbp
	ret


getDay:
	push rbp
	mov rbp,rsp
	mov rax,0h
	call RTCConfig
	mov rax,0h
	mov al,07h
	out 70h,al
	in al,71h
	movzx rax,al
	mov rsp,rbp
	pop rbp
	ret