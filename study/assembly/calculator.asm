%include "io64.inc"

section .text
global CMAIN
CMAIN:    mov rbp, rsp; for correct debugging    ;write your code here
    
    GET_DEC 1, al         ;input 창에 입력을 받는 명령어 GET_DEC [크기], [저장할 위치]
    GET_DEC 1, num
    
    PRINT_DEC 1, al
    NEWLINE
    PRINT_DEC 1, num
    NEWLINE
    
    ; 더하기 연산
    ; add a, b  (a = a + b)
    ; a는 레지스터 혹은 메모리
    ; b는 레지스터 혹은 메모리 혹은 상수
    ; 단, a, b 모두 메모리는 안된다
    
    ; 빼기 연산
    ; sub a, b (a = a - b) 
    ; 나머지 규칙은 더하기와 동일
    
    add al, 1 ; 레지스터 + 상수 형태
    PRINT_DEC 1, al  ; 1+1=2
    NEWLINE
    
    add al, [num]  ; 레지스터 + 메모리
    ; num을 대괄호로 감싸는 이유는 그냥 num은 메모리주소를 반환하고 al이라는 레지스터는 1byte의 크기이기 때문에
    PRINT_DEC 1, al  ; 2 + 2 = 4
    NEWLINE
    
    mov bl, 3   ; 레지스터에 값 할당
    add al, bl  ; 레지스터 + 레지스터 형태
    PRINT_DEC 1, al  ; 4 + 3 = 7
    NEWLINE
    
    add [num], byte 1  ; 메모리 + 상수 형태 메모리에 상수를 더할때는 상수의 크기를 지정해줘야합니다.
    PRINT_DEC 1, [num]  ; 2 + 1 = 3
    NEWLINE
    
    add [num], al  ; 메모리 + 지스터 형태 
    PRINT_DEC 1, [num]  ; 3 + 7 = 10
    NEWLINE
    
    
    ; 곱하기 연산
    ; mul 레지스터   
    ; - mul bl  ( al * bl 의 결과를 ax에 저장 ) al은 고정으로 연산
    ; -mul bx   ( ax * bx )
    ; 연산결과는 dx(상위 16비트) ax(하위 16비트)에 나눠 저장
    
    ; ex ) 5 * 8 은?
    
    mov ax, 0
    mov al, 5
    mov bl, 8
    mul bl
    PRINT_DEC 2, ax
    NEWLINE
    
    ; 나누기 연산
    ; div 레지스터
    ; - div bl   ( ax / bl )
    ; 연산 결과는 al(몫) ah(나머지)
    
    ; ex) 100 / 3 은?
    mov ax, 100
    mov bl, 3
    div bl
    PRINT_DEC 1, al   ; 몫으로 33
    NEWLINE
    mov al, ah  ; ah는 PRINT_DEC 로 바로 출력을 못해서 al 로 복사하여 출력
    PRINT_DEC 1, al   ; 나머지로 1
    
    
    
    xor rax, rax
    ret

section .bss
    num resb 1