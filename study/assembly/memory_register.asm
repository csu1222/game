%include "io64.inc"

section .text
global CMAIN
CMAIN:
    mov rbp, rsp; for correct debuggingo    mov rbp, rsp; for correct debugging

    ; mov 레지스터, 상수        //상수를 레지스터에 할당한다
    ; mov 레지스터1, 레지스터2  //레지스터2를 레지스터1에 복사핮다
    
    mov eax, 0x1234
    mov rbx, 0x12345678
    mov cl, 0xff
    
    mov al, 0x00
    ;mov rax, rbx
    ;   mov rax, a          ; a변수의 값을 rax 레지스터에 복사한게 아니라 a변수의 주소가 복사되었습니다.
    ;   mov rax, [a]            ; [a]를 이용하여 a변수의 값 자체를 rax 레지스터에 복사하였습니다.
    mov al, [a]
    
    mov [a], byte 0x55
    mov [a], word 0x6666
    mov [a], cl
    
    xor rax, rax
    ret
                
        ; 변수의 선언 및 사용
        ; 처음 변수를 선언할 때 이름과 크기를 지정해줍니다.
        ; 변수에는 구분할 수 있도록 각각의 주소가 있습니다.

section .data   ; 이부분이 메모리의 데이터 섹션을 다루는 부분 
                ;초기화 된 데이터가 있는 섹션입니다
                ;변수 선언 문법은
                ; [변수이름] [크기] [초기값]
                ; [크기]를 가르키는 단어 (define [크기] 의 약자): db(1byte), dw(2byte), dd(4byte), dq(8byte)
    ;예시
    a db 0x11
    b dw 0x2222
    c dd 0x33333333
    d dq 0x4444444444444444

section .bss    ; bss 섹션을 지정하였습니다.
                ; 초기화 되지 않은 데이터들이 있습니다.
                ; 변수 선언은 
                ; [변수이름] [크기] [개수]
                ; bss 영역의 크기는 : resb(1byte), resw(2byte), resd(4byte), resq(8byte)
    e resb 10