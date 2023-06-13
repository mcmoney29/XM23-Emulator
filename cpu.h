#ifndef CPU_H
#define CPU_H

#include "emulator.h"
#include "decode.h"
//#include "control.h" <- tf is this?

void fetch();                  
void tick();                
void decode();
void execute(unsigned short[], unsigned char);

void printArithmetic(char*, unsigned char, unsigned char, unsigned short, unsigned short);
void printBranch(char*, short, unsigned short, unsigned short);

void runProgram(program*, int, word_byte*);

//void ADD_to_BIS(char[], unsigned char*);


// PSW(dst.word, src.word, res.word, W/B)



#endif