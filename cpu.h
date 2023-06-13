#ifndef CPU_H
#define CPU_H

#include "emulator.h"
#include "decode.h"

/* Main CPU Functions */
void tick();    
  void fetch();
  void decode();
  void execute(unsigned short argument[]);

/* Test Functions */
void printArithmetic(char*, unsigned char, unsigned char, unsigned short, unsigned short);
void printBranch(char*, short, unsigned short, unsigned short);

/* Emulation Functions */
void runProgram(program*, int, word_byte*);

// PSW(dst.word, src.word, res.word, W/B)
#endif