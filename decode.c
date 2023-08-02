/*
Thursday, July 20, 2023 - decode.c
- Defines functions that assist in the decoding process done by decode()
- All functions take in the argument array and modify the array, replacing 
  the previous instruction's arguments with the decoded data.
*/

#include "decode.h"

void decode_BL(unsigned short argument[]){
  argument[0] = BL_G;
  argument[1] = (IR & (1 << 12)) ? (IR | 0xE000) << 1 : (IR & 0x1FFF) << 1;   // Offset (migos)
}

void decode_BEQ_to_BRA(unsigned short argument[]){
  switch((IR >> 10) & 0x07){
    case BEQ: argument[0] = BEQ_G;  break;
    case BNE: argument[0] = BNE_G;  break;
    case BC:  argument[0] = BC_G;   break;
    case BNC: argument[0] = BNC_G;  break;
    case BN:  argument[0] = BN_G;   break;
    case BGE: argument[0] = BGE_G;  break;
    case BLT: argument[0] = BLT_G;  break;
    case BRA: argument[0] = BRA_G;  break;
  }
  argument[1] = offset(IR, 9) << 1;    // Offset (migos)
}

void decode_ADD_to_ST(unsigned short argument[]){
  switch((IR >> 10) & 0x07){
    case 0: case 1: case 2: decode_ADD_to_BIS(argument); break;
    case 3: decode_MOV_to_SXT(argument);                 break;
    case 4: decode_CEX(argument);                        break;
    case 6: decode_LD_to_ST(argument, LD_G);     break;
    case 7: decode_LD_to_ST(argument, ST_G);     break;
  } 
}

void decode_MOVL_to_MOVH(unsigned short argument[]){
  switch(IR >> 11 & 0x03){
    case 0: argument[0] = MOVL_G;  break;
    case 1: argument[0] = MOVLZ_G; break;
    case 2: argument[0] = MOVLS_G; break;
    case 3: argument[0] = MOVH_G;  break;
  }
  argument[1] = databyte(IR); // Byte of Data
  argument[2] = DST(IR);      // Destination Register
}

void decode_LDR(unsigned short argument[]){
  argument[0] = LDR_G;
  argument[1] = relative_offset(IR);
  argument[2] = WB(IR);     // Word/Byte
  argument[3] = SRC(IR);    // Source Register
  argument[4] = DST(IR);    // Destination Register
}

void decode_STR(unsigned short argument[]){
  argument[0] = STR_G;
  argument[1] = relative_offset(IR);
  argument[2] = WB(IR);     // Word/Byte
  argument[3] = SRC(IR);    // Source Register
  argument[4] = DST(IR);    // Destination Register
}

/* ADD to ST */
void decode_ADD_to_BIS(unsigned short argument[]){
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

void decode_SRA_to_SXT(unsigned short argument[]){
  switch((IR >> 3) & 0x07){
    case 0: argument[0] = SRA_G;  break;
    case 1: argument[0] = RRC_G;  break;
    case 2: argument[0] = COMP_G; break;
    case 3: argument[0] = SWPB_G; break;
    case 4: argument[0] = SXT_G;  break;
  }
  /* arguments are within decode_MOV_to_SXT() */
}

void decode_MOV_to_SXT(unsigned short argument[]){
  switch((IR >> 7) & 0x03){
    case 0: argument[0] = MOV_G;  break;
    case 1: argument[0] = SWAP_G; break;
    case 2: decode_SRA_to_SXT(argument); break;
    case 3: decode_SETPRI_to_CLRCC(argument); return;
  }
  argument[1] = WB(IR);     // Word/Byte
  argument[2] = SRC(IR);    // Source Register (ignored by SRA to SXT during executing) 
  argument[3] = DST(IR);    // Destination Register
}

void decode_SETPRI_to_CLRCC(unsigned short argument[]){
  switch((IR >> 5) & 0x03){
    case 0: /* SETPRI/SVC */ break;
    case 1: argument[0] = SETCC_G; break;
    case 2: argument[0] = CLRCC_G; break;
  }
  argument[1] = IR & 0x001F;
}

void decode_LD_to_ST(unsigned short argument[], unsigned char LD_ST_Flag){
  argument[0] = LD_ST_Flag;
  argument[1] = PDI(IR);    // Pre/Post, Decrement, Increment
  argument[2] = WB(IR);     // Word/Byte
  argument[3] = SRC(IR);    // Source Register
  argument[4] = DST(IR);    // Destination Register
}

void decode_CEX( unsigned short argument[]){
  argument[0] = CEX_G;
  argument[1] = COND(IR);   // Condition
  argument[2] = SRC(IR);    // True (Register)
  argument[3] = DST(IR);    // False (Register)
}