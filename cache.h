/*
Tuesday, August 1, 2023 - cache.h
- Declares cache functions and print function
- Declares cachelines and cache structures
*/
#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "emulator.h"

extern void bus(unsigned short addr, unsigned short* data, int readORwrite, int wordORbyte);

typedef struct cacheline{
  unsigned char age;            // Line Age  -> 1 Byte
  unsigned short address;       // Address   -> 2 Bytes
  word_byte* data;              // Data      -> 1 word_byte (2 Bytes)
  unsigned dirtyBit:1;          // Dirt Bit  -> 1 Bit
} cacheline;

/* Structure Functions */
cacheline* createCacheline(unsigned short address, unsigned short data);

void cacheBus(unsigned short address, unsigned short *data, char readORwrite, char wordORbyte);
void ageCache(unsigned int maxAge);
int searchCache(unsigned short address);
int removeOldest();
void printCache();
int searchForVacantLine();
void removeCacheLine(int lineID);


#endif /* CACHE_E */