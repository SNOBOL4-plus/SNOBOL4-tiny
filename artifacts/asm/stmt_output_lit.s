; stmt_output_lit.s — Sprint A10 Step 1: OUTPUT = 'hello world'
; 
; Calls into snobol4_stmt_rt.c via extern linkage.
; DESCR_t = 16 bytes: [0..7]=DTYPE_t+pad (rax), [8..15]=union ptr (rdx)
;
; Build:
;   nasm -f elf64 stmt_output_lit.s -o stmt_output_lit.o
;   gcc stmt_output_lit.o snobol4_stmt_rt.o snobol4.o mock_includes.o \
;       snobol4_pattern.o mock_engine.o -lgc -lm -o stmt_output_lit
; Run: ./stmt_output_lit   → prints "hello world\n" to stdout

    global  main
    extern  stmt_init
    extern  stmt_strval
    extern  stmt_output
    extern  stmt_is_fail
    extern  stmt_finish

section .note.GNU-stack noalloc noexec nowrite progbits

section .data
    str_hello   db  'hello world', 0

section .text

main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 32             ; space for one DESCR_t (16) + alignment

    ; stmt_init()
    call    stmt_init

    ; DESCR_t _v = stmt_strval("hello world")
    lea     rdi, [rel str_hello]
    call    stmt_strval
    ; rax = DESCR_t bytes[0..7], rdx = DESCR_t bytes[8..15]
    mov     [rbp-16], rax
    mov     [rbp-8],  rdx

    ; if (stmt_is_fail(_v)) skip
    mov     rdi, [rbp-16]
    mov     rsi, [rbp-8]
    call    stmt_is_fail
    test    eax, eax
    jnz     .skip

    ; stmt_output(_v)
    mov     rdi, [rbp-16]
    mov     rsi, [rbp-8]
    call    stmt_output

.skip:
    ; stmt_finish()
    call    stmt_finish

    xor     eax, eax
    leave
    ret
