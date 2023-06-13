#include "decode.h"

void BEQ_to_BRA(unsigned short argument[], unsigned char* argumentCount){
  switch((IR&0x1C00) >> 10){
    case BEQ: argument[0] = BEQ_G;  break;
    case BNE: argument[0] = BNE_G;  break;
    case BC:  argument[0] = BC_G;   break;
    case BNC: argument[0] = BNC_G;  break;
    case BN:  argument[0] = BN_G;   break;
    case BGE: argument[0] = BGE_G;  break;
    case BLT: argument[0] = BLT_G;  break;
    case BRA: argument[0] = BRA_G;  break;
  }
  argument[1] = offset(IR, 9);
  *argumentCount = 2;
}

void ADD_to_ST(unsigned short argument[], unsigned char* argumentCount){
  printf("decode.c l.19");
  switch((IR >> 10) & 0x07){ // 12? shouldn't it be 10? wait 13? no 10
    case 0: ADD_to_BIS(argument, argumentCount); break;
    case 1: ADD_to_BIS(argument, argumentCount); break;
    case 2: ADD_to_BIS(argument, argumentCount); break;
    case 3: /* MOV to CLRCC */
    case 4: /* CEX */
    case 5:
    case 6: LD_to_ST(argument, argumentCount, LD_G); break;
    case 7: LD_to_ST(argument, argumentCount, ST_G); break;
  } 
}

void MOVL_to_MOVH(unsigned short argument[], unsigned char* argumentCount){
  switch(IR >> 11 & 0x03){
    case 0: argument[0] = MOVL_G;  break;
    case 1: argument[0] = MOVLZ_G; break;
    case 2: argument[0] = MOVLS_G; break;
    case 3: argument[0] = MOVH_G;  break;
  }
  argument[1] = databyte(IR);
  argument[2] = DST(IR);
  *argumentCount = 3;
}

void LDR(unsigned short argument[], unsigned char* argumentCount){
  printf("decode.c l.45");
  argument[0] = LDR_G;
  argument[1] = relative_offset(IR);
  argument[2] = WB(IR);
  argument[3] = SRC(IR);
  argument[4] = DST(IR);
  *argumentCount = 5;
}

void STR(unsigned short argument[], unsigned char* argumentCount){
  argument[0] = STR_G;
  argument[1] = relative_offset(IR);
  argument[2] = WB(IR);
  argument[3] = SRC(IR);
  argument[4] = DST(IR);
  *argumentCount = 5;
}

void LD_to_ST(unsigned short argument[], unsigned char* argumentCount, unsigned char LD_ST_Flag){
  printf("[decode.c 44]");
  argument[0] = LD_ST_Flag;
  argument[1] = PDI(IR);
  argument[2] = WB(IR);
  argument[3] = SRC(IR);
  argument[4] = DST(IR);
  *argumentCount = 5; 
}

void ADD_to_BIS(unsigned short argument[], unsigned char* argumentCount){
  switch(IR >> 8 & 0x0F){
    case 0:  argument[0] = ADD_G;  break;
    case 1:  argument[0] = ADDC_G; break;
    case 2:  argument[0] = SUB_G;  break;
    case 3:  argument[0] = SUBC_G; break;
    case 4:  argument[0] = DADD_G; break;
    case 5:  argument[0] = CMP_G;  break;
    case 6:  argument[0] = XOR_G;  break;
    case 7:  argument[0] = AND_G;  break;
    case 8:  argument[0] = OR_G;   break;
    case 9:  argument[0] = BIT_G;  break;
    case 10: argument[0] = BIC_G;  break;
    case 11: argument[0] = BIS_G;  break;
  }
  argument[1] = RC(IR);     // Register/Constant
  argument[2] = WB(IR);     // Word/Byte
  argument[3] = SRC(IR);    // Source Register
  argument[4] = DST(IR);    // Destination Register
  *argumentCount = 5;
}