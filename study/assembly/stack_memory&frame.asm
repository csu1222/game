%include "io64.inc"

section .text
global CMAIN
CMAIN:
    ; 스택 메모리 스택 프레임
    
    ; 레지스터는 다양한 용도로 사용됩니다.
    ; - a, b, c, d 등 지금까지 사용했던 범용 레지스터
    ; - 포인터 레지스터 (포인터 = 위치를 가르키는~)
    ; -- ip (Instuction Pointer) : 다음 수행 명령어의 위치
    ; -- sp (Stack Pointer) : 현재 스택 top 위치 (일종의 cursor)
    ; -- bp (base Pointer) : 스택의 상대주소 계산용
    
    ; 스택의 사용법은 데이터를 넣을때는 push 뺄 때는 pop
    
    push 3
    push 2
    call MAX        ; 여기 call을 할때 스택에 다시 여기로 돌아오기위한 rip (Instuction Pointer)를 저장합니다.
    PRINT_DEC 8, rax
    NEWLINE
    pop rax
    pop rax     
    
    xor rax, rax
    ret
    
    
MAX:
    push rbp        ; 이전 bp를 스택에 저장합니다. 밑 pop rbp 로 원래 베이스 포인터로 가기위해 저장합니다.
    mov rbp, rsp    ; 함수내의 베이스 포인터를 저장해서 이전에 저장했던 스택을 상대주소를 이용하기 쉽게 합니다.
                    ; rsp 그대로 상대주소로 쓰지 않는것은 rsp는 변동적이기 때문에 고정 값으로 이 함수내 rbp를 고정하는것입니다.
    mov rax, [rbp+16] ; rbp + 16 인 이유는 한 스택당 8바이트의 크기를 가지고 있고 바로위의 스택은 call할때의 rip가 저장 되어 있기 때문입니다.
    mov rbx, [rbp+24]
    cmp rax, rbx
    jg L1
    mov rax, rbx
L1: 
    pop rbp         ; 함수내에서 ret으로 빠져나가기 전에 원래 rbp로 이동합니다.
    ret
    
section .data

section .bss