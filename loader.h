/*
Thursday, July 20, 2023 - loader.h
- Declares functions that allow for the emulator to read s-records and
  write them into memory
*/

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

#endif /* LOADER_H */