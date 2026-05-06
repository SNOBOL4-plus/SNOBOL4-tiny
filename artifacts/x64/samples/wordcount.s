# -----------------------------------------------------------------------
# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-3)
# 53 SM instructions. Links against libscrip_rt.so.
# Architecture: two emitters -- SM straight-line via sm_macros.s
#   macros (inline x86); BB boxes via emit_bb_box() one-proc-per-box.
# See archive/EMITTER-MODE4-ARCH.md for the full design.
# -----------------------------------------------------------------------
	.intel_syntax noprefix
# Include SM opcode macro library (one macro per opcode group)
# .include "sm_macros.s"  # assembled separately; macros used by name below
	.text
	.globl  main
	.type   main, @function
main:
	push    rbp
	mov     rbp, rsp
	# scrip_rt_init(argc, argv) -- argc in edi, argv in rsi
	call    scrip_rt_init@PLT
.Lpc0:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1:                  
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2:                  
                        movabs  rdi, 233100672              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4:                  
                        movabs  rdi, 233100704              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc5:                  
                        movabs  rdi, 233100736              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc6:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc7:                  
                        movabs  rdi, 233081744              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc8:                  
                        movabs  rdi, 233081776              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc9:                  
                        movabs  rdi, 233081808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc10:                 
                        movabs  rdi, 233081840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc11:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc12:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc13:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc14:                 
                        movabs  rdi, 233081872              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc15:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc16:                 
                        movabs  rdi, 233081904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc17:                 
# -- BB box scaffold pc=17 op=SM_PAT_BREAK --
# proc .bb_box_17
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc18:                 
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc19:                 
                        movabs  rdi, 233081936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc20:                 
# -- BB box scaffold pc=20 op=SM_PAT_SPAN --
# proc .bb_box_20
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc21:                 
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc22:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc23:                 
                        movabs  rdi, 233081968              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc24:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc25:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc26:                 
                        movabs  rdi, 233082064              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc27:                 
                        movabs  rdi, 233082096              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc28:                 
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc29:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc30:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc31:                 
                        movabs  rdi, 233082224              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc32:                 
# -- BB box scaffold pc=32 op=SM_PAT_DEREF --
# proc .bb_box_32
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc33:                 
                        movabs  rdi, 233082256              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc34:                 
                        movabs  rdi, 233082288              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc35:                 
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc36:                 
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc37:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc38:                 
                        movabs  rdi, 233082320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc39:                 
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc40:                 
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc41:                 
                        movabs  rdi, 233082352              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc42:                 
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc43:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc44:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc45:                 
                        movabs  rdi, 233082448              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc46:                 
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc47:                 
                        movabs  rdi, 233082480              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc48:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc49:                 
                        movabs  rdi, 233082512              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc50:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc51:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc52:                 
                        call    scrip_rt_pop_int@PLT        #  rc <- TOS
                        mov     edi, eax                    
                        call    scrip_rt_halt@PLT           
	# -- epilogue -------------------------------------------
	call    scrip_rt_finalize@PLT
	pop     rbp
	ret
	.size   main, .-main
	.section .note.GNU-stack,"",@progbits
