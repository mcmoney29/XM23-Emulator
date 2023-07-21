/*
Thursday, July 20, 2023 - emulator.c
- Function definitions for debugging commands (and bus)
*/

#include "emulator.h"

/* External Globals */
extern PSW_Bits* PSW;
//extern cacheline* cache[CACHE_SIZE];  
extern int cpu_time, stepFlag, loadFlag;
extern char cacheMode;
extern void tick();
extern unsigned short breakPoint;       
extern volatile sig_atomic_t ctrl_c_fnd;


/* Bus Function */
extern mem memory;
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

/* Run Program Function */
void runProgram(){
  int runTime, startTime = cpu_time;

	/* Check that a program has been loaded */
  if(!loadFlag){
    printf("No program has been loaded!\n");
    return;
  }

	/* Complete CPU Cycles based on Step Mode */
  if(stepFlag){
		/* Single Step */
    tick();
  } else{
    /* Continious */
    while(!ctrl_c_fnd && PC.word != breakPoint) tick();
  }

	/* Calc and Print Process Information */
  runTime = cpu_time - startTime;
  printf("Ran for %d CPU clock cycle", runTime);
  if(runTime > 1) printf("s");
  printf(", PC = %04X\n", PC.word);
  ctrl_c_fnd = 0;		// reset ctrl_c flag
}

/* Memory Dump */
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

void mainMemoryDump(char* command){
  unsigned short bound1, bound2;
  int length = strlen(command);

	switch(length){
		case 1: // len = 1(M)
			memoryDump(&bound1, &bound2, 0);
		break;
		case 6: // len = 1(M) + 1(Space) + 4(Bound)
			sscanf(&command[2], "%4hx", &bound1);
			memoryDump(&bound1, &bound1, 1);
		break;
		case 11: // len = 1(M) + 1(Space) + 4(Bound) + 1(Space) + 4(Bound)
			sscanf(&command[2], "%4hx %4hx", &bound1, &bound2);
			memoryDump(&bound1, &bound2, 2);
		break;
		default:
		printf("error\n");
		break;
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
  printf("[R] Run Program\n");
  printf("[L] Load File\n");
  printf("[P] Update PC\n");
	printf("[B] Manage Breakpoint\n");
	printf("[M] Write to Memory\n");
  printf("[S] Switch Step Mode\n");
  printf("[C] Change Cache Modes\n");
	printf("[D] Print Debugging Commands\n");
  //printDebugCommands();
  printf("[?] Display Command Options\n");
  printf("[Q] Quit\n");
}
void printDebugCommands(){
  printf("[1] Memory Dump\n");
  printf("[2] Print Register File\n");
  printf("[3] Print Status Word\n");
  printf("[4] Print CPU Time\n");
  printf("[5] Print Cache\n");
}

/* Update PC*/
void updatePC(){
  unsigned short temp = PC.word;                   // Save old PC
  printf("Enter new PC in hex: ");                 // Prompt user for new PC                              // Save currnet PC to temp    
  scanf("%4hx", &PC.word); getchar();              // Get new PC
  printf("PC: [%04X] -> [%04X]\n", temp, PC.word); // Print old PC and new PC
}

/* Print Register File */
void printRegisterFile(){
  for(int i = 0; i < 8; i++) 
  printf("R[%d] = 0x%04X\n", i, Rx(i).word);
}

/* Manually Write to Memory */
void writeToMemory(){
  unsigned short location, data;
  unsigned char word_or_byte;
  
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
	fgets(command, MAX_CMND_LENGTH, stdin);
	int len = strlen(command);
  if(len > 0 && command[len - 1] == '\n'){
		command[len - 1] = '\0';	// replace newline char with null char
	}
	
	// scanf("%s", command); getchar();

  printf("\n");
}

/* Manage Breakpoint */
void manageBreakpoint(unsigned short* breakPoint){
  unsigned short temp = *breakPoint;
  printf("Enter new breakpoint: ");
  scanf("%4hx", breakPoint); getchar();
  printf("Breakpoint: %04X -> %04X\n", temp, *breakPoint);
}



/* Switches CPU Step Modes (Continious/Single Step)*/
void switchStepMode(){
  stepFlag = ~stepFlag;
  if(stepFlag){
    printf("Now in Single-Step Mode\n");
  } else{
    printf("Now in Continious Mode\n");
  }
}

/* Prompt user for Cache Mode and Change Global */
void switchCacheMode(){
	printf("Select Cache Mode:\n");
	printf("[1] - Accociative Mapping\n");
	printf("[2] - Direct Mapping\n");
	printf("[3] - Hybrid Mapping\n-> ");

	cacheMode = 0;
	while(cacheMode < '1' || cacheMode > '3'){
		scanf("%c", &cacheMode); getchar();
		switch(cacheMode){
			case '1': printf("Now using Accoicative Mapping\n"); break;
			case '2': printf("Now using Direct Mapping\n"); break;
			case '3': printf("Now using Hybrid Mapping\n"); break;
			default: printf("Unknown input, please try again\n"); break;
		}
	}
}