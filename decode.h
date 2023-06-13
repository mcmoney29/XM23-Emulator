#ifndef DECODE_H
#define DECODE_H
#include "emulator.h" // hopefully we don't need this

void BEQ_to_BRA(unsigned short[], unsigned char*);
void ADD_to_ST(unsigned short[], unsigned char*);
  void ADD_to_BIS(unsigned short[], unsigned char*);
  void LD_to_ST(unsigned short[], unsigned char*, unsigned char);
void MOVL_to_MOVH(unsigned short[], unsigned char*);
void LDR(unsigned short[], unsigned char*);
void STR(unsigned short[], unsigned char*);


#endif /* DECODE_H */