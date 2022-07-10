%include "io64.inc"

section .text
global CMAIN
CMAIN:
    ;write your code here
    
    mov eax, 0x12345678     ; eax 레지스터에 0x12345678 이라는 값을 넣었습니다.
    PRINT_HEX 4, eax        ; eax 4byte를 print 했습니다.
    NEWLINE
    shl eax, 8              ; sh시프트l레프트 eax를 8칸(bit)
    PRINT_HEX 4, eax        ; 결과 8비트 즉 2바이트가 앞쪽 부터 없어졌습니다. 34567800
    NEWLINE
    shr eax, 8              ; 오른쪽으로 시프트 2byte
    PRINT_HEX 4, eax        ; 다시 오른쪽으로 시프트 했지만 이미 날라간 데이터는 복구되지 않았습니다. 345678
    NEWLINE
    ; 곱셈/ 나눗셈
    ; 게임서버에서 ObjectID를 만들때 
    
    ; 논리연산
    ; not and or xor
    
    ; 조건A : 잘생겼다
    ; 조건B : 키가 크다
    
    ; not A : 잘생겼다의 반대 -> (0이면 1, 1이면 0) 값의 반전
    ; A and B : 잘생겼고 and 키도 크고 -> 둘다 1 이면 1, 아니면 0   두 조건 모두 충족하면 TRUE
    ; A or B : 잘생겼거나 or 키가 크거나 -> 두 조건중 하나라도 충족하면 TRUE
    ; A xor B : 잘생기고 작거나 or 못생기고 크거나   -> 둘다 1이거나 둘다 0이면 0, 아니면 1
    
    mov al, 0b10010101
    mov bl, 0b01111100
    
    ; 예상) 0b00010100 = 0x14
    and al, bl  ; al = al and bl
    PRINT_HEX 1, al
    NEWLINE
    
    ; 예상) 0b11101011 = 0xeb
    not al      ; al = not al 
    PRINT_HEX 1, al
    NEWLINE     
    
    mov al, 0b10010101
    mov bl, 0b01111100
    NEWLINE
    PRINT_HEX 1, al
    NEWLINE
    xor al, bl
    PRINT_HEX 1, al
    NEWLINE
    xor al, bl
    PRINT_HEX 1, al
    NEWLINE
    ; 동일한 값으로 xor 두번하면 자기 자신으로 되돌아오는 특성
    ; 암호학에서 유용하다 (value xor key)
    
    ;  자기자신을 xor 하면 mov al, 0 과 같은 결과
    xor al, al
    PRINT_HEX 1, al
          
    xor rax, rax
    ret