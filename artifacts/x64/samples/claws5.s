# -----------------------------------------------------------------------
# scrip --jit-emit --x64  (M-JITEM-X64 / EM-1..EM-3)
# 437 SM instructions. Links against libscrip_rt.so.
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
                        movabs  rdi, 153613680              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc3:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc4:                  
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc5:                  
                        movabs  rdi, 153613712              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc6:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc7:                  
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc8:                  
                        movabs  rdi, 153613744              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc9:                  
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc10:                 
                        movabs  rdi, 153596832              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc11:                 
                        movabs  rdi, 153596864              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc12:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc13:                 
                        movabs  rdi, 153611312              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc14:                 
                        movabs  rdi, 153596896              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc15:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc16:                 
                        movabs  rdi, 9                      
                        call    scrip_rt_push_int@PLT       
.Lpc17:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc18:                 
                        movabs  rdi, 153596960              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc19:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc20:                 
                        movabs  rdi, 153596992              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc21:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc22:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc23:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc24:                 
                        movabs  rdi, 153597056              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc25:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc26:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc27:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc28:                 
                        movabs  rdi, 153597120              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc29:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc30:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc31:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc32:                 
                        movabs  rdi, 153597184              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc33:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc34:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc35:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc36:                 
                        movabs  rdi, 153597248              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc37:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc38:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc39:                 
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc40:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc41:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc42:                 
                        movabs  rdi, 153597408              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc43:                 
                        movabs  rdi, 153597440              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc44:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc45:                 
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc46:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc47:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc48:                 
                        movabs  rdi, 153597568              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc49:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc50:                 
                        movabs  rdi, 153597632              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc51:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc52:                 
                        movabs  rdi, 153597696              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc53:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc54:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc55:                 
                        movabs  rdi, 153597760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc56:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc57:                 
                        movabs  rdi, 153597824              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc58:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc59:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc60:                 
                        movabs  rdi, 153597888              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc61:                 
                        mov     edi, 66                     # SM_NRETURN_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc62:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc63:                 
                        movabs  rdi, 153597920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc64:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc65:                 
                        movabs  rdi, 153593232              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc66:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc67:                 
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc68:                 
                        movabs  rdi, 153593296              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc69:                 
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc70:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc71:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc72:                 
                        movabs  rdi, 153593392              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc73:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc74:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc75:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc76:                 
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc77:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc78:                 
                        movabs  rdi, 153593488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc79:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc80:                 
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc81:                 
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc82:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc83:                 
                        movabs  rdi, 153593584              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc84:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc85:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc86:                 
                        movabs  rdi, 153593680              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc87:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc88:                 
                        movabs  rdi, 153593712              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc89:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc90:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc91:                 
                        movabs  rdi, 153593808              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc92:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc93:                 
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc94:                 
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc95:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc96:                 
                        movabs  rdi, 153593936              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc97:                 
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc98:                 
                        movabs  rdi, 153594000              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc99:                 
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc100:                
                        movabs  rdi, 153594032              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc101:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc102:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc103:                
                        movabs  rdi, 153594128              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc104:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc105:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc106:                
                        movabs  rdi, 153594192              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc107:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc108:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc109:                
                        movabs  rdi, 153594288              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc110:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc111:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc112:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc113:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc114:                
                        movabs  rdi, 153594416              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc115:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc116:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc117:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc118:                
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc119:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc120:                
                        movabs  rdi, 153594512              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc121:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc122:                
                        movabs  rdi, 153594576              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc123:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc124:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc125:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc126:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc127:                
                        movabs  rdi, 153594672              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc128:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc129:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc130:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc131:                
                        movabs  rdi, 153594736              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc132:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc133:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc134:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc135:                
                        movabs  rdi, 153594800              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc136:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc137:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc138:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc139:                
                        movabs  rdi, 153594864              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc140:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc141:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc142:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc143:                
                        movabs  rdi, 153594928              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc144:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc145:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc146:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc147:                
                        movabs  rdi, 153611104              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc148:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc149:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc150:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc151:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc152:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc153:                
                        movabs  rdi, 153595120              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc154:                
                        movabs  rdi, 153595152              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc155:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc156:                
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc157:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc158:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc159:                
                        movabs  rdi, 153606192              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc160:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc161:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc162:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc163:                
                        movabs  rdi, 153606288              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc164:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc165:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc166:                
                        movabs  rdi, 153606352              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc167:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc168:                
                        movabs  rdi, 153606416              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc169:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc170:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc171:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc172:                
                        movabs  rdi, 153606512              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc173:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc174:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc175:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc176:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc177:                
                        movabs  rdi, 153606608              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc178:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc179:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc180:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc181:                
                        movabs  rdi, 153606704              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc182:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc183:                
                        movabs  rdi, 153606768              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc184:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc185:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc186:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc187:                
                        movabs  rdi, 153606864              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc188:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc189:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc190:                
                        movabs  rdi, 153606960              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc191:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc192:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc193:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc194:                
                        movabs  rdi, 153607056              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc195:                
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc196:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc197:                
                        movabs  rdi, 153607088              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc198:                
                        movabs  rdi, 4                      
                        call    scrip_rt_push_int@PLT       
.Lpc199:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc200:                
                        movabs  rdi, 153607152              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc201:                
                        movabs  rdi, 153607184              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc202:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc203:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc204:                
                        movabs  rdi, 153607216              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc205:                
                        mov     edi, 59                     # SM_RETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc206:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc207:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc208:                
                        movabs  rdi, 153607312              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc209:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc210:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc211:                
                        movabs  rdi, 153607408              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc212:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc213:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc214:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc215:                
                        movabs  rdi, 153607504              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc216:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc217:                
                        mov     edi, 65                     # SM_FRETURN_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc218:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc219:                
                        movabs  rdi, 153607568              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc220:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc221:                
                        movabs  rdi, 153607632              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc222:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc223:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc224:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc225:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc226:                
                        movabs  rdi, 153607728              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc227:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc228:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc229:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc230:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc231:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc232:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc233:                
                        movabs  rdi, 153607920              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc234:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc235:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc236:                
                        movabs  rdi, 153607952              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc237:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc238:                
                        movabs  rdi, 153607984              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc239:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc240:                
                        movabs  rdi, 153608048              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc241:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc242:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc243:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc244:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc245:                
                        movabs  rdi, 153608144              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc246:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc247:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc248:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc249:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc250:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc251:                
                        movabs  rdi, 153608304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc252:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc253:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc254:                
                        movabs  rdi, 153608336              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc255:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc256:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc257:                
                        movabs  rdi, 153608400              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc258:                
                        movabs  rdi, 153608432              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc259:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc260:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc261:                
                        movabs  rdi, 153608496              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc262:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc263:                
                        movabs  rdi, 153608560              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc264:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc265:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc266:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc267:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc268:                
                        movabs  rdi, 153608656              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc269:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc270:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc271:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc272:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc273:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc274:                
                        movabs  rdi, 153608816              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc275:                
                        movabs  rdi, 153608848              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc276:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc277:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc278:                
                        movabs  rdi, 153608944              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc279:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc280:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc281:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc282:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc283:                
                        movabs  rdi, 153609040              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc284:                
                        movabs  rdi, 153609072              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc285:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc286:                
                        movabs  rdi, 153609136              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc287:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc288:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc289:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc290:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc291:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc292:                
                        movabs  rdi, 153609264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc293:                
                        movabs  rdi, 153609296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc294:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc295:                
                        movabs  rdi, 153609360              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc296:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc297:                
                        movabs  rdi, 153609424              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc298:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc299:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc300:                
                        call    scrip_rt_pop_void@PLT       #  SM_POP: discard TOS
.Lpc301:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc302:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc303:                
                        movabs  rdi, 153609552              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc304:                
                        movabs  rdi, 153597952              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc305:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc306:                
                        movabs  rdi, 153598016              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc307:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc308:                
                        movabs  rdi, 153598080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc309:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc310:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc311:                
                        mov     edi, 17                     # SM_ADD
                        call    scrip_rt_arith@PLT          
.Lpc312:                
                        movabs  rdi, 153598144              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc313:                
                        movabs  rdi, 153598176              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc314:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc315:                
                        movabs  rdi, 153598240              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc316:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc317:                
                        movabs  rdi, 153598304              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc318:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc319:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc320:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc321:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc322:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc323:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc324:                
                        movabs  rdi, 153598496              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc325:                
                        movabs  rdi, 153598528              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc326:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc327:                
                        movabs  rdi, 153598592              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc328:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc329:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc330:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc331:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc332:                
                        movabs  rdi, 1                      
                        call    scrip_rt_push_int@PLT       
.Lpc333:                
                        movabs  rdi, 153599760              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc334:                
                        movabs  rdi, 153599792              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc335:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc336:                
                        movabs  rdi, 153599856              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc337:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc338:                
                        movabs  rdi, 153599920              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc339:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc340:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc341:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc342:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc343:                
                        movabs  rdi, 153600048              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc344:                
                        movabs  rdi, 153600080              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc345:                
                        movabs  rdi, 153600112              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc346:                
                        movabs  rdi, 153600144              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc347:                
                        movabs  rdi, 153600176              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc348:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc349:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc350:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc351:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc352:                
                        movabs  rdi, 153600208              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc353:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc354:                
                        movabs  rdi, 153600240              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc355:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc356:                
                        movabs  rdi, 153600304              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc357:                
                        mov     edi, 61                     # SM_NRETURN
                        call    scrip_rt_unhandled_op@PLT   
.Lpc358:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc359:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc360:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc361:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc362:                
                        mov     edi, 32                     # SM_PAT_POS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc363:                
                        mov     edi, 53                     # SM_PAT_USERCALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc364:                
                        movabs  rdi, 153600432              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc365:                
# -- BB box scaffold pc=365 op=SM_PAT_SPAN --
# proc .bb_box_365
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc366:                
# -- BB box scaffold pc=366 op=SM_PAT_CAPTURE --
# proc .bb_box_366
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc367:                
# -- BB box scaffold pc=367 op=SM_PAT_LIT --
# proc .bb_box_367
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc368:                
                        mov     edi, 53                     # SM_PAT_USERCALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc369:                
# -- BB box scaffold pc=369 op=SM_PAT_CAT --
# proc .bb_box_369
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc370:                
# -- BB box scaffold pc=370 op=SM_PAT_CAT --
# proc .bb_box_370
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc371:                
                        movabs  rdi, 153600560              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc372:                
# -- BB box scaffold pc=372 op=SM_PAT_NOTANY --
# proc .bb_box_372
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 28                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc373:                
                        movabs  rdi, 153600592              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc374:                
# -- BB box scaffold pc=374 op=SM_PAT_BREAK --
# proc .bb_box_374
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 30                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc375:                
# -- BB box scaffold pc=375 op=SM_PAT_CAT --
# proc .bb_box_375
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc376:                
# -- BB box scaffold pc=376 op=SM_PAT_CAPTURE --
# proc .bb_box_376
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
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
                        movabs  rdi, 153600688              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc379:                
# -- BB box scaffold pc=379 op=SM_PAT_ANY --
# proc .bb_box_379
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 27                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc380:                
                        movabs  rdi, 153600720              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc381:                
                        movabs  rdi, 153600752              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc382:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc383:                
# -- BB box scaffold pc=383 op=SM_PAT_SPAN --
# proc .bb_box_383
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 29                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc384:                
# -- BB box scaffold pc=384 op=SM_PAT_CAT --
# proc .bb_box_384
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc385:                
# -- BB box scaffold pc=385 op=SM_PAT_CAPTURE --
# proc .bb_box_385
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 50                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc386:                
                        mov     edi, 53                     # SM_PAT_USERCALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc387:                
# -- BB box scaffold pc=387 op=SM_PAT_CAT --
# proc .bb_box_387
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc388:                
# -- BB box scaffold pc=388 op=SM_PAT_CAT --
# proc .bb_box_388
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc389:                
# -- BB box scaffold pc=389 op=SM_PAT_CAT --
# proc .bb_box_389
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc390:                
# -- BB box scaffold pc=390 op=SM_PAT_ALT --
# proc .bb_box_390
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 46                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc391:                
# -- BB box scaffold pc=391 op=SM_PAT_LIT --
# proc .bb_box_391
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 26                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc392:                
# -- BB box scaffold pc=392 op=SM_PAT_CAT --
# proc .bb_box_392
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc393:                
# -- BB box scaffold pc=393 op=SM_PAT_ARBNO --
# proc .bb_box_393
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 37                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc394:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc395:                
                        mov     edi, 33                     # SM_PAT_RPOS
                        call    scrip_rt_unhandled_op@PLT   
.Lpc396:                
# -- BB box scaffold pc=396 op=SM_PAT_CAT --
# proc .bb_box_396
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc397:                
# -- BB box scaffold pc=397 op=SM_PAT_CAT --
# proc .bb_box_397
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc398:                
# -- BB box scaffold pc=398 op=SM_PAT_CAT --
# proc .bb_box_398
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 47                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc399:                
                        mov     edi, 81                     # SM_PAT_BOXVAL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc400:                
                        movabs  rdi, 153600880              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc401:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc402:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc403:                
                        movabs  rdi, 153600944              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc404:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc405:                
                        mov     edi, 58                     # SM_CALL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc406:                
                        movabs  rdi, 153601008              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc407:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc408:                
                        movabs  rdi, 153601040              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc409:                
                        movabs  rdi, 153601072              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc410:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc411:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc412:                
                        movabs  rdi, 153601168              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc413:                
                        movabs  rdi, 153601200              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc414:                
                        mov     edi, 3                      # SM_JUMP_F
                        call    scrip_rt_unhandled_op@PLT   
.Lpc415:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc416:                
                        movabs  rdi, 153601264              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc417:                
                        movabs  rdi, 153601296              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc418:                
                        movabs  rdi, 153601328              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc419:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc420:                
                        mov     edi, 23                     # SM_CONCAT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc421:                
                        movabs  rdi, 153601360              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc422:                
                        mov     edi, 1                      # SM_JUMP
                        call    scrip_rt_unhandled_op@PLT   
.Lpc423:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc424:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc425:                
                        movabs  rdi, 153601456              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc426:                
# -- BB box scaffold pc=426 op=SM_PAT_DEREF --
# proc .bb_box_426
#   .alpha:   (not yet baked)
#   .beta:    (not yet baked)
#   .gamma:   (connected to next box alpha)
#   .omega:   (connected to enclosing beta)
# endp
                        mov     edi, 48                     #  UNHANDLED BB box
                        call    scrip_rt_unhandled_op@PLT   
.Lpc427:                
                        movabs  rdi, 153601488              #  SM_PUSH_VAR: nv_get stub EM-3
                        call    scrip_rt_nv_get@PLT         
.Lpc428:                
                        movabs  rdi, 0                      
                        call    scrip_rt_push_int@PLT       
.Lpc429:                
                        mov     edi, 55                     # SM_EXEC_STMT
                        call    scrip_rt_unhandled_op@PLT   
.Lpc430:                
                        mov     edi, 2                      # SM_JUMP_S
                        call    scrip_rt_unhandled_op@PLT   
.Lpc431:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc432:                
                        movabs  rdi, 153601552              
                        mov     esi, 0                      
                        call    scrip_rt_push_str@PLT       
.Lpc433:                
                        movabs  rdi, 153601584              #  SM_STORE_VAR: nv_set stub EM-3
                        call    scrip_rt_nv_set@PLT         
.Lpc434:                
                        mov     edi, 0                      # SM_LABEL
                        call    scrip_rt_unhandled_op@PLT   
.Lpc435:                
                        mov     edi, 5                      # SM_STNO
                        call    scrip_rt_unhandled_op@PLT   
.Lpc436:                
                        call    scrip_rt_pop_int@PLT        #  rc <- TOS
                        mov     edi, eax                    
                        call    scrip_rt_halt@PLT           
	# -- epilogue -------------------------------------------
	call    scrip_rt_finalize@PLT
	pop     rbp
	ret
	.size   main, .-main
	.section .note.GNU-stack,"",@progbits
