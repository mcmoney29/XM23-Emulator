/*
Thursday, July 20, 2023 - emulator.h
- Used as the main include file
  -> Has all library includes and is included in each other file
- Has all macros, enums, and structures
- Includes general debugging functions (and bus)
*/

#ifndef EMULATOR_H
#define EMULATOR_H

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <math.h>
#include <signal.h>
//#include "cache.h"



/* CPU */
#define REG_SIZE [2][8]                   // Register Size [Number of Files][Registers per Files]
#define LR regFile[0][5]                  // Link Register
#define SP regFile[0][6]                  // Stack Pointer (R6)
#define PC regFile[0][7]                  // Program Counter (R7)
#define Rx(x) regFile[0][x]               // Register Array 
// #define TICK_SPEED 0                      // pause between ticks in seconds
#define COND(x) (x >> 6) & 0x0F
#define DST(x) x & 0x07                   // Mask macro for destination bits (Bits 0, 1, 2) (also used for CEX-FALSE)
#define SRC(x) (x >> 3) & 0x07            // Mask macro for source bits (Bits 3, 4, 5) (also used for CEX-TRUE)
#define WB(x) (x >> 6) & 0x01             // Mask macro for word or byte flag
#define RC(x) (x >> 7) & 0x01             // Mask macro for read or write flag
#define offset(x, y) ((x & (1 << y)) ? (x | 0xFF00) : (x & 0x01FF))
#define databyte(x) x >> 3 & 0x00FF       // Mask macro for data byte
#define PDI(x) (x >> 7) & 0x7             // Mask for pre/post inc/dec
#define relative_offset(x) ((x & (1 << 7)) ? (x >> 7 | 1 << 7) : (x >> 7 & 0x007F))
#define Bn(n, x) (x >> n) & 0x01;


/* Memory */
#define MEM_SIZE 1 << 16  // Memory Size (1<<16 = 64 KiB)

/* Loader */
//#define MPC 10                    // Max Programs Sent to the Emulator
#define MAX_CMND_LENGTH 50          // Max selection length
#define MAX_FILE_NAME_LENGTH 50     // Max length of an executable
#define MAX_RECORDS 100             // Max S-Records in a .xme file
#define MAX_RECORD_LENGTH 100       // Max length of a S-Record
#define MAX_OPERANDS 5              // Max operand for an instruction
#define TEST_PATH_LEN 18            // Length of the string "tests/test__.xme"

/* Cache */
#define CACHE_SIZE 16
#define LSN(x) x & 0x000F

/* Flags */
enum RW_Flag {READ, WRITE};
enum WB_Flag {WORD, BYTE};
enum RC_Flag {REG, CONST};
enum TRUE_FALSE {FALSE, TRUE};
enum LD_ST_Flag {LD, ST};
enum cacheModes {ASC = 49, DIR, HYB};   // Asccoitative = '1', Direct = '2', Hybrid = '3'

/* Instruction */
enum Instructions {BL_G = 1, BEQ_G, BNE_G, BC_G, BNC_G, BN_G, BGE_G, BLT_G, BRA_G, ADD_G, ADDC_G, SUB_G, SUBC_G, DADD_G, CMP_G, XOR_G, AND_G, OR_G, BIT_G, BIC_G, BIS_G, MOV_G, SWAP_G, SRA_G, RRC_G, COMP_G, SWPB_G, SXT_G, SETPRI_G, SVC_G, SETCC_G, CLRCC_G, CEX_G, LD_G, ST_G, MOVL_G, MOVLZ_G, MOVLS_G, MOVH_G, LDR_G, STR_G};
enum Instruction_Groups {BL, BEQ_to_BRA, ADD_to_BIS, MOV, SWAP, SRA_to_SXT, CEX, LD_to_ST, MOVL_to_MOVH, LDR, STR};
enum Branch_Instructions {BEQ, BNE, BC, BNC, BN, BGE, BLT, BRA};
enum Arithmetic_Instructions {ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, OR, BIT, BIC, BIS};

/* Register File Structure */
typedef union word_byte{
  unsigned short word;
  unsigned char byte[2];
} word_byte;

/* Define Memory Structure */
typedef union mem{
  unsigned short word[MEM_SIZE >> 1];
  unsigned char byte[MEM_SIZE];
} mem;

/* Nibble Structure (4-Bits) */
typedef struct nibble{
  unsigned nib:4;
} nibble;

/* Binary Coded Decimal */
typedef union BCD_NUM{
  unsigned short word;
  struct nibble nib[4];
} BCD_NUM;

/* BCD to WORD_BYTE */
word_byte BCD_to_WORDBYTE(BCD_NUM);
BCD_NUM WORDBYTE_to_BCD(word_byte);

/* PSW */
typedef struct PSW_Bits{
  unsigned        c: 1;      // Carry
  unsigned        z: 1;      // Zero
  unsigned        n: 1;      // Negative
  unsigned      slp: 3;      // SLeeP (unused in Assignment 1)
  unsigned        v: 1;      // oVerflow
  unsigned  current: 3;      // Unused in assignment 1
  unsigned    fault: 1;      //          "
  unsigned reserved: 4;      //          "
  unsigned     prev: 3;      //          "
} PSW_Bits;

void bus(unsigned short, unsigned short*, int, int);

extern unsigned short MDR, MAR, IR;    // declare Mem Data Reg, Mem Address Reg, and Instruction Reg
extern word_byte regFile REG_SIZE;

// void runProgram();
void runProgram();

// void printProgramOptions(char * command);
void memoryDump(unsigned short * bound1, unsigned short* bound2, int bouncCount);
void printRegisterFile();
void writeToMemory();
void printCommands();
void updatePC();
void mainMemoryDump(char* command);
void switchCacheMode();
void printDebugCommands();
void commandPrompt(char *selection);
void manageBreakpoint(unsigned short* breakPoint);

void switchStepMode();

#endif /* EMULATOR_H */