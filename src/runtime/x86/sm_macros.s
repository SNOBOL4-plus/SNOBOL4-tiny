# sm_macros.s -- GNU-as macro library for SM opcode emission
#
# Authors: Lon Jones Cherryholmes, Claude Sonnet
# Date: 2026-05-06
#
# One macro per SM opcode group. Parallel to the proven snobol4_asm.mac
# (151 macros, 106/106 SPITBOL oracle). Used by sm_codegen_x64_emit.c
# in text-asm mode (--jit-emit --x64).
#
# Three-column SNOBOL4 layout throughout:
#   LABEL:          ACTION (macro name + args)      ; goto comment
#
# Calling convention: System V AMD64.
# SM value stack lives in libscrip_rt.so (accessed via scrip_rt_* ABI
# for now; EM-5+ may bake direct r12-based stack ops).
# libscrip_rt.so boundary: NV table, pattern matcher, GC, builtins.
# Inline via these macros: push/pop literals, arithmetic, control flow.
#
# Inline register use (EM-3 scope, all via scrip_rt_* calls):
#   rdi/rsi/rdx/rcx/r8/r9 -- SysV argument registers (caller-saved)
#   rax -- return value (caller-saved)
#   r12, r13 -- reserved for future baked-direct stack ops (callee-saved)
#
# -----------------------------------------------------------------------
# Value push macros
# -----------------------------------------------------------------------

# SM_PUSH_INT val -- push 64-bit signed integer literal
.macro SM_PUSH_INT val
    movabs  rdi, \val
    call    scrip_rt_push_int@PLT
.endm

# SM_PUSH_STR ptr, slen -- push string literal (ptr=imm64 addr, slen=imm32)
.macro SM_PUSH_STR ptr, slen
    movabs  rdi, \ptr
    mov     esi, \slen
    call    scrip_rt_push_str@PLT
.endm

# SM_PUSH_VAR name_ptr -- load named variable onto SM stack
# name_ptr is an imm64 address of a NUL-terminated C string
.macro SM_PUSH_VAR name_ptr
    movabs  rdi, \name_ptr
    call    scrip_rt_nv_get@PLT
.endm

# SM_STORE_VAR name_ptr -- pop TOS and store into named variable
.macro SM_STORE_VAR name_ptr
    movabs  rdi, \name_ptr
    call    scrip_rt_nv_set@PLT
.endm

# SM_POP -- discard TOS
.macro SM_POP
    call    scrip_rt_pop_void@PLT
.endm

# -----------------------------------------------------------------------
# Arithmetic macros (integer only in EM-3; full coercion in later rungs)
# SM opcode integer values from sm_prog.h kept as literals:
#   SM_ADD=17  SM_SUB=18  SM_MUL=19  SM_DIV=20  SM_MOD=22
# -----------------------------------------------------------------------

.macro SM_ADD
    mov     edi, 17
    call    scrip_rt_arith@PLT
.endm

.macro SM_SUB
    mov     edi, 18
    call    scrip_rt_arith@PLT
.endm

.macro SM_MUL
    mov     edi, 19
    call    scrip_rt_arith@PLT
.endm

.macro SM_DIV
    mov     edi, 20
    call    scrip_rt_arith@PLT
.endm

.macro SM_MOD
    mov     edi, 22
    call    scrip_rt_arith@PLT
.endm

# -----------------------------------------------------------------------
# Control flow macros
# Labels are .LpcN symbols resolved at emit time.
# -----------------------------------------------------------------------

# SM_HALT -- pop TOS as rc, record it, fall through to epilogue
.macro SM_HALT
    call    scrip_rt_pop_int@PLT
    mov     edi, eax
    call    scrip_rt_halt@PLT
.endm

# SM_JUMP label -- unconditional branch
.macro SM_JUMP label
    jmp     \label
.endm

# SM_JUMP_S label -- jump if last SM result succeeded
# scrip_rt_last_ok() returns 1 if last op succeeded
.macro SM_JUMP_S label
    call    scrip_rt_last_ok@PLT
    test    eax, eax
    jnz     \label
.endm

# SM_JUMP_F label -- jump if last SM result failed
.macro SM_JUMP_F label
    call    scrip_rt_last_ok@PLT
    test    eax, eax
    jz      \label
.endm

# -----------------------------------------------------------------------
# BB box port macros (one proc per box, four local labels)
# Used by emit_bb_box() in sm_codegen_x64_emit.c
# -----------------------------------------------------------------------

# BB_BOX_PROLOGUE name -- open a box proc, save callee-saved regs
.macro BB_BOX_PROLOGUE name
    .globl  \name
    .type   \name, @function
\name:
    push    rbp
    mov     rbp, rsp
.endm

# BB_BOX_EPILOGUE name -- close a box proc
.macro BB_BOX_EPILOGUE name
    .size   \name, .-\name
.endm

# BB_PORT_ALPHA -- entry label for forward attempt
.macro BB_PORT_ALPHA
    pop     rbp
    ret
.endm

# BB_PORT_GAMMA -- success exit
.macro BB_PORT_GAMMA
    pop     rbp
    ret
.endm

# BB_PORT_OMEGA -- failure exit: return 0 (no match)
.macro BB_PORT_OMEGA
    xor     eax, eax
    pop     rbp
    ret
.endm

# -----------------------------------------------------------------------
# Stack section marker (required by GNU ld for non-executable stack)
# Include once per emitted asm file (the top-level emitter adds it).
# -----------------------------------------------------------------------
# .section .note.GNU-stack,"",@progbits
