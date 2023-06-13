#ifndef EMULATOR_H
#define EMULATOR_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/* CPU */
#define REG_SIZE [2][8]                   // Register Size [Number of Files][Registers per Files]
#define LR regFile[0][5]                  // Link Register
#define SP regFile[0][6]                  // Stack Pointer (R6)
#define PC regFile[0][7]                  // Program Counter (R7)
#define Rx(x) regFile[0][x]               // Register Array 
#define TICK_SPEED 0                      // pause between ticks in seconds
#define DST(x) x & 0x07                   // Mask macro for destination bits (Bits 0, 1, 2)
#define SRC(x) (x >> 3) & 0x07            // Mask macro for source bits (Bits 3, 4, 5)
#define WB(x) (x >> 6) & 0x01             // Mask macro for word or byte flag
#define RC(x) (x >> 7) & 0x01             // Mask macro for read or write flag
#define offset(x, y) ((x & (1 << y)) ? (x | 0xC000) : (x & 0x01FF))
#define databyte(x) x >> 3 & 0x00FF       // Mask macro for data byte
//#define PRPO(x) (x >> 9) & 0x01;          // Mask for precrement or postcrement {LD, ST}
//#define DEC(x) (x >> 8) & 0x01;           // Mask for decremenet {LD, ST}
//#define INC(x) (x >> 7) & 0x01;
#define PDI(x) (x >> 7) & 0x7            // Mask for pre/post inc/dec
#define relative_offset(x) ((x & (1 << 7)) ? (x >> 7 | 1 << 7) : (x >> 7 & 0x007F))

/* Memory */
#define MEM_SIZE 1 << 16  // Memory Size (1<<16 = 64 KiB)

/* Loader */
#define MRL 100   // Max Record Length
#define MFL 100   // Max Records in a .XME file
#define MPC 10    // Max Programs Sent to the Emulator
#define MAX_FILE_NAME_LENGTH 50 // Max length of an executable
#define MAX_RECORDS 100
#define MAX_LENGTH 100
#define MAX_OPERANDS 7

/* Emulator */
#define CAP(x) toupper(x);

/* Flags */
enum RW_Flag {READ, WRITE};
enum WB_Flag {WORD, BYTE};
enum RC_Flag {REG, CONST};

/* Instruction */
enum Instructions {BL_G = 1, BEQ_G, BNE_G, BC_G, BNC_G, BN_G, BGE_G, BLT_G, BRA_G, ADD_G, ADDC_G, SUB_G, SUBC_G, DADD_G, CMP_G, XOR_G, AND_G, OR_G, BIT_G, BIC_G, BIS_G, MOV_G, SWAP_G, SRA_G, RRC_G, COMP_G, SWPB_G, SXT_G, SETPRI_G, SVC_G, SETCC_G, CLRCC_G, CEX_G, LD_G, ST_G, MOVL_G, MOVLZ_G, MOVLS_G, MOVH_G, LDR_G, STR_G};
enum Branch_Instructions {BEQ, BNE, BC, BNC, BN, BGE, BLT, BRA};
enum Arithmetic_Instructions {ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT, BIC, BIS};

/* Define Memory Structure */
typedef union mem{
  unsigned short word[MEM_SIZE >> 1];
  unsigned char byte[MEM_SIZE];
} mem;

/* Register File Structure */
typedef union word_byte{
  unsigned short word;
  unsigned char byte[2];
} word_byte;

/* Program Program Structure */
typedef struct program{
  char name[MAX_FILE_NAME_LENGTH];
  unsigned short startingAddress;
  struct program* next;
} program;

/* Nibble Structure (4-Bits) */
typedef struct nibble{
  unsigned nib:4;
} nibble;

/* Binary Coded Decimal */
typedef union BCD_NUM{
  unsigned short word;
  struct nibble nib[4];
} BCD_NUM;

void bus(unsigned short, unsigned short*, int, int);

extern unsigned short MDR, MAR, IR;    // declare Mem Data Reg, Mem Address Reg, and Instruction Reg
extern word_byte regFile REG_SIZE;

void printProgramOptions(program*);
void memoryDump();
void printRegisterFile();
void writeToMemory();

#endif /* EMULATOR_H */