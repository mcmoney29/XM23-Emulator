#include "cpu.h"

extern mem memory;
extern int cpu_time;
extern PSW_Bits* PSW;

void tick(){
  unsigned short arg[MAX_OPERANDS];
  fetch();
  decode(arg);
  execute(arg);
}

void fetch() {
  bus(PC.word, &IR, READ, WORD);
  PC.word = PC.word + 2;
  cpu_time++;
}

void decode(unsigned short* arg){
  switch((IR >> 13) & 0x07){
    case 0: decode_BL(arg); break;
    case 1: decode_BEQ_to_BRA(arg); break;
    case 2: decode_ADD_to_ST(arg); break;
    case 3: decode_MOVL_to_MOVH(arg); break;
    case 4: case 5: decode_LDR(arg); break;
    case 6: case 7: decode_STR(arg); break;
    default: printf("Unable to decode instruction 0x%04X\n", IR); break;
  }
  cpu_time++;
}

void execute(unsigned short arg[]){
  switch (arg[0]) {
    /******************************************
    - BL
    *****************************************/
    case BL_G: 
      LR.word = PC.word;
      PC.word += arg[1];
    break;
    /******************************************
    - BEQ, BNE, BC, BNC, BN, BGE, BLT, BRA
    ******************************************/
    case BEQ_G: PSW->z ? PC.word += arg[1] : 0; break;
    case BNE_G: !PSW->z ? PC.word += arg[1] : 0; break;
    case BC_G:  PSW->c ? PC.word += arg[1] : 0; break;
    case BNC_G: !PSW->c ? PC.word += arg[1] : 0; break;
    case BN_G:  PSW->n ? PC.word += arg[1] : 0; break;
    case BGE_G: (PSW->n ^ PSW->v) == 0 ? PC.word += arg[1] : 0; break;
    case BLT_G: (PSW->n ^ PSW->v) == 1 ? PC.word += arg[1] : 0; break;
    case BRA_G: PC.word += arg[1]; break;
    /******************************************
    Arithmetic Instructions
    Arguments: #1 = REG/CON  #2 = Word/Byte  #3 = Source  #4 = Destination
    - ADD, ADDC, SUB, SUBC, DADD, CMP, XOR, AND, BIT, BIC, BIS
    ******************************************/
    case ADD_G:  ADD_SUB(arg[4], regFile[arg[1]][arg[3]].word, arg[2], 0);      break;
    case ADDC_G: ADD_SUB(arg[4], regFile[arg[1]][arg[3]].word, arg[2], PSW->c); break;
    case SUB_G:  ADD_SUB(arg[4], ~regFile[arg[1]][arg[3]].word+1, arg[2], 0);      break;
    case SUBC_G: ADD_SUB(arg[4], ~regFile[arg[1]][arg[3]].word+1, arg[2], PSW->c); break;
    case DADD_G: DADD_Func(arg[4],regFile[arg[1]][arg[3]], arg[2]);        break;
    case CMP_G:  CMP_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);        break;
    case XOR_G:  XOR_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);        break;
    case AND_G:  AND_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);        break;
    case OR_G:   OR_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);         break;
    case BIT_G:  BIT_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);        break;
    case BIC_G:  BIC_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);        break;
    case BIS_G:  BIS_Func(arg[4], regFile[arg[1]][arg[3]], arg[2]);        break;
    /******************************************
    Arguments: #1 = Word/Byte  #2 = Source  #3 = Destination
    - MOV, SWAP, SRA, RRC, COMP, SWPB, SXT
    ******************************************/
    case MOV_G: MOV_Func(arg[3], arg[2], arg[1]); break;
    case SWAP_G: SWAP_Func(arg[3], arg[2]); break;
    case SRA_G: SRA_Func(arg[3], arg[1]); break;
    case RRC_G: RRC_Func(arg[3], arg[1]); break;
    case COMP_G: COMP_Func(arg[3], arg[1]); break;
    case SWPB_G: SWPB_Func(arg[3]); break;
    case SXT_G: SXT_Func(arg[3]); break;
    /******************************************
    Arguments: #1 = Pre-Post/Dec/Inc  #2 = Word/Byte  #3 = Source  #4 = Destination
    - LD, ST
    ******************************************/
    case LD_G:  LD_Func(arg[4], arg[3], arg[1], arg[2]); break;
    case ST_G:  ST_Func(arg[4], arg[3], arg[1], arg[2]); break;
    /******************************************
    - MOVL, MOVLZ, MOVLS, MOVH
    ******************************************/
    case MOVL_G:
      regFile[REG][arg[2]].byte[0] = arg[1];
    break;
    case MOVLZ_G:
      regFile[REG][arg[2]].byte[0] = arg[1];
      regFile[REG][arg[2]].byte[1] = 0x00;
    break;
    case MOVLS_G:
      regFile[REG][arg[2]].byte[0] = arg[1];
      regFile[REG][arg[2]].byte[1] = 0xFF;
    break;
    case MOVH_G:
      regFile[REG][arg[2]].byte[1] = arg[1];
    break;
    /******************************************
    - LDR
    - STR
    ******************************************/
    case LDR_G:
      if(arg[2])        // BYTE
        Rx(arg[4]).byte[0] = memory.byte[Rx(arg[3]).byte[0]]; 
      else              // WORD
        Rx(arg[4]).word = memory.word[Rx(arg[3]).word>>1];
    break;
    case STR_G:
      if(arg[2])        // BYTE
        memory.byte[Rx(arg[3]).byte[0]] = Rx(arg[4]).byte[0]; 
      else              // WORD
        memory.word[Rx(arg[3]).word>>1] = Rx(arg[4]).word;
    break;
  }
  cpu_time++;
}