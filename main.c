/*
Thursday, July 20, 2023 - main.c
- Submitted for A1, Computer Architecture 3403
- XM23 Emulator v.1.0.1
Copyright McMoney Engineering LTD.
*/

/* Include Header Files */
#include "loader.h"
#include "emulator.h"
#include "cpu.h"

extern void sigint_hdlr();        // CTRL-C Signal Handler

/* Declare Global Variables */
word_byte regFile REG_SIZE = {    // regFile[S/C][R#]
  {0, 0, 0, 0, 0, 0, 0, 0},       // Register Values
  {0, 1, 2, 4, 8, 16, 32, -1}     // Constant Values
};

mem memory[MEM_SIZE];                   // declare memory
cacheline* cache[CACHE_SIZE];           // declare cache
unsigned short MDR, MAR, IR;            // declare Mem Data Reg, Mem Address Reg, and Instruction Register
int cpu_time = 0, stepFlag, loadFlag;  // declare cpu clock
char cacheMode = '1';
PSW_Bits* PSW;                          // declare status control block
volatile sig_atomic_t ctrl_c_fnd;       // Boolean that indicates whether ^C detected
unsigned short breakPoint;

int main(){
  /* Initalize Variables */
  PSW = malloc(sizeof(PSW_Bits));            // Allocate Memory for PSW.
  char selection[MAX_CMND_LENGTH];

  /* Initalize Control-C Software */
  ctrl_c_fnd = 0;
  signal(SIGINT, sigint_hdlr);

  /* Print Options */
  printCommands();

  /* Main Loop */
  while(toupper(selection[0]) != 'Q'){
    commandPrompt(selection);
    switch(toupper(selection[0])){
      /* Emulation Commands */
      case 'R': runProgram(stepFlag); break;                         // Run Program
      case 'L': loadFile(); break;                                      // Load File
      case 'B': manageBreakpoint(&breakPoint); break;
      case 'P': updatePC(); break;
      case 'M': writeToMemory(); break;                                 // Write to Memory
      case 'S': switchStepMode(); break;
      case 'C': switchCacheMode(); break;
      case 'D': printDebugCommands(); break;
      case '?': printCommands(); break;
      case 'Q': break;                                // Quit
      /* Debugging Commands */
      case '1': mainMemoryDump(selection); break;                       // Memory Dump
      case '2': printRegisterFile(); break;                             // Print Register File
      case '3': printf("[C|%d] [N|%d] [V|%d] [Z|%d]\n", PSW->c, PSW->n, PSW->v, PSW->z); break; // Print PSW
      case '4': printf("CPU Time = %d\n", cpu_time); break;             // Print CPU Time
      case '5': printCache(); break;
      /* Unexpected Command */
      default:
        printf("Unexpected input. Please try again.\n"); 
        printCommands();
      break;
    }
  }
  return 0;
}