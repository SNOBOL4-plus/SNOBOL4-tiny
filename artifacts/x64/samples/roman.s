# -----------------------------------------------------------------------
# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-3)
# 86 SM instructions. Links against libscrip_rt.so.
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
                        movabs  rdi, 421303744              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4:                  
                        movabs  rdi, 1000000000             
                        call    scrip_rt_push_int@PLT       
.Lpc5:                  
                        movabs  rdi, 421284752              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc6:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc7:                  
                        movabs  rdi, 421284784              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc8:                  
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc9:                  
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc10:                 
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc11:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc12:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc13:                 
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc14:                 
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc15:                 
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc16:                 
# -- BB box scaffold pc=16 op=SM_PAT_LEN --
# proc .bb_box_16
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 31                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc17:                 
# -- BB box scaffold pc=17 op=SM_PAT_CAPTURE --
# proc .bb_box_17
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc18:                 
# -- BB box scaffold pc=18 op=SM_PAT_CAT --
# proc .bb_box_18
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc19:                 
                        movabs  rdi, 421284976              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc20:                 
                        movabs  rdi, 421285008              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc21:                 
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc22:                 
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc23:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc24:                 
                        movabs  rdi, 421285040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc25:                 
# -- BB box scaffold pc=25 op=SM_PAT_DEREF --
# proc .bb_box_25
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc26:                 
                        movabs  rdi, 421285072              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc27:                 
# -- BB box scaffold pc=27 op=SM_PAT_BREAK --
# proc .bb_box_27
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc28:                 
# -- BB box scaffold pc=28 op=SM_PAT_CAPTURE --
# proc .bb_box_28
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc29:                 
# -- BB box scaffold pc=29 op=SM_PAT_CAT --
# proc .bb_box_29
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc30:                 
                        movabs  rdi, 421309984              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc31:                 
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc32:                 
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc33:                 
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc34:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc35:                 
                        movabs  rdi, 421285136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc36:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc37:                 
                        movabs  rdi, 421285200              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc38:                 
                        movabs  rdi, 421285232              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc39:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc40:                 
                        movabs  rdi, 421285296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc41:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc42:                 
                        movabs  rdi, 421285328              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc43:                 
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc44:                 
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc45:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc46:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc47:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc48:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc49:                 
                        movabs  rdi, 421285456              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc50:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc51:                 
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc52:                 
                        movabs  rdi, 421285488              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc53:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc54:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc55:                 
                        movabs  rdi, 421285584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc56:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc57:                 
                        movabs  rdi, 421285648              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc58:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc59:                 
                        movabs  rdi, 421285680              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc60:                 
                        movabs  rdi, 100000                 
                        call    scrip_rt_push_int@PLT       
.Lpc61:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc62:                 
                        movabs  rdi, 421285744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc63:                 
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc64:                 
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc65:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc66:                 
                        movabs  rdi, 421301680              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc67:                 
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc68:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc69:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc70:                 
                        movabs  rdi, 421301744              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc71:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc72:                 
                        movabs  rdi, 421301776              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc73:                 
                        movabs  rdi, 421301808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc74:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc75:                 
                        movabs  rdi, 421301840              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc76:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc77:                 
                        movabs  rdi, 421301872              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc78:                 
                        movabs  rdi, 421301904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc79:                 
                        movabs  rdi, 421301936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc80:                 
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc81:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc82:                 
                        movabs  rdi, 421301968              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc83:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc84:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc85:                 
                        call    scrip_rt_pop_int@PLT        #  rc <- TOS
                        mov     edi, eax                    
                        call    scrip_rt_halt@PLT           
	# -- epilogue -------------------------------------------
	call    scrip_rt_finalize@PLT
	pop     rbp
	ret
	.size   main, .-main
	.section .note.GNU-stack,"",@progbits
