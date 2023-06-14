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

void decode_BL(unsigned short[]);
void decode_BEQ_to_BRA(unsigned short[]);
void decode_ADD_to_ST(unsigned short[]);
  void decode_ADD_to_BIS(unsigned short[]);
  void decode_MOV_to_SXT(unsigned short[]);
    void decode_SRA_to_SXT(unsigned short[]);
  void decode_CEX( unsigned short[]);
  void decode_LD_to_ST(unsigned short[], unsigned char);
void decode_MOVL_to_MOVH(unsigned short[]);
void decode_LDR(unsigned short[]);
void decode_STR(unsigned short[]);

#endif /* DECODE_H */