/*
Thursday, July 20, 2023 - cpu.h
- All processes that are carried out by the CPU are emulated here
*/

#ifndef CPU_H
#define CPU_H

#include "emulator.h"
#include "decode.h"
#include "execute.h"
#include "cache.h"

/* Main CPU Functions */
void tick();    
  void fetch();
  void decode(unsigned short* arg);
  void execute(unsigned short argument[]);
#endif