#include "emulator.h"

extern mem memory;

/* Bus Function */
void bus(unsigned short MAR, unsigned short* MDR, int readORwrite, int wordORbyte){
 switch(readORwrite){
  case READ:
  switch(wordORbyte){
    case WORD: *MDR = memory.word[MAR >> 1]; break; 
    case BYTE: *MDR = memory.byte[MAR] & 0xFF; break;  
  } break;
  case WRITE:
  switch(wordORbyte){
    case WORD: memory.word[MAR >> 1] = *MDR; break;  
    case BYTE: memory.byte[MAR] = *MDR & 0xFF; break;
  } break;
 }
}

/* Prints List of Programs */
void printProgramOptions(program* program){
  int i = 0;
  while(1){
  printf("[%d] %s ", i, program->name);
  if(program->next == NULL){
    printf(" -> ");
    return;
  } else{
    program = program->next;
  }
  i++;
  }
  printf("\n");
}

/* Memory Dump */
void memoryDump(){
  unsigned short bound1, bound2, ubound, lbound;
  printf("Enter upper and lower bounds in hex -> ");
  scanf("%4hX %4hX", &bound1, &bound2); getchar();

  /* Set Lower and Upper Bounds */
  if(bound1 > bound2){
    lbound = bound2;
    ubound = bound1;
  } else{
    lbound = bound1;
    ubound = bound2;
  }

  /* Loop through memory and print */
  while(lbound <= ubound){
    printf("\n%04X: ", lbound);
    for(int i = 0; i < 0x10; i++){
      printf("%02X ", memory.byte[lbound]);
      lbound++;
    }
  }
  printf("\n");
}

/* Print Register File */
void printRegisterFile(){
  for(int i = 0; i < 8; i++) printf("R[%d] = 0x%04X\n", i, regFile[REG][i].word);
}

/* Manually Write to Memory */
void writeToMemory(){
  unsigned short location, data;
  unsigned char word_or_byte;
  //`word_byte temp_byte;
  
  printf("Enter memory location -> "); scanf("%4hx", &location); getchar();
  printf("Write as [W] or [B] -> "); scanf("%c", &word_or_byte); getchar();
  printf("Enter Data -> ");
  if(toupper(word_or_byte) == 'B'){
    scanf("%2hx", &data); getchar();
    bus(location, &data, WRITE, BYTE);
  } else if (toupper(word_or_byte) == 'W'){
    scanf("%4hx", &data); getchar();
    bus(location, &data, WRITE, WORD);
  } else{
    printf("Error - unknown command, expected word or byte\n");
  }      
}