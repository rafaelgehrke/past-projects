#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertToString(char out[], short cmd){
  for(short i = 15; i >= 0; i--){
    out[i] = (char) ((((unsigned short)((short)(cmd << i))) >> 15) + 48);
  }
}

void convertToStringImg(char out[], short cmd){
  for(short i = 7; i >= 0; i--){
    out[i] = (char) ((((unsigned short)((short)(cmd << (i+8)))) >> 15) + 48);
  }
}

void getCmd(FILE* input, int* PC, short reg[], short* priv, short* NZP, short* exception, short cmd, short memo[]){
  char output[50];
  if(*PC > 0x01FFF && *PC < 0x8000){
    *exception = 1;
    printf("Exception code %d (See homework file for codes 1 to 3)", *exception);
    return;
  }
  else if (*PC > 0x8000 && *priv == 0){
    *exception = 3;
    printf("Exception code %d (See homework file for codes 1 to 3)", *exception);
    return;
  }
  
  char bin[16];
  convertToString(bin, cmd);
  short first_p = ((unsigned short)cmd >> 12);
  if(first_p == 0b0000){
    short second_p = cmd >> 9;
    if(second_p == 0b000){
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "NOP\n");
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1; 
      //return string NOP
    }
    else if(second_p == 0b001){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRp %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (*NZP == 1) *PC = *PC + 1 + number;
      else *PC = *PC + 1;
      //return srtring BRp number
    }
    else if(second_p == 0b010){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRz %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (*NZP == 2) *PC = *PC + 1 + number;
      else *PC = *PC + 1;
      //return srtring BRz number
    }
    else if(second_p == 0b011){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRzp %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (*NZP == 1 || *NZP == 2) *PC = *PC + 1 + number;
      else *PC = *PC + 1;
      //return srtring BRzp number
    }
    else if(second_p == 0b100){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRn %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (*NZP == 4) *PC = *PC + 1 + number;
      else *PC = *PC + 1;
      //return srtring BRn number
    }
    else if(second_p == 0b101){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRnp %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (*NZP == 1 || *NZP == 4) *PC = *PC + 1 + number;
      else *PC = *PC + 1;
      //return srtring BRnp number
    }
    else if(second_p == 0b110){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRnz %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (*NZP == 2 || *NZP == 4) *PC = *PC + 1 + number;
      else *PC = *PC + 1;
      //return srtring BRnz number
    }
    else if(second_p == 0b111){
      short number = (short)(cmd << 7) >> 7;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "BRnzp %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1 + number;
      //return string BRnzp number
    }
    else{
      sprintf(output, "INVALID INSTRUCTION\n");
      *exception = 4;
      printf("Exception INVALID INSTRUCTION");
      return;
    }
  }
  else if(first_p == 0b0001){
    short second_p = ((unsigned short)((short)(cmd << 10))) >> 13;
    if(second_p == 0b000){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] + reg[t];
      short print = reg[d];
      if(print > 0) *NZP = 1;
      else if(print == 0) *NZP = 2;
      else if(print < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "ADD R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string ADD Rx Ry Rz
    }
    else if(second_p == 0b001){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] * reg[t];
      short print = reg[d];
      if(print > 0) *NZP = 1;
      else if(print == 0) *NZP = 2;
      else if(print < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print, *NZP);
      //sprintf(output, "MUL R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string MUL Rx Ry Rz
    }
    else if(second_p == 0b010){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] - reg[t];
      short print = reg[d];
      if(print > 0) *NZP = 1;
      else if(print == 0) *NZP = 2;
      else if(print < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print, *NZP);
      //sprintf(output, "SUB R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string SUB Rx Ry Rz
    }
    else if(second_p == 0b011){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] / reg[t];
      short print = reg[d];
      if(print > 0) *NZP = 1;
      else if(print == 0) *NZP = 2;
      else if(print < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print, *NZP);
      //sprintf(output, "DIV R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string DIV Rx Ry Rz
    }
    else{
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short number = (short)(cmd << 11) >> 11;
      reg[d] = reg[s] + number;
      short printMe = reg[d];
      if(printMe > 0) *NZP = 1;
      else if(printMe == 0) *NZP = 2;
      else if(printMe < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, printMe, *NZP);
      
      //sprintf(output, "ADD R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string ADD Rx Ry number
    }
  }
  else if(first_p == 0b0101){
    short second_p = ((unsigned short)((short)(cmd << 10))) >> 13;
    if(second_p == 0b000){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] & reg[t];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "AND R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string AND Rx Ry Rz
    }
    else if(second_p == 0b001){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = ~reg[s];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "NOT R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string NOT Rx Ry
    }
    else if(second_p == 0b010){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] | reg[t];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "OR R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string OR Rx Ry Rz
    }
    else if(second_p == 0b011){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] ^ reg[t];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "XOR R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string XOR Rx Ry Rz
    }
    else{
      short number = (short)(cmd << 11) >> 11;
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      reg[d] = reg[s] & number;
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "AND R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string AND Rx Ry number
    }
  }
  else if(first_p == 0b0110){
    short number = (short)(cmd << 10) >> 10;
    short d = (unsigned short)((short)(cmd << 4)) >> 13;
    short s = (unsigned short)((short)(cmd << 7)) >> 13;   //3275
    unsigned short memoryAdd = (short)reg[s] + (short)number;
    if((memoryAdd >= 0x0000 && memoryAdd <= 0x1FFF) || (memoryAdd >= 0x8000 && memoryAdd <= 0x9FFF)){
      *exception = 2;
      printf("Exception code %d (See homework file for codes 1 to 3)", *exception);
      return;
    }
    else if(memoryAdd > 0x9FFF && *priv == 0){
      *exception = 3;
      printf("Exception code %d (See homework file for codes 1 to 3)", *exception);
      return;
    }
    short prevS = reg[s];
    short inter = memo[memoryAdd];
    reg[d] = inter;
    short print2 = reg[d];
    short print3 = prevS + number;
    if(reg[d] > 0) *NZP = 1;
    else if(reg[d] == 0) *NZP = 2;
    else if(reg[d] < 0) *NZP = 4;
    sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 %04hX %04hX\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print2, *NZP, print3, print2);
    //sprintf(output, "LDR R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, number);
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
    *PC = *PC + 1;
    //return string LDR Rx Ry number
  }
  else if(first_p == 0b0111){
    
    short number = (short)(cmd << 10) >> 10;
    short t = (unsigned short)((short)(cmd << 4)) >> 13;
    short s = (unsigned short)((short)(cmd << 7)) >> 13;
    unsigned short memoryAdd = reg[s] + number;
    
    if((memoryAdd >= 0x0000 && memoryAdd <= 0x1FFF) || (memoryAdd >= 0x8000 && memoryAdd <= 0x9FFF)){
      *exception = 2;
      printf("Exception code %d (See homework file for codes 1 to 3)", *exception);
      return;
    }
    else if(memoryAdd > 0x9FFF && *priv == 0){
      *exception = 3;
      printf("Exception code %d (See homework file for codes 1 to 3)", *exception);
      return;
    }
    int rec[8] = {reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]};
    //printf("     test\n");
    //printf("\n\n%04X: %04hX", *PC, reg[4]);
    short inter = reg[t];
    //printf("%hd", reg[s]);
    memo[memoryAdd] = inter;
    for(int i = 0; i < 8; i++){
      reg[i] = rec[i];
    }
    //printf("\n%04X: %04hX\n\n", *PC, reg[4]);
    sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 1 %04hX %04hX\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], memoryAdd, reg[t]);
    //sprintf(output, "STR R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, number);
    
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
    *PC = *PC + 1;
    //return string STR Rx Ry number
  }
  else if(first_p == 0b1001){
    short number = (short)(cmd << 7) >> 7;
    short d = (unsigned short)((short)(cmd << 4)) >> 13;
    reg[d] = number;
    if(reg[d] > 0) *NZP = 1;
    else if(reg[d] == 0) *NZP = 2;
    else if(reg[d] < 0) *NZP = 4;
    sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
    //sprintf(output, "CONST R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, number);
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
    *PC = *PC + 1;
    //return string CONST Rx number
  }
  else if(first_p == 0b1101){
    short number = ((unsigned short)((short)(cmd << 8))) >> 8;
    short d = (unsigned short)((short)(cmd << 4)) >> 13;
    reg[d] = (reg[d] & 0xFF) | (number << 8);
    if(reg[d] > 0) *NZP = 1;
    else if(reg[d] == 0) *NZP = 2;
    else if(reg[d] < 0) *NZP = 4;
    sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
    //sprintf(output, "HICONST R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 8)) >> 8);
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
    *PC = *PC + 1;
    //return string HICONST Rx numberUnsigned
  }
  else if(first_p == 0b0010){
    short second_p = ((unsigned short)((short)(cmd << 7))) >> 14;
    if(second_p == 0b00){
      short s = (unsigned short)((short)(cmd << 4)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      short result = reg[s] - reg[t];
      if(result > 0) *NZP = 1;
      else if(result == 0) *NZP = 2;
      else if(result < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], *NZP);
      //sprintf(output, "CMP R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string CMP Rx Ry
    }
    else if(second_p == 0b01){
      short s = (unsigned short)((short)(cmd << 4)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      short numS = reg[s];
      short numT = reg[t];
      if(numS < 0) numS = -numS;
      if(numT < 0) numT = -numT;
      short result = numS - numT;
      if(result > 0) *NZP = 1;
      else if(result == 0) *NZP = 2;
      else if(result < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], *NZP);
      //sprintf(output, "CMPU R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 13)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string CMPU Rx Ry
    }
    else if(second_p == 0b10){
      short number = (short)(cmd << 9) >> 9;
      short s = (unsigned short)((short)(cmd << 4)) >> 13;
      short result = reg[s] - number;
      if(result > 0) *NZP = 1;
      else if(result == 0) *NZP = 2;
      else if(result < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], *NZP);
      //sprintf(output, "CMPI R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string CMPI Rx number
    }
    else if(second_p == 0b11){
      short s = (unsigned short)((short)(cmd << 4)) >> 13;
      short number = (unsigned short)((short)(cmd << 9)) >> 9;
      short numS = reg[s];
      short numNum = number;
      if(numS < 0) numS = -numS;
      if(numNum < 0) numNum = -numNum;
      short result = numS - numNum;
      if(result > 0) *NZP = 1;
      else if(result == 0) *NZP = 2;
      else if(result < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], *NZP);
      //sprintf(output, "CMPIU R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 9)) >> 9);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string CMPIU Rs numberUnsigned
    }
  }
  else if(first_p == 0b1010){
    short second_p = ((unsigned short)((short)(cmd << 10))) >> 14;
    if(second_p == 0b00){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short number = (unsigned short)((short)(cmd << 12)) >> 12;
      reg[d] = reg[s] << number;
      short print = reg[d];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print, *NZP);
      //sprintf(output, "SLL R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 12)) >> 12);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string SLL Rx Ry numberUnsigned
    }
    else if(second_p == 0b01){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short number = (unsigned short)((short)(cmd << 12)) >> 12;
      reg[d] = reg[s] >> number;
      short print = reg[d];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print, *NZP);
      //sprintf(output, "SRA R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 12)) >> 12);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string SRA Rx Ry numberUnsigned
    }
    else if(second_p == 0b10){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short number = (unsigned short)((short)(cmd << 12)) >> 12;
      reg[d] = (unsigned short) (reg[s]) >> number;
      short print = reg[d];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04hX 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, print, *NZP);
      //sprintf(output, "SRL R%d R%d %d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 12)) >> 12);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string SRL Rx Ry numberUnsigned
    }
    else if(second_p == 0b11){
      short d = (unsigned short)((short)(cmd << 4)) >> 13;
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short t = (unsigned short)((short)(cmd << 13)) >> 13;
      reg[d] = reg[s] % reg[t];
      if(reg[d] > 0) *NZP = 1;
      else if(reg[d] == 0) *NZP = 2;
      else if(reg[d] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04X 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], d, reg[d], *NZP);
      //sprintf(output, "MOD R%d R%d R%d\n", (unsigned short)((short)(cmd << 4)) >> 13, (unsigned short)((short)(cmd << 7)) >> 13, (unsigned short)((short)(cmd << 12)) >> 12);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1;
      //return string MOD Rx Ry Rz
    }
  }
  else if(first_p == 0b0100){
    short second_p = ((unsigned short)((short)(cmd << 4))) >> 15;
    if(second_p == 0b0){
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      short remember = reg[7];
      reg[7] = *PC + 1;
      if(reg[7] > 0) *NZP = 1;
      else if(reg[7] == 0) *NZP = 2;
      else if(reg[7] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04X 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], 7, reg[7], *NZP);
      //sprintf(output, "JSRR R%d\n", (unsigned short)((short)(cmd << 7)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      if (s == 7) *PC = remember;
      else *PC = reg[s];
      //return string JSRR Rx
    }
    else if(second_p == 0b1){
      short number = ((unsigned short)((short)(cmd << 5))) >> 5;
      reg[7] = *PC + 1;
      if(reg[7] > 0) *NZP = 1;
      else if(reg[7] == 0) *NZP = 2;
      else if(reg[7] < 0) *NZP = 4;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 %d %04X 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], 7, reg[7], *NZP);
      //sprintf(output, "JSR %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = (*PC & 0x8000) | ((number) << 4);
      //return string JSR number
    }
  }
  else if(first_p == 0b1100){
    short second_p = ((unsigned short)((short)(cmd << 4))) >> 15;
    if(second_p == 0b0){
      short s = (unsigned short)((short)(cmd << 7)) >> 13;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "JMPR R%d\n", (unsigned short)((short)(cmd << 7)) >> 13);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = reg[s];
      //return string JMPR Rx
    }
    else if(second_p == 0b1){
      short number = (short)(cmd << 5) >> 5;
      sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
      //sprintf(output, "JMP %d\n", number);
      short aux;
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
      *PC = *PC + 1 + number;
      //return string JMP number
    }
  }
  else if(first_p == 0b1111){
    short number = (unsigned short)((short)(cmd << 8)) >> 8;
    reg[7] = *PC + 1;
    if(reg[7] > 0) *NZP = 1;
    else if(reg[7] == 0) *NZP = 2;
    else if(reg[7] < 0) *NZP = 4;
    sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 1 7 %04X 1 %d 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15], reg[7], *NZP);
    //sprintf(output, "TRAP %d\n", (unsigned short)((short)(cmd << 8)) >> 8);
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
    *PC = (0x8000 | number);
    *priv = 1;
    //return string TRAP number
  }
  else if(first_p == 0b1000){
    sprintf(output, "%04X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c 0 0 0000 0 0 0 0000 0000\n", *PC, bin[0], bin[1], bin[2], bin[3], bin[4], bin[5], bin[6], bin[7], bin[8], bin[9], bin[10], bin[11], bin[12], bin[13], bin[14], bin[15]);
    //sprintf(output, "RTI\n");
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
    *PC = reg[7];
    *priv = 0;
    //return string RTI
  }
  else{
    sprintf(output, "INVALID INSTRUCTION\n");
    *exception = 4;
    printf("Exception INVALID INSTRUCTION");
    return;
    //return string invalid command
  }
}
