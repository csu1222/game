%include "io64.inc"

section .text
global CMAIN
CMAIN:
    ;write your code here
    
    ; 배열과 주소
    
    ; 배열 : 동일한 타입의 데이터를 묶어 놓은것
    ; - 배열을 구성하는 각 값을 요소(element) 라고 합니다.
    ; - 배열에서 위치를 가리키는 숫자를 인덱스(index) 라고 합니다.
    
    ; 주소
    ; [시작주소 + 인덱스*크기]
    mov rax, a  ; rax에 변수 a의 첫번째 값을 넣은게 아닌 첫번째 값의 주소가 담겼습니다.
    
    PRINT_HEX 1, [a]    ; 변수 a의 값을 끄집어 내려면 []로 감싸야 합니다.
    NEWLINE
    ; a 의 1 이후의 값들은 메모리상으로 연달아 있으므로 주소에서 1씩 더해가면 다음 값들을 꺼낼수 있습니다
    ; 연습문제) loop를 이용해 a의 값들을 꺼내보기
    mov ecx, 4
    mov edx, 0
LABEL_ARRAY:
    PRINT_HEX 1, [a+edx]
    NEWLINE
    inc edx
    
    cmp edx, ecx
    
    jle LABEL_ARRAY
    
    ; 연습 문제 ) b의 값들을 꺼내보기
    xor ecx, ecx
LABEL_PRINT_B:
    PRINT_HEX 2, [b+ecx]  ; [b+ecx*2]로 가능.
    NEWLINE
    add ecx, 2          ; b의 바이트크기가 2입니다.
    
    cmp ecx, 10         ; b의 전체 코드는 10 입니다.
    
    jne LABEL_PRINT_B
    
    
    xor rax, rax
    ret
    
section .data
    msg db 'Hello World', 0x00
    a db 0x01, 0x02, 0x03, 0x04, 0x05  ; 이처럼 한번에 같은 크기의 여러 값을 저장하는 것도 배열의 문법  
                                 ; 크기는 1바이트가 5개 있으니 5바이트
    b times 5 dw 1               ; 이 문법은 dw(2byte)의 크기의 메모리에 1이라는 값을 가진 데이터 5개를 배열로 만든 것
                                 ; dw(2byte) 가 5개 있으니 10바이트

section .bss            ; 초기화 되지 않은 값들이 있는 메모리

    c resb 10           ; 초기화 되지 않은 값이니 1byte크기의 데이터가 10개 있습니다.
    