/*
Tuesday, August 1, 2023 - main.c
- XM23 Emulator v.1.1.0
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
CEX_State* CEX_;                        // declare CEX state
unsigned short MDR, MAR, IR;            // declare Memory Data Reg, Memory Address Reg, and Instruction Register
int cpu_time = 0;
unsigned char stepFlag, loadFlag, cacheMode, replPol;     // declare global state variables
//char cacheMode = ASC;                 // declare cache mode
PSW_Bits* PSW;                          // declare status control block
volatile sig_atomic_t ctrl_c_fnd;       // Boolean that indicates whether ^C detected
unsigned short breakPoint;

int main(){
  /* Initialize Variables */
  PSW = malloc(sizeof(PSW_Bits));       // Allocate Memory for PSW
  CEX_ = malloc(sizeof(CEX_State));     // Allocate Memory for CEX
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
      case 'G': runProgram(); break;                                 // Run Program
      case 'L': loadFile(); break;                                   // Load S-Records
      case 'B': manageBreakpoint(&breakPoint); break;                // Change Breakpoint
      case 'R': printRegisterFile(); break;                          // Print Register File
      case 'P': printf("[C|%d] [N|%d] [V|%d] [Z|%d]\n", PSW->c, PSW->n, PSW->v, PSW->z); break; // Print PSW
      case 'T': printf("CPU Time = %d\n", cpu_time); break;          // Print CPU Time
      case 'M': mainMemoryDump(selection); break;                    // Memory Dump
      case 'C': printCache(); break;                                 // Print Cache
      case 'S': printSettingCommands(); break;                       // Print Debug Options
      case '?': printCommands(); break;                              // Print Command Options
      case 'Q': break;                                               // Quit
      /* Settings Commands */
      case '1': writeToMemory(); break;                              // Write to Memory
      case '2': updatePC(); break;                                   // Change PC
      case '3': switchStepMode(); break;                             // Switch Step Modes
      case '4': switchCacheMode(); break;                            // Switch Cache Modes
      case '5': switchReplPols(); break;                             // Switch Replacement Policies
      case '6': printf("State = %d, TC = %d, FC = %d\n", CEX_->state, CEX_->TC, CEX_->FC); break; // Print CEX State
      
      /* Unexpected Command */
      default:
        printf("Unexpected input. Please try again.\n"); 
        printCommands();
      break;
    }
  }
  return 0;
}