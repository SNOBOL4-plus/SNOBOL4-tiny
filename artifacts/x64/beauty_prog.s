# -----------------------------------------------------------------------
# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-3)
# 4728 SM instructions. Links against libscrip_rt.so.
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
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3:                  
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4:                  
                        movabs  rdi, 881168848              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc5:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc6:                  
                        movabs  rdi, 524288                 
                        call    scrip_rt_push_int@PLT       
.Lpc7:                  
                        movabs  rdi, 881168880              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc8:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc9:                  
                        movabs  rdi, 881168912              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc10:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc11:                 
                        movabs  rdi, 881168976              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc12:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc13:                 
                        movabs  rdi, 15                     
                        call    scrip_rt_push_int@PLT       
.Lpc14:                 
                        movabs  rdi, 881169008              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc15:                 
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc16:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc17:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc18:                 
                        movabs  rdi, 30                     
                        call    scrip_rt_push_int@PLT       
.Lpc19:                 
                        movabs  rdi, 881169072              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc20:                 
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc21:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc22:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc23:                 
                        movabs  rdi, 33                     
                        call    scrip_rt_push_int@PLT       
.Lpc24:                 
                        movabs  rdi, 881169136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc25:                 
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc26:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc27:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc28:                 
                        movabs  rdi, 80                     
                        call    scrip_rt_push_int@PLT       
.Lpc29:                 
                        movabs  rdi, 881169200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc30:                 
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc31:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc32:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc33:                 
                        movabs  rdi, 5                      
                        call    scrip_rt_push_int@PLT       
.Lpc34:                 
                        movabs  rdi, 881169264              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc35:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc36:                 
                        movabs  rdi, 42                     
                        call    scrip_rt_push_int@PLT       
.Lpc37:                 
                        movabs  rdi, 881169296              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc38:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc39:                 
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc40:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc41:                 
                        movabs  rdi, 881169360              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc42:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc43:                 
                        movabs  rdi, 881169392              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc44:                 
                        movabs  rdi, 881169424              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc45:                 
# -- BB box scaffold pc=45 op=SM_PAT_BREAK --
# proc .bb_box_45
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc46:                 
                        movabs  rdi, 881169456              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc47:                 
# -- BB box scaffold pc=47 op=SM_PAT_DEREF --
# proc .bb_box_47
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc48:                 
# -- BB box scaffold pc=48 op=SM_PAT_ALT --
# proc .bb_box_48
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc49:                 
# -- BB box scaffold pc=49 op=SM_PAT_CAPTURE --
# proc .bb_box_49
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc50:                 
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc51:                 
# -- BB box scaffold pc=51 op=SM_PAT_LIT --
# proc .bb_box_51
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc52:                 
                        movabs  rdi, 881169552              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc53:                 
# -- BB box scaffold pc=53 op=SM_PAT_BREAK --
# proc .bb_box_53
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc54:                 
                        movabs  rdi, 881169584              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc55:                 
# -- BB box scaffold pc=55 op=SM_PAT_DEREF --
# proc .bb_box_55
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc56:                 
# -- BB box scaffold pc=56 op=SM_PAT_ALT --
# proc .bb_box_56
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc57:                 
# -- BB box scaffold pc=57 op=SM_PAT_CAPTURE --
# proc .bb_box_57
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc58:                 
# -- BB box scaffold pc=58 op=SM_PAT_CAT --
# proc .bb_box_58
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc59:                 
# -- BB box scaffold pc=59 op=SM_PAT_LIT --
# proc .bb_box_59
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc60:                 
# -- BB box scaffold pc=60 op=SM_PAT_CAPTURE --
# proc .bb_box_60
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc61:                 
# -- BB box scaffold pc=61 op=SM_PAT_ALT --
# proc .bb_box_61
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc62:                 
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc63:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc64:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc65:                 
                        movabs  rdi, 881173824              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc66:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc67:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc68:                 
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc69:                 
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc70:                 
                        movabs  rdi, 881165184              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc71:                 
# -- BB box scaffold pc=71 op=SM_PAT_SPAN --
# proc .bb_box_71
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc72:                 
# -- BB box scaffold pc=72 op=SM_PAT_CAT --
# proc .bb_box_72
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc73:                 
                        movabs  rdi, 881165216              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc74:                 
                        movabs  rdi, 881165248              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc75:                 
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc76:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc77:                 
                        movabs  rdi, 881165280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc78:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc79:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc80:                 
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc81:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc82:                 
                        movabs  rdi, 881165376              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc83:                 
# -- BB box scaffold pc=83 op=SM_PAT_DEREF --
# proc .bb_box_83
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc84:                 
                        movabs  rdi, 881165408              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc85:                 
                        movabs  rdi, 881165440              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc86:                 
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc87:                 
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc88:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc89:                 
                        movabs  rdi, 881165504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc90:                 
                        movabs  rdi, 881165536              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc91:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc92:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc93:                 
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc94:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc95:                 
                        movabs  rdi, 11                     
                        call    scrip_rt_push_int@PLT       
.Lpc96:                 
                        movabs  rdi, 881165632              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc97:                 
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc98:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc99:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc100:                
                        movabs  rdi, 26                     
                        call    scrip_rt_push_int@PLT       
.Lpc101:                
                        movabs  rdi, 881165696              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc102:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc103:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc104:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc105:                
                        movabs  rdi, 29                     
                        call    scrip_rt_push_int@PLT       
.Lpc106:                
                        movabs  rdi, 881165760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc107:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc108:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc109:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc110:                
                        movabs  rdi, 55                     
                        call    scrip_rt_push_int@PLT       
.Lpc111:                
                        movabs  rdi, 881165824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc112:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc113:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc114:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc115:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc116:                
                        movabs  rdi, 881165888              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc117:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc118:                
                        movabs  rdi, 21                     
                        call    scrip_rt_push_int@PLT       
.Lpc119:                
                        movabs  rdi, 881165920              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc120:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc121:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc122:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc123:                
                        movabs  rdi, 881166016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc124:                
                        movabs  rdi, 881166048              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc125:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc126:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc127:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc128:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc129:                
                        movabs  rdi, 13                     
                        call    scrip_rt_push_int@PLT       
.Lpc130:                
                        movabs  rdi, 881166144              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc131:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc132:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc133:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc134:                
                        movabs  rdi, 28                     
                        call    scrip_rt_push_int@PLT       
.Lpc135:                
                        movabs  rdi, 881166208              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc136:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc137:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc138:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc139:                
                        movabs  rdi, 31                     
                        call    scrip_rt_push_int@PLT       
.Lpc140:                
                        movabs  rdi, 881166272              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc141:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc142:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc143:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc144:                
                        movabs  rdi, 60                     
                        call    scrip_rt_push_int@PLT       
.Lpc145:                
                        movabs  rdi, 881166336              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc146:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc147:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc148:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc149:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc150:                
                        movabs  rdi, 881166400              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc151:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc152:                
                        movabs  rdi, 24                     
                        call    scrip_rt_push_int@PLT       
.Lpc153:                
                        movabs  rdi, 881166432              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc154:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc155:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc156:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc157:                
                        movabs  rdi, 881166528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc158:                
                        movabs  rdi, 881166560              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc159:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc160:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc161:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc162:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc163:                
                        movabs  rdi, 15                     
                        call    scrip_rt_push_int@PLT       
.Lpc164:                
                        movabs  rdi, 881166656              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc165:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc166:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc167:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc168:                
                        movabs  rdi, 30                     
                        call    scrip_rt_push_int@PLT       
.Lpc169:                
                        movabs  rdi, 881166720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc170:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc171:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc172:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc173:                
                        movabs  rdi, 33                     
                        call    scrip_rt_push_int@PLT       
.Lpc174:                
                        movabs  rdi, 881166784              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc175:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc176:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc177:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc178:                
                        movabs  rdi, 80                     
                        call    scrip_rt_push_int@PLT       
.Lpc179:                
                        movabs  rdi, 881166848              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc180:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc181:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc182:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc183:                
                        movabs  rdi, 5                      
                        call    scrip_rt_push_int@PLT       
.Lpc184:                
                        movabs  rdi, 881166912              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc185:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc186:                
                        movabs  rdi, 42                     
                        call    scrip_rt_push_int@PLT       
.Lpc187:                
                        movabs  rdi, 881166944              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc188:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc189:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc190:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc191:                
                        movabs  rdi, 881167040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc192:                
                        movabs  rdi, 881167072              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc193:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc194:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc195:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc196:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc197:                
                        movabs  rdi, 17                     
                        call    scrip_rt_push_int@PLT       
.Lpc198:                
                        movabs  rdi, 881167168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc199:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc200:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc201:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc202:                
                        movabs  rdi, 32                     
                        call    scrip_rt_push_int@PLT       
.Lpc203:                
                        movabs  rdi, 881169744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc204:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc205:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc206:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc207:                
                        movabs  rdi, 35                     
                        call    scrip_rt_push_int@PLT       
.Lpc208:                
                        movabs  rdi, 881169808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc209:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc210:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc211:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc212:                
                        movabs  rdi, 96                     
                        call    scrip_rt_push_int@PLT       
.Lpc213:                
                        movabs  rdi, 881169872              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc214:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc215:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc216:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc217:                
                        movabs  rdi, 6                      
                        call    scrip_rt_push_int@PLT       
.Lpc218:                
                        movabs  rdi, 881169936              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc219:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc220:                
                        movabs  rdi, 54                     
                        call    scrip_rt_push_int@PLT       
.Lpc221:                
                        movabs  rdi, 881169968              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc222:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc223:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc224:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc225:                
                        movabs  rdi, 881170064              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc226:                
                        movabs  rdi, 881170096              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc227:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc228:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc229:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc230:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc231:                
                        movabs  rdi, 19                     
                        call    scrip_rt_push_int@PLT       
.Lpc232:                
                        movabs  rdi, 881170192              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc233:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc234:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc235:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc236:                
                        movabs  rdi, 34                     
                        call    scrip_rt_push_int@PLT       
.Lpc237:                
                        movabs  rdi, 881170256              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc238:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc239:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc240:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc241:                
                        movabs  rdi, 37                     
                        call    scrip_rt_push_int@PLT       
.Lpc242:                
                        movabs  rdi, 881170320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc243:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc244:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc245:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc246:                
                        movabs  rdi, 110                    
                        call    scrip_rt_push_int@PLT       
.Lpc247:                
                        movabs  rdi, 881170384              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc248:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc249:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc250:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc251:                
                        movabs  rdi, 7                      
                        call    scrip_rt_push_int@PLT       
.Lpc252:                
                        movabs  rdi, 881170448              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc253:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc254:                
                        movabs  rdi, 68                     
                        call    scrip_rt_push_int@PLT       
.Lpc255:                
                        movabs  rdi, 881170480              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc256:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc257:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc258:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc259:                
                        movabs  rdi, 881170576              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc260:                
                        movabs  rdi, 881170608              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc261:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc262:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc263:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc264:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc265:                
                        movabs  rdi, 881170704              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc266:                
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc267:                
                        movabs  rdi, 881170736              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc268:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc269:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc270:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc271:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc272:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc273:                
                        movabs  rdi, 881170864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc274:                
                        movabs  rdi, 881170896              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc275:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc276:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc277:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc278:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc279:                
                        movabs  rdi, 881170992              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc280:                
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc281:                
                        movabs  rdi, 881171024              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc282:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc283:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc284:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc285:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc286:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc287:                
                        movabs  rdi, 881171152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc288:                
                        movabs  rdi, 881171184              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc289:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc290:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc291:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc292:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc293:                
                        movabs  rdi, 881171280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc294:                
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc295:                
                        movabs  rdi, 881171312              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc296:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc297:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc298:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc299:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc300:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc301:                
                        movabs  rdi, 881171440              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc302:                
                        movabs  rdi, 881171472              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc303:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc304:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc305:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc306:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc307:                
                        movabs  rdi, 881171568              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc308:                
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc309:                
                        movabs  rdi, 881171600              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc310:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc311:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc312:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc313:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc314:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc315:                
                        movabs  rdi, 881171728              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc316:                
                        movabs  rdi, 881171760              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc317:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc318:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc319:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc320:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc321:                
                        movabs  rdi, 881171856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc322:                
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc323:                
                        movabs  rdi, 881171888              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc324:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc325:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc326:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc327:                
                        movabs  rdi, 881171984              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc328:                
                        movabs  rdi, 881172016              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc329:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc330:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc331:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc332:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc333:                
                        movabs  rdi, 881172112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc334:                
                        mov     edi, 24                     # SM_COERCE_NUM
                        call    scrip_rt_unhandled_op@PLT   
.Lpc335:                
                        movabs  rdi, 881172144              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc336:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc337:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc338:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc339:                
                        movabs  rdi, 881172240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc340:                
                        movabs  rdi, 881172272              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc341:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc342:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc343:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc344:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc345:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc346:                
                        movabs  rdi, 881173408              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc347:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc348:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc349:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc350:                
                        movabs  rdi, 881343264              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc351:                
                        movabs  rdi, 881173504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc352:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc353:                
                        movabs  rdi, 881173536              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc354:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc355:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc356:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc357:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc358:                
                        movabs  rdi, 881173632              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc359:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc360:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc361:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc362:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc363:                
                        movabs  rdi, 9                      
                        call    scrip_rt_push_int@PLT       
.Lpc364:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc365:                
                        movabs  rdi, 881173760              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc366:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc367:                
                        movabs  rdi, 881173792              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc368:                
                        movabs  rdi, 881626560              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc369:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc370:                
# -- BB box scaffold pc=370 op=SM_PAT_SPAN --
# proc .bb_box_370
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc371:                
# -- BB box scaffold pc=371 op=SM_PAT_LIT --
# proc .bb_box_371
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc372:                
# -- BB box scaffold pc=372 op=SM_PAT_ALT --
# proc .bb_box_372
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc373:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc374:                
                        movabs  rdi, 881626624              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc375:                
                        movabs  rdi, 881626656              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc376:                
# -- BB box scaffold pc=376 op=SM_PAT_ANY --
# proc .bb_box_376
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc377:                
# -- BB box scaffold pc=377 op=SM_PAT_LIT --
# proc .bb_box_377
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc378:                
# -- BB box scaffold pc=378 op=SM_PAT_ALT --
# proc .bb_box_378
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc379:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc380:                
                        movabs  rdi, 881626720              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc381:                
# -- BB box scaffold pc=381 op=SM_PAT_ANY --
# proc .bb_box_381
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc382:                
# -- BB box scaffold pc=382 op=SM_PAT_LIT --
# proc .bb_box_382
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc383:                
# -- BB box scaffold pc=383 op=SM_PAT_ALT --
# proc .bb_box_383
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc384:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc385:                
                        movabs  rdi, 881626784              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc386:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc387:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc388:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc389:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc390:                
                        movabs  rdi, 881626816              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc391:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc392:                
                        movabs  rdi, 881626848              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc393:                
# -- BB box scaffold pc=393 op=SM_PAT_BREAK --
# proc .bb_box_393
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc394:                
# -- BB box scaffold pc=394 op=SM_PAT_CAPTURE --
# proc .bb_box_394
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc395:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc396:                
                        movabs  rdi, 881626912              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc397:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc398:                
                        movabs  rdi, 881626944              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc399:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc400:                
                        movabs  rdi, 881626976              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc401:                
                        movabs  rdi, 881627008              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc402:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc403:                
# -- BB box scaffold pc=403 op=SM_PAT_SPAN --
# proc .bb_box_403
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc404:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc405:                
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc406:                
# -- BB box scaffold pc=406 op=SM_PAT_CAT --
# proc .bb_box_406
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc407:                
# -- BB box scaffold pc=407 op=SM_PAT_CAPTURE --
# proc .bb_box_407
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc408:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc409:                
                        movabs  rdi, 881627072              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc410:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc411:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc412:                
                        movabs  rdi, 881627104              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc413:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc414:                
                        movabs  rdi, 2000                   
                        call    scrip_rt_push_int@PLT       
.Lpc415:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc416:                
                        movabs  rdi, 881627168              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc417:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc418:                
                        movabs  rdi, 881627200              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc419:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc420:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc421:                
                        movabs  rdi, 881627264              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc422:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc423:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc424:                
                        movabs  rdi, 881627296              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc425:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc426:                
                        movabs  rdi, 881627328              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc427:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc428:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc429:                
                        movabs  rdi, 881627392              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc430:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc431:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc432:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc433:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc434:                
                        movabs  rdi, 881627520              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc435:                
                        movabs  rdi, 881627552              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc436:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc437:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc438:                
                        movabs  rdi, 881627616              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc439:                
                        movabs  rdi, 881627648              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc440:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc441:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc442:                
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc443:                
                        movabs  rdi, 881627680              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc444:                
# -- BB box scaffold pc=444 op=SM_PAT_ANY --
# proc .bb_box_444
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc445:                
# -- BB box scaffold pc=445 op=SM_PAT_CAT --
# proc .bb_box_445
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc446:                
                        movabs  rdi, 881627712              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc447:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc448:                
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc449:                
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc450:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc451:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc452:                
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc453:                
                        movabs  rdi, 881627744              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc454:                
# -- BB box scaffold pc=454 op=SM_PAT_ANY --
# proc .bb_box_454
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc455:                
# -- BB box scaffold pc=455 op=SM_PAT_CAT --
# proc .bb_box_455
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc456:                
                        movabs  rdi, 881627776              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc457:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc458:                
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc459:                
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc460:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc461:                
                        movabs  rdi, 881627840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc462:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc463:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc464:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc465:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc466:                
                        movabs  rdi, 881627936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc467:                
# -- BB box scaffold pc=467 op=SM_PAT_DEREF --
# proc .bb_box_467
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc468:                
                        movabs  rdi, 881627968              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc469:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc470:                
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc471:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc472:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc473:                
                        movabs  rdi, 881628032              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc474:                
                        movabs  rdi, 881628064              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc475:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc476:                
                        movabs  rdi, 881628096              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc477:                
# -- BB box scaffold pc=477 op=SM_PAT_DEREF --
# proc .bb_box_477
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc478:                
                        movabs  rdi, 881628128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc479:                
                        movabs  rdi, 881628160              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc480:                
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc481:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc482:                
                        movabs  rdi, 881628192              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc483:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc484:                
                        movabs  rdi, 881628256              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc485:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc486:                
                        movabs  rdi, 881628288              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc487:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc488:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc489:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc490:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc491:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc492:                
                        movabs  rdi, 881628384              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc493:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc494:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc495:                
                        movabs  rdi, 881628416              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc496:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc497:                
                        movabs  rdi, 881628448              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc498:                
                        movabs  rdi, 881628480              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc499:                
                        movabs  rdi, 881628512              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc500:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc501:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc502:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc503:                
                        movabs  rdi, 881628640              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc504:                
                        movabs  rdi, 881628672              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc505:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc506:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc507:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc508:                
                        movabs  rdi, 881628768              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc509:                
                        movabs  rdi, 881628800              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc510:                
                        movabs  rdi, 881628832              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc511:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc512:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc513:                
                        movabs  rdi, 881610160              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc514:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc515:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc516:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc517:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc518:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc519:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc520:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc521:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc522:                
                        movabs  rdi, 881610288              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc523:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc524:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc525:                
                        movabs  rdi, 881610384              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc526:                
                        movabs  rdi, 881610416              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc527:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc528:                
                        movabs  rdi, 881610480              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc529:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc530:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc531:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc532:                
                        movabs  rdi, 881610512              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc533:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc534:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc535:                
                        movabs  rdi, 881610576              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc536:                
                        movabs  rdi, 881610608              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc537:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc538:                
                        movabs  rdi, 881610640              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc539:                
                        movabs  rdi, 881610672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc540:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc541:                
                        movabs  rdi, 881610736              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc542:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc543:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc544:                
                        movabs  rdi, 881610832              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc545:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc546:                
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc547:                
                        movabs  rdi, 881610864              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc548:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc549:                
                        movabs  rdi, 881610896              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc550:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc551:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc552:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc553:                
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc554:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc555:                
                        movabs  rdi, 881610992              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc556:                
                        movabs  rdi, 881611024              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc557:                
                        movabs  rdi, 881611056              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc558:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc559:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc560:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc561:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc562:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc563:                
                        movabs  rdi, 881611184              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc564:                
                        movabs  rdi, 881611216              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc565:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc566:                
                        movabs  rdi, 881611280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc567:                
                        movabs  rdi, 881611312              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc568:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc569:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc570:                
                        movabs  rdi, 881611376              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc571:                
                        movabs  rdi, 881611408              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc572:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc573:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc574:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc575:                
                        movabs  rdi, 881611504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc576:                
                        movabs  rdi, 881611536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc577:                
                        movabs  rdi, 881611568              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc578:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc579:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc580:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc581:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc582:                
                        movabs  rdi, 881611696              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc583:                
                        movabs  rdi, 9                      
                        call    scrip_rt_push_int@PLT       
.Lpc584:                
                        mov     edi, 19                     # SM_MUL
                        call    scrip_rt_arith@PLT          
.Lpc585:                
                        movabs  rdi, 10                     
                        call    scrip_rt_push_int@PLT       
.Lpc586:                
                        mov     edi, 20                     # SM_DIV
                        call    scrip_rt_arith@PLT          
.Lpc587:                
                        movabs  rdi, 881611728              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc588:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc589:                
                        movabs  rdi, 881611760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc590:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc591:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc592:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc593:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc594:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc595:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc596:                
                        movabs  rdi, 881611856              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc597:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc598:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc599:                
                        movabs  rdi, 881611952              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc600:                
                        movabs  rdi, 881611984              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc601:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc602:                
                        movabs  rdi, 881612048              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc603:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc604:                
                        movabs  rdi, 881612080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc605:                
                        movabs  rdi, 20                     
                        call    scrip_rt_push_int@PLT       
.Lpc606:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc607:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc608:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc609:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc610:                
                        movabs  rdi, 20                     
                        call    scrip_rt_push_int@PLT       
.Lpc611:                
                        movabs  rdi, 881612176              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc612:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc613:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc614:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc615:                
                        movabs  rdi, 881612272              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc616:                
                        movabs  rdi, 6                      
                        call    scrip_rt_push_int@PLT       
.Lpc617:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc618:                
                        movabs  rdi, 881612304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc619:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc620:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc621:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc622:                
                        movabs  rdi, 881612368              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc623:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc624:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc625:                
                        movabs  rdi, 40                     
                        call    scrip_rt_push_int@PLT       
.Lpc626:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc627:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc628:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc629:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc630:                
                        movabs  rdi, 40                     
                        call    scrip_rt_push_int@PLT       
.Lpc631:                
                        movabs  rdi, 881612496              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc632:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc633:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc634:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc635:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc636:                
                        movabs  rdi, 11                     
                        call    scrip_rt_push_int@PLT       
.Lpc637:                
                        movabs  rdi, 881612624              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc638:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc639:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc640:                
                        movabs  rdi, 40                     
                        call    scrip_rt_push_int@PLT       
.Lpc641:                
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc642:                
                        movabs  rdi, 8                      
                        call    scrip_rt_push_int@PLT       
.Lpc643:                
                        mov     edi, 19                     # SM_MUL
                        call    scrip_rt_arith@PLT          
.Lpc644:                
                        movabs  rdi, 70                     
                        call    scrip_rt_push_int@PLT       
.Lpc645:                
                        mov     edi, 20                     # SM_DIV
                        call    scrip_rt_arith@PLT          
.Lpc646:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc647:                
                        movabs  rdi, 881612688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc648:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc649:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc650:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc651:                
                        movabs  rdi, 881612752              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc652:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc653:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc654:                
                        movabs  rdi, 15                     
                        call    scrip_rt_push_int@PLT       
.Lpc655:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc656:                
                        movabs  rdi, 881612816              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc657:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc658:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc659:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc660:                
                        movabs  rdi, 881612880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc661:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc662:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc663:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc664:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc665:                
                        movabs  rdi, 881612944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc666:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc667:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc668:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc669:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc670:                
                        movabs  rdi, 881613008              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc671:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc672:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc673:                
                        movabs  rdi, 40                     
                        call    scrip_rt_push_int@PLT       
.Lpc674:                
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc675:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc676:                
                        mov     edi, 19                     # SM_MUL
                        call    scrip_rt_arith@PLT          
.Lpc677:                
                        movabs  rdi, 70                     
                        call    scrip_rt_push_int@PLT       
.Lpc678:                
                        mov     edi, 20                     # SM_DIV
                        call    scrip_rt_arith@PLT          
.Lpc679:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc680:                
                        movabs  rdi, 881613072              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc681:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc682:                
                        movabs  rdi, 881613104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc683:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc684:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc685:                
                        movabs  rdi, 881613168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc686:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc687:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc688:                
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc689:                
                        movabs  rdi, 5                      
                        call    scrip_rt_push_int@PLT       
.Lpc690:                
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc691:                
                        movabs  rdi, 881613232              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc692:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc693:                
                        movabs  rdi, 881613264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc694:                
                        movabs  rdi, 10                     
                        call    scrip_rt_push_int@PLT       
.Lpc695:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc696:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc697:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc698:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc699:                
                        movabs  rdi, 10                     
                        call    scrip_rt_push_int@PLT       
.Lpc700:                
                        movabs  rdi, 881613360              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc701:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc702:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc703:                
                        movabs  rdi, 881613456              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc704:                
                        movabs  rdi, 881613488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc705:                
                        movabs  rdi, 881613520              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc706:                
                        movabs  rdi, 881613552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc707:                
                        movabs  rdi, 881613584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc708:                
                        movabs  rdi, 881613616              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc709:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc710:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc711:                
                        movabs  rdi, 881613680              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc712:                
                        movabs  rdi, 881613712              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc713:                
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc714:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc715:                
                        movabs  rdi, 881613776              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc716:                
                        movabs  rdi, 881613808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc717:                
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc718:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc719:                
                        movabs  rdi, 881613872              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc720:                
                        movabs  rdi, 881613904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc721:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc722:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc723:                
                        movabs  rdi, 881613968              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc724:                
                        movabs  rdi, 881614000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc725:                
                        movabs  rdi, 881614032              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc726:                
                        movabs  rdi, 881614064              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc727:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc728:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc729:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc730:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc731:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc732:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc733:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc734:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc735:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc736:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc737:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc738:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc739:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc740:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc741:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc742:                
                        movabs  rdi, 881614096              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc743:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc744:                
                        movabs  rdi, 881614128              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc745:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc746:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc747:                
                        movabs  rdi, 881614192              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc748:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc749:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc750:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc751:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc752:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc753:                
                        movabs  rdi, 881614320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc754:                
# -- BB box scaffold pc=754 op=SM_PAT_SPAN --
# proc .bb_box_754
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc755:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc756:                
                        movabs  rdi, 881614352              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc757:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc758:                
                        movabs  rdi, 881614384              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc759:                
                        movabs  rdi, 881614416              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc760:                
                        movabs  rdi, 881614448              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc761:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc762:                
# -- BB box scaffold pc=762 op=SM_PAT_BREAK --
# proc .bb_box_762
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc763:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc764:                
                        movabs  rdi, 881614480              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc765:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc766:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc767:                
                        movabs  rdi, 881614512              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc768:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc769:                
                        movabs  rdi, 881614544              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc770:                
                        movabs  rdi, 881614576              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc771:                
                        movabs  rdi, 881614608              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc772:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc773:                
# -- BB box scaffold pc=773 op=SM_PAT_BREAK --
# proc .bb_box_773
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc774:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc775:                
                        movabs  rdi, 881614640              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc776:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc777:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc778:                
                        movabs  rdi, 881614672              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc779:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc780:                
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc781:                
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc782:                
# -- BB box scaffold pc=782 op=SM_PAT_ALT --
# proc .bb_box_782
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc783:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc784:                
                        movabs  rdi, 881614768              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc785:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc786:                
                        movabs  rdi, 881614800              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc787:                
# -- BB box scaffold pc=787 op=SM_PAT_SPAN --
# proc .bb_box_787
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc788:                
# -- BB box scaffold pc=788 op=SM_PAT_LIT --
# proc .bb_box_788
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc789:                
                        movabs  rdi, 881614864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc790:                
# -- BB box scaffold pc=790 op=SM_PAT_SPAN --
# proc .bb_box_790
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc791:                
                        movabs  rdi, 881614896              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc792:                
# -- BB box scaffold pc=792 op=SM_PAT_DEREF --
# proc .bb_box_792
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc793:                
# -- BB box scaffold pc=793 op=SM_PAT_ALT --
# proc .bb_box_793
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc794:                
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc795:                
# -- BB box scaffold pc=795 op=SM_PAT_CAT --
# proc .bb_box_795
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc796:                
                        movabs  rdi, 881614928              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc797:                
# -- BB box scaffold pc=797 op=SM_PAT_DEREF --
# proc .bb_box_797
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc798:                
# -- BB box scaffold pc=798 op=SM_PAT_ALT --
# proc .bb_box_798
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc799:                
# -- BB box scaffold pc=799 op=SM_PAT_LIT --
# proc .bb_box_799
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc800:                
# -- BB box scaffold pc=800 op=SM_PAT_LIT --
# proc .bb_box_800
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc801:                
# -- BB box scaffold pc=801 op=SM_PAT_ALT --
# proc .bb_box_801
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc802:                
# -- BB box scaffold pc=802 op=SM_PAT_LIT --
# proc .bb_box_802
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc803:                
# -- BB box scaffold pc=803 op=SM_PAT_LIT --
# proc .bb_box_803
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc804:                
                        movabs  rdi, 881615088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc805:                
# -- BB box scaffold pc=805 op=SM_PAT_DEREF --
# proc .bb_box_805
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc806:                
# -- BB box scaffold pc=806 op=SM_PAT_ALT --
# proc .bb_box_806
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc807:                
# -- BB box scaffold pc=807 op=SM_PAT_ALT --
# proc .bb_box_807
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc808:                
                        movabs  rdi, 881615120              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc809:                
# -- BB box scaffold pc=809 op=SM_PAT_SPAN --
# proc .bb_box_809
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc810:                
# -- BB box scaffold pc=810 op=SM_PAT_CAT --
# proc .bb_box_810
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc811:                
# -- BB box scaffold pc=811 op=SM_PAT_CAT --
# proc .bb_box_811
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc812:                
# -- BB box scaffold pc=812 op=SM_PAT_CAT --
# proc .bb_box_812
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc813:                
# -- BB box scaffold pc=813 op=SM_PAT_CAT --
# proc .bb_box_813
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc814:                
                        movabs  rdi, 881615152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc815:                
# -- BB box scaffold pc=815 op=SM_PAT_SPAN --
# proc .bb_box_815
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc816:                
# -- BB box scaffold pc=816 op=SM_PAT_LIT --
# proc .bb_box_816
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc817:                
                        movabs  rdi, 881615216              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc818:                
# -- BB box scaffold pc=818 op=SM_PAT_SPAN --
# proc .bb_box_818
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc819:                
                        movabs  rdi, 881615248              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc820:                
# -- BB box scaffold pc=820 op=SM_PAT_DEREF --
# proc .bb_box_820
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc821:                
# -- BB box scaffold pc=821 op=SM_PAT_ALT --
# proc .bb_box_821
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc822:                
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc823:                
# -- BB box scaffold pc=823 op=SM_PAT_CAT --
# proc .bb_box_823
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc824:                
# -- BB box scaffold pc=824 op=SM_PAT_CAT --
# proc .bb_box_824
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc825:                
# -- BB box scaffold pc=825 op=SM_PAT_ALT --
# proc .bb_box_825
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc826:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc827:                
                        movabs  rdi, 881615280              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc828:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc829:                
                        movabs  rdi, 881615312              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc830:                
                        movabs  rdi, 881615344              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc831:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc832:                
# -- BB box scaffold pc=832 op=SM_PAT_ANY --
# proc .bb_box_832
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc833:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc834:                
                        movabs  rdi, 881615376              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc835:                
                        movabs  rdi, 881615408              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc836:                
                        movabs  rdi, 881615440              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc837:                
                        movabs  rdi, 881615472              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc838:                
                        movabs  rdi, 881615504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc839:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc840:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc841:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc842:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc843:                
# -- BB box scaffold pc=843 op=SM_PAT_SPAN --
# proc .bb_box_843
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc844:                
                        movabs  rdi, 881615536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc845:                
# -- BB box scaffold pc=845 op=SM_PAT_DEREF --
# proc .bb_box_845
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc846:                
# -- BB box scaffold pc=846 op=SM_PAT_ALT --
# proc .bb_box_846
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc847:                
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc848:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc849:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc850:                
                        movabs  rdi, 881615568              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc851:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc852:                
                        movabs  rdi, 881615600              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc853:                
                        movabs  rdi, 881615632              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc854:                
                        movabs  rdi, 881615664              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc855:                
                        movabs  rdi, 881615696              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc856:                
                        movabs  rdi, 881615728              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc857:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc858:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc859:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc860:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc861:                
# -- BB box scaffold pc=861 op=SM_PAT_SPAN --
# proc .bb_box_861
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc862:                
# -- BB box scaffold pc=862 op=SM_PAT_CAPTURE --
# proc .bb_box_862
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc863:                
                        mov     edi, 51                     # SM_PAT_CAPTURE_FN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc864:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc865:                
                        movabs  rdi, 881615824              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc866:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc867:                
                        movabs  rdi, 881617920              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc868:                
                        movabs  rdi, 881617952              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc869:                
                        movabs  rdi, 881617984              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc870:                
                        movabs  rdi, 881618016              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc871:                
                        movabs  rdi, 881618048              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc872:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc873:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc874:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc875:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc876:                
# -- BB box scaffold pc=876 op=SM_PAT_SPAN --
# proc .bb_box_876
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc877:                
# -- BB box scaffold pc=877 op=SM_PAT_CAPTURE --
# proc .bb_box_877
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc878:                
                        mov     edi, 51                     # SM_PAT_CAPTURE_FN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc879:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc880:                
                        movabs  rdi, 881618144              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc881:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc882:                
                        movabs  rdi, 881618176              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc883:                
                        movabs  rdi, 881618208              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc884:                
                        movabs  rdi, 881618240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc885:                
                        movabs  rdi, 881618272              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc886:                
                        movabs  rdi, 881618304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc887:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc888:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc889:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc890:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc891:                
# -- BB box scaffold pc=891 op=SM_PAT_SPAN --
# proc .bb_box_891
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc892:                
# -- BB box scaffold pc=892 op=SM_PAT_CAPTURE --
# proc .bb_box_892
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc893:                
                        mov     edi, 51                     # SM_PAT_CAPTURE_FN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc894:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc895:                
                        movabs  rdi, 881618400              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc896:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc897:                
                        movabs  rdi, 881618432              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc898:                
                        movabs  rdi, 881618464              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc899:                
                        movabs  rdi, 881618496              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc900:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc901:                
# -- BB box scaffold pc=901 op=SM_PAT_SPAN --
# proc .bb_box_901
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc902:                
# -- BB box scaffold pc=902 op=SM_PAT_CAPTURE --
# proc .bb_box_902
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc903:                
                        mov     edi, 51                     # SM_PAT_CAPTURE_FN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc904:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc905:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc906:                
                        movabs  rdi, 881618592              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc907:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc908:                
                        movabs  rdi, 881618624              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc909:                
                        movabs  rdi, 881618656              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc910:                
                        movabs  rdi, 881618688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc911:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc912:                
# -- BB box scaffold pc=912 op=SM_PAT_SPAN --
# proc .bb_box_912
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc913:                
# -- BB box scaffold pc=913 op=SM_PAT_CAPTURE --
# proc .bb_box_913
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc914:                
                        mov     edi, 51                     # SM_PAT_CAPTURE_FN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc915:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc916:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc917:                
                        movabs  rdi, 881618784              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc918:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc919:                
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc920:                
                        movabs  rdi, 881618848              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc921:                
# -- BB box scaffold pc=921 op=SM_PAT_DEREF --
# proc .bb_box_921
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc922:                
# -- BB box scaffold pc=922 op=SM_PAT_ALT --
# proc .bb_box_922
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc923:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc924:                
                        movabs  rdi, 881618880              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc925:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc926:                
                        movabs  rdi, 881618912              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc927:                
                        movabs  rdi, 881618944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc928:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc929:                
# -- BB box scaffold pc=929 op=SM_PAT_SPAN --
# proc .bb_box_929
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc930:                
                        movabs  rdi, 881618976              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc931:                
# -- BB box scaffold pc=931 op=SM_PAT_DEREF --
# proc .bb_box_931
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc932:                
# -- BB box scaffold pc=932 op=SM_PAT_LIT --
# proc .bb_box_932
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc933:                
# -- BB box scaffold pc=933 op=SM_PAT_LIT --
# proc .bb_box_933
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc934:                
# -- BB box scaffold pc=934 op=SM_PAT_ALT --
# proc .bb_box_934
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc935:                
                        movabs  rdi, 881619072              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc936:                
                        movabs  rdi, 881619104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc937:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc938:                
# -- BB box scaffold pc=938 op=SM_PAT_SPAN --
# proc .bb_box_938
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc939:                
                        movabs  rdi, 881619136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc940:                
# -- BB box scaffold pc=940 op=SM_PAT_DEREF --
# proc .bb_box_940
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc941:                
# -- BB box scaffold pc=941 op=SM_PAT_ALT --
# proc .bb_box_941
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc942:                
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc943:                
# -- BB box scaffold pc=943 op=SM_PAT_CAT --
# proc .bb_box_943
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc944:                
# -- BB box scaffold pc=944 op=SM_PAT_CAT --
# proc .bb_box_944
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc945:                
                        movabs  rdi, 881619168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc946:                
# -- BB box scaffold pc=946 op=SM_PAT_DEREF --
# proc .bb_box_946
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc947:                
# -- BB box scaffold pc=947 op=SM_PAT_ALT --
# proc .bb_box_947
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc948:                
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc949:                
# -- BB box scaffold pc=949 op=SM_PAT_CAT --
# proc .bb_box_949
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc950:                
                        movabs  rdi, 881619200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc951:                
# -- BB box scaffold pc=951 op=SM_PAT_DEREF --
# proc .bb_box_951
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc952:                
# -- BB box scaffold pc=952 op=SM_PAT_LIT --
# proc .bb_box_952
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc953:                
# -- BB box scaffold pc=953 op=SM_PAT_LIT --
# proc .bb_box_953
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc954:                
# -- BB box scaffold pc=954 op=SM_PAT_ALT --
# proc .bb_box_954
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc955:                
                        movabs  rdi, 881619296              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc956:                
                        movabs  rdi, 881619328              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc957:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc958:                
# -- BB box scaffold pc=958 op=SM_PAT_SPAN --
# proc .bb_box_958
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc959:                
                        movabs  rdi, 881619360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc960:                
# -- BB box scaffold pc=960 op=SM_PAT_DEREF --
# proc .bb_box_960
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc961:                
# -- BB box scaffold pc=961 op=SM_PAT_ALT --
# proc .bb_box_961
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc962:                
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc963:                
# -- BB box scaffold pc=963 op=SM_PAT_CAT --
# proc .bb_box_963
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc964:                
# -- BB box scaffold pc=964 op=SM_PAT_CAT --
# proc .bb_box_964
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc965:                
# -- BB box scaffold pc=965 op=SM_PAT_ALT --
# proc .bb_box_965
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc966:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc967:                
                        movabs  rdi, 881619392              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc968:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc969:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc970:                
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc971:                
# -- BB box scaffold pc=971 op=SM_PAT_LIT --
# proc .bb_box_971
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc972:                
# -- BB box scaffold pc=972 op=SM_PAT_ALT --
# proc .bb_box_972
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc973:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc974:                
                        movabs  rdi, 881619456              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc975:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc976:                
# -- BB box scaffold pc=976 op=SM_PAT_LIT --
# proc .bb_box_976
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc977:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc978:                
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc979:                
# -- BB box scaffold pc=979 op=SM_PAT_ALT --
# proc .bb_box_979
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc980:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc981:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc982:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc983:                
                        movabs  rdi, 881619552              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc984:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc985:                
                        movabs  rdi, 881157408              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc986:                
                        movabs  rdi, 881619584              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc987:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc988:                
                        movabs  rdi, 881303312              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc989:                
                        movabs  rdi, 881619616              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc990:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc991:                
                        movabs  rdi, 881301648              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc992:                
                        movabs  rdi, 881299984              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc993:                
                        movabs  rdi, 881327632              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc994:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc995:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc996:                
                        movabs  rdi, 881619648              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc997:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc998:                
                        movabs  rdi, 881298000              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc999:                
                        movabs  rdi, 881296464              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1000:               
                        movabs  rdi, 881273696              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1001:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1002:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1003:               
                        movabs  rdi, 881619680              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1004:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1005:               
                        movabs  rdi, 881296064              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1006:               
                        movabs  rdi, 881295296              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1007:               
                        movabs  rdi, 881619712              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1008:               
                        movabs  rdi, 881619776              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1009:               
                        movabs  rdi, 881619840              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1010:               
                        movabs  rdi, 881619904              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1011:               
                        movabs  rdi, 881619968              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1012:               
                        movabs  rdi, 881620032              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1013:               
                        movabs  rdi, 881620096              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1014:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1015:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1016:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1017:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1018:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1019:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1020:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1021:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1022:               
                        movabs  rdi, 881620160              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1023:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1024:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1025:               
# -- BB box scaffold pc=1025 op=SM_PAT_LIT --
# proc .bb_box_1025
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1026:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1027:               
# -- BB box scaffold pc=1027 op=SM_PAT_CAT --
# proc .bb_box_1027
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1028:               
# -- BB box scaffold pc=1028 op=SM_PAT_CAT --
# proc .bb_box_1028
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1029:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1030:               
                        movabs  rdi, 881620288              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1031:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1032:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1033:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1034:               
# -- BB box scaffold pc=1034 op=SM_PAT_LIT --
# proc .bb_box_1034
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1035:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1036:               
# -- BB box scaffold pc=1036 op=SM_PAT_CAT --
# proc .bb_box_1036
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1037:               
# -- BB box scaffold pc=1037 op=SM_PAT_CAT --
# proc .bb_box_1037
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1038:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1039:               
                        movabs  rdi, 881620448              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1040:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1041:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1042:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1043:               
# -- BB box scaffold pc=1043 op=SM_PAT_LIT --
# proc .bb_box_1043
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1044:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1045:               
# -- BB box scaffold pc=1045 op=SM_PAT_CAT --
# proc .bb_box_1045
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1046:               
# -- BB box scaffold pc=1046 op=SM_PAT_CAT --
# proc .bb_box_1046
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1047:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1048:               
                        movabs  rdi, 881620608              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1049:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1050:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1051:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1052:               
# -- BB box scaffold pc=1052 op=SM_PAT_LIT --
# proc .bb_box_1052
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1053:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1054:               
# -- BB box scaffold pc=1054 op=SM_PAT_CAT --
# proc .bb_box_1054
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1055:               
# -- BB box scaffold pc=1055 op=SM_PAT_CAT --
# proc .bb_box_1055
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1056:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1057:               
                        movabs  rdi, 881620768              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1058:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1059:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1060:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1061:               
# -- BB box scaffold pc=1061 op=SM_PAT_LIT --
# proc .bb_box_1061
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1062:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1063:               
# -- BB box scaffold pc=1063 op=SM_PAT_CAT --
# proc .bb_box_1063
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1064:               
# -- BB box scaffold pc=1064 op=SM_PAT_CAT --
# proc .bb_box_1064
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1065:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1066:               
                        movabs  rdi, 881620928              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1067:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1068:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1069:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1070:               
# -- BB box scaffold pc=1070 op=SM_PAT_LIT --
# proc .bb_box_1070
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1071:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1072:               
# -- BB box scaffold pc=1072 op=SM_PAT_CAT --
# proc .bb_box_1072
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1073:               
# -- BB box scaffold pc=1073 op=SM_PAT_CAT --
# proc .bb_box_1073
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1074:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1075:               
                        movabs  rdi, 881621088              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1076:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1077:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1078:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1079:               
# -- BB box scaffold pc=1079 op=SM_PAT_LIT --
# proc .bb_box_1079
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1080:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1081:               
# -- BB box scaffold pc=1081 op=SM_PAT_CAT --
# proc .bb_box_1081
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1082:               
# -- BB box scaffold pc=1082 op=SM_PAT_CAT --
# proc .bb_box_1082
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1083:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1084:               
                        movabs  rdi, 881621248              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1085:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1086:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1087:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1088:               
# -- BB box scaffold pc=1088 op=SM_PAT_LIT --
# proc .bb_box_1088
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1089:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1090:               
# -- BB box scaffold pc=1090 op=SM_PAT_CAT --
# proc .bb_box_1090
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1091:               
# -- BB box scaffold pc=1091 op=SM_PAT_CAT --
# proc .bb_box_1091
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1092:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1093:               
                        movabs  rdi, 881621408              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1094:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1095:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1096:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1097:               
# -- BB box scaffold pc=1097 op=SM_PAT_LIT --
# proc .bb_box_1097
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1098:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1099:               
# -- BB box scaffold pc=1099 op=SM_PAT_CAT --
# proc .bb_box_1099
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1100:               
# -- BB box scaffold pc=1100 op=SM_PAT_CAT --
# proc .bb_box_1100
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1101:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1102:               
                        movabs  rdi, 881621568              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1103:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1104:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1105:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1106:               
# -- BB box scaffold pc=1106 op=SM_PAT_LIT --
# proc .bb_box_1106
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1107:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1108:               
# -- BB box scaffold pc=1108 op=SM_PAT_CAT --
# proc .bb_box_1108
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1109:               
# -- BB box scaffold pc=1109 op=SM_PAT_CAT --
# proc .bb_box_1109
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1110:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1111:               
                        movabs  rdi, 881621728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1112:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1113:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1114:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1115:               
# -- BB box scaffold pc=1115 op=SM_PAT_LIT --
# proc .bb_box_1115
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1116:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1117:               
# -- BB box scaffold pc=1117 op=SM_PAT_CAT --
# proc .bb_box_1117
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1118:               
# -- BB box scaffold pc=1118 op=SM_PAT_CAT --
# proc .bb_box_1118
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1119:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1120:               
                        movabs  rdi, 881621888              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1121:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1122:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1123:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1124:               
# -- BB box scaffold pc=1124 op=SM_PAT_LIT --
# proc .bb_box_1124
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1125:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1126:               
# -- BB box scaffold pc=1126 op=SM_PAT_CAT --
# proc .bb_box_1126
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1127:               
# -- BB box scaffold pc=1127 op=SM_PAT_CAT --
# proc .bb_box_1127
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1128:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1129:               
                        movabs  rdi, 881622048              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1130:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1131:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1132:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1133:               
# -- BB box scaffold pc=1133 op=SM_PAT_LIT --
# proc .bb_box_1133
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1134:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1135:               
# -- BB box scaffold pc=1135 op=SM_PAT_CAT --
# proc .bb_box_1135
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1136:               
# -- BB box scaffold pc=1136 op=SM_PAT_CAT --
# proc .bb_box_1136
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1137:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1138:               
                        movabs  rdi, 881622208              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1139:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1140:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1141:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1142:               
# -- BB box scaffold pc=1142 op=SM_PAT_LIT --
# proc .bb_box_1142
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1143:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1144:               
# -- BB box scaffold pc=1144 op=SM_PAT_CAT --
# proc .bb_box_1144
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1145:               
# -- BB box scaffold pc=1145 op=SM_PAT_CAT --
# proc .bb_box_1145
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1146:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1147:               
                        movabs  rdi, 881622368              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1148:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1149:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1150:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1151:               
# -- BB box scaffold pc=1151 op=SM_PAT_LIT --
# proc .bb_box_1151
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1152:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1153:               
# -- BB box scaffold pc=1153 op=SM_PAT_CAT --
# proc .bb_box_1153
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1154:               
# -- BB box scaffold pc=1154 op=SM_PAT_CAT --
# proc .bb_box_1154
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1155:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1156:               
                        movabs  rdi, 881622528              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1157:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1158:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1159:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1160:               
# -- BB box scaffold pc=1160 op=SM_PAT_LIT --
# proc .bb_box_1160
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1161:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1162:               
# -- BB box scaffold pc=1162 op=SM_PAT_CAT --
# proc .bb_box_1162
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1163:               
# -- BB box scaffold pc=1163 op=SM_PAT_CAT --
# proc .bb_box_1163
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1164:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1165:               
                        movabs  rdi, 881622688              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1166:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1167:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1168:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1169:               
# -- BB box scaffold pc=1169 op=SM_PAT_LIT --
# proc .bb_box_1169
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1170:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1171:               
# -- BB box scaffold pc=1171 op=SM_PAT_CAT --
# proc .bb_box_1171
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1172:               
# -- BB box scaffold pc=1172 op=SM_PAT_CAT --
# proc .bb_box_1172
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1173:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1174:               
                        movabs  rdi, 881622848              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1175:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1176:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1177:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1178:               
# -- BB box scaffold pc=1178 op=SM_PAT_LIT --
# proc .bb_box_1178
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1179:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1180:               
# -- BB box scaffold pc=1180 op=SM_PAT_CAT --
# proc .bb_box_1180
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1181:               
# -- BB box scaffold pc=1181 op=SM_PAT_CAT --
# proc .bb_box_1181
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1182:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1183:               
                        movabs  rdi, 881623008              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1184:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1185:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1186:               
# -- BB box scaffold pc=1186 op=SM_PAT_LIT --
# proc .bb_box_1186
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1187:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1188:               
# -- BB box scaffold pc=1188 op=SM_PAT_CAT --
# proc .bb_box_1188
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1189:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1190:               
                        movabs  rdi, 881623136              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1191:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1192:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1193:               
# -- BB box scaffold pc=1193 op=SM_PAT_LIT --
# proc .bb_box_1193
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1194:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1195:               
# -- BB box scaffold pc=1195 op=SM_PAT_CAT --
# proc .bb_box_1195
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1196:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1197:               
                        movabs  rdi, 881623264              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1198:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1199:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1200:               
# -- BB box scaffold pc=1200 op=SM_PAT_LIT --
# proc .bb_box_1200
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1201:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1202:               
# -- BB box scaffold pc=1202 op=SM_PAT_CAT --
# proc .bb_box_1202
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1203:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1204:               
                        movabs  rdi, 881623392              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1205:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1206:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1207:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1208:               
# -- BB box scaffold pc=1208 op=SM_PAT_LIT --
# proc .bb_box_1208
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1209:               
# -- BB box scaffold pc=1209 op=SM_PAT_CAT --
# proc .bb_box_1209
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1210:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1211:               
                        movabs  rdi, 881623520              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1212:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1213:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1214:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1215:               
# -- BB box scaffold pc=1215 op=SM_PAT_LIT --
# proc .bb_box_1215
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1216:               
# -- BB box scaffold pc=1216 op=SM_PAT_CAT --
# proc .bb_box_1216
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1217:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1218:               
                        movabs  rdi, 881623648              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1219:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1220:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1221:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1222:               
# -- BB box scaffold pc=1222 op=SM_PAT_LIT --
# proc .bb_box_1222
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1223:               
# -- BB box scaffold pc=1223 op=SM_PAT_CAT --
# proc .bb_box_1223
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1224:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1225:               
                        movabs  rdi, 881623776              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1226:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1227:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1228:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1229:               
# -- BB box scaffold pc=1229 op=SM_PAT_DEREF --
# proc .bb_box_1229
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1230:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1231:               
                        movabs  rdi, 881623904              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1232:               
                        movabs  rdi, 881623936              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1233:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1234:               
# -- BB box scaffold pc=1234 op=SM_PAT_DEREF --
# proc .bb_box_1234
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1235:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1236:               
# -- BB box scaffold pc=1236 op=SM_PAT_DEREF --
# proc .bb_box_1236
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1237:               
# -- BB box scaffold pc=1237 op=SM_PAT_CAT --
# proc .bb_box_1237
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1238:               
# -- BB box scaffold pc=1238 op=SM_PAT_CAT --
# proc .bb_box_1238
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1239:               
# -- BB box scaffold pc=1239 op=SM_PAT_CAT --
# proc .bb_box_1239
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1240:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1241:               
                        movabs  rdi, 881624032              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1242:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1243:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1244:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1245:               
                        movabs  rdi, 881624128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1246:               
                        movabs  rdi, 881624160              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1247:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1248:               
# -- BB box scaffold pc=1248 op=SM_PAT_DEREF --
# proc .bb_box_1248
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1249:               
# -- BB box scaffold pc=1249 op=SM_PAT_ALT --
# proc .bb_box_1249
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1250:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1251:               
                        movabs  rdi, 881624224              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1252:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1253:               
# -- BB box scaffold pc=1253 op=SM_PAT_DEREF --
# proc .bb_box_1253
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1254:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1255:               
# -- BB box scaffold pc=1255 op=SM_PAT_CAT --
# proc .bb_box_1255
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1256:               
                        movabs  rdi, 881624320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1257:               
# -- BB box scaffold pc=1257 op=SM_PAT_DEREF --
# proc .bb_box_1257
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1258:               
# -- BB box scaffold pc=1258 op=SM_PAT_ALT --
# proc .bb_box_1258
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1259:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1260:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1261:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1262:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1263:               
                        movabs  rdi, 881624352              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1264:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1265:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1266:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1267:               
                        movabs  rdi, 881624384              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1268:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1269:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1270:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1271:               
                        movabs  rdi, 881624416              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1272:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1273:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1274:               
                        movabs  rdi, 881624480              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1275:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1276:               
# -- BB box scaffold pc=1276 op=SM_PAT_DEREF --
# proc .bb_box_1276
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1277:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1278:               
                        movabs  rdi, 881624576              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1279:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1280:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1281:               
# -- BB box scaffold pc=1281 op=SM_PAT_DEREF --
# proc .bb_box_1281
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1282:               
# -- BB box scaffold pc=1282 op=SM_PAT_CAT --
# proc .bb_box_1282
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1283:               
# -- BB box scaffold pc=1283 op=SM_PAT_CAT --
# proc .bb_box_1283
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1284:               
                        movabs  rdi, 881624640              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1285:               
# -- BB box scaffold pc=1285 op=SM_PAT_DEREF --
# proc .bb_box_1285
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1286:               
# -- BB box scaffold pc=1286 op=SM_PAT_ALT --
# proc .bb_box_1286
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1287:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1288:               
# -- BB box scaffold pc=1288 op=SM_PAT_CAT --
# proc .bb_box_1288
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1289:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1290:               
                        movabs  rdi, 881624672              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1291:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1292:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1293:               
                        movabs  rdi, 881624736              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1294:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1295:               
# -- BB box scaffold pc=1295 op=SM_PAT_DEREF --
# proc .bb_box_1295
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1296:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1297:               
                        movabs  rdi, 881624832              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1298:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1299:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1300:               
# -- BB box scaffold pc=1300 op=SM_PAT_DEREF --
# proc .bb_box_1300
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1301:               
# -- BB box scaffold pc=1301 op=SM_PAT_CAT --
# proc .bb_box_1301
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1302:               
# -- BB box scaffold pc=1302 op=SM_PAT_CAT --
# proc .bb_box_1302
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1303:               
                        movabs  rdi, 881624896              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1304:               
# -- BB box scaffold pc=1304 op=SM_PAT_DEREF --
# proc .bb_box_1304
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1305:               
# -- BB box scaffold pc=1305 op=SM_PAT_ALT --
# proc .bb_box_1305
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1306:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1307:               
# -- BB box scaffold pc=1307 op=SM_PAT_CAT --
# proc .bb_box_1307
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1308:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1309:               
                        movabs  rdi, 881624928              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1310:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1311:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1312:               
                        movabs  rdi, 881624992              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1313:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1314:               
# -- BB box scaffold pc=1314 op=SM_PAT_DEREF --
# proc .bb_box_1314
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1315:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1316:               
                        movabs  rdi, 881625088              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1317:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1318:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1319:               
# -- BB box scaffold pc=1319 op=SM_PAT_DEREF --
# proc .bb_box_1319
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1320:               
# -- BB box scaffold pc=1320 op=SM_PAT_CAT --
# proc .bb_box_1320
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1321:               
# -- BB box scaffold pc=1321 op=SM_PAT_CAT --
# proc .bb_box_1321
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1322:               
                        movabs  rdi, 881625152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1323:               
# -- BB box scaffold pc=1323 op=SM_PAT_DEREF --
# proc .bb_box_1323
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1324:               
# -- BB box scaffold pc=1324 op=SM_PAT_ALT --
# proc .bb_box_1324
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1325:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1326:               
# -- BB box scaffold pc=1326 op=SM_PAT_CAT --
# proc .bb_box_1326
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1327:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1328:               
                        movabs  rdi, 881625184              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1329:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1330:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1331:               
# -- BB box scaffold pc=1331 op=SM_PAT_DEREF --
# proc .bb_box_1331
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1332:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1333:               
                        movabs  rdi, 881625280              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1334:               
                        movabs  rdi, 881625312              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1335:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1336:               
# -- BB box scaffold pc=1336 op=SM_PAT_DEREF --
# proc .bb_box_1336
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1337:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1338:               
# -- BB box scaffold pc=1338 op=SM_PAT_DEREF --
# proc .bb_box_1338
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1339:               
# -- BB box scaffold pc=1339 op=SM_PAT_CAT --
# proc .bb_box_1339
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1340:               
# -- BB box scaffold pc=1340 op=SM_PAT_CAT --
# proc .bb_box_1340
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1341:               
# -- BB box scaffold pc=1341 op=SM_PAT_CAT --
# proc .bb_box_1341
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1342:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1343:               
                        movabs  rdi, 881625408              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1344:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1345:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1346:               
# -- BB box scaffold pc=1346 op=SM_PAT_DEREF --
# proc .bb_box_1346
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1347:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1348:               
                        movabs  rdi, 881625504              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1349:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1350:               
# -- BB box scaffold pc=1350 op=SM_PAT_DEREF --
# proc .bb_box_1350
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1351:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1352:               
# -- BB box scaffold pc=1352 op=SM_PAT_CAT --
# proc .bb_box_1352
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1353:               
                        movabs  rdi, 881625600              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1354:               
# -- BB box scaffold pc=1354 op=SM_PAT_DEREF --
# proc .bb_box_1354
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1355:               
# -- BB box scaffold pc=1355 op=SM_PAT_ALT --
# proc .bb_box_1355
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1356:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1357:               
# -- BB box scaffold pc=1357 op=SM_PAT_CAT --
# proc .bb_box_1357
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1358:               
# -- BB box scaffold pc=1358 op=SM_PAT_CAT --
# proc .bb_box_1358
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1359:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1360:               
                        movabs  rdi, 881625632              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1361:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1362:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1363:               
# -- BB box scaffold pc=1363 op=SM_PAT_DEREF --
# proc .bb_box_1363
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1364:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1365:               
                        movabs  rdi, 881625728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1366:               
                        movabs  rdi, 881625760              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1367:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1368:               
# -- BB box scaffold pc=1368 op=SM_PAT_DEREF --
# proc .bb_box_1368
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1369:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1370:               
# -- BB box scaffold pc=1370 op=SM_PAT_DEREF --
# proc .bb_box_1370
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1371:               
# -- BB box scaffold pc=1371 op=SM_PAT_CAT --
# proc .bb_box_1371
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1372:               
# -- BB box scaffold pc=1372 op=SM_PAT_CAT --
# proc .bb_box_1372
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1373:               
# -- BB box scaffold pc=1373 op=SM_PAT_CAT --
# proc .bb_box_1373
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1374:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1375:               
                        movabs  rdi, 881625856              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1376:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1377:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1378:               
# -- BB box scaffold pc=1378 op=SM_PAT_DEREF --
# proc .bb_box_1378
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1379:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1380:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1381:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1382:               
# -- BB box scaffold pc=1382 op=SM_PAT_CAT --
# proc .bb_box_1382
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1383:               
                        movabs  rdi, 881626016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1384:               
# -- BB box scaffold pc=1384 op=SM_PAT_DEREF --
# proc .bb_box_1384
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1385:               
# -- BB box scaffold pc=1385 op=SM_PAT_ALT --
# proc .bb_box_1385
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1386:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1387:               
# -- BB box scaffold pc=1387 op=SM_PAT_CAT --
# proc .bb_box_1387
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1388:               
# -- BB box scaffold pc=1388 op=SM_PAT_CAT --
# proc .bb_box_1388
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1389:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1390:               
                        movabs  rdi, 881626048              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1391:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1392:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1393:               
                        movabs  rdi, 881626112              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1394:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1395:               
# -- BB box scaffold pc=1395 op=SM_PAT_DEREF --
# proc .bb_box_1395
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1396:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1397:               
                        movabs  rdi, 881626208              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1398:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1399:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1400:               
# -- BB box scaffold pc=1400 op=SM_PAT_DEREF --
# proc .bb_box_1400
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1401:               
# -- BB box scaffold pc=1401 op=SM_PAT_CAT --
# proc .bb_box_1401
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1402:               
# -- BB box scaffold pc=1402 op=SM_PAT_CAT --
# proc .bb_box_1402
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1403:               
                        movabs  rdi, 881626272              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1404:               
# -- BB box scaffold pc=1404 op=SM_PAT_DEREF --
# proc .bb_box_1404
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1405:               
# -- BB box scaffold pc=1405 op=SM_PAT_ALT --
# proc .bb_box_1405
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1406:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1407:               
# -- BB box scaffold pc=1407 op=SM_PAT_CAT --
# proc .bb_box_1407
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1408:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1409:               
                        movabs  rdi, 881626304              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1410:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1411:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1412:               
                        movabs  rdi, 881626368              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1413:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1414:               
# -- BB box scaffold pc=1414 op=SM_PAT_DEREF --
# proc .bb_box_1414
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1415:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1416:               
                        movabs  rdi, 881626464              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1417:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1418:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1419:               
# -- BB box scaffold pc=1419 op=SM_PAT_DEREF --
# proc .bb_box_1419
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1420:               
# -- BB box scaffold pc=1420 op=SM_PAT_CAT --
# proc .bb_box_1420
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1421:               
# -- BB box scaffold pc=1421 op=SM_PAT_CAT --
# proc .bb_box_1421
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1422:               
                        movabs  rdi, 881626528              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1423:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1424:               
# -- BB box scaffold pc=1424 op=SM_PAT_DEREF --
# proc .bb_box_1424
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1425:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1426:               
                        movabs  rdi, 881694480              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1427:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1428:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1429:               
# -- BB box scaffold pc=1429 op=SM_PAT_DEREF --
# proc .bb_box_1429
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1430:               
# -- BB box scaffold pc=1430 op=SM_PAT_CAT --
# proc .bb_box_1430
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1431:               
# -- BB box scaffold pc=1431 op=SM_PAT_CAT --
# proc .bb_box_1431
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1432:               
                        movabs  rdi, 881694544              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1433:               
# -- BB box scaffold pc=1433 op=SM_PAT_DEREF --
# proc .bb_box_1433
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1434:               
# -- BB box scaffold pc=1434 op=SM_PAT_ALT --
# proc .bb_box_1434
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1435:               
# -- BB box scaffold pc=1435 op=SM_PAT_ALT --
# proc .bb_box_1435
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1436:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1437:               
# -- BB box scaffold pc=1437 op=SM_PAT_CAT --
# proc .bb_box_1437
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1438:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1439:               
                        movabs  rdi, 881694576              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1440:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1441:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1442:               
                        movabs  rdi, 881694640              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1443:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1444:               
# -- BB box scaffold pc=1444 op=SM_PAT_DEREF --
# proc .bb_box_1444
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1445:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1446:               
                        movabs  rdi, 881694736              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1447:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1448:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1449:               
# -- BB box scaffold pc=1449 op=SM_PAT_DEREF --
# proc .bb_box_1449
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1450:               
# -- BB box scaffold pc=1450 op=SM_PAT_CAT --
# proc .bb_box_1450
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1451:               
# -- BB box scaffold pc=1451 op=SM_PAT_CAT --
# proc .bb_box_1451
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1452:               
                        movabs  rdi, 881694800              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1453:               
# -- BB box scaffold pc=1453 op=SM_PAT_DEREF --
# proc .bb_box_1453
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1454:               
# -- BB box scaffold pc=1454 op=SM_PAT_ALT --
# proc .bb_box_1454
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1455:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1456:               
# -- BB box scaffold pc=1456 op=SM_PAT_CAT --
# proc .bb_box_1456
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1457:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1458:               
                        movabs  rdi, 881694832              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1459:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1460:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1461:               
                        movabs  rdi, 881694896              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1462:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1463:               
# -- BB box scaffold pc=1463 op=SM_PAT_DEREF --
# proc .bb_box_1463
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1464:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1465:               
                        movabs  rdi, 881694992              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1466:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1467:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1468:               
# -- BB box scaffold pc=1468 op=SM_PAT_DEREF --
# proc .bb_box_1468
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1469:               
# -- BB box scaffold pc=1469 op=SM_PAT_CAT --
# proc .bb_box_1469
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1470:               
# -- BB box scaffold pc=1470 op=SM_PAT_CAT --
# proc .bb_box_1470
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1471:               
                        movabs  rdi, 881695056              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1472:               
# -- BB box scaffold pc=1472 op=SM_PAT_DEREF --
# proc .bb_box_1472
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1473:               
# -- BB box scaffold pc=1473 op=SM_PAT_ALT --
# proc .bb_box_1473
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1474:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1475:               
# -- BB box scaffold pc=1475 op=SM_PAT_CAT --
# proc .bb_box_1475
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1476:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1477:               
                        movabs  rdi, 881695088              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1478:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1479:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1480:               
                        movabs  rdi, 881695152              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1481:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1482:               
# -- BB box scaffold pc=1482 op=SM_PAT_DEREF --
# proc .bb_box_1482
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1483:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1484:               
                        movabs  rdi, 881695248              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1485:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1486:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1487:               
# -- BB box scaffold pc=1487 op=SM_PAT_DEREF --
# proc .bb_box_1487
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1488:               
# -- BB box scaffold pc=1488 op=SM_PAT_CAT --
# proc .bb_box_1488
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1489:               
# -- BB box scaffold pc=1489 op=SM_PAT_CAT --
# proc .bb_box_1489
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1490:               
                        movabs  rdi, 881695312              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1491:               
# -- BB box scaffold pc=1491 op=SM_PAT_DEREF --
# proc .bb_box_1491
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1492:               
# -- BB box scaffold pc=1492 op=SM_PAT_ALT --
# proc .bb_box_1492
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1493:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1494:               
# -- BB box scaffold pc=1494 op=SM_PAT_CAT --
# proc .bb_box_1494
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1495:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1496:               
                        movabs  rdi, 881695344              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1497:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1498:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1499:               
                        movabs  rdi, 881695408              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1500:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1501:               
# -- BB box scaffold pc=1501 op=SM_PAT_DEREF --
# proc .bb_box_1501
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1502:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1503:               
                        movabs  rdi, 881695504              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1504:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1505:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1506:               
# -- BB box scaffold pc=1506 op=SM_PAT_DEREF --
# proc .bb_box_1506
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1507:               
# -- BB box scaffold pc=1507 op=SM_PAT_CAT --
# proc .bb_box_1507
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1508:               
# -- BB box scaffold pc=1508 op=SM_PAT_CAT --
# proc .bb_box_1508
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1509:               
                        movabs  rdi, 881695568              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1510:               
# -- BB box scaffold pc=1510 op=SM_PAT_DEREF --
# proc .bb_box_1510
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1511:               
# -- BB box scaffold pc=1511 op=SM_PAT_ALT --
# proc .bb_box_1511
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1512:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1513:               
# -- BB box scaffold pc=1513 op=SM_PAT_CAT --
# proc .bb_box_1513
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1514:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1515:               
                        movabs  rdi, 881695600              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1516:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1517:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1518:               
                        movabs  rdi, 881695664              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1519:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1520:               
# -- BB box scaffold pc=1520 op=SM_PAT_DEREF --
# proc .bb_box_1520
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1521:               
                        movabs  rdi, 881695728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1522:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1523:               
# -- BB box scaffold pc=1523 op=SM_PAT_DEREF --
# proc .bb_box_1523
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1524:               
                        movabs  rdi, 881695792              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1525:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1526:               
# -- BB box scaffold pc=1526 op=SM_PAT_DEREF --
# proc .bb_box_1526
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1527:               
# -- BB box scaffold pc=1527 op=SM_PAT_ALT --
# proc .bb_box_1527
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1528:               
# -- BB box scaffold pc=1528 op=SM_PAT_ALT --
# proc .bb_box_1528
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1529:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1530:               
                        movabs  rdi, 881695888              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1531:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1532:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1533:               
# -- BB box scaffold pc=1533 op=SM_PAT_DEREF --
# proc .bb_box_1533
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1534:               
# -- BB box scaffold pc=1534 op=SM_PAT_CAT --
# proc .bb_box_1534
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1535:               
# -- BB box scaffold pc=1535 op=SM_PAT_CAT --
# proc .bb_box_1535
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1536:               
                        movabs  rdi, 881695952              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1537:               
# -- BB box scaffold pc=1537 op=SM_PAT_DEREF --
# proc .bb_box_1537
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1538:               
# -- BB box scaffold pc=1538 op=SM_PAT_ALT --
# proc .bb_box_1538
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1539:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1540:               
# -- BB box scaffold pc=1540 op=SM_PAT_CAT --
# proc .bb_box_1540
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1541:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1542:               
                        movabs  rdi, 881695984              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1543:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1544:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1545:               
                        movabs  rdi, 881696048              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1546:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1547:               
# -- BB box scaffold pc=1547 op=SM_PAT_DEREF --
# proc .bb_box_1547
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1548:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1549:               
                        movabs  rdi, 881696144              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1550:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1551:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1552:               
# -- BB box scaffold pc=1552 op=SM_PAT_DEREF --
# proc .bb_box_1552
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1553:               
# -- BB box scaffold pc=1553 op=SM_PAT_CAT --
# proc .bb_box_1553
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1554:               
# -- BB box scaffold pc=1554 op=SM_PAT_CAT --
# proc .bb_box_1554
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1555:               
                        movabs  rdi, 881696208              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1556:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1557:               
# -- BB box scaffold pc=1557 op=SM_PAT_DEREF --
# proc .bb_box_1557
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1558:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1559:               
                        movabs  rdi, 881696304              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1560:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1561:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1562:               
# -- BB box scaffold pc=1562 op=SM_PAT_DEREF --
# proc .bb_box_1562
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1563:               
# -- BB box scaffold pc=1563 op=SM_PAT_CAT --
# proc .bb_box_1563
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1564:               
# -- BB box scaffold pc=1564 op=SM_PAT_CAT --
# proc .bb_box_1564
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1565:               
                        movabs  rdi, 881696368              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1566:               
# -- BB box scaffold pc=1566 op=SM_PAT_DEREF --
# proc .bb_box_1566
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1567:               
# -- BB box scaffold pc=1567 op=SM_PAT_ALT --
# proc .bb_box_1567
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1568:               
# -- BB box scaffold pc=1568 op=SM_PAT_ALT --
# proc .bb_box_1568
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1569:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1570:               
# -- BB box scaffold pc=1570 op=SM_PAT_CAT --
# proc .bb_box_1570
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1571:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1572:               
                        movabs  rdi, 881696400              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1573:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1574:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1575:               
                        movabs  rdi, 881696464              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1576:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1577:               
# -- BB box scaffold pc=1577 op=SM_PAT_DEREF --
# proc .bb_box_1577
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1578:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1579:               
                        movabs  rdi, 881696560              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1580:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1581:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1582:               
# -- BB box scaffold pc=1582 op=SM_PAT_DEREF --
# proc .bb_box_1582
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1583:               
# -- BB box scaffold pc=1583 op=SM_PAT_CAT --
# proc .bb_box_1583
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1584:               
# -- BB box scaffold pc=1584 op=SM_PAT_CAT --
# proc .bb_box_1584
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1585:               
                        movabs  rdi, 881696624              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1586:               
# -- BB box scaffold pc=1586 op=SM_PAT_DEREF --
# proc .bb_box_1586
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1587:               
# -- BB box scaffold pc=1587 op=SM_PAT_ALT --
# proc .bb_box_1587
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1588:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1589:               
# -- BB box scaffold pc=1589 op=SM_PAT_CAT --
# proc .bb_box_1589
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1590:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1591:               
                        movabs  rdi, 881696656              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1592:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1593:               
# -- BB box scaffold pc=1593 op=SM_PAT_LIT --
# proc .bb_box_1593
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1594:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1595:               
                        movabs  rdi, 881696752              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1596:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1597:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1598:               
# -- BB box scaffold pc=1598 op=SM_PAT_DEREF --
# proc .bb_box_1598
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1599:               
# -- BB box scaffold pc=1599 op=SM_PAT_CAT --
# proc .bb_box_1599
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1600:               
# -- BB box scaffold pc=1600 op=SM_PAT_CAT --
# proc .bb_box_1600
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1601:               
# -- BB box scaffold pc=1601 op=SM_PAT_LIT --
# proc .bb_box_1601
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1602:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1603:               
                        movabs  rdi, 881696880              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1604:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1605:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1606:               
# -- BB box scaffold pc=1606 op=SM_PAT_DEREF --
# proc .bb_box_1606
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1607:               
# -- BB box scaffold pc=1607 op=SM_PAT_CAT --
# proc .bb_box_1607
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1608:               
# -- BB box scaffold pc=1608 op=SM_PAT_CAT --
# proc .bb_box_1608
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1609:               
# -- BB box scaffold pc=1609 op=SM_PAT_LIT --
# proc .bb_box_1609
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1610:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1611:               
                        movabs  rdi, 881697008              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1612:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1613:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1614:               
# -- BB box scaffold pc=1614 op=SM_PAT_DEREF --
# proc .bb_box_1614
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1615:               
# -- BB box scaffold pc=1615 op=SM_PAT_CAT --
# proc .bb_box_1615
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1616:               
# -- BB box scaffold pc=1616 op=SM_PAT_CAT --
# proc .bb_box_1616
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1617:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1618:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1619:               
                        movabs  rdi, 881697072              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1620:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1621:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1622:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1623:               
                        movabs  rdi, 881697104              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1624:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1625:               
# -- BB box scaffold pc=1625 op=SM_PAT_DEREF --
# proc .bb_box_1625
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1626:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1627:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1628:               
                        movabs  rdi, 881697168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1629:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1630:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1631:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1632:               
                        movabs  rdi, 881697200              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1633:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1634:               
# -- BB box scaffold pc=1634 op=SM_PAT_DEREF --
# proc .bb_box_1634
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1635:               
# -- BB box scaffold pc=1635 op=SM_PAT_LIT --
# proc .bb_box_1635
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1636:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1637:               
                        movabs  rdi, 881697328              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1638:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1639:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1640:               
# -- BB box scaffold pc=1640 op=SM_PAT_DEREF --
# proc .bb_box_1640
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1641:               
# -- BB box scaffold pc=1641 op=SM_PAT_CAT --
# proc .bb_box_1641
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1642:               
# -- BB box scaffold pc=1642 op=SM_PAT_CAT --
# proc .bb_box_1642
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1643:               
# -- BB box scaffold pc=1643 op=SM_PAT_LIT --
# proc .bb_box_1643
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1644:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1645:               
                        movabs  rdi, 881697456              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1646:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1647:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1648:               
# -- BB box scaffold pc=1648 op=SM_PAT_DEREF --
# proc .bb_box_1648
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1649:               
# -- BB box scaffold pc=1649 op=SM_PAT_CAT --
# proc .bb_box_1649
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1650:               
# -- BB box scaffold pc=1650 op=SM_PAT_CAT --
# proc .bb_box_1650
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1651:               
# -- BB box scaffold pc=1651 op=SM_PAT_LIT --
# proc .bb_box_1651
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1652:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1653:               
                        movabs  rdi, 881697584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1654:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1655:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1656:               
# -- BB box scaffold pc=1656 op=SM_PAT_DEREF --
# proc .bb_box_1656
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1657:               
# -- BB box scaffold pc=1657 op=SM_PAT_CAT --
# proc .bb_box_1657
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1658:               
# -- BB box scaffold pc=1658 op=SM_PAT_CAT --
# proc .bb_box_1658
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1659:               
# -- BB box scaffold pc=1659 op=SM_PAT_LIT --
# proc .bb_box_1659
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1660:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1661:               
                        movabs  rdi, 881697712              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1662:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1663:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1664:               
# -- BB box scaffold pc=1664 op=SM_PAT_DEREF --
# proc .bb_box_1664
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1665:               
# -- BB box scaffold pc=1665 op=SM_PAT_CAT --
# proc .bb_box_1665
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1666:               
# -- BB box scaffold pc=1666 op=SM_PAT_CAT --
# proc .bb_box_1666
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1667:               
# -- BB box scaffold pc=1667 op=SM_PAT_LIT --
# proc .bb_box_1667
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1668:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1669:               
                        movabs  rdi, 881697840              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1670:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1671:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1672:               
# -- BB box scaffold pc=1672 op=SM_PAT_DEREF --
# proc .bb_box_1672
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1673:               
# -- BB box scaffold pc=1673 op=SM_PAT_CAT --
# proc .bb_box_1673
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1674:               
# -- BB box scaffold pc=1674 op=SM_PAT_CAT --
# proc .bb_box_1674
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1675:               
# -- BB box scaffold pc=1675 op=SM_PAT_LIT --
# proc .bb_box_1675
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1676:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1677:               
                        movabs  rdi, 881697968              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1678:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1679:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1680:               
# -- BB box scaffold pc=1680 op=SM_PAT_DEREF --
# proc .bb_box_1680
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1681:               
# -- BB box scaffold pc=1681 op=SM_PAT_CAT --
# proc .bb_box_1681
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1682:               
# -- BB box scaffold pc=1682 op=SM_PAT_CAT --
# proc .bb_box_1682
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1683:               
# -- BB box scaffold pc=1683 op=SM_PAT_LIT --
# proc .bb_box_1683
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1684:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1685:               
                        movabs  rdi, 881698096              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1686:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1687:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1688:               
# -- BB box scaffold pc=1688 op=SM_PAT_DEREF --
# proc .bb_box_1688
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1689:               
# -- BB box scaffold pc=1689 op=SM_PAT_CAT --
# proc .bb_box_1689
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1690:               
# -- BB box scaffold pc=1690 op=SM_PAT_CAT --
# proc .bb_box_1690
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1691:               
# -- BB box scaffold pc=1691 op=SM_PAT_LIT --
# proc .bb_box_1691
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1692:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1693:               
                        movabs  rdi, 881698224              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1694:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1695:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1696:               
# -- BB box scaffold pc=1696 op=SM_PAT_DEREF --
# proc .bb_box_1696
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1697:               
# -- BB box scaffold pc=1697 op=SM_PAT_CAT --
# proc .bb_box_1697
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1698:               
# -- BB box scaffold pc=1698 op=SM_PAT_CAT --
# proc .bb_box_1698
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1699:               
# -- BB box scaffold pc=1699 op=SM_PAT_LIT --
# proc .bb_box_1699
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1700:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1701:               
                        movabs  rdi, 881698352              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1702:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1703:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1704:               
# -- BB box scaffold pc=1704 op=SM_PAT_DEREF --
# proc .bb_box_1704
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1705:               
# -- BB box scaffold pc=1705 op=SM_PAT_CAT --
# proc .bb_box_1705
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1706:               
# -- BB box scaffold pc=1706 op=SM_PAT_CAT --
# proc .bb_box_1706
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1707:               
# -- BB box scaffold pc=1707 op=SM_PAT_LIT --
# proc .bb_box_1707
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1708:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1709:               
                        movabs  rdi, 881698480              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1710:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1711:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1712:               
# -- BB box scaffold pc=1712 op=SM_PAT_DEREF --
# proc .bb_box_1712
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1713:               
# -- BB box scaffold pc=1713 op=SM_PAT_CAT --
# proc .bb_box_1713
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1714:               
# -- BB box scaffold pc=1714 op=SM_PAT_CAT --
# proc .bb_box_1714
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1715:               
# -- BB box scaffold pc=1715 op=SM_PAT_LIT --
# proc .bb_box_1715
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1716:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1717:               
                        movabs  rdi, 881698608              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1718:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1719:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1720:               
# -- BB box scaffold pc=1720 op=SM_PAT_DEREF --
# proc .bb_box_1720
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1721:               
# -- BB box scaffold pc=1721 op=SM_PAT_CAT --
# proc .bb_box_1721
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1722:               
# -- BB box scaffold pc=1722 op=SM_PAT_CAT --
# proc .bb_box_1722
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1723:               
# -- BB box scaffold pc=1723 op=SM_PAT_LIT --
# proc .bb_box_1723
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1724:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1725:               
                        movabs  rdi, 881698736              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1726:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1727:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1728:               
# -- BB box scaffold pc=1728 op=SM_PAT_DEREF --
# proc .bb_box_1728
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1729:               
# -- BB box scaffold pc=1729 op=SM_PAT_CAT --
# proc .bb_box_1729
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1730:               
# -- BB box scaffold pc=1730 op=SM_PAT_CAT --
# proc .bb_box_1730
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1731:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1732:               
# -- BB box scaffold pc=1732 op=SM_PAT_ALT --
# proc .bb_box_1732
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1733:               
# -- BB box scaffold pc=1733 op=SM_PAT_ALT --
# proc .bb_box_1733
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1734:               
# -- BB box scaffold pc=1734 op=SM_PAT_ALT --
# proc .bb_box_1734
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1735:               
# -- BB box scaffold pc=1735 op=SM_PAT_ALT --
# proc .bb_box_1735
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1736:               
# -- BB box scaffold pc=1736 op=SM_PAT_ALT --
# proc .bb_box_1736
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1737:               
# -- BB box scaffold pc=1737 op=SM_PAT_ALT --
# proc .bb_box_1737
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1738:               
# -- BB box scaffold pc=1738 op=SM_PAT_ALT --
# proc .bb_box_1738
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1739:               
# -- BB box scaffold pc=1739 op=SM_PAT_ALT --
# proc .bb_box_1739
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1740:               
# -- BB box scaffold pc=1740 op=SM_PAT_ALT --
# proc .bb_box_1740
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1741:               
# -- BB box scaffold pc=1741 op=SM_PAT_ALT --
# proc .bb_box_1741
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1742:               
# -- BB box scaffold pc=1742 op=SM_PAT_ALT --
# proc .bb_box_1742
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1743:               
# -- BB box scaffold pc=1743 op=SM_PAT_ALT --
# proc .bb_box_1743
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1744:               
# -- BB box scaffold pc=1744 op=SM_PAT_ALT --
# proc .bb_box_1744
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1745:               
# -- BB box scaffold pc=1745 op=SM_PAT_ALT --
# proc .bb_box_1745
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1746:               
# -- BB box scaffold pc=1746 op=SM_PAT_ALT --
# proc .bb_box_1746
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1747:               
# -- BB box scaffold pc=1747 op=SM_PAT_ALT --
# proc .bb_box_1747
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1748:               
# -- BB box scaffold pc=1748 op=SM_PAT_ALT --
# proc .bb_box_1748
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1749:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1750:               
                        movabs  rdi, 881698832              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1751:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1752:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1753:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1754:               
# -- BB box scaffold pc=1754 op=SM_PAT_DEREF --
# proc .bb_box_1754
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1755:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1756:               
                        movabs  rdi, 881698960              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1757:               
                        movabs  rdi, 881698992              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1758:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1759:               
# -- BB box scaffold pc=1759 op=SM_PAT_DEREF --
# proc .bb_box_1759
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1760:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1761:               
# -- BB box scaffold pc=1761 op=SM_PAT_DEREF --
# proc .bb_box_1761
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1762:               
# -- BB box scaffold pc=1762 op=SM_PAT_CAT --
# proc .bb_box_1762
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1763:               
# -- BB box scaffold pc=1763 op=SM_PAT_CAT --
# proc .bb_box_1763
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1764:               
# -- BB box scaffold pc=1764 op=SM_PAT_CAT --
# proc .bb_box_1764
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1765:               
                        movabs  rdi, 881699088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1766:               
# -- BB box scaffold pc=1766 op=SM_PAT_DEREF --
# proc .bb_box_1766
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1767:               
# -- BB box scaffold pc=1767 op=SM_PAT_ALT --
# proc .bb_box_1767
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1768:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1769:               
# -- BB box scaffold pc=1769 op=SM_PAT_CAT --
# proc .bb_box_1769
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1770:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1771:               
                        movabs  rdi, 881699120              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1772:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1773:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1774:               
                        movabs  rdi, 881699184              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1775:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1776:               
# -- BB box scaffold pc=1776 op=SM_PAT_DEREF --
# proc .bb_box_1776
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1777:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1778:               
                        movabs  rdi, 881699280              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1779:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1780:               
# -- BB box scaffold pc=1780 op=SM_PAT_DEREF --
# proc .bb_box_1780
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1781:               
# -- BB box scaffold pc=1781 op=SM_PAT_CAT --
# proc .bb_box_1781
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1782:               
# -- BB box scaffold pc=1782 op=SM_PAT_CAT --
# proc .bb_box_1782
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1783:               
                        movabs  rdi, 881699344              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1784:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1785:               
# -- BB box scaffold pc=1785 op=SM_PAT_DEREF --
# proc .bb_box_1785
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1786:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1787:               
                        movabs  rdi, 881699440              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1788:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1789:               
# -- BB box scaffold pc=1789 op=SM_PAT_DEREF --
# proc .bb_box_1789
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1790:               
# -- BB box scaffold pc=1790 op=SM_PAT_CAT --
# proc .bb_box_1790
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1791:               
# -- BB box scaffold pc=1791 op=SM_PAT_CAT --
# proc .bb_box_1791
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1792:               
# -- BB box scaffold pc=1792 op=SM_PAT_ALT --
# proc .bb_box_1792
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1793:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1794:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1795:               
                        movabs  rdi, 881699536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1796:               
# -- BB box scaffold pc=1796 op=SM_PAT_DEREF --
# proc .bb_box_1796
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1797:               
# -- BB box scaffold pc=1797 op=SM_PAT_ALT --
# proc .bb_box_1797
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1798:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1799:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1800:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1801:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1802:               
                        movabs  rdi, 881699568              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1803:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1804:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1805:               
# -- BB box scaffold pc=1805 op=SM_PAT_DEREF --
# proc .bb_box_1805
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1806:               
                        movabs  rdi, 881699632              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1807:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1808:               
# -- BB box scaffold pc=1808 op=SM_PAT_DEREF --
# proc .bb_box_1808
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1809:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1810:               
                        movabs  rdi, 881699728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1811:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1812:               
# -- BB box scaffold pc=1812 op=SM_PAT_DEREF --
# proc .bb_box_1812
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1813:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1814:               
                        movabs  rdi, 881699824              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1815:               
                        movabs  rdi, 881699856              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1816:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1817:               
# -- BB box scaffold pc=1817 op=SM_PAT_DEREF --
# proc .bb_box_1817
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1818:               
# -- BB box scaffold pc=1818 op=SM_PAT_CAT --
# proc .bb_box_1818
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1819:               
# -- BB box scaffold pc=1819 op=SM_PAT_CAT --
# proc .bb_box_1819
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1820:               
                        movabs  rdi, 881699920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1821:               
# -- BB box scaffold pc=1821 op=SM_PAT_DEREF --
# proc .bb_box_1821
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1822:               
                        movabs  rdi, 881699952              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1823:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc1824:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1825:               
# -- BB box scaffold pc=1825 op=SM_PAT_DEREF --
# proc .bb_box_1825
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1826:               
# -- BB box scaffold pc=1826 op=SM_PAT_CAT --
# proc .bb_box_1826
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1827:               
# -- BB box scaffold pc=1827 op=SM_PAT_ALT --
# proc .bb_box_1827
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1828:               
                        movabs  rdi, 881700016              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1829:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1830:               
# -- BB box scaffold pc=1830 op=SM_PAT_DEREF --
# proc .bb_box_1830
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1831:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1832:               
# -- BB box scaffold pc=1832 op=SM_PAT_DEREF --
# proc .bb_box_1832
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1833:               
# -- BB box scaffold pc=1833 op=SM_PAT_CAT --
# proc .bb_box_1833
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1834:               
# -- BB box scaffold pc=1834 op=SM_PAT_CAT --
# proc .bb_box_1834
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1835:               
# -- BB box scaffold pc=1835 op=SM_PAT_CAT --
# proc .bb_box_1835
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1836:               
# -- BB box scaffold pc=1836 op=SM_PAT_CAT --
# proc .bb_box_1836
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1837:               
# -- BB box scaffold pc=1837 op=SM_PAT_CAT --
# proc .bb_box_1837
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1838:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1839:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1840:               
                        movabs  rdi, 881700112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1841:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1842:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1843:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1844:               
                        movabs  rdi, 881700144              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1845:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1846:               
# -- BB box scaffold pc=1846 op=SM_PAT_DEREF --
# proc .bb_box_1846
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1847:               
                        movabs  rdi, 881700208              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1848:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1849:               
# -- BB box scaffold pc=1849 op=SM_PAT_DEREF --
# proc .bb_box_1849
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1850:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1851:               
                        movabs  rdi, 881700304              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1852:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1853:               
# -- BB box scaffold pc=1853 op=SM_PAT_DEREF --
# proc .bb_box_1853
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1854:               
                        movabs  rdi, 881700368              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1855:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1856:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1857:               
# -- BB box scaffold pc=1857 op=SM_PAT_DEREF --
# proc .bb_box_1857
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1858:               
# -- BB box scaffold pc=1858 op=SM_PAT_CAT --
# proc .bb_box_1858
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1859:               
# -- BB box scaffold pc=1859 op=SM_PAT_CAT --
# proc .bb_box_1859
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1860:               
# -- BB box scaffold pc=1860 op=SM_PAT_CAT --
# proc .bb_box_1860
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1861:               
# -- BB box scaffold pc=1861 op=SM_PAT_CAT --
# proc .bb_box_1861
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1862:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1863:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1864:               
                        movabs  rdi, 881700432              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1865:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1866:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1867:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1868:               
                        movabs  rdi, 881700464              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1869:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1870:               
# -- BB box scaffold pc=1870 op=SM_PAT_DEREF --
# proc .bb_box_1870
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1871:               
                        movabs  rdi, 881700528              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1872:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1873:               
# -- BB box scaffold pc=1873 op=SM_PAT_DEREF --
# proc .bb_box_1873
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1874:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1875:               
                        movabs  rdi, 881700624              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1876:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1877:               
# -- BB box scaffold pc=1877 op=SM_PAT_DEREF --
# proc .bb_box_1877
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1878:               
                        movabs  rdi, 881700688              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1879:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc1880:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1881:               
# -- BB box scaffold pc=1881 op=SM_PAT_DEREF --
# proc .bb_box_1881
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1882:               
# -- BB box scaffold pc=1882 op=SM_PAT_CAT --
# proc .bb_box_1882
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1883:               
# -- BB box scaffold pc=1883 op=SM_PAT_CAT --
# proc .bb_box_1883
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1884:               
# -- BB box scaffold pc=1884 op=SM_PAT_CAT --
# proc .bb_box_1884
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1885:               
# -- BB box scaffold pc=1885 op=SM_PAT_CAT --
# proc .bb_box_1885
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1886:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1887:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1888:               
                        movabs  rdi, 881700752              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1889:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1890:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1891:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1892:               
                        movabs  rdi, 881700784              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1893:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1894:               
# -- BB box scaffold pc=1894 op=SM_PAT_DEREF --
# proc .bb_box_1894
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1895:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1896:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1897:               
                        movabs  rdi, 881700848              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1898:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1899:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1900:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1901:               
                        movabs  rdi, 881700880              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1902:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1903:               
# -- BB box scaffold pc=1903 op=SM_PAT_DEREF --
# proc .bb_box_1903
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1904:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1905:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1906:               
                        movabs  rdi, 881700944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1907:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1908:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1909:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1910:               
                        movabs  rdi, 881700976              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1911:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1912:               
# -- BB box scaffold pc=1912 op=SM_PAT_DEREF --
# proc .bb_box_1912
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1913:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1914:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1915:               
                        movabs  rdi, 881701040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1916:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1917:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1918:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1919:               
                        movabs  rdi, 881701072              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1920:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1921:               
# -- BB box scaffold pc=1921 op=SM_PAT_DEREF --
# proc .bb_box_1921
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1922:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1923:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1924:               
                        movabs  rdi, 881701136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1925:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1926:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1927:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1928:               
                        movabs  rdi, 881701168              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1929:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1930:               
# -- BB box scaffold pc=1930 op=SM_PAT_DEREF --
# proc .bb_box_1930
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1931:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1932:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1933:               
                        movabs  rdi, 881701232              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc1934:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1935:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1936:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1937:               
                        movabs  rdi, 881701264              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1938:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1939:               
# -- BB box scaffold pc=1939 op=SM_PAT_DEREF --
# proc .bb_box_1939
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1940:               
# -- BB box scaffold pc=1940 op=SM_PAT_ALT --
# proc .bb_box_1940
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1941:               
# -- BB box scaffold pc=1941 op=SM_PAT_ALT --
# proc .bb_box_1941
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1942:               
# -- BB box scaffold pc=1942 op=SM_PAT_ALT --
# proc .bb_box_1942
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1943:               
# -- BB box scaffold pc=1943 op=SM_PAT_ALT --
# proc .bb_box_1943
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1944:               
# -- BB box scaffold pc=1944 op=SM_PAT_ALT --
# proc .bb_box_1944
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1945:               
# -- BB box scaffold pc=1945 op=SM_PAT_ALT --
# proc .bb_box_1945
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1946:               
# -- BB box scaffold pc=1946 op=SM_PAT_ALT --
# proc .bb_box_1946
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1947:               
# -- BB box scaffold pc=1947 op=SM_PAT_ALT --
# proc .bb_box_1947
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1948:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1949:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1950:               
                        movabs  rdi, 881701328              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1951:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1952:               
# -- BB box scaffold pc=1952 op=SM_PAT_LIT --
# proc .bb_box_1952
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1953:               
# -- BB box scaffold pc=1953 op=SM_PAT_LIT --
# proc .bb_box_1953
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1954:               
# -- BB box scaffold pc=1954 op=SM_PAT_ALT --
# proc .bb_box_1954
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1955:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1956:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1957:               
                        movabs  rdi, 881701424              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1958:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1959:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1960:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1961:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1962:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1963:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1964:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1965:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1966:               
                        movabs  rdi, 881701488              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1967:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1968:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1969:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1970:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1971:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1972:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1973:               
                        mov     edi, 52                     # SM_PAT_CAPTURE_FN_ARGS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1974:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1975:               
                        movabs  rdi, 881701552              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc1976:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1977:               
# -- BB box scaffold pc=1977 op=SM_PAT_LIT --
# proc .bb_box_1977
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1978:               
# -- BB box scaffold pc=1978 op=SM_PAT_LIT --
# proc .bb_box_1978
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1979:               
# -- BB box scaffold pc=1979 op=SM_PAT_ALT --
# proc .bb_box_1979
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1980:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1981:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1982:               
                        movabs  rdi, 881701648              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1983:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1984:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1985:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1986:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1987:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1988:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1989:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1990:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1991:               
                        movabs  rdi, 881701712              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc1992:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1993:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1994:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1995:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1996:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1997:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1998:               
                        mov     edi, 52                     # SM_PAT_CAPTURE_FN_ARGS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc1999:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2000:               
                        movabs  rdi, 881701776              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2001:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2002:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2003:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2004:               
# -- BB box scaffold pc=2004 op=SM_PAT_ALT --
# proc .bb_box_2004
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2005:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2006:               
                        movabs  rdi, 881701872              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2007:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2008:               
                        movabs  rdi, 881701904              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2009:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2010:               
# -- BB box scaffold pc=2010 op=SM_PAT_DEREF --
# proc .bb_box_2010
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2011:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2012:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2013:               
                        movabs  rdi, 881701968              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2014:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2015:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2016:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2017:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2018:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2019:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2020:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2021:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2022:               
                        movabs  rdi, 881702032              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2023:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2024:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2025:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2026:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2027:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2028:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2029:               
                        mov     edi, 52                     # SM_PAT_CAPTURE_FN_ARGS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2030:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2031:               
                        movabs  rdi, 881702128              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2032:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2033:               
# -- BB box scaffold pc=2033 op=SM_PAT_DEREF --
# proc .bb_box_2033
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2034:               
# -- BB box scaffold pc=2034 op=SM_PAT_CAT --
# proc .bb_box_2034
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2035:               
# -- BB box scaffold pc=2035 op=SM_PAT_CAT --
# proc .bb_box_2035
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2036:               
                        movabs  rdi, 881702192              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2037:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2038:               
# -- BB box scaffold pc=2038 op=SM_PAT_DEREF --
# proc .bb_box_2038
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2039:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2040:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2041:               
                        movabs  rdi, 881702256              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2042:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2043:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2044:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2045:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2046:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2047:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2048:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2049:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2050:               
                        movabs  rdi, 881628864              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2051:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2052:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2053:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2054:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2055:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2056:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2057:               
                        mov     edi, 52                     # SM_PAT_CAPTURE_FN_ARGS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2058:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2059:               
                        movabs  rdi, 881628960              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2060:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2061:               
# -- BB box scaffold pc=2061 op=SM_PAT_DEREF --
# proc .bb_box_2061
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2062:               
# -- BB box scaffold pc=2062 op=SM_PAT_CAT --
# proc .bb_box_2062
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2063:               
# -- BB box scaffold pc=2063 op=SM_PAT_CAT --
# proc .bb_box_2063
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2064:               
# -- BB box scaffold pc=2064 op=SM_PAT_ALT --
# proc .bb_box_2064
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2065:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2066:               
                        movabs  rdi, 881629024              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2067:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2068:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2069:               
# -- BB box scaffold pc=2069 op=SM_PAT_LIT --
# proc .bb_box_2069
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2070:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2071:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2072:               
                        movabs  rdi, 881629184              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2073:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2074:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2075:               
# -- BB box scaffold pc=2075 op=SM_PAT_DEREF --
# proc .bb_box_2075
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2076:               
                        movabs  rdi, 881629248              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2077:               
                        movabs  rdi, 881629280              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2078:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2079:               
# -- BB box scaffold pc=2079 op=SM_PAT_DEREF --
# proc .bb_box_2079
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2080:               
# -- BB box scaffold pc=2080 op=SM_PAT_CAT --
# proc .bb_box_2080
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2081:               
# -- BB box scaffold pc=2081 op=SM_PAT_CAT --
# proc .bb_box_2081
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2082:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2083:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2084:               
# -- BB box scaffold pc=2084 op=SM_PAT_ALT --
# proc .bb_box_2084
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2085:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2086:               
                        movabs  rdi, 881629440              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2087:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2088:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2089:               
# -- BB box scaffold pc=2089 op=SM_PAT_DEREF --
# proc .bb_box_2089
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2090:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2091:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2092:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2093:               
# -- BB box scaffold pc=2093 op=SM_PAT_ALT --
# proc .bb_box_2093
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2094:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2095:               
                        movabs  rdi, 881629632              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2096:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2097:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2098:               
# -- BB box scaffold pc=2098 op=SM_PAT_DEREF --
# proc .bb_box_2098
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2099:               
# -- BB box scaffold pc=2099 op=SM_PAT_CAT --
# proc .bb_box_2099
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2100:               
# -- BB box scaffold pc=2100 op=SM_PAT_CAT --
# proc .bb_box_2100
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2101:               
# -- BB box scaffold pc=2101 op=SM_PAT_CAT --
# proc .bb_box_2101
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2102:               
                        movabs  rdi, 881629696              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2103:               
                        movabs  rdi, 881629728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2104:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2105:               
# -- BB box scaffold pc=2105 op=SM_PAT_DEREF --
# proc .bb_box_2105
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2106:               
# -- BB box scaffold pc=2106 op=SM_PAT_ALT --
# proc .bb_box_2106
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2107:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2108:               
# -- BB box scaffold pc=2108 op=SM_PAT_CAT --
# proc .bb_box_2108
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2109:               
# -- BB box scaffold pc=2109 op=SM_PAT_CAT --
# proc .bb_box_2109
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2110:               
# -- BB box scaffold pc=2110 op=SM_PAT_CAT --
# proc .bb_box_2110
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2111:               
# -- BB box scaffold pc=2111 op=SM_PAT_ALT --
# proc .bb_box_2111
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2112:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2113:               
# -- BB box scaffold pc=2113 op=SM_PAT_CAT --
# proc .bb_box_2113
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2114:               
# -- BB box scaffold pc=2114 op=SM_PAT_CAT --
# proc .bb_box_2114
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2115:               
# -- BB box scaffold pc=2115 op=SM_PAT_CAT --
# proc .bb_box_2115
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2116:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2117:               
                        movabs  rdi, 881629792              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2118:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2119:               
                        movabs  rdi, 881629824              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2120:               
                        movabs  rdi, 881629856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2121:               
                        movabs  rdi, 881629888              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2122:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2123:               
# -- BB box scaffold pc=2123 op=SM_PAT_BREAK --
# proc .bb_box_2123
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2124:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2125:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2126:               
                        movabs  rdi, 881629920              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2127:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2128:               
                        movabs  rdi, 881629952              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2129:               
                        movabs  rdi, 881629984              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2130:               
# -- BB box scaffold pc=2130 op=SM_PAT_BREAK --
# proc .bb_box_2130
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2131:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2132:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2133:               
                        movabs  rdi, 881630016              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2134:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2135:               
                        movabs  rdi, 881630048              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2136:               
                        movabs  rdi, 881630080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2137:               
                        movabs  rdi, 881630112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2138:               
                        movabs  rdi, 881630144              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2139:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2140:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2141:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2142:               
# -- BB box scaffold pc=2142 op=SM_PAT_BREAK --
# proc .bb_box_2142
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2143:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2144:               
                        movabs  rdi, 881630176              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2145:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2146:               
                        movabs  rdi, 881630240              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2147:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2148:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2149:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2150:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2151:               
                        movabs  rdi, 881630368              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2152:               
                        movabs  rdi, 881630400              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2153:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2154:               
# -- BB box scaffold pc=2154 op=SM_PAT_DEREF --
# proc .bb_box_2154
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2155:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2156:               
                        movabs  rdi, 881630496              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2157:               
                        movabs  rdi, 881630528              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2158:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2159:               
# -- BB box scaffold pc=2159 op=SM_PAT_DEREF --
# proc .bb_box_2159
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2160:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2161:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2162:               
# -- BB box scaffold pc=2162 op=SM_PAT_CAT --
# proc .bb_box_2162
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2163:               
# -- BB box scaffold pc=2163 op=SM_PAT_CAT --
# proc .bb_box_2163
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2164:               
                        movabs  rdi, 881630656              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2165:               
                        movabs  rdi, 881630688              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2166:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2167:               
# -- BB box scaffold pc=2167 op=SM_PAT_DEREF --
# proc .bb_box_2167
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2168:               
                        movabs  rdi, 881630752              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2169:               
                        movabs  rdi, 881630784              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2170:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2171:               
# -- BB box scaffold pc=2171 op=SM_PAT_DEREF --
# proc .bb_box_2171
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2172:               
# -- BB box scaffold pc=2172 op=SM_PAT_CAT --
# proc .bb_box_2172
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2173:               
# -- BB box scaffold pc=2173 op=SM_PAT_ALT --
# proc .bb_box_2173
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2174:               
# -- BB box scaffold pc=2174 op=SM_PAT_CAT --
# proc .bb_box_2174
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2175:               
# -- BB box scaffold pc=2175 op=SM_PAT_CAT --
# proc .bb_box_2175
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2176:               
                        movabs  rdi, 881630848              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2177:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2178:               
# -- BB box scaffold pc=2178 op=SM_PAT_DEREF --
# proc .bb_box_2178
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2179:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2180:               
# -- BB box scaffold pc=2180 op=SM_PAT_ALT --
# proc .bb_box_2180
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2181:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2182:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2183:               
                        movabs  rdi, 881631008              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2184:               
                        movabs  rdi, 881631040              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2185:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2186:               
# -- BB box scaffold pc=2186 op=SM_PAT_DEREF --
# proc .bb_box_2186
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2187:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2188:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2189:               
# -- BB box scaffold pc=2189 op=SM_PAT_CAT --
# proc .bb_box_2189
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2190:               
# -- BB box scaffold pc=2190 op=SM_PAT_CAT --
# proc .bb_box_2190
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2191:               
                        movabs  rdi, 881631168              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2192:               
                        movabs  rdi, 881631200              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2193:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2194:               
# -- BB box scaffold pc=2194 op=SM_PAT_DEREF --
# proc .bb_box_2194
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2195:               
                        movabs  rdi, 881631264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2196:               
                        movabs  rdi, 881631296              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2197:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2198:               
# -- BB box scaffold pc=2198 op=SM_PAT_DEREF --
# proc .bb_box_2198
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2199:               
# -- BB box scaffold pc=2199 op=SM_PAT_CAT --
# proc .bb_box_2199
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2200:               
# -- BB box scaffold pc=2200 op=SM_PAT_ALT --
# proc .bb_box_2200
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2201:               
# -- BB box scaffold pc=2201 op=SM_PAT_CAT --
# proc .bb_box_2201
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2202:               
                        movabs  rdi, 881631360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2203:               
                        movabs  rdi, 881631392              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2204:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2205:               
# -- BB box scaffold pc=2205 op=SM_PAT_DEREF --
# proc .bb_box_2205
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2206:               
                        movabs  rdi, 881631456              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2207:               
                        movabs  rdi, 881631488              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2208:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2209:               
# -- BB box scaffold pc=2209 op=SM_PAT_DEREF --
# proc .bb_box_2209
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2210:               
# -- BB box scaffold pc=2210 op=SM_PAT_CAT --
# proc .bb_box_2210
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2211:               
# -- BB box scaffold pc=2211 op=SM_PAT_ALT --
# proc .bb_box_2211
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2212:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2213:               
# -- BB box scaffold pc=2213 op=SM_PAT_CAT --
# proc .bb_box_2213
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2214:               
# -- BB box scaffold pc=2214 op=SM_PAT_CAT --
# proc .bb_box_2214
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2215:               
                        movabs  rdi, 881631552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2216:               
                        movabs  rdi, 881631584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2217:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2218:               
# -- BB box scaffold pc=2218 op=SM_PAT_DEREF --
# proc .bb_box_2218
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2219:               
                        movabs  rdi, 881631648              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2220:               
                        movabs  rdi, 881631680              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2221:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2222:               
# -- BB box scaffold pc=2222 op=SM_PAT_DEREF --
# proc .bb_box_2222
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2223:               
                        movabs  rdi, 881631744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2224:               
                        movabs  rdi, 881631776              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2225:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2226:               
# -- BB box scaffold pc=2226 op=SM_PAT_DEREF --
# proc .bb_box_2226
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2227:               
# -- BB box scaffold pc=2227 op=SM_PAT_CAT --
# proc .bb_box_2227
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2228:               
# -- BB box scaffold pc=2228 op=SM_PAT_CAT --
# proc .bb_box_2228
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2229:               
# -- BB box scaffold pc=2229 op=SM_PAT_ALT --
# proc .bb_box_2229
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2230:               
# -- BB box scaffold pc=2230 op=SM_PAT_ALT --
# proc .bb_box_2230
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2231:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2232:               
# -- BB box scaffold pc=2232 op=SM_PAT_CAT --
# proc .bb_box_2232
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2233:               
# -- BB box scaffold pc=2233 op=SM_PAT_CAT --
# proc .bb_box_2233
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2234:               
                        movabs  rdi, 881631840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2235:               
                        movabs  rdi, 881631872              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2236:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2237:               
# -- BB box scaffold pc=2237 op=SM_PAT_DEREF --
# proc .bb_box_2237
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2238:               
                        movabs  rdi, 881631936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2239:               
                        movabs  rdi, 881631968              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2240:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2241:               
# -- BB box scaffold pc=2241 op=SM_PAT_DEREF --
# proc .bb_box_2241
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2242:               
                        movabs  rdi, 881632032              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2243:               
                        movabs  rdi, 881632064              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2244:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2245:               
# -- BB box scaffold pc=2245 op=SM_PAT_DEREF --
# proc .bb_box_2245
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2246:               
                        movabs  rdi, 881632128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2247:               
                        movabs  rdi, 881632160              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2248:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2249:               
# -- BB box scaffold pc=2249 op=SM_PAT_DEREF --
# proc .bb_box_2249
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2250:               
# -- BB box scaffold pc=2250 op=SM_PAT_CAT --
# proc .bb_box_2250
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2251:               
# -- BB box scaffold pc=2251 op=SM_PAT_CAT --
# proc .bb_box_2251
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2252:               
# -- BB box scaffold pc=2252 op=SM_PAT_CAT --
# proc .bb_box_2252
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2253:               
# -- BB box scaffold pc=2253 op=SM_PAT_ALT --
# proc .bb_box_2253
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2254:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2255:               
                        movabs  rdi, 881632256              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2256:               
                        movabs  rdi, 881632288              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2257:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2258:               
# -- BB box scaffold pc=2258 op=SM_PAT_DEREF --
# proc .bb_box_2258
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2259:               
                        movabs  rdi, 881632352              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2260:               
                        movabs  rdi, 881632384              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2261:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2262:               
# -- BB box scaffold pc=2262 op=SM_PAT_DEREF --
# proc .bb_box_2262
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2263:               
# -- BB box scaffold pc=2263 op=SM_PAT_CAT --
# proc .bb_box_2263
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2264:               
# -- BB box scaffold pc=2264 op=SM_PAT_ALT --
# proc .bb_box_2264
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2265:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2266:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2267:               
# -- BB box scaffold pc=2267 op=SM_PAT_CAT --
# proc .bb_box_2267
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2268:               
# -- BB box scaffold pc=2268 op=SM_PAT_CAT --
# proc .bb_box_2268
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2269:               
# -- BB box scaffold pc=2269 op=SM_PAT_CAT --
# proc .bb_box_2269
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2270:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2271:               
                        movabs  rdi, 881632480              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2272:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2273:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2274:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2275:               
                        movabs  rdi, 881632576              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2276:               
# -- BB box scaffold pc=2276 op=SM_PAT_DEREF --
# proc .bb_box_2276
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2277:               
# -- BB box scaffold pc=2277 op=SM_PAT_ALT --
# proc .bb_box_2277
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2278:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2279:               
# -- BB box scaffold pc=2279 op=SM_PAT_CAT --
# proc .bb_box_2279
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2280:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2281:               
                        movabs  rdi, 881632608              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2282:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2283:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2284:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2285:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2286:               
                        movabs  rdi, 881632672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2287:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2288:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2289:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2290:               
                        movabs  rdi, 881632704              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2291:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2292:               
# -- BB box scaffold pc=2292 op=SM_PAT_DEREF --
# proc .bb_box_2292
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2293:               
                        movabs  rdi, 881632768              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2294:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2295:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2296:               
# -- BB box scaffold pc=2296 op=SM_PAT_DEREF --
# proc .bb_box_2296
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2297:               
                        movabs  rdi, 881632832              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2298:               
# -- BB box scaffold pc=2298 op=SM_PAT_DEREF --
# proc .bb_box_2298
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2299:               
# -- BB box scaffold pc=2299 op=SM_PAT_CAT --
# proc .bb_box_2299
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2300:               
# -- BB box scaffold pc=2300 op=SM_PAT_CAT --
# proc .bb_box_2300
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2301:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2302:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2303:               
                        movabs  rdi, 881632864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2304:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2305:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2306:               
                        mov     edi, 13                     # SM_PUSH_CHUNK
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2307:               
                        movabs  rdi, 881632896              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2308:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2309:               
# -- BB box scaffold pc=2309 op=SM_PAT_DEREF --
# proc .bb_box_2309
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2310:               
                        movabs  rdi, 881632960              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2311:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2312:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2313:               
# -- BB box scaffold pc=2313 op=SM_PAT_DEREF --
# proc .bb_box_2313
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2314:               
                        movabs  rdi, 881633024              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2315:               
# -- BB box scaffold pc=2315 op=SM_PAT_DEREF --
# proc .bb_box_2315
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2316:               
# -- BB box scaffold pc=2316 op=SM_PAT_LIT --
# proc .bb_box_2316
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2317:               
# -- BB box scaffold pc=2317 op=SM_PAT_ALT --
# proc .bb_box_2317
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2318:               
# -- BB box scaffold pc=2318 op=SM_PAT_CAT --
# proc .bb_box_2318
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2319:               
# -- BB box scaffold pc=2319 op=SM_PAT_CAT --
# proc .bb_box_2319
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2320:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2321:               
                        movabs  rdi, 881633120              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2322:               
                        movabs  rdi, 7                      
                        call    scrip_rt_push_int@PLT       
.Lpc2323:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2324:               
# -- BB box scaffold pc=2324 op=SM_PAT_DEREF --
# proc .bb_box_2324
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2325:               
                        movabs  rdi, 881633184              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2326:               
# -- BB box scaffold pc=2326 op=SM_PAT_DEREF --
# proc .bb_box_2326
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2327:               
# -- BB box scaffold pc=2327 op=SM_PAT_LIT --
# proc .bb_box_2327
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2328:               
# -- BB box scaffold pc=2328 op=SM_PAT_ALT --
# proc .bb_box_2328
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2329:               
# -- BB box scaffold pc=2329 op=SM_PAT_CAT --
# proc .bb_box_2329
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2330:               
# -- BB box scaffold pc=2330 op=SM_PAT_CAT --
# proc .bb_box_2330
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2331:               
# -- BB box scaffold pc=2331 op=SM_PAT_ALT --
# proc .bb_box_2331
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2332:               
# -- BB box scaffold pc=2332 op=SM_PAT_ALT --
# proc .bb_box_2332
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2333:               
                        mov     edi, 41                     # SM_PAT_FENCE1
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2334:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2335:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2336:               
                        movabs  rdi, 881633248              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2337:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2338:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2339:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2340:               
# -- BB box scaffold pc=2340 op=SM_PAT_ARBNO --
# proc .bb_box_2340
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 37                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2341:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2342:               
                        movabs  rdi, 881633344              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2343:               
                        movabs  rdi, 881633376              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2344:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2345:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2346:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2347:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2348:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2349:               
                        movabs  rdi, 881633472              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2350:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2351:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2352:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2353:               
# -- BB box scaffold pc=2353 op=SM_PAT_ARBNO --
# proc .bb_box_2353
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 37                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2354:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2355:               
                        movabs  rdi, 881633568              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2356:               
                        movabs  rdi, 881633600              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2357:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2358:               
                        movabs  rdi, 881633664              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2359:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2360:               
# -- BB box scaffold pc=2360 op=SM_PAT_DEREF --
# proc .bb_box_2360
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2361:               
# -- BB box scaffold pc=2361 op=SM_PAT_LIT --
# proc .bb_box_2361
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2362:               
                        movabs  rdi, 881633760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2363:               
# -- BB box scaffold pc=2363 op=SM_PAT_BREAK --
# proc .bb_box_2363
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2364:               
                        movabs  rdi, 881633792              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2365:               
# -- BB box scaffold pc=2365 op=SM_PAT_DEREF --
# proc .bb_box_2365
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2366:               
# -- BB box scaffold pc=2366 op=SM_PAT_CAT --
# proc .bb_box_2366
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2367:               
# -- BB box scaffold pc=2367 op=SM_PAT_CAT --
# proc .bb_box_2367
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2368:               
                        movabs  rdi, 881633824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2369:               
# -- BB box scaffold pc=2369 op=SM_PAT_DEREF --
# proc .bb_box_2369
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2370:               
# -- BB box scaffold pc=2370 op=SM_PAT_ALT --
# proc .bb_box_2370
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2371:               
                        movabs  rdi, 881633856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2372:               
# -- BB box scaffold pc=2372 op=SM_PAT_BREAK --
# proc .bb_box_2372
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2373:               
                        movabs  rdi, 881633888              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2374:               
# -- BB box scaffold pc=2374 op=SM_PAT_DEREF --
# proc .bb_box_2374
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2375:               
# -- BB box scaffold pc=2375 op=SM_PAT_CAT --
# proc .bb_box_2375
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2376:               
# -- BB box scaffold pc=2376 op=SM_PAT_ARBNO --
# proc .bb_box_2376
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 37                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2377:               
# -- BB box scaffold pc=2377 op=SM_PAT_CAT --
# proc .bb_box_2377
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2378:               
# -- BB box scaffold pc=2378 op=SM_PAT_CAT --
# proc .bb_box_2378
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2379:               
                        movabs  rdi, 881633920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2380:               
# -- BB box scaffold pc=2380 op=SM_PAT_DEREF --
# proc .bb_box_2380
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2381:               
# -- BB box scaffold pc=2381 op=SM_PAT_ALT --
# proc .bb_box_2381
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2382:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2383:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2384:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2385:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2386:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2387:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2388:               
                        movabs  rdi, 881633984              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2389:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2390:               
                        movabs  rdi, 881634016              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2391:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2392:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2393:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2394:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2395:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2396:               
                        movabs  rdi, 881634176              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2397:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2398:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2399:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2400:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2401:               
                        movabs  rdi, 881634240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2402:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2403:               
                        movabs  rdi, 881634304              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2404:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2405:               
                        movabs  rdi, 881634336              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2406:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2407:               
                        movabs  rdi, 881634400              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2408:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2409:               
                        movabs  rdi, 881634432              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2410:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2411:               
                        movabs  rdi, 881634496              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2412:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2413:               
                        movabs  rdi, 881634528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2414:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2415:               
                        movabs  rdi, 881634592              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2416:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2417:               
                        movabs  rdi, 881634624              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2418:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc2419:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2420:               
                        movabs  rdi, 881634688              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2421:               
                        movabs  rdi, 881634720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2422:               
                        movabs  rdi, 881634752              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2423:               
                        movabs  rdi, 881634784              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2424:               
                        movabs  rdi, 881634816              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2425:               
                        movabs  rdi, 881634848              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2426:               
                        movabs  rdi, 881634880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2427:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2428:               
                        movabs  rdi, 881634944              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2429:               
                        movabs  rdi, 881634976              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2430:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2431:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2432:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2433:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2434:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2435:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2436:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2437:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2438:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2439:               
                        movabs  rdi, 881635008              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2440:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2441:               
                        movabs  rdi, 881635040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2442:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2443:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2444:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2445:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2446:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2447:               
                        movabs  rdi, 881635168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2448:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc2449:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2450:               
                        movabs  rdi, 881635232              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2451:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2452:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2453:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2454:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc2455:               
                        movabs  rdi, 881635360              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2456:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2457:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2458:               
                        movabs  rdi, 881635456              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2459:               
                        movabs  rdi, 881635488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2460:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2461:               
                        movabs  rdi, 881635552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2462:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2463:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc2464:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2465:               
                        movabs  rdi, 881635584              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2466:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2467:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2468:               
                        movabs  rdi, 881635616              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2469:               
                        movabs  rdi, 881635648              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2470:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2471:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2472:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2473:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2474:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2475:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2476:               
                        movabs  rdi, 881635808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2477:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2478:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2479:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2480:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2481:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2482:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2483:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2484:               
                        movabs  rdi, 881636000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2485:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2486:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2487:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2488:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2489:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2490:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2491:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2492:               
                        movabs  rdi, 881636192              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2493:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2494:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2495:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2496:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2497:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2498:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2499:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2500:               
                        movabs  rdi, 881636384              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2501:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2502:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2503:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2504:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2505:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2506:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2507:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2508:               
                        movabs  rdi, 881636576              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2509:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2510:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2511:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2512:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2513:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2514:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2515:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2516:               
                        movabs  rdi, 881636768              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2517:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2518:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2519:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2520:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2521:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2522:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2523:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2524:               
                        movabs  rdi, 881636960              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2525:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2526:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2527:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2528:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2529:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2530:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2531:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2532:               
                        movabs  rdi, 881637152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2533:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2534:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2535:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2536:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2537:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2538:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2539:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2540:               
                        movabs  rdi, 881637344              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2541:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2542:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2543:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2544:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2545:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2546:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2547:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2548:               
                        movabs  rdi, 881637536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2549:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2550:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2551:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2552:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2553:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2554:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2555:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2556:               
                        movabs  rdi, 881637728              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2557:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2558:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2559:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2560:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2561:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2562:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2563:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2564:               
                        movabs  rdi, 881637920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2565:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2566:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2567:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2568:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2569:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2570:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2571:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2572:               
                        movabs  rdi, 881638112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2573:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2574:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2575:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2576:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2577:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2578:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2579:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2580:               
                        movabs  rdi, 881638304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2581:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2582:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2583:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2584:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2585:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2586:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2587:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2588:               
                        movabs  rdi, 881638496              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2589:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2590:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2591:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2592:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2593:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2594:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2595:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2596:               
                        movabs  rdi, 881638688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2597:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2598:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2599:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2600:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2601:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2602:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2603:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2604:               
                        movabs  rdi, 881638880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2605:               
                        movabs  rdi, 881638912              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2606:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2607:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc2608:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2609:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2610:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2611:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2612:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2613:               
                        movabs  rdi, 881639040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2614:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2615:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2616:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2617:               
                        movabs  rdi, 881639104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2618:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2619:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2620:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2621:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2622:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2623:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2624:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2625:               
                        movabs  rdi, 881639264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2626:               
                        movabs  rdi, 881639296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2627:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2628:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc2629:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2630:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2631:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2632:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2633:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2634:               
                        movabs  rdi, 881639424              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2635:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2636:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2637:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2638:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2639:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2640:               
                        movabs  rdi, 881639520              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2641:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2642:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2643:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2644:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2645:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2646:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2647:               
                        movabs  rdi, 881639616              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2648:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2649:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2650:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2651:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2652:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2653:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2654:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2655:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2656:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2657:               
                        movabs  rdi, 881639744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2658:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2659:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2660:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2661:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2662:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2663:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2664:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2665:               
                        movabs  rdi, 881639904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2666:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2667:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2668:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2669:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2670:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2671:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2672:               
                        movabs  rdi, 881640032              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2673:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2674:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2675:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2676:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2677:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2678:               
                        movabs  rdi, 881640096              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2679:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2680:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2681:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2682:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2683:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2684:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2685:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2686:               
                        movabs  rdi, 881640256              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2687:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2688:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2689:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2690:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2691:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2692:               
                        movabs  rdi, 881640320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2693:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2694:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2695:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2696:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2697:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2698:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2699:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2700:               
                        movabs  rdi, 881640480              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2701:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2702:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2703:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2704:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2705:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2706:               
                        movabs  rdi, 881640544              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2707:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2708:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2709:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2710:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2711:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2712:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2713:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2714:               
                        movabs  rdi, 881640704              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2715:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2716:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2717:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2718:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2719:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2720:               
                        movabs  rdi, 881615856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2721:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2722:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2723:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2724:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2725:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2726:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2727:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2728:               
                        movabs  rdi, 881616016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2729:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2730:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2731:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2732:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2733:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2734:               
                        movabs  rdi, 881616080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2735:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2736:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2737:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2738:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2739:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2740:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2741:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2742:               
                        movabs  rdi, 881616240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2743:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2744:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2745:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2746:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2747:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2748:               
                        movabs  rdi, 881616304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2749:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2750:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2751:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2752:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2753:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2754:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2755:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2756:               
                        movabs  rdi, 881616464              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2757:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2758:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2759:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2760:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2761:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2762:               
                        movabs  rdi, 881616528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2763:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2764:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2765:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2766:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2767:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2768:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2769:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2770:               
                        movabs  rdi, 881616688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2771:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2772:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2773:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2774:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2775:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2776:               
                        movabs  rdi, 881616752              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2777:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2778:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2779:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2780:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2781:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2782:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2783:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2784:               
                        movabs  rdi, 881616912              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2785:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2786:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2787:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2788:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2789:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2790:               
                        movabs  rdi, 881616976              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2791:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2792:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2793:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2794:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2795:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2796:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2797:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2798:               
                        movabs  rdi, 881617136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2799:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2800:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2801:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2802:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2803:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2804:               
                        movabs  rdi, 881617200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2805:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2806:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2807:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2808:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2809:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2810:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2811:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2812:               
                        movabs  rdi, 881617360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2813:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2814:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2815:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2816:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2817:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2818:               
                        movabs  rdi, 881617424              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2819:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2820:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2821:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2822:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2823:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2824:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2825:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2826:               
                        movabs  rdi, 881617584              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2827:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2828:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2829:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2830:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2831:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2832:               
                        movabs  rdi, 881617648              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2833:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2834:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2835:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2836:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2837:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2838:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2839:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2840:               
                        movabs  rdi, 881617808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2841:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2842:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2843:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2844:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2845:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2846:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2847:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2848:               
                        movabs  rdi, 881647008              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2849:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2850:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2851:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2852:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2853:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2854:               
                        movabs  rdi, 881647072              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2855:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2856:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2857:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2858:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2859:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2860:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2861:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2862:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc2863:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2864:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2865:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2866:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2867:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2868:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2869:               
                        movabs  rdi, 881647296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2870:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2871:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2872:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2873:               
                        movabs  rdi, 881647392              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2874:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2875:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2876:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2877:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2878:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2879:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2880:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc2881:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2882:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2883:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2884:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2885:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2886:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2887:               
                        movabs  rdi, 881647584              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2888:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2889:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2890:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2891:               
                        movabs  rdi, 881647680              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2892:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2893:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2894:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2895:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2896:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2897:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2898:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc2899:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2900:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2901:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2902:               
                        movabs  rdi, 881647840              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2903:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2904:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2905:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2906:               
                        movabs  rdi, 881647904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2907:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc2908:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2909:               
                        movabs  rdi, 881647968              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2910:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2911:               
                        movabs  rdi, 881648000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2912:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2913:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2914:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2915:               
                        movabs  rdi, 881648096              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2916:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2917:               
                        movabs  rdi, 881648128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2918:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc2919:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2920:               
                        movabs  rdi, 881648192              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2921:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2922:               
                        movabs  rdi, 881648224              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2923:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc2924:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2925:               
                        movabs  rdi, 881648288              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2926:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2927:               
                        movabs  rdi, 881648320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2928:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc2929:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2930:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2931:               
                        movabs  rdi, 881648416              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2932:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2933:               
                        movabs  rdi, 881648448              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2934:               
                        movabs  rdi, 5                      
                        call    scrip_rt_push_int@PLT       
.Lpc2935:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2936:               
                        movabs  rdi, 881648512              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2937:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2938:               
                        movabs  rdi, 881648544              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2939:               
                        movabs  rdi, 6                      
                        call    scrip_rt_push_int@PLT       
.Lpc2940:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2941:               
                        movabs  rdi, 881648608              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2942:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2943:               
                        movabs  rdi, 881648640              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2944:               
                        movabs  rdi, 7                      
                        call    scrip_rt_push_int@PLT       
.Lpc2945:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2946:               
                        movabs  rdi, 881648704              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2947:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2948:               
                        movabs  rdi, 881648736              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2949:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2950:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2951:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2952:               
                        movabs  rdi, 881648800              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2953:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2954:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2955:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2956:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2957:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2958:               
                        movabs  rdi, 881648928              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc2959:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2960:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2961:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2962:               
                        movabs  rdi, 881648992              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2963:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2964:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2965:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2966:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2967:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2968:               
                        movabs  rdi, 881649088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2969:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc2970:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2971:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2972:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2973:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2974:               
                        movabs  rdi, 881649184              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2975:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2976:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2977:               
                        movabs  rdi, 881649280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2978:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2979:               
                        movabs  rdi, 881649344              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2980:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2981:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2982:               
                        movabs  rdi, 881649440              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2983:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc2984:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2985:               
                        movabs  rdi, 881649504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2986:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc2987:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2988:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2989:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2990:               
                        movabs  rdi, 881649536              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc2991:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2992:               
                        movabs  rdi, 881649568              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2993:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2994:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc2995:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2996:               
                        movabs  rdi, 881649632              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc2997:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2998:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc2999:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3000:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3001:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3002:               
                        movabs  rdi, 881649760              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3003:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3004:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3005:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3006:               
                        movabs  rdi, 881649824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3007:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3008:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3009:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3010:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3011:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3012:               
                        movabs  rdi, 881649920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3013:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3014:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3015:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3016:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3017:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3018:               
                        movabs  rdi, 881650016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3019:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3020:               
                        movabs  rdi, 881650080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3021:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3022:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3023:               
                        movabs  rdi, 881650176              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3024:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc3025:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3026:               
                        movabs  rdi, 881650240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3027:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3028:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3029:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3030:               
                        movabs  rdi, 881650272              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3031:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3032:               
                        movabs  rdi, 881650304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3033:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3034:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3035:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3036:               
                        movabs  rdi, 881650368              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3037:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3038:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3039:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3040:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3041:               
                        movabs  rdi, 881650464              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3042:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3043:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3044:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3045:               
                        movabs  rdi, 881650528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3046:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3047:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3048:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3049:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3050:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3051:               
                        movabs  rdi, 881650656              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3052:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3053:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3054:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3055:               
                        movabs  rdi, 881650720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3056:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3057:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3058:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3059:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3060:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3061:               
                        movabs  rdi, 881650880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3062:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3063:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3064:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3065:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3066:               
                        movabs  rdi, 881653040              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3067:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3068:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3069:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3070:               
                        movabs  rdi, 881653104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3071:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3072:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3073:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3074:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3075:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3076:               
                        movabs  rdi, 881653200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3077:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3078:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3079:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3080:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3081:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3082:               
                        movabs  rdi, 881653296              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3083:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3084:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3085:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3086:               
                        movabs  rdi, 881653360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3087:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3088:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3089:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3090:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3091:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3092:               
                        movabs  rdi, 881653488              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3093:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3094:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3095:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3096:               
                        movabs  rdi, 881653552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3097:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc3098:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3099:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3100:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3101:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3102:               
                        movabs  rdi, 881653648              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3103:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc3104:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3105:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3106:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3107:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3108:               
                        movabs  rdi, 881653744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3109:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3110:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3111:               
                        movabs  rdi, 881653840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3112:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc3113:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3114:               
                        movabs  rdi, 881653904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3115:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3116:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3117:               
                        movabs  rdi, 881653936              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3118:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3119:               
                        movabs  rdi, 881653968              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3120:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3121:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3122:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3123:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3124:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3125:               
                        movabs  rdi, 881654128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3126:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3127:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3128:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3129:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3130:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3131:               
                        movabs  rdi, 256                    
                        call    scrip_rt_push_int@PLT       
.Lpc3132:               
                        movabs  rdi, 881654256              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3133:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3134:               
                        movabs  rdi, 881654288              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3135:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3136:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3137:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3138:               
                        movabs  rdi, 881654352              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3139:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc3140:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3141:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3142:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3143:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3144:               
                        movabs  rdi, 881654448              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3145:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc3146:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3147:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3148:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3149:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3150:               
                        movabs  rdi, 881654544              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3151:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3152:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3153:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3154:               
                        movabs  rdi, 881654608              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3155:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3156:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3157:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3158:               
                        movabs  rdi, 881654672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3159:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3160:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3161:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3162:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3163:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3164:               
                        movabs  rdi, 881654800              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3165:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3166:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3167:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3168:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3169:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3170:               
                        movabs  rdi, 881654960              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3171:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3172:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3173:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3174:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3175:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3176:               
                        movabs  rdi, 881655088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3177:               
                        movabs  rdi, 881655120              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3178:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3179:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3180:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3181:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3182:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3183:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3184:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3185:               
                        movabs  rdi, 881655248              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3186:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3187:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3188:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3189:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3190:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3191:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3192:               
                        movabs  rdi, 881655344              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3193:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3194:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3195:               
                        movabs  rdi, 881655440              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3196:               
                        movabs  rdi, 881655472              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3197:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3198:               
                        movabs  rdi, 881655536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3199:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3200:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3201:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3202:               
                        movabs  rdi, 881655568              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3203:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3204:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3205:               
                        movabs  rdi, 881655600              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3206:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3207:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3208:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3209:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3210:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3211:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3212:               
                        movabs  rdi, 881655696              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3213:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3214:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3215:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3216:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3217:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3218:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3219:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3220:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3221:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3222:               
                        movabs  rdi, 881655824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3223:               
                        movabs  rdi, 881655856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3224:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3225:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3226:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3227:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3228:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3229:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3230:               
                        movabs  rdi, 881656016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3231:               
                        movabs  rdi, 881656048              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3232:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3233:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3234:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3235:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3236:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3237:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3238:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3239:               
                        movabs  rdi, 881656176              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3240:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3241:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3242:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3243:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3244:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3245:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3246:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3247:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3248:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3249:               
                        movabs  rdi, 881656304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3250:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3251:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3252:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3253:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3254:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3255:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3256:               
                        movabs  rdi, 881656400              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3257:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3258:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3259:               
                        movabs  rdi, 881656496              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3260:               
                        movabs  rdi, 881656528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3261:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3262:               
                        movabs  rdi, 881656592              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3263:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3264:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3265:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3266:               
                        movabs  rdi, 881656624              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3267:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3268:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3269:               
                        movabs  rdi, 881656688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3270:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3271:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3272:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3273:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3274:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3275:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3276:               
                        movabs  rdi, 881656784              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3277:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3278:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3279:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3280:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3281:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3282:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3283:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3284:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3285:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3286:               
                        movabs  rdi, 881656912              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3287:               
                        movabs  rdi, 881656944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3288:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3289:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3290:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3291:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3292:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3293:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3294:               
                        movabs  rdi, 881657104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3295:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3296:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3297:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3298:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3299:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3300:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3301:               
                        movabs  rdi, 881657200              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3302:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3303:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3304:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3305:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3306:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3307:               
                        movabs  rdi, 881657328              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3308:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3309:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3310:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3311:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3312:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3313:               
                        movabs  rdi, 881657392              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3314:               
                        movabs  rdi, 881657424              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3315:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3316:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3317:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3318:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3319:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3320:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3321:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3322:               
                        movabs  rdi, 881657552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3323:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3324:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3325:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3326:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3327:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3328:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3329:               
                        movabs  rdi, 881657648              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3330:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3331:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3332:               
                        movabs  rdi, 881657744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3333:               
                        movabs  rdi, 881657776              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3334:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3335:               
                        movabs  rdi, 881657840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3336:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3337:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3338:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3339:               
                        movabs  rdi, 881657872              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3340:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3341:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3342:               
                        movabs  rdi, 881657904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3343:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3344:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3345:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3346:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3347:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3348:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3349:               
                        movabs  rdi, 881658000              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3350:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3351:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3352:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3353:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3354:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3355:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3356:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3357:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3358:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3359:               
                        movabs  rdi, 881658128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3360:               
                        movabs  rdi, 881658160              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3361:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3362:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3363:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3364:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3365:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3366:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3367:               
                        movabs  rdi, 881658320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3368:               
                        movabs  rdi, 881658352              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3369:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3370:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3371:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3372:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3373:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3374:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3375:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3376:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc3377:               
                        movabs  rdi, 881658480              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3378:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3379:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3380:               
                        movabs  rdi, 881658576              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3381:               
                        movabs  rdi, 881658608              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3382:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3383:               
                        movabs  rdi, 881658672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3384:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3385:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3386:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3387:               
                        movabs  rdi, 881658704              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3388:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3389:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3390:               
                        movabs  rdi, 881658736              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3391:               
                        movabs  rdi, 881658768              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3392:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3393:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3394:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3395:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3396:               
                        movabs  rdi, 881658864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3397:               
                        movabs  rdi, 881658896              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3398:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3399:               
                        movabs  rdi, 881658960              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3400:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3401:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3402:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3403:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3404:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3405:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3406:               
                        movabs  rdi, 881659088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3407:               
                        movabs  rdi, 881659120              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3408:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3409:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3410:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3411:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3412:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3413:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3414:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3415:               
                        movabs  rdi, 881659248              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3416:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3417:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3418:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3419:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3420:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3421:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3422:               
                        movabs  rdi, 881659344              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3423:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3424:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3425:               
                        movabs  rdi, 881659440              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3426:               
                        movabs  rdi, 881659472              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3427:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3428:               
                        movabs  rdi, 881659536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3429:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3430:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc3431:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3432:               
                        movabs  rdi, 881659568              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3433:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3434:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3435:               
                        movabs  rdi, 881659600              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3436:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3437:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3438:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3439:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3440:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3441:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3442:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3443:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3444:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3445:               
                        movabs  rdi, 881659728              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3446:               
                        movabs  rdi, 881659760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3447:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3448:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3449:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3450:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3451:               
                        movabs  rdi, 881659856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3452:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3453:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3454:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3455:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3456:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3457:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3458:               
                        movabs  rdi, 881659952              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3459:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3460:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3461:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3462:               
                        movabs  rdi, 881660016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3463:               
                        movabs  rdi, 881660048              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3464:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3465:               
                        movabs  rdi, 881660112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3466:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3467:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3468:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3469:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3470:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3471:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3472:               
                        movabs  rdi, 881660240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3473:               
                        movabs  rdi, 881660272              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3474:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3475:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3476:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3477:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3478:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3479:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3480:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3481:               
                        movabs  rdi, 881660400              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3482:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3483:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3484:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3485:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3486:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3487:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3488:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3489:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3490:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3491:               
                        movabs  rdi, 881660528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3492:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3493:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3494:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3495:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3496:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3497:               
                        movabs  rdi, 881660624              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3498:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3499:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3500:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3501:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3502:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3503:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3504:               
                        movabs  rdi, 881660720              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3505:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3506:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3507:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3508:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3509:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3510:               
                        movabs  rdi, 881660848              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3511:               
                        movabs  rdi, 881660880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3512:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3513:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3514:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3515:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3516:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3517:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3518:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3519:               
                        movabs  rdi, 881661008              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3520:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3521:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3522:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3523:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3524:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3525:               
                        movabs  rdi, 881661104              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3526:               
                        movabs  rdi, 881661136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3527:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3528:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3529:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3530:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3531:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3532:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3533:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3534:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3535:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3536:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3537:               
                        movabs  rdi, 881661264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3538:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3539:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3540:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3541:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3542:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3543:               
                        movabs  rdi, 881661360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3544:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3545:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3546:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3547:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3548:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3549:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3550:               
                        movabs  rdi, 881661456              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3551:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3552:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3553:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3554:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3555:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3556:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3557:               
                        movabs  rdi, 881661584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3558:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3559:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3560:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3561:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3562:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3563:               
                        movabs  rdi, 881661760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3564:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3565:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3566:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3567:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3568:               
                        movabs  rdi, 881661824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3569:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3570:               
                        movabs  rdi, 1024                   
                        call    scrip_rt_push_int@PLT       
.Lpc3571:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3572:               
                        movabs  rdi, 881661888              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3573:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3574:               
                        movabs  rdi, 881661920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3575:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc3576:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3577:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3578:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3579:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3580:               
                        movabs  rdi, 881661984              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3581:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3582:               
                        movabs  rdi, 881662048              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3583:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3584:               
                        movabs  rdi, 881662080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3585:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3586:               
                        movabs  rdi, 881662144              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3587:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3588:               
                        movabs  rdi, 881662176              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3589:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3590:               
                        movabs  rdi, 881662240              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3591:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3592:               
                        movabs  rdi, 881662272              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3593:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3594:               
                        movabs  rdi, 881662336              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3595:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3596:               
                        movabs  rdi, 881662368              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3597:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc3598:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3599:               
                        movabs  rdi, 881662432              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3600:               
                        movabs  rdi, 881662464              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3601:               
                        movabs  rdi, 881662496              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3602:               
                        movabs  rdi, 881662528              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3603:               
                        movabs  rdi, 881662560              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3604:               
                        movabs  rdi, 881662592              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3605:               
                        movabs  rdi, 881662624              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3606:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3607:               
                        movabs  rdi, 881662688              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3608:               
                        movabs  rdi, 881662720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3609:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3610:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3611:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3612:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3613:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3614:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3615:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3616:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3617:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3618:               
                        movabs  rdi, 881662752              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3619:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3620:               
                        movabs  rdi, 881662784              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3621:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3622:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3623:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3624:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3625:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3626:               
                        movabs  rdi, 881662912              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3627:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3628:               
                        movabs  rdi, 881662976              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3629:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3630:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3631:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3632:               
                        movabs  rdi, 881663104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3633:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3634:               
                        movabs  rdi, 881663168              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3635:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3636:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3637:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3638:               
                        movabs  rdi, 881663296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3639:               
                        movabs  rdi, 881663328              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3640:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3641:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3642:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3643:               
                        movabs  rdi, 881663456              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3644:               
                        movabs  rdi, 881663488              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3645:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3646:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3647:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3648:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc3649:               
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3650:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3651:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc3652:               
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3653:               
# -- BB box scaffold pc=3653 op=SM_PAT_CAT --
# proc .bb_box_3653
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3654:               
# -- BB box scaffold pc=3654 op=SM_PAT_CAT --
# proc .bb_box_3654
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3655:               
                        movabs  rdi, 881663648              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3656:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc3657:               
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3658:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3659:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3660:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3661:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3662:               
                        movabs  rdi, 881663808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3663:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3664:               
                        movabs  rdi, 881663872              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3665:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3666:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3667:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3668:               
                        movabs  rdi, 881664000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3669:               
                        movabs  rdi, 881664032              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3670:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3671:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3672:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3673:               
                        movabs  rdi, 881664160              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3674:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3675:               
                        movabs  rdi, 881664224              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3676:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3677:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3678:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3679:               
                        movabs  rdi, 881664352              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3680:               
                        movabs  rdi, 881664384              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3681:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3682:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3683:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3684:               
                        movabs  rdi, 881664512              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3685:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3686:               
                        movabs  rdi, 881664576              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3687:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3688:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3689:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3690:               
                        movabs  rdi, 881664704              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3691:               
                        movabs  rdi, 881664736              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3692:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3693:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3694:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3695:               
                        movabs  rdi, 881664864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3696:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3697:               
                        movabs  rdi, 881664928              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3698:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3699:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3700:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3701:               
                        movabs  rdi, 881665056              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3702:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3703:               
                        movabs  rdi, 881665120              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3704:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3705:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3706:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3707:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3708:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3709:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3710:               
                        movabs  rdi, 881665248              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3711:               
                        movabs  rdi, 881665280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3712:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3713:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3714:               
                        movabs  rdi, 881665344              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3715:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3716:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3717:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3718:               
                        movabs  rdi, 881665408              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3719:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3720:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3721:               
                        movabs  rdi, 881665440              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3722:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3723:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3724:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3725:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3726:               
                        movabs  rdi, 881665568              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3727:               
                        movabs  rdi, 881665600              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3728:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3729:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3730:               
                        movabs  rdi, 881665664              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3731:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3732:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3733:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3734:               
                        movabs  rdi, 881665728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3735:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3736:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3737:               
                        movabs  rdi, 881665760              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3738:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3739:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3740:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3741:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3742:               
                        movabs  rdi, 881665888              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3743:               
                        movabs  rdi, 881665920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3744:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3745:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3746:               
                        movabs  rdi, 881665984              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3747:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc3748:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3749:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3750:               
                        movabs  rdi, 881666048              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3751:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3752:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3753:               
                        movabs  rdi, 881666080              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3754:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3755:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3756:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3757:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3758:               
                        movabs  rdi, 881666208              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3759:               
                        movabs  rdi, 881666240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3760:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3761:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3762:               
                        movabs  rdi, 881666304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3763:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc3764:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3765:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3766:               
                        movabs  rdi, 881666368              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3767:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3768:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3769:               
                        movabs  rdi, 881666400              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3770:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3771:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3772:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3773:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3774:               
                        movabs  rdi, 881666528              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3775:               
                        movabs  rdi, 881666560              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3776:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3777:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3778:               
                        movabs  rdi, 881666624              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3779:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc3780:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3781:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3782:               
                        movabs  rdi, 881666688              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3783:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3784:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3785:               
                        movabs  rdi, 881666720              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3786:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3787:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3788:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3789:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3790:               
                        movabs  rdi, 881666848              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3791:               
                        movabs  rdi, 881666880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3792:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3793:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3794:               
                        movabs  rdi, 881666944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3795:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc3796:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3797:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3798:               
                        movabs  rdi, 881667008              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3799:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3800:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3801:               
                        movabs  rdi, 881667040              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3802:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3803:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3804:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3805:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3806:               
                        movabs  rdi, 881667168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3807:               
                        movabs  rdi, 881667200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3808:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3809:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3810:               
                        movabs  rdi, 881667264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3811:               
                        movabs  rdi, 881667296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3812:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3813:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3814:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3815:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3816:               
                        movabs  rdi, 881667392              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3817:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3818:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3819:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3820:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3821:               
                        movabs  rdi, 881667488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3822:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3823:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3824:               
                        movabs  rdi, 881667552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3825:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3826:               
                        movabs  rdi, 881667616              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3827:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3828:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3829:               
                        movabs  rdi, 881667648              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3830:               
                        movabs  rdi, 881667680              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3831:               
                        movabs  rdi, 881667712              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3832:               
                        movabs  rdi, 881667744              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc3833:               
                        movabs  rdi, 881667776              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3834:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3835:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3836:               
                        movabs  rdi, 881667840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3837:               
                        movabs  rdi, 881667872              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3838:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3839:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3840:               
                        movabs  rdi, 881667936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3841:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3842:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3843:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3844:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc3845:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3846:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3847:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3848:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3849:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3850:               
                        movabs  rdi, 881668032              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3851:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3852:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3853:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3854:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3855:               
                        movabs  rdi, 881668128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3856:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3857:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3858:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3859:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3860:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3861:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3862:               
                        movabs  rdi, 881668256              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3863:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3864:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3865:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3866:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3867:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3868:               
                        movabs  rdi, 881668320              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3869:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3870:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3871:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3872:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3873:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3874:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3875:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3876:               
                        movabs  rdi, 881668480              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3877:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3878:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3879:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3880:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3881:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3882:               
                        movabs  rdi, 881668544              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3883:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3884:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3885:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3886:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3887:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3888:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3889:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3890:               
                        movabs  rdi, 881668704              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3891:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3892:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3893:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3894:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3895:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3896:               
                        movabs  rdi, 881668768              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3897:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3898:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3899:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3900:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3901:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3902:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3903:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3904:               
                        movabs  rdi, 881668928              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3905:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3906:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3907:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3908:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3909:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3910:               
                        movabs  rdi, 881668992              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3911:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3912:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3913:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3914:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3915:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3916:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3917:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3918:               
                        movabs  rdi, 881669152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3919:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3920:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3921:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3922:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3923:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3924:               
                        movabs  rdi, 881669216              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3925:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3926:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3927:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3928:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3929:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3930:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3931:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3932:               
                        movabs  rdi, 881669376              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3933:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3934:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3935:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3936:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3937:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3938:               
                        movabs  rdi, 881669440              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3939:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3940:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3941:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3942:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3943:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3944:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3945:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3946:               
                        movabs  rdi, 881669600              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3947:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3948:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3949:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3950:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3951:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3952:               
                        movabs  rdi, 881669664              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3953:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3954:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3955:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3956:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3957:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3958:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3959:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3960:               
                        movabs  rdi, 881669824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3961:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3962:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3963:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3964:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3965:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3966:               
                        movabs  rdi, 881669888              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3967:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3968:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3969:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3970:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3971:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3972:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3973:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3974:               
                        movabs  rdi, 881670048              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3975:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3976:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3977:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3978:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3979:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3980:               
                        movabs  rdi, 881670112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3981:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3982:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3983:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3984:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3985:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3986:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3987:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3988:               
                        movabs  rdi, 881670272              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3989:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc3990:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3991:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3992:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3993:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3994:               
                        movabs  rdi, 881670336              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc3995:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc3996:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3997:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc3998:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc3999:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4000:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4001:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4002:               
                        movabs  rdi, 881670496              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4003:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4004:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4005:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4006:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4007:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4008:               
                        movabs  rdi, 881670560              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4009:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4010:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4011:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4012:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4013:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4014:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4015:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4016:               
                        movabs  rdi, 881670720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4017:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4018:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4019:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4020:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4021:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4022:               
                        movabs  rdi, 881670784              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4023:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4024:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4025:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4026:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4027:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4028:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4029:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4030:               
                        movabs  rdi, 881670944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4031:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4032:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4033:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4034:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4035:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4036:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4037:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4038:               
                        movabs  rdi, 881671104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4039:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4040:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4041:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4042:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4043:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4044:               
                        movabs  rdi, 881671168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4045:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4046:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4047:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4048:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4049:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4050:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4051:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4052:               
                        movabs  rdi, 881671328              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4053:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4054:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4055:               
                        movabs  rdi, 881671392              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4056:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4057:               
                        movabs  rdi, 881671456              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4058:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4059:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4060:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4061:               
                        movabs  rdi, 881671488              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4062:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4063:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4064:               
                        movabs  rdi, 881671584              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4065:               
                        movabs  rdi, 881671616              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4066:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4067:               
                        movabs  rdi, 881671680              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4068:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4069:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4070:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4071:               
                        movabs  rdi, 881671712              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4072:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4073:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4074:               
                        movabs  rdi, 881671744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4075:               
                        movabs  rdi, 881671776              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4076:               
                        movabs  rdi, 881671808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4077:               
                        movabs  rdi, 881671840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4078:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4079:               
                        movabs  rdi, 881671904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4080:               
                        movabs  rdi, 881671936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4081:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4082:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4083:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4084:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4085:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4086:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4087:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4088:               
                        movabs  rdi, 881672032              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4089:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4090:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4091:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4092:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4093:               
                        movabs  rdi, 881672128              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4094:               
                        movabs  rdi, 881672160              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4095:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4096:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4097:               
                        movabs  rdi, 881672224              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4098:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc4099:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4100:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4101:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4102:               
                        movabs  rdi, 881672288              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4103:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4104:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4105:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4106:               
                        movabs  rdi, 881672320              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4107:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4108:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4109:               
                        movabs  rdi, 881672416              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4110:               
                        movabs  rdi, 881672448              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4111:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4112:               
                        movabs  rdi, 881672512              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4113:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4114:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4115:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4116:               
                        movabs  rdi, 881672544              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4117:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4118:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4119:               
                        movabs  rdi, 881672608              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4120:               
                        movabs  rdi, 881672640              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4121:               
                        movabs  rdi, 881672672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4122:               
                        movabs  rdi, 881672704              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4123:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4124:               
                        movabs  rdi, 881672768              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4125:               
                        movabs  rdi, 881672800              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4126:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4127:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4128:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc4129:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4130:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4131:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4132:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4133:               
                        movabs  rdi, 881672896              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4134:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4135:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4136:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4137:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4138:               
                        movabs  rdi, 881672992              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4139:               
                        movabs  rdi, 881673024              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4140:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4141:               
                        movabs  rdi, 881673056              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4142:               
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4143:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4144:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4145:               
                        movabs  rdi, 881673152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4146:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4147:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4148:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4149:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4150:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4151:               
                        movabs  rdi, 881673216              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4152:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4153:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4154:               
                        movabs  rdi, 881673280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4155:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4156:               
                        movabs  rdi, 881673344              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4157:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4158:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4159:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4160:               
                        movabs  rdi, 881673376              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4161:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4162:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4163:               
                        movabs  rdi, 881644944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4164:               
                        movabs  rdi, 881644976              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4165:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4166:               
                        movabs  rdi, 881645040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4167:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4168:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4169:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4170:               
                        movabs  rdi, 881645072              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4171:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4172:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4173:               
                        movabs  rdi, 881645104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4174:               
                        movabs  rdi, 881645136              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4175:               
                        movabs  rdi, 881645168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4176:               
                        movabs  rdi, 881645200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4177:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4178:               
                        movabs  rdi, 881645264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4179:               
                        movabs  rdi, 881645296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4180:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4181:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4182:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc4183:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4184:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4185:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4186:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4187:               
                        movabs  rdi, 881645392              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4188:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4189:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4190:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4191:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4192:               
                        movabs  rdi, 881645488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4193:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4194:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4195:               
                        movabs  rdi, 881645552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4196:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4197:               
                        movabs  rdi, 881645616              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4198:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4199:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4200:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4201:               
                        movabs  rdi, 881645648              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4202:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4203:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4204:               
                        movabs  rdi, 881645744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4205:               
                        movabs  rdi, 881645776              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4206:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4207:               
                        movabs  rdi, 881645840              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4208:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4209:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4210:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4211:               
                        movabs  rdi, 881645872              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4212:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4213:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4214:               
                        movabs  rdi, 881645904              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4215:               
                        movabs  rdi, 881645936              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4216:               
                        movabs  rdi, 881645968              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4217:               
                        movabs  rdi, 881646000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4218:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4219:               
                        movabs  rdi, 881646064              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4220:               
                        movabs  rdi, 881646096              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4221:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4222:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4223:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4224:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4225:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4226:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4227:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4228:               
                        movabs  rdi, 881646192              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4229:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4230:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4231:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4232:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4233:               
                        movabs  rdi, 881646288              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4234:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4235:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4236:               
                        movabs  rdi, 881646352              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4237:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4238:               
                        movabs  rdi, 881646416              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4239:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4240:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4241:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4242:               
                        movabs  rdi, 881646448              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4243:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4244:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4245:               
                        movabs  rdi, 881646544              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4246:               
                        movabs  rdi, 881646576              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4247:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4248:               
                        movabs  rdi, 881646640              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4249:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4250:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4251:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4252:               
                        movabs  rdi, 881646672              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4253:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4254:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4255:               
                        movabs  rdi, 881646704              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4256:               
                        movabs  rdi, 881646736              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4257:               
                        movabs  rdi, 881646768              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4258:               
                        movabs  rdi, 881646800              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4259:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4260:               
                        movabs  rdi, 881646864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4261:               
                        movabs  rdi, 881646896              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4262:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4263:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4264:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4265:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4266:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4267:               
                        movabs  rdi, 881677584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4268:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4269:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4270:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4271:               
                        movabs  rdi, 881677616              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4272:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4273:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4274:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4275:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4276:               
                        movabs  rdi, 881677712              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4277:               
                        movabs  rdi, 881677744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4278:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4279:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4280:               
                        movabs  rdi, 881677808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4281:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4282:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4283:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4284:               
                        movabs  rdi, 881677872              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4285:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4286:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4287:               
                        movabs  rdi, 881677904              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4288:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4289:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4290:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4291:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4292:               
                        movabs  rdi, 881678000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4293:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4294:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4295:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4296:               
                        movabs  rdi, 881678096              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4297:               
                        movabs  rdi, 881678128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4298:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4299:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4300:               
                        movabs  rdi, 881678192              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4301:               
                        movabs  rdi, 881678224              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4302:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4303:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4304:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4305:               
                        mov     edi, 18                     # SM_SUB
                        call    scrip_rt_arith@PLT          
.Lpc4306:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4307:               
                        movabs  rdi, 881678320              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4308:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4309:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4310:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4311:               
                        movabs  rdi, 881678352              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4312:               
                        mov     edi, 62                     # SM_RETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4313:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4314:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4315:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4316:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4317:               
                        movabs  rdi, 881678448              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4318:               
                        movabs  rdi, 881678480              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4319:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4320:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4321:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4322:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4323:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4324:               
                        movabs  rdi, 881678640              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4325:               
                        movabs  rdi, 881678672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4326:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4327:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4328:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4329:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4330:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4331:               
                        movabs  rdi, 881678736              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4332:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4333:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4334:               
                        movabs  rdi, 881678832              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4335:               
                        movabs  rdi, 881678864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4336:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4337:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4338:               
                        movabs  rdi, 881678960              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4339:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4340:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4341:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4342:               
                        movabs  rdi, 881678992              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4343:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4344:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4345:               
                        movabs  rdi, 881679024              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4346:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4347:               
                        movabs  rdi, 881679088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4348:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4349:               
                        movabs  rdi, 881679152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4350:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4351:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4352:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4353:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4354:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4355:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4356:               
                        movabs  rdi, 881679280              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4357:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4358:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4359:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4360:               
                        movabs  rdi, 881679344              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4361:               
                        movabs  rdi, 881679376              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4362:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4363:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4364:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4365:               
                        movabs  rdi, 881679504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4366:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4367:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4368:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4369:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4370:               
                        movabs  rdi, 881679568              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4371:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4372:               
                        movabs  rdi, 881679632              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4373:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4374:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4375:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4376:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4377:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4378:               
                        movabs  rdi, 881679728              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4379:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4380:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4381:               
                        movabs  rdi, 881679824              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4382:               
                        movabs  rdi, 881679856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4383:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4384:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4385:               
                        movabs  rdi, 881679952              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4386:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4387:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4388:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4389:               
                        movabs  rdi, 881679984              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4390:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4391:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4392:               
                        movabs  rdi, 881680016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4393:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4394:               
                        movabs  rdi, 881680080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4395:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4396:               
                        movabs  rdi, 881680144              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4397:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4398:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4399:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4400:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4401:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4402:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4403:               
                        movabs  rdi, 881680304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4404:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4405:               
                        movabs  rdi, 881680368              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4406:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4407:               
                        movabs  rdi, 881680432              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4408:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4409:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4410:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4411:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4412:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4413:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4414:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4415:               
                        movabs  rdi, 881680560              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4416:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4417:               
                        movabs  rdi, 881680624              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4418:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4419:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4420:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4421:               
                        movabs  rdi, 881680752              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4422:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4423:               
                        movabs  rdi, 881680816              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4424:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4425:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4426:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4427:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4428:               
                        movabs  rdi, 881680880              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4429:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4430:               
                        movabs  rdi, 881680944              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4431:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4432:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4433:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4434:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4435:               
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4436:               
                        movabs  rdi, 881681072              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4437:               
                        movabs  rdi, 881681104              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4438:               
                        movabs  rdi, 881681136              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4439:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4440:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4441:               
# -- BB box scaffold pc=4441 op=SM_PAT_SPAN --
# proc .bb_box_4441
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4442:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4443:               
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4444:               
# -- BB box scaffold pc=4444 op=SM_PAT_CAT --
# proc .bb_box_4444
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4445:               
# -- BB box scaffold pc=4445 op=SM_PAT_CAT --
# proc .bb_box_4445
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4446:               
                        movabs  rdi, 881681168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4447:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4448:               
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4449:               
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4450:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4451:               
                        movabs  rdi, 881681200              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4452:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4453:               
                        movabs  rdi, 881681264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4454:               
                        movabs  rdi, 881681296              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4455:               
                        movabs  rdi, 881681328              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4456:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4457:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4458:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4459:               
                        movabs  rdi, 881681360              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4460:               
                        mov     edi, 64                     # SM_FRETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4461:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4462:               
                        movabs  rdi, 881681392              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4463:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4464:               
                        movabs  rdi, 881681456              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4465:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4466:               
                        movabs  rdi, 881681488              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4467:               
                        mov     edi, 60                     # SM_FRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4468:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4469:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4470:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4471:               
                        movabs  rdi, 881681584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4472:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4473:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4474:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4475:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4476:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4477:               
                        movabs  rdi, 881681744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4478:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4479:               
                        movabs  rdi, 881681808              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4480:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4481:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4482:               
                        movabs  rdi, 881681840              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4483:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4484:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4485:               
                        movabs  rdi, 881640832              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4486:               
                        movabs  rdi, 881640864              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4487:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4488:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4489:               
                        movabs  rdi, 881640960              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4490:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4491:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4492:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4493:               
                        movabs  rdi, 881640992              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4494:               
                        mov     edi, 63                     # SM_RETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4495:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4496:               
                        movabs  rdi, 881641024              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4497:               
                        movabs  rdi, 881641056              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4498:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4499:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4500:               
                        movabs  rdi, 881641152              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4501:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4502:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4503:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4504:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4505:               
                        movabs  rdi, 881641216              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4506:               
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc4507:               
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc4508:               
                        movabs  rdi, 881641248              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4509:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4510:               
                        movabs  rdi, 881641280              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4511:               
                        movabs  rdi, 881641312              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4512:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4513:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4514:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc4515:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4516:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4517:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4518:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4519:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4520:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4521:               
                        movabs  rdi, 881641504              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4522:               
                        movabs  rdi, 881641536              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4523:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4524:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4525:               
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc4526:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4527:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4528:               
                        movabs  rdi, 881641696              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4529:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4530:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4531:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4532:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4533:               
                        movabs  rdi, 881641760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4534:               
                        movabs  rdi, 881641792              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4535:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4536:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4537:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4538:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4539:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4540:               
                        movabs  rdi, 881641952              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4541:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4542:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4543:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4544:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4545:               
                        movabs  rdi, 881642048              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4546:               
                        movabs  rdi, 881642080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4547:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4548:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4549:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4550:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4551:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4552:               
                        movabs  rdi, 881642240              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4553:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4554:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4555:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4556:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4557:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4558:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4559:               
                        movabs  rdi, 881642400              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4560:               
                        movabs  rdi, 881642432              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4561:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4562:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4563:               
                        movabs  rdi, 2                      
                        call    scrip_rt_push_int@PLT       
.Lpc4564:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4565:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4566:               
                        movabs  rdi, 881642592              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4567:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4568:               
                        movabs  rdi, 881642624              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4569:               
                        movabs  rdi, 881642656              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4570:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4571:               
                        movabs  rdi, 881642688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4572:               
                        movabs  rdi, 881642720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4573:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4574:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4575:               
                        movabs  rdi, 5                      
                        call    scrip_rt_push_int@PLT       
.Lpc4576:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4577:               
                        movabs  rdi, 881642848              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4578:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4579:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4580:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4581:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4582:               
                        movabs  rdi, 881642944              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4583:               
                        movabs  rdi, 3                      
                        call    scrip_rt_push_int@PLT       
.Lpc4584:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4585:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4586:               
                        movabs  rdi, 881643008              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4587:               
                        movabs  rdi, 881643040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4588:               
                        movabs  rdi, 38                     
                        call    scrip_rt_push_int@PLT       
.Lpc4589:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4590:               
                        movabs  rdi, 881643104              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4591:               
                        movabs  rdi, 881643136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4592:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4593:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4594:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4595:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4596:               
                        movabs  rdi, 881643168              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4597:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4598:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4599:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4600:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4601:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4602:               
                        movabs  rdi, 881643264              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4603:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4604:               
                        movabs  rdi, 881643296              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4605:               
                        movabs  rdi, 881643328              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4606:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4607:               
# -- BB box scaffold pc=4607 op=SM_PAT_SPAN --
# proc .bb_box_4607
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4608:               
                        movabs  rdi, 881643360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4609:               
# -- BB box scaffold pc=4609 op=SM_PAT_DEREF --
# proc .bb_box_4609
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4610:               
# -- BB box scaffold pc=4610 op=SM_PAT_ALT --
# proc .bb_box_4610
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4611:               
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4612:               
                        movabs  rdi, 881643392              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4613:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4614:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4615:               
                        movabs  rdi, 881643488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4616:               
                        movabs  rdi, 881643520              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4617:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4618:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4619:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4620:               
                        movabs  rdi, 881643648              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4621:               
                        movabs  rdi, 881643680              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4622:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4623:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4624:               
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4625:               
                        movabs  rdi, 881643712              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4626:               
# -- BB box scaffold pc=4626 op=SM_PAT_ANY --
# proc .bb_box_4626
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4627:               
# -- BB box scaffold pc=4627 op=SM_PAT_CAT --
# proc .bb_box_4627
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4628:               
                        movabs  rdi, 881643744              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4629:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4630:               
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4631:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4632:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4633:               
                        movabs  rdi, 881643808              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4634:               
                        movabs  rdi, 881643840              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4635:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4636:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4637:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4638:               
                        movabs  rdi, 881643936              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4639:               
                        movabs  rdi, 881643968              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4640:               
                        movabs  rdi, 881644000              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4641:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4642:               
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4643:               
                        movabs  rdi, 881644032              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4644:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4645:               
                        movabs  rdi, 881644064              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4646:               
                        movabs  rdi, 881644096              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4647:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4648:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4649:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4650:               
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4651:               
                        movabs  rdi, 881644160              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4652:               
# -- BB box scaffold pc=4652 op=SM_PAT_ANY --
# proc .bb_box_4652
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4653:               
# -- BB box scaffold pc=4653 op=SM_PAT_CAT --
# proc .bb_box_4653
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4654:               
                        movabs  rdi, 881644192              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4655:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4656:               
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4657:               
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4658:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4659:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4660:               
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4661:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4662:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4663:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4664:               
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4665:               
# -- BB box scaffold pc=4665 op=SM_PAT_CAT --
# proc .bb_box_4665
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4666:               
# -- BB box scaffold pc=4666 op=SM_PAT_CAT --
# proc .bb_box_4666
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4667:               
# -- BB box scaffold pc=4667 op=SM_PAT_CAT --
# proc .bb_box_4667
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4668:               
                        movabs  rdi, 881644288              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4669:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4670:               
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4671:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4672:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4673:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4674:               
                        movabs  rdi, 881644384              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4675:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4676:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4677:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4678:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4679:               
                        movabs  rdi, 881644480              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4680:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4681:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4682:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4683:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4684:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4685:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4686:               
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4687:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4688:               
                        mov     edi, 49                     # SM_PAT_REFNAME
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4689:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4690:               
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4691:               
# -- BB box scaffold pc=4691 op=SM_PAT_CAT --
# proc .bb_box_4691
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4692:               
# -- BB box scaffold pc=4692 op=SM_PAT_CAT --
# proc .bb_box_4692
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4693:               
# -- BB box scaffold pc=4693 op=SM_PAT_CAT --
# proc .bb_box_4693
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4694:               
                        movabs  rdi, 881644672              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4695:               
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc4696:               
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4697:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4698:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4699:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4700:               
                        movabs  rdi, 881644768              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4701:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4702:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4703:               
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4704:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4705:               
                        movabs  rdi, 881690080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4706:               
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4707:               
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc4708:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4709:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4710:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4711:               
                        movabs  rdi, 881690240              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4712:               
                        movabs  rdi, 881690272              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4713:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4714:               
                        movabs  rdi, 881690304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4715:               
                        movabs  rdi, 881690336              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4716:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4717:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4718:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4719:               
                        movabs  rdi, 881690464              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc4720:               
                        movabs  rdi, 881690496              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4721:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4722:               
                        movabs  rdi, 881690528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc4723:               
                        movabs  rdi, 881690560              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc4724:               
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4725:               
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4726:               
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4727:               
                        call    scrip_rt_pop_int@PLT        #  rc <- TOS
                        mov     edi, eax                    
                        call    scrip_rt_halt@PLT           
	# -- epilogue -------------------------------------------
	call    scrip_rt_finalize@PLT
	pop     rbp
	ret
	.size   main, .-main
	.section .note.GNU-stack,"",@progbits
