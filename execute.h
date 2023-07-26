/* 
Wednesday, July 26, 2023 - execute.h
- Declares functions that assist in the decoding process done by decode()
- Declares the update psw function
*/

#ifndef EXECUTE_H
#define EXECUTE_H

#include "emulator.h"

/* Execute Functions */
void ADD_SUB(unsigned DST, unsigned short SRC, unsigned WORD_BYTE_Flag, unsigned CARRY);
void DADD_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void CMP_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void XOR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void AND_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void OR_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void BIT_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void BIS_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void BIC_Func(unsigned DST, word_byte SRC, unsigned WORD_BYTE_Flag);
void SWAP_Func(unsigned DST, unsigned SRC);
void MOV_Func(unsigned DST, unsigned SRC, unsigned WORD_BYTE_Flag);
void SRA_Func(unsigned DST, unsigned WORD_BYTE_Flag);
void RRC_Func(unsigned DST, unsigned WORD_BYTE_Flag);
void COMP_Func(unsigned DST, unsigned WORD_BYTE_Flag);
void SWPB_Func(unsigned DST);
void SXT_Func(unsigned DST);
void LD_Func(unsigned DST, unsigned SRC, unsigned PDI, unsigned WORD_BYTE_Flag);
void ST_Func(unsigned DST, unsigned SRC, unsigned PDI, unsigned WORD_BYTE_Flag);

/* Update PSW Function */
void update_psw(unsigned short, unsigned short, unsigned short, unsigned short);

#endif /* EXECUTE_H */