#ifndef CPU_H
#define CPU_H

#include "emulator.h"
#include "decode.h"
#include "execute.h"

/* Main CPU Functions */
void tick();    
  void fetch();
  void decode(unsigned short* arg);
  void execute(unsigned short argument[]);
#endif