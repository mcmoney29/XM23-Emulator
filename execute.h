#ifndef EXECUTE_H
#define EXECUTE_H

#include "emulator.h"

/* PSW Logic Tables */
unsigned carry[2][2][2] = { 0, 0, 1, 0, 1, 0, 1, 1 };
unsigned overflow[2][2][2] = {0, 1, 0, 0, 0, 0, 1, 0};

/* Execute Functions (Legacy ?)*/
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
void execute_STR(unsigned short[]);

/* New Execute Functions */

void ADD_SUB(unsigned DST, unsigned SRC, unsigned WORD_BYTE_Flag, unsigned CARRY);
/* Update PSW Function */
void update_psw(unsigned short, unsigned short, unsigned short, unsigned short);

#endif /* EXECUTE_H */