/*
  Computer Architecture Emulator v.0.4.0
  Copyright McMoney Engineering LTD. 
  June 12, 2023
  - Create test programs for V & V
*/

/* Include Header Files */
#include "loader.h"
#include "emulator.h"
#include "cpu.h"

/* globals */
word_byte regFile REG_SIZE = {
  {0, 0, 0, 0, 0, 0, 0, 0},     // Register Values
  {0, 1, 2, 4, 8, 16, 32, -1}                                           // Constant Values
};

mem memory[MEM_SIZE];           // declare memory
unsigned short MDR, MAR, IR;    // declare Mem Data Reg, Mem Address Reg, and Instruction Register
int cpu_time = 0;               // declare cpu clock

int main(){
  program* baseProgram = malloc(sizeof(program)); // create base program for stack

  //unsigned char PSW;              // Control Register -> |-|-|-|-|V|N|Z|C|
  int programCount = 0;
  char selection = 1;
  unsigned short temp, temp2;
  unsigned char temp_byte;
  char fileName[MAX_FILE_NAME_LENGTH];
  char programName[MAX_FILE_NAME_LENGTH];
  unsigned short programStartingLocation;

  /* Main Loop */
  while(toupper(selection) != '\0'){

    /* Print Options */
    printf("\n");
    if(baseProgram->next != NULL) printf("[R] Run Program  ");
    printf("[L] Load File  [M] Print Memory Range  [F] Print Register File  [W] Write to Memory  [Q] Quit -> ");
    scanf("%c", &selection); getchar(); printf("\n");

    /* Option Switch Statement */
    switch(toupper(selection)){

      /* Run a Program */
      case 'R':
        if(baseProgram->next == NULL){    /* Check a program has been loaded */
          printf("No programs have been loaded to memory.\n");
          break;
        }

        /* Promt user to Select Program */
        printf("Select Program to Run:\n");               // Option Message
        printProgramOptions(baseProgram->next);           // Print list of loaded programs
        scanf("%c", &selection); getchar(); printf("\n"); // Get program selection

        /* Run Program */
        runProgram(baseProgram->next, selection-48, &PC); // Set PC to program's entry point
        //printf("PC = %04X\n", PC.word); // test
        // selection = 'A'; // reset selection so program doesnt carry A
      break; // end of run

      /* Load a Program */
      case 'L':
        printf("Enter File Name: ");                                // Prompt user for file name
        scanf("%s", fileName); getchar();                           // Read file name from terminal  
        if(load(fileName, &programStartingLocation, programName))   // Load file with that name
          pushProgram(baseProgram, createProgram(programName, programStartingLocation));
      break; // end of load
      
      case 'M': memoryDump(); break;        // Memory Dump
      case 'Q': selection = '\0'; break;    // Quit
      case 'F': printRegisterFile(); break; // Print Register File
      case 'W': writeToMemory(); break;     // Write to Memory
      default: printf("Unexpected input. Please try again.\n"); break;  // Unexpected Input
    }
  }
  return 0;
}