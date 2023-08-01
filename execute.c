/*
Tuesday, August 1, 2023 - execute.c
- Defines functions that assist in the decoding process done by decode()
- Defines the update PSW function
- Functions are passed values from the argument array
*/

#include "execute.h"

extern PSW_Bits* PSW;
extern mem memory;

/* Update PSW */
void update_psw(unsigned short src, unsigned short dst, unsigned short res, unsigned short wb){
  unsigned short mss, msd, msr;   // Most significant src, dst, and res bits 
  
  /* PSW Logic Tables */
  unsigned carry[2][2][2] = {0, 0, 1, 0, 1, 0, 1, 1};
  unsigned overflow[2][2][2] = {0, 1, 0, 0, 0, 0, 1, 0};

  if (wb == 0){ /* Word */
    mss = Bn(15, src);
    msd = Bn(15, dst);
    msr = Bn(15, res);
  } else{ /* Byte */
    mss = Bn(7, src);
    msd = Bn(7, dst);
    msr = Bn(7, res);
    res &= 0x00FF;	  /* Mask high byte for zero check */
  }
  PSW->c = carry[mss][msd][msr]; 	     // Set Carry Bit
  PSW->z = (res == 0);	               // Set Zero Bit
  PSW->n = (msr == 1);	               // Set Negative Bit
  PSW->v = overflow[mss][msd][msr]; 	 // Set oVerflow Bit
}

/* ADD, ADDC, SUB, SUBC */
void ADD_SUB(unsigned DST, unsigned short SRC, unsigned WORD_BYTE_Flag, unsigned CARRY){
  word_byte RES = Rx(DST);
  if(WORD_BYTE_Flag){
    RES.byte[0] = Rx(DST).byte[0] + (SRC & 0xFF) + CARRY;
    update_psw(SRC + CARRY & 0xFF, Rx(DST).word, RES.word, WORD_BYTE_Flag);
  }
  else{
    RES.word = Rx(DST).word + SRC + CARRY;
    update_psw(SRC + CARRY, Rx(DST).word, RES.word, WORD_BYTE_Flag);
  }
  Rx(DST) = RES;
}

/* DADD */
void DADD_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned char HALF_CARRY = 0, nibbles, res;
  BCD_NUM DST_BCD = WORDBYTE_to_BCD(Rx(DST));     // Convert DST to BCD
  BCD_NUM SRC_BCD = WORDBYTE_to_BCD(SRC);         // Convert SRC to BCD
  BCD_NUM result;

  nibbles = WORD_BYTE_Flag ? 2 : 4;   // Set nibbles based on WORD_/BYTE_ flag

  /* Calculate Decimal ADD */
  for(int i = 0; i < nibbles; i++){
    res = DST_BCD.nib[i].nib + SRC_BCD.nib[i].nib + HALF_CARRY;
    if(res >= 10){
      res -= 10;
      HALF_CARRY = 1;
    } else{
      HALF_CARRY = 0;
    }
    result.nib[i].nib = res;
  }

  /* Save RST to DST.word */
  Rx(DST).word = WORD_BYTE_Flag ? Rx(DST).word & 0xFF00 : 0;
  for(int i = 0; i < nibbles; i++){
    Rx(DST).word += result.nib[i].nib << (4*i);
  } 
}

void CMP_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? (Rx(DST).byte[0] + ~SRC.byte[0] + 1) : (Rx(DST).word + ~SRC.word + 1);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
}

void XOR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  if(WORD_BYTE_Flag){
    /* BYTE_ */
    res = Rx(DST).byte[0] ^ SRC.byte[0];
    update_psw(SRC.word, Rx(DST).word, res, BYTE_);
    Rx(DST).byte[0] = res & 0xFF;
  } else{
    /* WORD_ */
    res = Rx(DST).word ^ SRC.word;
    update_psw(SRC.word, Rx(DST).word, res, WORD_);
    Rx(DST).word = res;
  }
}

void AND_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  if(WORD_BYTE_Flag){
    /* BYTE_ */
    res = Rx(DST).byte[0] & SRC.byte[0];
    update_psw(SRC.word, Rx(DST).word, res, BYTE_);
    Rx(DST).byte[0] = res & 0xFF;
  } else{
    /* WORD_ */
    res = Rx(DST).word & SRC.word;
    update_psw(SRC.word, Rx(DST).word, res, WORD_);
    Rx(DST).word = res;
  }
}

void OR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res = Rx(DST).word;
  if(WORD_BYTE_Flag){
    /* BYTE_ */
    res = Rx(DST).byte[0] | SRC.byte[0];
    update_psw(SRC.word, Rx(DST).word, res, BYTE_);
    Rx(DST).byte[0] = res & 0xFF;
  } else{
    /* WORD_ */
    res = Rx(DST).word | SRC.word;
    update_psw(SRC.word, Rx(DST).word, res, WORD_);
    Rx(DST).word = res;
  }
}

void BIT_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = WORD_BYTE_Flag ? Rx(DST).byte[0] & (1 << SRC.byte[0]): Rx(DST).word & (1 << SRC.word);
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
}

void BIC_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = Rx(DST).word | ~(1<<(SRC.word));
  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
  Rx(DST).word = res;
}

void BIS_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag){
  unsigned short res;
  res = Rx(DST).word | (1 << (SRC.word));

  update_psw(SRC.word, Rx(DST).word, res, WORD_BYTE_Flag);
  Rx(DST).word = res;
}

void MOV_Func(unsigned DST, unsigned SRC, unsigned WORD_BYTE_Flag){
  if(WORD_BYTE_Flag)  
    Rx(DST).byte[0] = Rx(SRC).byte[0]; /* BYTE_ */
  else  
    Rx(DST).word = Rx(SRC).word;  /* WORD_ */
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

void LD_Func(unsigned DST, unsigned SRC, unsigned PDI, unsigned wordORbyte){
  printf("LD_Func()\n");
  switch(PDI){
    case 0: /* Unmodified Register [R] */
      if(wordORbyte){
        cacheBus(Rx(SRC).byte[0], &Rx(DST).word, READ, BYTE_);
       } else{
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, WORD_);
       }
    break; 

    case 1: /* Post Increment [R+] */          
      if(wordORbyte){
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, BYTE_);
        Rx(SRC).word++; 
      } else{ 
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, WORD_);
        Rx(SRC).word += 2;
      }
    break;

    case 2: /* Post Decrement [R-] */          
      if(wordORbyte){
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, BYTE_);
        Rx(SRC).word--; 
      } else{
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, WORD_);
        Rx(SRC).word -= 2;
      }
    break;

    case 5: /* Pre Increment [+R] */          
      if(wordORbyte){
        Rx(SRC).word++;
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, BYTE_);
      } else{
        Rx(SRC).word += 2;
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, WORD_);
      }
    break;

    case 6: /* Pre Decrement [-R] */
      if(wordORbyte){
        Rx(SRC).word--; 
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, BYTE_);
      }else{
        Rx(SRC).word -= 2;
        cacheBus(Rx(SRC).word, &Rx(DST).word, READ, WORD_);
      }
    break;
  }
}

void ST_Func(unsigned DST, unsigned SRC, unsigned PDI, unsigned WORD_BYTE_Flag){
  switch(PDI){
    case 0: /* Unmodified Register [R] */          
      if(WORD_BYTE_Flag)
        cacheBus(Rx(DST).word, &Rx(SRC).word, WRITE, BYTE_);
      else            
        cacheBus(Rx(DST).word, &Rx(SRC).word, WRITE, WORD_);
    break; 

    case 1: /* Post Increment [R+] */          
      if(WORD_BYTE_Flag){
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, BYTE_);
        Rx(SRC).word++; 
      } else{
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, WORD_);
        Rx(SRC).word += 2;
      }
    break;

    case 2: /* Post Decrement [R-] */          
      if(WORD_BYTE_Flag){
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, BYTE_);
        Rx(SRC).word--; 
      } else{
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, WORD_);
        Rx(SRC).word -= 2;
      }
    break;

    case 5: /* Pre Increment [+R] */          
      if(WORD_BYTE_Flag){
        Rx(SRC).word++; 
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, BYTE_);
      } else{         
        Rx(SRC).word += 2;
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, WORD_);
      }
    break;

    case 6: /* Pre Decrement [-R] */
      if(WORD_BYTE_Flag){
        Rx(SRC).word--; 
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, BYTE_);
      } else{          
        Rx(SRC).word -= 2;
        cacheBus(Rx(SRC).word, &Rx(DST).word, WRITE, WORD_);
      }
    break;
  }
}