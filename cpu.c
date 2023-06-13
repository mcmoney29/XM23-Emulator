#include "cpu.h"

extern mem memory;
extern int cpu_time;

/* Tick */
void tick(){
  fetch();
  decode();
  // execute(); happens in decode now - elegancy??
  // cpu_time++;   /* Increment CPU Timer??? */
}

/* Fetch, Decode, Execute */

void fetch() {
  bus(PC.word, &IR, READ, WORD);
  PC.word = PC.word + 2;
}

void decode(){
  printf("Decoding %04X", IR);
  unsigned char argumentCount;
  unsigned short argument[MAX_OPERANDS];

  switch((IR >> 13) & 0x07){
    case 0: /* BL */
      argument[0] = BL_G;
      argument[1] = offset(IR, 13);
      argumentCount = 2;
    break;
    case 1: BEQ_to_BRA(argument, &argumentCount); break;
    case 2: ADD_to_ST(argument, &argumentCount); break;
    case 3: MOVL_to_MOVH(argument, &argumentCount); break;
    case 4: LDR(argument, &argumentCount); break;
    case 5: LDR(argument, &argumentCount); break;
    case 6:
      /* STR */
    break;
    case 7:
      /* STR */
    break;
    default: printf("Unable to decode :(\n"); break;
  }
  execute(argument, argumentCount);
}

void execute(unsigned short argument[], unsigned char argumentCount){
  printf("\nExecuting... \n");
  for(int i = 0; i < argumentCount; i++){
    printf("Argument[%d] = %04X\n\n", i, argument[i]);
  }

  switch (argument[0]) {
    /******************************************
    - BL
    *****************************************/
        case BL_G:
            // Code for BL_G instruction
            break;
    /******************************************
    - BEQ
    - BNE
    - BC
    - BNC
    - BN
    - BGE
    - BLT
    - BRA
    ******************************************/
        case BEQ_G:
            // Code for BEQ_G instruction
            break;
        case BNE_G:
            // Code for BNE_G instruction
            break;
        case BC_G:
            // Code for BC_G instruction
            break;
        case BNC_G:
            // Code for BNC_G instruction
            break;
        case BN_G:
            // Code for BN_G instruction
            break;
        case BGE_G:
            // Code for BGE_G instruction
            break;
        case BLT_G:
            // Code for BLT_G instruction
            break;
        case BRA_G:
            // Code for BRA_G instruction
            break;
    /******************************************
    - ADD
    - ADDC
    - SUB
    - SUBC
    - DADD
    - CMP
    - XOR
    - AND
    - BIT
    - BIC
    - BIS
    ******************************************/
        case ADD_G:
            // Code for ADD_G instruction
            break;
        case ADDC_G:
            // Code for ADDC_G instruction
            break;
        case SUB_G:
            // Code for SUB_G instruction
            break;
        case SUBC_G:
            // Code for SUBC_G instruction
            break;
        case DADD_G:
            // Code for DADD_G instruction
            break;
        case CMP_G:
            // Code for CMP_G instruction
            break;
        case XOR_G:
            // Code for XOR_G instruction
            break;
        case AND_G:
            // Code for AND_G instruction
            break;
        case OR_G:
            // Code for OR_G instruction
            break;
        case BIT_G:
            // Code for BIT_G instruction
            break;
        case BIC_G:
            // Code for BIC_G instruction
            break;
        case BIS_G:
            // Code for BIS_G instruction
            break;
    /******************************************
    - MOV
    - SWAP
    - SRA
    - RRC
    - COMP
    - SWPB
    - SXT
    - SETPRI
    - SVC
    - SETCC
    -CLRCC
    ******************************************/
        case MOV_G:
            // Code for MOV_G instruction
            break;
        case SWAP_G:
            // Code for SWAP_G instruction
            break;
        case SRA_G:
            // Code for SRA_G instruction
            break;
        case RRC_G:
            // Code for RRC_G instruction
            break;
        case COMP_G:
            // Code for COMP_G instruction
            break;
        case SWPB_G:
            // Code for SWPB_G instruction
            break;
        case SXT_G:
            // Code for SXT_G instruction
            break;
        case SETPRI_G:
            // Code for SETPRI_G instruction
            break;
        case SVC_G:
            // Code for SVC_G instruction
            break;
        case SETCC_G:
            // Code for SETCC_G instruction
            break;
        case CLRCC_G:
            // Code for CLRCC_G instruction
            break;
    /******************************************
    - CEX
    ******************************************/
        case CEX_G:
            // Code for CEX_G instruction
            break;

    /******************************************
    - LD -> working as of lab 3
    - ST
    ******************************************/
    case LD_G:
      /* Code for LD_G instruction */
      switch(argument[1]){
        case 0:
          /* Unmodified Register [R] */
          if(argument[2]) // if byte instruction
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
          else            // if word instruction
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
        break; /* [R] */
        case 1:
          /* Post Increment [R+] */
          if(argument[2]){ // if byte instruction
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
            Rx(argument[3]).word++; 
          } else{          // if word instruction
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
            Rx(argument[3]).word = Rx(argument[3]).word + 2;
          }
        break; /* [R+] */
        case 2:
          /* Post Decrement [R-] */
          if(argument[2]){ // if byte instruction
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
            Rx(argument[3]).word--; 
          } else{          // if word instruction
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
            Rx(argument[3]).word = Rx(argument[3]).word - 2;
          } /* [R-] */
        break;
        case 5:
          /* Pre Increment [+R] */
          if(argument[2]){ // if byte instruction
            Rx(argument[3]).word++; 
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
          } else{          // if word instruction
            Rx(argument[3]).word = Rx(argument[3]).word + 2;
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
          }
        break;
        case 6:
          /* Pre Decrement [-R] */
          if(argument[2]){ // if byte instruction
            Rx(argument[3]).word--; 
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
          } else{          // if word instruction
            Rx(argument[3]).word = Rx(argument[3]).word - 2;
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
          }
        break;
      }
    break;
        case ST_G:
            // Code for ST_G instruction
            break;

    /******************************************
    - MOVL  ->  working as of lab 3
    - MOVLZ ->          "
    - MOVLS ->          "
    - MOVH  ->          "
    ******************************************/
    case MOVL_G:
      regFile[REG][argument[2]].byte[0] = argument[1];
    break;
    case MOVLZ_G:
      regFile[REG][argument[2]].byte[0] = argument[1];
      regFile[REG][argument[2]].byte[1] = 0x00;
    break;
    case MOVLS_G:
      regFile[REG][argument[2]].byte[0] = argument[1];
      regFile[REG][argument[2]].byte[1] = 0xFF;
    break;
    case MOVH_G:
      regFile[REG][argument[2]].byte[1] = argument[1];
    break;

    /******************************************
    - LDR
    - STR
    ******************************************/
    case LDR_G:
      /* Code for LDR_G instruction */
      if(argument[2])   // BYTE
        Rx(argument[4]).byte[0] = memory.byte[Rx(argument[3]).byte[0]]; 
      else              // WORD
        Rx(argument[4]).word = memory.word[Rx(argument[3]).word>>1];
    break;
        case STR_G:
            // Code for STR_G instruction
            break;
        default:
            // Code for handling unknown instruction
            break;
    }



}

void printArithmetic(char* mnemonic, unsigned char RC_Flag, unsigned char WB_Flag, unsigned short source, unsigned short desination){
  printf("Instruction: %s\n", mnemonic);
  printf("R/C: %d\n", RC_Flag);
  printf("W/B: %d\n", WB_Flag);
  printf("Source: %d%d%d\n", (source>>2)&1,(source>>1)&1, source&1);
  printf("Destination: %d%d%d\n", (desination>>2)&1,(desination>>1)&1, desination&1);
}

void printBranch(char* mnemonic, short branch_offset, unsigned short PC_Current, unsigned short PC_Branch){
  printf("Instruction %s\n", mnemonic);
  printf("Encoded Offset: %04X\n", branch_offset);
  printf("Current PC: %04X\n", PC_Current);
  printf("Branch PC: %04X\n", PC_Branch);
}

void runProgram(program* program, int programNumber, word_byte* prgmCntr){
  int i = 0;
  char selection = 1;
  unsigned short temp;
  while(i < programNumber){
    program = program->next;
    i++;
  }
  prgmCntr->word = program->startingAddress;
  printf("Running program %c%s%c, PC = %04X\n", 34, program->name, 34, program->startingAddress);
  while(toupper(selection) != -1){
    /* Get User Command */
    printf("\n[C] Continue  [P] Change PC  [M] Print Memory Range  [F] Print Register File  [Q] Terminate Program -> ");
    scanf("%c", &selection); getchar(); printf("\n");

    /* Execute User Command */
    switch(toupper(selection)){
      case 'P':
        printf("Enter new PC in hex: ");                 // Prompt user for PC
        temp = PC.word;                                  // Save currnet PC to temp    
        scanf("%4hx", &PC.word); getchar();              // Get new PC
        printf("PC: [%04X] -> [%04X]\n", temp, PC.word); // Print old PC and new PC
      break;
      case 'C': tick(); break;
      case 'M': memoryDump(); break;
      case 'Q': selection = -1; break;
      case 'F': printRegisterFile(); break;
      default: printf("Unexpected command, please try again.\n"); break;
    }
  }
}