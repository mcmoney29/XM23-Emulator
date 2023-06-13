#ifndef DECODE_H
#define DECODE_H
#include "emulator.h"

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

void BL(unsigned short[]);
void BEQ_to_BRA(unsigned short[]);
void ADD_to_ST(unsigned short[]);
  void ADD_to_BIS(unsigned short[]);
  void MOV_to_SXT(unsigned short[]);
    void SRA_to_SXT(unsigned short[]);
  void CEX( unsigned short[]);
  void LD_to_ST(unsigned short[], unsigned char);
void MOVL_to_MOVH(unsigned short[]);
void LDR(unsigned short[]);
void STR(unsigned short[]);

#endif /* DECODE_H */