#include "loader.h"

extern program* baseProgram;

int loadFile(){
  /* Initialize Automatics */
  char* record[MAX_RECORDS];                     // Array of strings (S-Records)
  char fptr[MAX_RECORDS], programName[MAX_RECORDS];              // File Name
  unsigned short i = 0, pgmCnt;                  // Record index
  FILE* infile;                                  // File pointer
  char temp[TEST_PATH_LEN] = "tests/test";       // Test Directory 

  /* Prompt User for File */
  printf("Enter File Name: ");                    // Prompt user for file name
  scanf("%s", fptr); getchar();                   // Read file name from terminal
  
  /* Check for Test File and Open File */
  if(strlen(fptr) == 3 && toupper(fptr[0]) == 'T'){
    strcat(temp, &fptr[1]); strcat(temp, ".xme");
    infile = fopen(temp, "r");                    // Open test file specified
  } else{
    infile = fopen(fptr, "r");                    // Open file passed
  }

  /* Confirm file has been opened */
  if (infile == NULL) {
    printf("File not found.\n");
    fclose(infile);
    return 0;
  }

  /* Allocate memory for each string in the record array */
  for (i = 0; i < MAX_RECORDS; i++) {
    record[i] = malloc(MAX_RECORD_LENGTH * sizeof(char));
    if (record[i] == NULL) {
      printf("Memory allocation error.\n");
      return 0;
    }
  }

  i = 0;                                                                      // Reset the Counter
  while(fscanf(infile, "%s", record[i]) != EOF && i < MAX_RECORDS) i++;       // Read Strings
  fclose(infile);                                                             // Close File
  for(int n = 0; n < i; n++) executeRecord(record[n], &pgmCnt, programName);  // Execute Records
  for(i = 0; i < MAX_RECORDS; i++) free(record[i]);                           // Free the allocated memory
  pushProgram(baseProgram, createProgram(programName, pgmCnt));               // Push Program
  printf("\nLoaded %s\n", programName);                                       // Print Confirmation Message
  return 1;
}

/* Get Type */
unsigned char getType(char buffer[], unsigned char* checkSum){
  unsigned char type = buffer[1] - 48;    // Convert char to interger
  *checkSum = *checkSum + type;           // Add to checksum
  return type;
} 

/* Get Address */
unsigned short getAddress(char buffer[], unsigned char* checkSum){
  unsigned int adrLo, adrHi;
  unsigned short adr;
  sscanf(&buffer[4], "%2X%2X", &adrHi, &adrLo);
  adr = adrHi << 8 | adrLo;
  *checkSum = *checkSum + adrHi + adrLo;
  return adr;
}

/* Get Length */
unsigned char getLength(char buffer[], unsigned char* checkSum){
  unsigned char length;
  sscanf(&buffer[2], "%2x", &length);
  return length;
}

/* Executre Record */
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
  unsigned char data[MAX_RECORD_LENGTH];
  int i = 8, n = 0;             // i = index for buffer, n = index for data array
  int EOD = strlen(buffer) - 2; // define end of data for i
  while(i < EOD && i < MAX_RECORDS){
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
      for(int j = 0; data[j] >= 32 && data[j] <= 126; j++){ // loop while printable
        programName[j] = data[j];
        programName[j+1] = '\0';
      }
    break;
    case 1:
      /* Save Data to Memory */
      for(int i = 0; i < length-3; i++)
        bus(address+i, &data[i], WRITE, BYTE);
    break;
    case 9:
      /* Set Program Counter */
      *pgmCnt = address;
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
  return newProgram;
}

/* Push Program */
void pushProgram(program* base, program* newProgram){
  while(base->next != NULL)   // Get to top
    base = base->next;
  base->next = newProgram;    // Push Program to Top
}