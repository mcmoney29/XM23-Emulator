#include "execute.h"

extern PSW_Bits* PSW;
extern mem memory;

/* Update PSW */
void update_psw(unsigned short src, unsigned short dst, unsigned short res, unsigned short wb){
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
  PSW->c = carry[mss][msd][msr]; 	     /* Set Carry Bit */
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
  if(WORD_BYTE_Flag)
    RES.byte[0] = Rx(DST).byte[0] + SRC.byte[0] + CARRY;
  else
    RES.word = Rx(DST).word + SRC.word + CARRY;
  update_psw(SRC.word, Rx(DST).word, RES.word, WORD_BYTE_Flag);
  Rx(DST) = RES;
}

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

void BIC_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? Rx(DST).byte[0] & (1 << ~SRC.byte[0]) : Rx(DST).word & (1 << ~SRC.word);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
  Rx(DST).word = res;
}

void BIS_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? Rx(DST).byte[0] | (1 << SRC.byte[0]) : Rx(DST).word | (1 << SRC.word);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
  Rx(DST).word = res;
}

void MOV_Func(unsigned DST, unsigned SRC, unsigned WORD_BYTE_Flag){
  if(WORD_BYTE_Flag)  
    Rx(DST).byte[0] = Rx(SRC).byte[0]; /* BYTE */
  else  
    Rx(DST).word = Rx(SRC).word;  /* WORD */
}

void SWAP_Func(unsigned DST, unsigned SRC){
  unsigned short temp = Rx(DST).word;
  Rx(DST).word = Rx(SRC).word;
  Rx(SRC).word = temp;
}

void SRA_Func(unsigned DST, unsigned WORD_BYTE_Flag){
  if(WORD_BYTE_Flag)
    Rx(DST).byte[0] >>= 1;
  else
    Rx(DST).word >>= 1;
}

void RRC_Func(unsigned DST, unsigned WORD_BYTE_Flag){
  unsigned bit = Rx(DST).byte[0] & 0x1;
  if(WORD_BYTE_Flag){
    Rx(DST).byte[0] >>= 1;
    Rx(DST).byte[0] |= (PSW->c << 7);
  } else{
    Rx(DST).word >>= 1;
    Rx(DST).word |= (PSW->c << 15);
  }
  PSW->c = bit;
}

void COMP_Func(unsigned DST, unsigned WORD_BYTE_Flag){
  if(WORD_BYTE_Flag){
    Rx(DST).byte[0] = ~Rx(DST).byte[0];
  } else{
    Rx(DST).word = ~Rx(DST).word;
  }
}

void SWPB_Func(unsigned DST){
  unsigned char temp = Rx(DST).byte[0];
  Rx(DST).byte[0] = Rx(DST).byte[1];
  Rx(DST).byte[1] = temp;
}

void SXT_Func(unsigned DST){
  if(Rx(DST).byte[0] & 0x80)
    Rx(DST).byte[1] = 0xFF;
  else
    Rx(DST).byte[1] = 0x00;
}

void LD_Func(unsigned DST, unsigned SRC, unsigned PDI, unsigned WORD_BYTE_Flag){
  switch(PDI){
    case 0: /* Unmodified Register [R] */          
      if(WORD_BYTE_Flag)
        Rx(DST).byte[0] = memory.byte[Rx(SRC).word];
      else            
        Rx(DST).word = memory.word[Rx(SRC).word >> 1];
    break; 

    case 1: /* Post Increment [R+] */          
      if(WORD_BYTE_Flag){ // if byte instruction
        Rx(DST).byte[0] = memory.byte[Rx(SRC).word];
        Rx(SRC).word++; 
      } else{          // if word instruction
        Rx(DST).word = memory.word[Rx(SRC).word >> 1];
        Rx(SRC).word += 2;
      }
    break;

    case 2: /* Post Decrement [R-] */          
      if(WORD_BYTE_Flag){ // if byte instruction
        Rx(DST).byte[0] = memory.byte[Rx(SRC).word];
        Rx(SRC).word--; 
      } else{          // if word instruction
        Rx(DST).word = memory.word[Rx(SRC).word >> 1];
        Rx(SRC).word -= 2;
      }
    break;

    case 5: /* Pre Increment [+R] */          
      if(WORD_BYTE_Flag){ // if byte instruction
        Rx(SRC).word++; 
        Rx(DST).byte[0] = memory.byte[Rx(SRC).word];
      } else{          // if word instruction
        Rx(SRC).word += 2;
        Rx(DST).word = memory.word[Rx(SRC).word >> 1];
      }
    break;

    case 6: /* Pre Decrement [-R] */
      if(WORD_BYTE_Flag){ // if byte instruction
        Rx(SRC).word--; 
        Rx(DST).byte[0] = memory.byte[Rx(SRC).word];
      } else{          // if word instruction
        Rx(SRC).word -= 2;
        Rx(DST).word = memory.word[Rx(SRC).word >> 1];
      }
    break;
  }
}

void ST_Func(unsigned DST, unsigned SRC, unsigned PDI, unsigned WORD_BYTE_Flag){
  switch(PDI){
    case 0: /* Unmodified Register [R] */          
      if(WORD_BYTE_Flag)
        memory.byte[Rx(SRC).word] = Rx(DST).byte[0];
      else            
        memory.word[Rx(SRC).word >> 1] = Rx(DST).word;
    break; 

    case 1: /* Post Increment [R+] */          
      if(WORD_BYTE_Flag){ // if byte instruction
        memory.byte[Rx(SRC).word] = Rx(DST).byte[0];
        Rx(SRC).word++; 
      } else{          // if word instruction
        memory.word[Rx(SRC).word >> 1] = Rx(DST).word;
        Rx(SRC).word += 2;
      }
    break;

    case 2: /* Post Decrement [R-] */          
      if(WORD_BYTE_Flag){ // if byte instruction
        memory.byte[Rx(SRC).word] = Rx(DST).byte[0];
        Rx(SRC).word--; 
      } else{          // if word instruction
        memory.word[Rx(SRC).word >> 1] = Rx(DST).word;
        Rx(SRC).word -= 2;
      }
    break;

    case 5: /* Pre Increment [+R] */          
      if(WORD_BYTE_Flag){ // if byte instruction
        Rx(SRC).word++; 
        memory.byte[Rx(SRC).word] = Rx(DST).byte[0];
      } else{          // if word instruction
        Rx(SRC).word += 2;
        memory.word[Rx(SRC).word >> 1] = Rx(DST).word;
      }
    break;

    case 6: /* Pre Decrement [-R] */
      if(WORD_BYTE_Flag){ // if byte instruction
        Rx(SRC).word--; 
        memory.byte[Rx(SRC).word] = Rx(DST).byte[0];
      } else{          // if word instruction
        Rx(SRC).word -= 2;
        memory.word[Rx(SRC).word >> 1] = Rx(DST).word;
      }
    break;
  }
}