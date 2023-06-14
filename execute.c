#include "execute.h"

extern PSW_Bits PSW;

void execute_BL(unsigned short argument[]){
  PC.word += argument[1];
}

void execute_BEQ_to_BRA(unsigned short argument[]){
  /* Args: 1=Offset */
  switch(argument[0]){
    case BEQ_G: PSW.z == 1 ? PC.word += argument[1] : 0; break;   //(PSW.z)
    case BNE_G: PSW.z == 0 ? PC.word += argument[1] : 0; break;   // !(PSW.z)
    case BC_G:  PSW.c == 1 ? PC.word += argument[1] : 0; break;
    case BNC_G: PSW.c == 0 ? PC.word += argument[1] : 0; break;
    case BN_G:  PSW.n == 1 ? PC.word += argument[1] : 0; break;
    case BGE_G: (PSW.n ^ PSW.v) == 0 ? PC.word += argument[1] : 0; break;
    case BLT_G: (PSW.n ^ PSW.v) == 1 ? PC.word += argument[1] : 0; break;
    case BRA_G: PC.word += argument[1]; break;
  }
}

void execute_ADD_to_ST(unsigned short argument[]){
  /* Switch into options */
 
}
void execute_ADD_to_BIS(unsigned short argument[]){
  /* Args: 1=R/C 2=W/B 3=SRC 4=DST */
  word_byte res = Rx(argument[4]);
  switch(argument[0]){
    /* ADD to SUBC */
    case ADD_G:
      if(argument[2])
        res.byte[0] = Rx(argument[4]).byte[0] + regFile[argument[1]][argument[4]].byte[0];
      else 
        res.word = Rx(argument[4]).word + regFile[argument[1]][argument[4]].word;
    break;
    case ADDC_G: 
      if(argument[2]) res.byte[0] = Rx(argument[4]).byte[0] + regFile[argument[1]][argument[4]].byte[0] + PSW.c;
      else res.word = Rx(argument[4]).word + regFile[argument[1]][argument[4]].word + PSW.c;
    break;
    case SUB_G:
      if(argument[2]) res.byte[0] = Rx(argument[4]).byte[0] + ~regFile[argument[1]][argument[4]].byte[0];
      else res.word = Rx(argument[4]).word + ~regFile[argument[1]][argument[4]].word;
    break;
    case SUBC_G:
      if(argument[2]) res.byte[0] = Rx(argument[4]).byte[0] + ~regFile[argument[1]][argument[4]].byte[0] + PSW.c;
      else res.word = Rx(argument[4]).word + ~regFile[argument[1]][argument[4]].word + PSW.c;
    break;
    /* DADD */
    case DADD_G:
    break;

  }
  update_psw(argument[3], argument[4], res.word, argument[2]);
  Rx(argument[4]) = res;    // DST <- RES
}

void execute_MOV_to_SXT(unsigned short argument[]){

}
void execute_SRA_to_SXT(unsigned short argument[]){

}
void execute_CEX( unsigned short argument[]){
  
}
void execute_LD_to_ST(unsigned short argument[], unsigned char LS_ST_Flag){
  
}
void execute_MOVL_to_MOVH(unsigned short argument[]){
  
}
void execute_LDR(unsigned short argument[]){
  
}
void execute_STR(unsigned short argument[]){
  
}

/* New Functions */

void ADD_SUB(unsigned DST, unsigned SRC, unsigned WORD_BYTE_Flag, unsigned CARRY){
  word_byte res = Rx(DST);
  if(WORD_BYTE_Flag){
    res.byte[0] = Rx(DST).byte[0] + Rx(SRC).byte[0] + CARRY;
  } else{
    res.word = Rx(DST).word + Rx(SRC).word + CARRY;
  }
  update_psw(Rx(SRC).word, Rx(DST).word, res.word, WORD_BYTE_Flag);
  Rx(DST) = res;
}


/* Update PSW */
void update_psw(unsigned short src, unsigned short dst, unsigned short res, unsigned short wb){
  /* 
  - Update the PSW bits (V, N, Z, C) 
  - Using src, dst, and res values and whether word or byte 
  - ADD, ADDC, SUB, and SUBC <- are these the only ones??
  */
  unsigned short mss, msd, msr; /* Most significant src, dst, and res bits */

  if (wb == 0){
    mss = Bn(15, src);
    msd = Bn(15, dst);
    msr = Bn(15, res);
  } else /* Byte */{
    mss = Bn(7, src);
    msd = Bn(7, dst);
    msr = Bn(7, res);
    res &= 0x00FF;	/* Mask high byte for 'z' check */
  }
  PSW.c = carry[mss][msd][msr]; 	   /* Set Carry Bit */
  PSW.z = (res == 0);	               /* Set Zero Bit */
  PSW.n = (msr == 1);	               /* Set Negative Bit */
  PSW.v = overflow[mss][msd][msr]; 	 /* Set oVerflow Bit */
}
