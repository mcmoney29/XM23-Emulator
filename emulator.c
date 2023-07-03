#include "emulator.h"

/* External Globals */
extern program* baseProgram;
extern PSW_Bits* PSW;
extern int cpu_time;
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

/* Main Function that runs Programs */
void runProgram(){
  /* Check a program has been loaded */
  if(baseProgram->next == NULL){
    printf("No programs have been loaded to memory.\n");
    return;
  }

  /* Initalize Variables */
  int programNumber, tickSpeed;
  unsigned short bound1, bound2;      // Bounds for memoryDump();
  char command[MAX_CMND_LENGTH];
  program* current = baseProgram -> next;
  ctrl_c_fnd = 0;

  /* Promt user to Select Program */
  printf("Select Program to Run:\n");                      // Option Message
  printProgramOptions();                                   // Print list of loaded programs
  scanf("%d", &programNumber); getchar(); printf("\n");    // Get program selection

  /* Get to Program */
  for(int i; i < programNumber; i++) current = current->next;
  
  /* Set PC to Program's Starting Address */
  PC.word = current->startingAddress;
  printf("Running program %c%s%c, PC = %04X\n\n", 34, current->name, 34, PC.word);

  /* Prompt for Clock Speed */
  printf("Enter CPU speed in seconds: ");
  scanf("%d", &tickSpeed); getchar();

  /* Init Commands */
  if(tickSpeed) {
    printDebugCommands();
    printf("-> ");
    scanf("%s", command); getchar();
    printf("\n");

    /* Prompt for Memory Range */
    for(int i = 0; i < strlen(command) && i < MAX_CMND_LENGTH; i++){
      if(toupper(command[i]) == 'M'){
        printf("Enter upper and lower bounds in hex -> ");
        scanf("%4hx %4hx", &bound1, &bound2); getchar();
      }
    }
  } else {
    printProgramCommands();
  }

  /* Execution Loop */
  while(!ctrl_c_fnd && command[0] != -1 && PC.word != breakPoint){
    if(!tickSpeed){
      /* Get User Command */
      printf("-> ");
      scanf("%s", command); getchar();
      printf("\n");
    } else {
      tick();
      sleep(tickSpeed);
      if(strlen(command)) printf("\n");
    }
      /* Execute User Command */
      for(int i = 0; i < strlen(command); i++){
        switch(toupper(command[i])){
          case 'P': updatePC();  break;
          case 'C': tick();  break;
          case 'M': memoryDump(&bound1, &bound2, tickSpeed);  break;
          case 'Q': command[0] = -1;  break;
          case 'F': printRegisterFile();  break;
          case 'T': printf("CPU Time = %d\n", cpu_time);  break;
          case 'S': printf("[C|%d] [N|%d] [V|%d] [Z|%d]\n", PSW->c, PSW->n, PSW->v, PSW->z);  break;
          default: 
            printf("Unexpected command, please try again.\n"); 
            printProgramCommands();
          break;
        }
      } 
  }
  ctrl_c_fnd = 0;
}

/* Prints List of Programs */
void printProgramOptions(){
  int i = 0;
  program* current;
  current = baseProgram->next;
  for(;;){
    printf("[%d] %s ", i, current->name);
    if(current->next == NULL){
      printf("\n-> ");          // No more programs, print arrow and return
      return;
    } else{
      current = current->next; // Goto next program
    }
    i++;
  }
}

/* Memory Dump */
void memoryDump(unsigned short * bound1, unsigned short* bound2, int tickSpeed){
  unsigned short ubound, lbound;
  if(!tickSpeed){
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
  }
  printf("\n");
}

void mainMemoryDump(){
  unsigned short bound1, bound2;
  memoryDump(&bound1, &bound2, 0);
}

/* BCD to WORD_BYTE */
word_byte BCD_to_WORDBYTE(BCD_NUM BCD){
  word_byte result;
  result.word = BCD.word;
  return result;
}

BCD_NUM WORDBYTE_to_BCD(word_byte WORDBYTE){
  BCD_NUM result;
  for(int i = 0; i < 4; i++){
    result.nib[i].nib = (WORDBYTE.word >> 4*i) & 0x000F;
  }
  result.word = WORDBYTE.word;
  return result;
}

/* Command Options Print Functions */
void printProgramCommands(){
  printf("[C] CPU Tick\n");
  printf("[P] Update PC\n");
  printf("[W] Write to Memory\n");
  printDebugCommands();
  printf("[Q] Quit\n");
}

void printDebugCommands(){
  printf("[M] Print Memory Range\n");
  printf("[F] Print Register File\n");
  printf("[S] Print Status Word\n");
  printf("[T] Print CPU Time\n");
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

void seletionPrompt(char *selection){
  printf("\n");
  if(baseProgram->next != NULL) printf("[R] Run Program\n");
  printf("[L] Load File\n");
  printf("[M] Print Memory Range\n");
  printf("[F] Print Register File\n");
  printf("[P] Print PSW\n");
  printf("[W] Write to Memory\n");
  printf("[T] Print CPU Time\n");
  printf("[B] Manage Breakpoint\n");
  printf("[Q] Quit\n");
  printf("-> ");
  scanf("%s", selection); getchar();
  printf("\n");
}

void manageBreakpoint(unsigned short* breakPoint){
  unsigned short temp = *breakPoint;
  printf("Enter new breakpoint: ");
  scanf("%4hx", breakPoint); getchar();
  printf("Breakpoint: %04X -> %04X\n", temp, *breakPoint);
}