#ifndef EXECUTE_H
#define EXECUTE_H

#include "emulator.h"



/* Execute Functions (Legacy ?)
void execute_BL(unsigned short[]);
void execute_BEQ_to_BRA(unsigned short[]);
void execute_ADD_to_ST(unsigned short[]);
  void execute_ADD_to_BIS(unsigned short[]);
  void execute_MOV_to_SXT(unsigned short[]);
    void execute_SRA_to_SXT(unsigned short[]);
  void execute_CEX( unsigned short[]);
  void execute_LD_to_ST(unsigned short[], unsigned char);
void execute_MOVL_to_MOVH(unsigned short[]);
void execute_LDR(unsigned short[]);
void execute_STR(unsigned short[]);*/

/* New Execute Functions */

void ADD_SUB(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag, unsigned CARRY);
void DADD_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void CMP_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void XOR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void AND_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void OR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void BIT_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);

/* Update PSW Function */
void update_psw(unsigned short, unsigned short, unsigned short, unsigned short);

#endif /* EXECUTE_H */