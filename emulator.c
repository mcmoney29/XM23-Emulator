/*
Tuesday, August 1, 2023 - emulator.c
- Function definitions for debugging/emulation commands
*/

#include "emulator.h"

/* External Globals */
extern PSW_Bits* PSW;
extern int cpu_time;
extern char cacheMode, stepFlag, loadFlag, replPol;
extern void tick();
extern unsigned short breakPoint;       
extern volatile sig_atomic_t ctrl_c_fnd;
extern mem memory;

/* Run Program Function */
void runProgram(){
  int runTime, startTime = cpu_time;

	/* Check that a program has been loaded */
  if(!loadFlag){
    printf("No program has been loaded!\n");
    return;
  }

	/* Complete CPU Cycles based on Step Mode */
  if(stepFlag) tick(); /* Single Step */
  else do{tick();} while(!ctrl_c_fnd && PC.word != breakPoint); /* Continious */

	/* Calc and Print Process Information */
  runTime = cpu_time - startTime;
  printf("Ran for %d CPU clock cycles, PC = %04X\n", runTime, PC.word);

  /* Reset ctrl_c State*/
  ctrl_c_fnd = 0;
}

/* Memory Dump */
void mainMemoryDump(char* command){
  unsigned short bound1, bound2;
  int length = strlen(command);

	switch(length){
		case 6: // len = 1(M) + 1(Space) + 4(Bound) = 6
			sscanf(&command[2], "%4hx", &bound1);
			memoryDump(&bound1, &bound1, 1);
		break;
		case 11: // len = 1(M) + 1(Space) + 4(Bound) + 1(Space) + 4(Bound) = 11
			sscanf(&command[2], "%4hx %4hx", &bound1, &bound2);
			memoryDump(&bound1, &bound2, 2);
		break;
		default:
		  memoryDump(&bound1, &bound2, 0);
		break;
	}
}
void memoryDump(unsigned short * bound1, unsigned short* bound2, int boundCount){
  unsigned short ubound, lbound;
  
	if(!boundCount){
  	printf("Enter upper and lower bounds in hex -> ");
  	scanf("%4hX %4hX", bound1, bound2); getchar();
	}
  
  /* Set Lower and Upper Bounds */
  if(*bound1 > *bound2){
    lbound = *bound2;
    ubound = *bound1;
  } else{
    lbound = *bound1;
    ubound = *bound2;
  }

  /* Loop through memory and print */
  while(lbound <= ubound){
    printf("%04X: ", lbound);
    for(int i = 0; i < 0x10; i++){
      printf("%02X ", memory.byte[lbound]);
      lbound++;
    }
		printf("\n");
  }
}

/* BCD to WORD_BYTE */
word_byte BCD_to_WORDBYTE(BCD_NUM BCD){
  word_byte result;
  result.word = BCD.word;
  return result;
}

/* WORD_BYTE to BCD */
BCD_NUM WORDBYTE_to_BCD(word_byte WORDBYTE){
  BCD_NUM result;
  for(int i = 0; i < 4; i++){
    result.nib[i].nib = (WORDBYTE.word >> 4*i) & 0x000F;
  }
  result.word = WORDBYTE.word;
  return result;
}

/* Command Options Print Functions */
void printCommands(){
  printf("[G] Go (Cycle CPU)\n");
  printf("[L] Load File\n");
	printf("[B] Manage Breakpoint\n");
  printf("[R] Print Register File\n");
  printf("[P] Print Program Status Word\n");
  printf("[T] Print CPU Time\n");
  printf("[M] Memory Dump\n");
  printf("[C] Print Cache\n");
	printf("[S] More Settings\n");
  printf("[?] Display Command Options\n");
  printf("[Q] Quit\n");
}
void printSettingCommands(){
	printf("[1] Write to Memory\n");
  printf("[2] Update PC\n");
  printf("[3] Switch Step Mode\n");
  printf("[4] Change Cache Modes\n");
  printf("[5] Switch Replacement Policy\n");
  printf("[6] Print CEX State\n");
}

/* Update PC*/
void updatePC(){
  unsigned short temp = PC.word;                   // Save old PC
  printf("Enter new PC in hex: ");                 // Prompt user for new PC  
  scanf("%4hx", &PC.word); getchar();              // Get new PC
  printf("PC: [%04X] -> [%04X]\n", temp, PC.word); // Print old PC and new PC
}

/* Print Register File */
void printRegisterFile(){
  for(int i = 0; i < REG_COUNT; i++) printf("R[%d] = 0x%04X\n", i, Rx(i).word);
}

/* Manually Write to Memory */
void writeToMemory(){
  unsigned short location, data;
  unsigned char word_or_byte;
  
  printf("Enter memory location -> "); scanf("%4hx", &location); getchar();
  printf("Write as [W] or [B] -> "); scanf("%c", &word_or_byte); getchar();
  printf("Enter Data -> ");

  if(toupper(word_or_byte) == 'B'){
    scanf("%2hx", &data); getchar();        // Scan Byte
    bus(location, &data, WRITE, BYTE_);     // Save byte
  } else if (toupper(word_or_byte) == 'W'){
    scanf("%4hx", &data); getchar();        // Scan word
    bus(location, &data, WRITE, WORD_);     // Save word
  } else{
    printf("Error - unknown command, expected word or byte\n");
  }      
}

/* Control-C Handler*/
void sigint_hdlr(){
  ctrl_c_fnd = 1;
  signal(SIGINT, sigint_hdlr);
}

/* Prompt the user for a command */
void commandPrompt(char * command){
  printf("\n");
  printf("-> ");

	/* Scan New Command */
	fgets(command, MAX_CMND_LENGTH, stdin); // Save command string
	int len = strlen(command);
  if(len > 0 && command[len - 1] == '\n'){
		command[len - 1] = '\0';	            // Replace newline char with null char
	}
  printf("\n");
}

/* Manage Breakpoint */
void manageBreakpoint(unsigned short* breakPoint){
  unsigned short temp = *breakPoint;
  printf("Enter new breakpoint: ");
  scanf("%4hx", breakPoint); getchar();
  printf("Breakpoint: %04X -> %04X\n", temp, *breakPoint);
}

/* Switches CPU Step Modes (Continuous/Single Step)*/
void switchStepMode(){
  stepFlag = ~stepFlag;
  if(stepFlag){
    printf("Now in Single-Step Mode\n");
  } else{
    printf("Now in Continuous Mode\n");
  }
}

/* Switches Cache Modes */
void switchCacheMode(){
  cacheMode = ~cacheMode;
  if(cacheMode){
    printf("Now using Associative Mapping\n");
  } else{
    printf("Now using Direct Mapping\n");
  }
}

/* Switched Replacement Policies */
void switchReplPols(){
  replPol = ~replPol;
  if(replPol){
    printf("Replacement Policy is now Write-Back\n");
  } else{
    printf("Replacement Policy is now Write-Through\n");
  }
}

unsigned short PSW_to_WORD(PSW_Bits* PSW){
  unsigned short temp = (PSW->prev << 13) + (PSW->fault << 8) + (PSW->current << 5) + (PSW->v << 4) 
  + (PSW->slp << 3) + (PSW->n << 2) + (PSW->z << 1) + PSW->c;
  return temp;
}

void WORD_TO_PSW(PSW_Bits* PSW, unsigned short word){
  PSW->c = Bn(0, word);
  PSW->z = Bn(1, word);
  PSW->n = Bn(2, word);
  PSW->slp = Bn(3, word);
  PSW->v = Bn(4, word);
  PSW->current = (word >> 5) & 0x07;
  PSW->fault = Bn(8, word);
  PSW->prev = (word >> 13) & 0x07;
}