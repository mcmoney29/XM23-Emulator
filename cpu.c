#include "cpu.h"

extern mem memory;
extern int cpu_time;
extern PSW_Bits* PSW;

/* Tick */
void tick(){
  fetch();
  decode();
  // execute(); happens in decode now
  // cpu_time++;   /* Increment CPU Timer??? */
}

/* Fetch, Decode, Execute */

void fetch() {
  bus(PC.word, &IR, READ, WORD);
  PC.word = PC.word + 2;
}

void decode(){
  printf("Decoding %04X", IR);
  unsigned short argument[MAX_OPERANDS];

  switch((IR >> 13) & 0x07){
    case 0: decode_BL(argument); break;
    case 1: decode_BEQ_to_BRA(argument); break;
    case 2: decode_ADD_to_ST(argument); break;
    case 3: decode_MOVL_to_MOVH(argument); break;
    case 4: case 5: decode_LDR(argument); break;
    case 6: case 7: decode_STR(argument); break;
    default: printf("Unable to decode instruction 0x%04X\n", IR); break;
  }
  execute(argument);
}

void execute(unsigned short argument[]){
  switch (argument[0]) {
    /******************************************
    - BL
    *****************************************/
    case BL_G: PC.word += argument[1];
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
    case BEQ_G: PSW->z == 1 ? PC.word += argument[1] : 0; break;   // (PSW->z)
    case BNE_G: PSW->z == 0 ? PC.word += argument[1] : 0; break;   // !(PSW->z)
    case BC_G:  PSW->c == 1 ? PC.word += argument[1] : 0; break;
    case BNC_G: PSW->c == 0 ? PC.word += argument[1] : 0; break;
    case BN_G:  PSW->n == 1 ? PC.word += argument[1] : 0; break;
    case BGE_G: (PSW->n ^ PSW->v) == 0 ? PC.word += argument[1] : 0; break;
    case BLT_G: (PSW->n ^ PSW->v) == 1 ? PC.word += argument[1] : 0; break;
    case BRA_G: PC.word += argument[1]; break;
    /******************************************
    Arithmetic Instructions
    Arguments: #1 = REG/CON  #2 = Word/Byte  #3 = Source  #4 = Destination
    - ADD
    - ADDC
    - SUB   -> Need to fix ADD_SUB() to take value in not register # so we can send in
    - SUBC  -> 1's compliments values to SUB and SUBC
    - DADD
    - CMP
    - XOR
    - AND
    - BIT
    - BIC
    - BIS
    
    ******************************************/
        case ADD_G:  ADD_SUB(argument[4], regFile[argument[1]][argument[3]], argument[2], 0);      break;
        case ADDC_G: ADD_SUB(argument[4], regFile[argument[1]][argument[3]], argument[2], PSW->c); break;
        case SUB_G:  ADD_SUB(argument[4], regFile[argument[1]][argument[3]], argument[2], 0);      break;
        case SUBC_G: ADD_SUB(argument[4], regFile[argument[1]][argument[3]], argument[2], PSW->c); break;
        case DADD_G: DADD_Func(argument[4],regFile[argument[1]][argument[3]], argument[2]);        break;
        case CMP_G:  CMP_Func(argument[4], regFile[argument[1]][argument[3]], argument[2]);        break;
        case XOR_G:  XOR_Func(argument[4], regFile[argument[1]][argument[3]], argument[2]);        break;
        case AND_G:  AND_Func(argument[4], regFile[argument[1]][argument[3]], argument[2]);        break;
        case OR_G:   OR_Func(argument[4], regFile[argument[1]][argument[3]], argument[2]);         break;
        case BIT_G:  BIT_Func(argument[4], regFile[argument[1]][argument[3]], argument[2]);        break;
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
    - CLRCC
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
      switch(argument[1]){
        case 0: /* Unmodified Register [R] */          
          if(argument[2]) // if byte instruction
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
          else            // if word instruction
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
        break; /* [R] */
        case 1: /* Post Increment [R+] */          
          if(argument[2]){ // if byte instruction
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
            Rx(argument[3]).word++; 
          } else{          // if word instruction
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
            Rx(argument[3]).word = Rx(argument[3]).word + 2;
          }
        break; /* [R+] */
        case 2: /* Post Decrement [R-] */          
          if(argument[2]){ // if byte instruction
            regFile[REG][argument[4]].byte[0] = memory.byte[regFile[REG][argument[3]].word];
            Rx(argument[3]).word--; 
          } else{          // if word instruction
            regFile[REG][argument[4]].word = memory.word[regFile[REG][argument[3]].word >> 1];
            Rx(argument[3]).word = Rx(argument[3]).word - 2;
          } /* [R-] */
        break;
        case 5: /* Pre Increment [+R] */          
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