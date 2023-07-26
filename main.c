/*
Wednesday, July 26, 2023 - main.c
- XM23 Emulator v.1.0.2
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
unsigned short MDR, MAR, IR;            // declare Memory Data Reg, Memory Address Reg, and Instruction Register
int cpu_time = 0, stepFlag, loadFlag;   // declare cpu clock
char cacheMode = ASC;                   // declare cache mode
PSW_Bits* PSW;                          // declare status control block
volatile sig_atomic_t ctrl_c_fnd;       // Boolean that indicates whether ^C detected
unsigned short breakPoint;

int main(){
  /* Initialize Variables */
  PSW = malloc(sizeof(PSW_Bits));       // Allocate Memory for PSW.
  char selection[MAX_CMND_LENGTH];

  /* Initialize Control-C Software */
  ctrl_c_fnd = 0;
  signal(SIGINT, sigint_hdlr);

  /* Print Options */
  printCommands();

  /* Main Loop */
  while(toupper(selection[0]) != 'Q'){
    commandPrompt(selection); // Get new command
    switch(toupper(selection[0])){
      /* Emulation Commands */
      case 'R': runProgram(stepFlag); break;                         // Run a Program
      case 'L': loadFile(); break;                                   // Load S-Records
      case 'B': manageBreakpoint(&breakPoint); break;                // Change Breakpoint
      case 'P': updatePC(); break;                                   // Change PC
      case 'M': writeToMemory(); break;                              // Write to Memory
      case 'S': switchStepMode(); break;                             // Switch Step Modes
      case 'C': switchCacheMode(); break;                            // Switch Cache Modes
      case 'D': printDebugCommands(); break;                         // Print Debug Options
      case '?': printCommands(); break;                              // Print Command Options
      case 'Q': break;                                               // Quit
      /* Debugging Commands */
      case '1': mainMemoryDump(selection); break;                    // Memory Dump
      case '2': printRegisterFile(); break;                          // Print Register File
      case '3': printf("[C|%d] [N|%d] [V|%d] [Z|%d]\n", PSW->c, PSW->n, PSW->v, PSW->z); break; // Print PSW
      case '4': printf("CPU Time = %d\n", cpu_time); break;          // Print CPU Time
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