; stmt_goto.s — Sprint A10 Step 3: label + goto
;
; Implements:
;        N = 1
; LOOP   OUTPUT = N
;        N = N + 1   (via stmt_intval + stmt_concat — simplified: direct int)
;        GT(N,3)              :F(LOOP)
; END
;
; Expected output: 1\n2\n3\n

    global  main
    extern  stmt_init
    extern  stmt_strval
    extern  stmt_intval
    extern  stmt_get
    extern  stmt_set
    extern  stmt_output
    extern  stmt_is_fail
    extern  stmt_finish
    extern  stmt_gt          ; we'll add: int stmt_gt(DESCR_t a, DESCR_t b)

section .data
    str_N       db  'N', 0

section .bss
    ; store current integer N directly — bypass DESCR_t for simple loop counter
    loop_n      resq 1

section .note.GNU-stack noalloc noexec nowrite progbits

section .text

main:
    push    rbp
    mov     rbp, rsp
    sub     rsp, 48             ; space for 2x DESCR_t + counter

    call    stmt_init

    ; N = 1  (store integer 1 in loop_n)
    mov     qword [rel loop_n], 1

.LOOP:
    ; OUTPUT = N  (convert loop_n to string DESCR_t)
    mov     rdi, [rel loop_n]
    call    stmt_intval         ; DESCR_t = integer N
    mov     [rbp-16], rax
    mov     [rbp-8],  rdx

    mov     rdi, [rbp-16]
    mov     rsi, [rbp-8]
    call    stmt_output

    ; N = N + 1
    mov     rax, [rel loop_n]
    inc     rax
    mov     [rel loop_n], rax

    ; GT(N, 3) :F(LOOP)  — if N <= 3, loop back
    ; GT succeeds when N > 3, fails when N <= 3
    cmp     qword [rel loop_n], 4
    jl      .LOOP               ; N < 4 → loop

    call    stmt_finish
    xor     eax, eax
    leave
    ret
