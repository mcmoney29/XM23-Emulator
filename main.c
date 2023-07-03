/*
  Computer Architecture Emulator v.1.0.0
  Copyright McMoney Engineering LTD. 
  July 1, 2023
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

mem memory[MEM_SIZE];             // declare memory
unsigned short MDR, MAR, IR;      // declare Mem Data Reg, Mem Address Reg, and Instruction Register
int cpu_time = 0;                 // declare cpu clock
program* baseProgram;             // Program Structure to use as base for list of loaded programs
PSW_Bits* PSW;
volatile sig_atomic_t ctrl_c_fnd; // Boolean that indicates whether ^C detected
unsigned short breakPoint;

int main(){
  /* Initalize Variables */
  baseProgram = malloc(sizeof(program));     // Create base program for stack
  PSW = malloc(sizeof(PSW_Bits));            // Allocate Memory for PSW.
  char loopFlag = TRUE, selection[256];

  /* Initalize Control-C Software */
  ctrl_c_fnd = 0;
  signal(SIGINT, sigint_hdlr);

  /* Main Loop */
  while(loopFlag){
    seletionPrompt(selection);
    for(int i = 0; i < strlen(selection); i++){
      switch(toupper(selection[i])){
        case 'R': runProgram(); break;                                    // Run Program
        case 'L': loadFile(); break;                                      // Load File
        case 'M': mainMemoryDump(); break;                                    // Memory Dump
        case 'Q': loopFlag = 0; break;                                    // Quit
        case 'F': printRegisterFile(); break;                             // Print Register File
        case 'W': writeToMemory(); break;                                 // Write to Memory
        case 'T': printf("CPU Time = %d\n", cpu_time); break;             // Print CPU Time
        case 'P': printf("[C|%d] [N|%d] [V|%d] [Z|%d]\n", PSW->c, PSW->n, PSW->v, PSW->z); break; // Print PSW
        case 'B': manageBreakpoint(&breakPoint);  break;
        default: printf("Unexpected input. Please try again.\n"); break;  // Unexpected Input
      }
    }
  }
  return 0;
}