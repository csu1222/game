%include "io64.inc"

section .text
global CMAIN
CMAIN:

    ; 분기문 (if)
    ; 특정 조건에 따라 코드 흐름을 제어하는 것
    ; ex) 스킬 버튼을 눌렀는가? yes-> 스킬사용
    ; ex) 제한 시간내에 던전 입장 수락 버튼을 눌렀는가? yes->던전입장, no->던전 취소
    
    ; 조건 -> 흐름
    
    ; CMP dst, src (dst가 기준)
    ; 비교를 한 결과물은 Flag Register에 저장 
    
    ; JMP [label] 시리즈 #JMP는 jump의 줄임말
    ; JMP : 무조건 jump
    ; JE : JumpEquals 같으면 jump
    ; JNE : JumpNotEquals 다르면 jump
    ; JG : JumpGreater 크면 jump
    ; JGE : JumpGreaterEquals 크거나 같으면 jump
    ; JL : JumpLess 작으면 jump
    ; JLE : JumpLessEquals 작거나 같으면 Jump
    
    ; 실습
    ; 두 숫자가 같으면 1, 아니면 0을 출력하는 프로그램
    mov rax, 10
    mov rbx, 10
    
    cmp rax, rbx
    
    je LABEL_EQUAL
    ; je에 의해 점프를 안했다면, 같지 않다는 의미
    mov rcx, 0
    jmp LABEL_EQUAL_END
    
LABEL_EQUAL:    ; je 가 참이면 여기로 점프
    mov rcx, 1
LABEL_EQUAL_END:; je 가 거짓이면 여기로 점프

    PRINT_HEX 1, rcx
    NEWLINE
    

    ; 연습 문제 : 어떤 숫자(1~100)이 짝수면 1, 홀수면 0을 출력하는 프로그램
    mov ax, 91
    mov bl, 2
    
    div bl  ; rax / bl -> 몫은 al로 , 나머지는 ah
    
    cmp ah, 0
    
    je L1
    
    mov cl, 0
    
    jmp L2
    
L1:
    mov cl, 1
L2:
    
    PRINT_HEX 1, cl
    NEWLINE
    
    
    xor rax, rax
    ret