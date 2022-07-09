%include "io64.inc"

section .text
global CMAIN
CMAIN:

    ; mov 레지스터, 상수        //상수를 레지스터에 할당한다
    ; mov 레지스터1, 레지스터2  //레지스터2를 레지스터1에 복사핮다
    
    mov eax, 0x1234
    mov rbx, 0x12345678
    mov cl, 0xff
    
    mov al, 0x00
    mov rax, rbx
    
    xor rax, rax
    ret

section .data
    msg db 'Hello, world', 0x00