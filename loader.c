#include "loader.h"

int load(char* fptr, unsigned short* pgmCnt, char* programName){
  /* Initialize Automatics */
  char* record[MAX_RECORDS];          // Array of strings (S-Records)
  int i = 0;                          // Record index
  FILE* infile;                       // File pointer
  infile = fopen(fptr, "r");          // Open file passed

  /* Confirm file has been opened */
  if (infile == NULL) {
    printf("File not found.\n");
    fclose(infile); // needed?
    return 0; // Exit the function
  }

  /* Allocate memory for each string in the record array */
  for (i = 0; i < MAX_RECORDS; i++) {
    record[i] = malloc(MAX_LENGTH * sizeof(char));
    if (record[i] == NULL) {
      printf("Memory allocation error.\n");
      return 0; // Exit the function
    }
  }

  i = 0;                                                                  // Reset the Counter
  while (fscanf(infile, "%s", record[i]) != EOF && i < MAX_RECORDS) i++;  // Read Strings
  fclose(infile);                                                         // Close File

  /* Execute Records */
  for(int n = 0; n < i; n++) executeRecord(record[n], pgmCnt, programName);

  /* Free the allocated memory */
  for(i = 0; i < MAX_RECORDS; i++) free(record[i]);

  /* Print Confirmation Message */
  printf("\nLoaded %s\n", programName);
  return 1;
}

/* Basic Functions */
unsigned char getType(char buffer[], unsigned char* checkSum){
  unsigned char type = buffer[1] - 48;
  *checkSum = *checkSum + type;
  return type;
} // keep

unsigned short getAddress(char buffer[], unsigned char* checkSum){
  unsigned int adrLo, adrHi;
  unsigned short adr;
  sscanf(&buffer[4], "%2X%2X", &adrHi, &adrLo);
  adr = adrHi << 8 | adrLo;
  *checkSum = *checkSum + adrHi + adrLo;
  return adr;
} // keep

unsigned char getLength(char buffer[], unsigned char* checkSum){
  unsigned char length;
  sscanf(&buffer[2], "%2s", &length);
  return length;
} // keep

void executeRecord(char buffer[], unsigned short* pgmCnt, char* programName){
  unsigned char checkSum = 0;
  /* Check for 'S' */
  if(buffer[0] != 'S'){
    printf("Invald S-Record\n");
    return;
  }
  
  int type = getType(buffer, &checkSum);                  // Get Type
  unsigned char length = getLength(buffer, &checkSum);    // Get Length
  unsigned short address = getAddress(buffer, &checkSum); // Get Address

  /* Get Data */
  unsigned char data[MRL];
  int i = 8, n = 0;           // i = index for buffer, n = index for data array
  int EOD = (length + 2) << 1;  // define end of data for i
  while(i < EOD){
    sscanf(&buffer[i], "%2hx", &data[n]);
    checkSum = checkSum + data[n];
    n++;
    i = i + 2;
  }

  /* Check Sum */
  unsigned char checkSumByte;
  sscanf(&buffer[i], "%2s", &checkSumByte);
  checkSum = checkSum + checkSumByte;
  if(!(checkSum = 0xFF)){
    printf("Invalid record\n");
    return;
  }

  /* Execute Switch stmt */
  switch(type){
  case 0:
    /* Save Program Name */
    for(int i = 0; data[i] >= 32 && data[i] <= 126; i++){
      programName[i] = data[i];
      //printf("Set programName[%d](%c - %d), to data[%d](%c - %d)\n", i, programName[i], programName[i], i, data[i], data[i]);
    }
    programName[i] = '\0';
    break;
  case 1:
    /* Save Data to Memory */
    for(int i = 0; i < length-3; i++) 
      //memory.byte[address+i] = data[i];
      bus(address+i, &data[i], WRITE, BYTE);
    break;
  case 9:
    /* Set Program Counter */
    *pgmCnt = address;
    break;
  default:
    break;
  }
  return;
}

program* createProgram(char* name, unsigned short address){
  program* newProgram = malloc(sizeof(program));
  int i = 0;
  while(name[i] != '\0'){
    newProgram->name[i] = name[i];
    i++;
  }
  newProgram->name[i] = '\0';
  newProgram->startingAddress = address;
  newProgram->next = NULL;
  //printf("Created program [Address: %04X, Name: %s]", newProgram->startingAddress, newProgram->name);
  return newProgram;
}

void pushProgram(program* base, program* newProgram){
  /* Get top of base */
  while(base->next != NULL){
    base = base->next;
  }
  /* Push new program */
  base->next = newProgram;
  //printf("Pushed new program (%s,%04X)\n",newProgram->name, newProgram->startingAddress);
}