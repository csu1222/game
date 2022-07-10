%include "io64.inc"

section .text
global CMAIN
CMAIN:    mov rbp, rsp; for correct debugging    ;write your code here
    
    PRINT_STRING msg
    
    xor rax, rax
    ret
    

section .data
    msg db 0x48,0x65,0x6c,0x6c,0x6f,0x20,0x57,0x6f,0x72,0x6c,0x64,0x00 
    ; 첫 hello world 파일에서 사용했던 코드입니다. 레지스터 메모리 때 배운 변수 선언 방법이랑 차이가 있습니다.
    ; msg라는 변수이름에 db 라고 1바이트의 크기를 할당했는데 초기 값이 Hello World 라는 문자열로 크기를 초과 해보입니다.
    
    a db 17   ; [0x11]
    ; a 변수에 17 이라는 십진수를 할당하든 0x11이라는 16진수를 할당하든 메모리상에서 변환하면 값은 같습니다. 결국 컴퓨터는 2진수를 
    ; 변환해서 이해합니다.
        
    b db 0x22, 0x22, 0x22, 0x22
    ; b 변수에 한번에 많은 초기 값을 할당하면 b는 첫번째 값을 가르키고 그 뒤로 할당했던 횟수 만큼 메모리를 가지게 됩니다.
section .bss