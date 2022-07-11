%include "io64.inc"

section .text
global CMAIN
CMAIN:
    ;write your code here
    
    ; 반복문 (while, for)
    ; 특정 조건은 만족 할 때 까지 반복
    
    ; ex) Hello World를 10번 반복 출력하려면?
    mov ax, 0       ; 혼자 구현해본 루프 
    mov bx, 10
init:
    cmp ax, bx
    jl loop1
    jmp loop_end
loop1:
    add ax, 1
    PRINT_STRING msg
    NEWLINE
    jmp init
loop_end:
    PRINT_STRING end
    NEWLINE
    
    
    mov ecx, 10      ; 강의에서 알려준 루프문 
    
LABEL_LOOP:
    PRINT_STRING msg
    NEWLINE
    dec ecx         ; 설정해놓은 10을 하나씩 줄이는 방법 sub ecx, 1 도 가능하지만 dec(decrescent) 가 좀 더 빠르다고 합니다.
    cmp ecx, 0      ; inc ecx 는 하나씩 늘리는 방법
    jne LABEL_LOOP
    
    ; 연습문제) 1~100 까지의 숫자를 합하면?
    mov ax, 0
    mov cx, 100
    
add_loop:
    add ax, cx
    
    dec ecx
    cmp ecx, 0
    jne add_loop

    PRINT_DEC 1, ax
    NEWLINE
    
    ; 어셈블리 자체적으로 loop 구문이 있습니다.
    ; loop [label]     형식
    ; - loop 할 때 마다 ecx의 값이 1씩 줄어들고 ecx가 0이면 빠져나가고 아니면 label로 루프합니다.
    ; 위 분기문으로 구현 한 예제를 loop로 다시 구현해보겠습니다.
    mov ecx, 100
    xor ebx, ebx ; mov ebx, 0 과 같은 결과
    
LABEL_LOOP_SUM:
    add ebx, ecx
    loop LABEL_LOOP_SUM
    
    PRINT_DEC 1, ebx
    NEWLINE
    
    xor rax, rax
    ret

section .data
    msg db 'Hello World', 0x00
    end db 'end', 0x00