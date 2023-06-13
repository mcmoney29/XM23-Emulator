/*
Decode Tree Outline Index

-> BL
-> BEQ - BRA
-> ADD - ST
  '-> ADD - BIS
  '-> MOV - SXT
    '-> MOV
    '-> SWAP
    '-> SRA - SXT
  '-> CEX
  '-> LD - ST
-> MOVL - MOVH
-> LDR
-> STR
*/

#include "decode.h"

void BL(unsigned short argument[]){
  argument[0] = BL_G;
  argument[1] = offset(IR, 13);
}

void BEQ_to_BRA(unsigned short argument[]){
  switch((IR >> 10) & 0x03){
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
}

void ADD_to_ST(unsigned short argument[]){
  switch((IR >> 10) & 0x07){
    case 0: case 1: case 2: ADD_to_BIS(argument); break;
    case 3: MOV_to_SXT(argument);                 break;
    case 4: CEX(argument);                        break;
    case 6: case 7: LD_to_ST(argument, LD_G);     break;
  } 
}

void MOVL_to_MOVH(unsigned short argument[]){
  switch(IR >> 11 & 0x03){
    case 0: argument[0] = MOVL_G;  break;
    case 1: argument[0] = MOVLZ_G; break;
    case 2: argument[0] = MOVLS_G; break;
    case 3: argument[0] = MOVH_G;  break;
  }
  argument[1] = databyte(IR);
  argument[2] = DST(IR);
}

void LDR(unsigned short argument[]){
  argument[0] = LDR_G;
  argument[1] = relative_offset(IR);
  argument[2] = WB(IR);
  argument[3] = SRC(IR);
  argument[4] = DST(IR);
}

void STR(unsigned short argument[]){
  argument[0] = STR_G;
  argument[1] = relative_offset(IR);
  argument[2] = WB(IR);
  argument[3] = SRC(IR);
  argument[4] = DST(IR);
}

/* ADD to ST */
void ADD_to_BIS(unsigned short argument[]){
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
}

void SRA_to_SXT(unsigned short argument[]){
  switch((IR >> 3) & 0x07){
    case 0: argument[0] = SRA_G;  break;
    case 1: argument[0] = RRC_G;  break;
    case 2: argument[0] = COMP_G; break;
    case 3: argument[0] = SWPB_G; break;
    case 4: argument[0] = SXT_G;  break;
  }
  /* arguments are within MOV_to_SXT() */
}

void MOV_to_SXT(unsigned short argument[]){
  switch((IR >> 7) & 0x03){
    case 0: argument[0] = MOV_G;  break;
    case 1: argument[0] = SWAP_G; break;
    case 2: SRA_to_SXT(argument); break;
    case 3: break;    // error - SETPRI to CLRCC, not included in assignment 1
  }
  argument[1] = WB(IR);
  argument[2] = SRC(IR);    // argument[2] is ignored during executing of SRA to SXT
  argument[3] = DST(IR);
}

void LD_to_ST(unsigned short argument[], unsigned char LD_ST_Flag){
  argument[0] = LD_ST_Flag;
  argument[1] = PDI(IR);
  argument[2] = WB(IR);
  argument[3] = SRC(IR);
  argument[4] = DST(IR);
}

void CEX( unsigned short argument[]){
  argument[0] = CEX_G;
  argument[1] = COND(IR); // Condition
  argument[2] = SRC(IR);  // True
  argument[3] = DST(IR);  // False
}