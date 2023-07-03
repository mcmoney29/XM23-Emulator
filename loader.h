#ifndef LOADER_H
#define LOADER_H

#include "emulator.h"

/* Main Function */
int loadFile();

/* Basic Functions */
unsigned char getType(char[], unsigned char*);
unsigned char getLength(char[], unsigned char*);  
unsigned short getAddress(char[], unsigned char*);  
void executeRecord(char[], unsigned short*, char*);

/* Program Functions */
program* createProgram(char*, unsigned short);
void pushProgram(program*, program*);

#endif /* LOADER_H */