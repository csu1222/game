%include "io64.inc"

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debugging

    mov eax, 0x1234
    mov rbx, 0x12345678
    mov cl, 0xff
    
    mov al, 0x00 ; 레지스터의 기존 값에 입력 만큼 덮어 씌운다
    mov rax, rbx 
    
    xor rax, rax
    ret
    

;section .data