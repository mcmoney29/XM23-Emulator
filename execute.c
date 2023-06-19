#include "execute.h"

extern struct PSW_Bits* PSW;

/* Update PSW */
void update_psw(unsigned short src, unsigned short dst, unsigned short res, unsigned short wb){
  /* 
  - Update the PSW bits (V, N, Z, C) 
  - Using src, dst, and res values and whether word or byte 
  - ADD, ADDC, SUB, and SUBC <- are these the only ones??
  */
  unsigned short mss, msd, msr;   /* Most significant src, dst, and res bits */
  
  /* PSW Logic Tables */
  unsigned carry[2][2][2] = { 0, 0, 1, 0, 1, 0, 1, 1 };
  unsigned overflow[2][2][2] = {0, 1, 0, 0, 0, 0, 1, 0};

  if (wb == 0){
    /* Word */
    mss = Bn(15, src);
    msd = Bn(15, dst);
    msr = Bn(15, res);
  } else{
    /* Byte */
    mss = Bn(7, src);
    msd = Bn(7, dst);
    msr = Bn(7, res);
    res &= 0x00FF;	  /* Mask high byte for zero check */
  }
  PSW->c = carry[mss][msd][msr]; 	   /* Set Carry Bit */
  //printf("Set PSW.c = %d\n", carry[mss][msd][msr]);
  PSW->z = (res == 0);	               /* Set Zero Bit */
  //printf("Set PSW.z = %d\n", (res == 0));
  PSW->n = (msr == 1);	               /* Set Negative Bit */
  //printf("Set PSW.n = %d\n", (msr == 1));
  PSW->v = overflow[mss][msd][msr]; 	 /* Set oVerflow Bit */
  //printf("Set PSW.v = %d\n", overflow[mss][msd][msr]);
}

/* ADD, ADDC, SUB, SUBC */
void ADD_SUB(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag, unsigned CARRY){
  word_byte RES = Rx(DST);
  if(WORD_BYTE_Flag){
    RES.byte[0] = Rx(DST).byte[0] + SRC.byte[0] + CARRY;
  } else{
    RES.word = Rx(DST).word + SRC.word + CARRY;
  }
  update_psw(SRC.word, Rx(DST).word, RES.word, WORD_BYTE_Flag);
  Rx(DST) = RES;
} /* Complete */

/* DADD */
void DADD_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned char HALF_CARRY = 0;
  BCD_NUM DST_BCD = WORDBYTE_to_BCD(Rx(DST));
  BCD_NUM SRC_BCD = WORDBYTE_to_BCD(SRC);
  BCD_NUM result;

  unsigned char res;
  for(int i = 0; i < 4; i++){
    res = DST_BCD.nib[i].nib + SRC_BCD.nib[i].nib + HALF_CARRY;
    if(res > 10){
      res -= 10;
      HALF_CARRY = 1;
    } else{
      HALF_CARRY = 0;
    }
    result.nib[i].nib = res;
  }
  Rx(DST).word = 0x0000;
  for(int i = 0; i < 4; i++){
    Rx(DST).word += result.nib[i].nib << (4*i);
  } 
  /*DADD_FUNC(D, S, Half Carry){
  word_byte RES = Rx(DST);
	RES.word = Rx(DST).word + Rx(SRC).word + HALF_CARRY;
	if (RES.word > 9){
		RES.word -= 10;
		HALF_CARRY = 1;
  }
	else
		HALF_CARRY = 0;
*/
}

void CMP_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? (Rx(DST).byte[0] + ~SRC.byte[0] + 1) : (Rx(DST).word + ~SRC.word + 1);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
}

void XOR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? (Rx(DST).byte[0] ^ SRC.byte[0]) : (Rx(DST).word ^ SRC.word);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
  Rx(DST).word |= res;
}

void AND_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  //res = WORD_BYTE_Flag ? (Rx(DST).byte[0] & SRC.byte[0]) : (Rx(DST).word & SRC.word);
  if(WORD_BYTE_Flag){
    /* BYTE */
    res = Rx(DST).byte[0] & SRC.byte[0];
    update_psw(SRC.word, Rx(DST).word, res, BYTE);
    Rx(DST).byte[0] = res & 0xFF;
  } else{
    /* WORD */
    res = Rx(DST).word & SRC.word;
    update_psw(SRC.word, Rx(DST).word, res, WORD);
    Rx(DST).word = res;
  }
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
}

void OR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? (Rx(DST).byte[0] | SRC.byte[0]) : (Rx(DST).word | SRC.word);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
  Rx(DST).word |= res;
}

void BIT_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? Rx(DST).byte[0] & (1 << SRC.byte[0]): Rx(DST).word & (1 << SRC.word);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
}