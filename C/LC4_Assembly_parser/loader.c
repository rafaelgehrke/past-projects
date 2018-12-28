#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LC4.h"

FILE* loadMe(char* name, short* mem){
  FILE* input = fopen(name, "r");
  unsigned long size = 0;
  while(feof(input) != 1){
    fgetc(input);
    size++;
  } 
  rewind(input);
  short objData[size];
  for(int j = 0; j < size; j++){
    objData[j] = (short)(fgetc(input));
  }
  //currently I have every byte stored at the short array "objData"
  short Fbyte = 0;
  short Sbyte = 0;
  short Faddr = 0;
  short Saddr = 0;
  short Flength = 0;
  short Slength = 0;
  int length = 0;
  int keying = 0;
  int keyMode = 0;

  for(int j = 0; j < size - 1;){
    while(keying == 0){
    if(j >= size - 1) break;
      Fbyte = objData[j];
      j++;
      Sbyte = objData[j];
      j++;
      if(Fbyte == 0xCA && Sbyte == 0xDE){ //code
        keyMode = 1;
        Faddr = objData[j];
        j++;
        Saddr = objData[j];
        j++;
        Flength = objData[j];
        j++;
        Slength = objData[j];
        j++;
        length = (int)(Flength << 8) + (int)(Slength);
        keying = 1;
      }
      else if(Fbyte == 0xDA && Sbyte == 0xDA){ //data
        keyMode = 2;
        Faddr = objData[j];
        j++;
        Saddr = objData[j];
        j++;
        Flength = objData[j];
        j++;
        Slength = objData[j];
        j++;
        length = (int)(Flength << 8) + (int)(Slength);
        keying = 1;
      }
      else if(Fbyte == 0xC3 && Sbyte == 0xB7){ //symbol
        keyMode = 3;
        j += 2;
        Flength = objData[j];
        j++;
        Slength = objData[j];
        j++;
        j += (int)(Flength << 8) + (int)(Slength);
        keying = 1;
        length = 0;
      }
      else if(Fbyte == 0xF1 && Sbyte == 0x7E){ //File name
        keyMode = 4;
        Flength = objData[j];
        j++;
        Slength = objData[j];
        j++;
        j += (int)(Flength << 8) + (int)(Slength);
        keying = 1;
        length = 0;
      }
      else if(Fbyte == 0x71 && Sbyte == 0x5E){ //line number
        keyMode = 5;
        j += 6;
        keying = 1;
        length = 0;
      }
    }// out of while
      //it is either code or data. keep subtracting length, adding address and putting code in memory!
    for(int k = 0; k < 2*length && length != 0; k += 2){
      mem[(int)(Faddr << 8) + (int)(Saddr) + k/2] = (objData[j + k] << 8) + objData[j + k + 1];
    }
    keying = 0;
    j += 2*length;
      //printf(" currently ended at %d, that is the code %hX, the next is %hX \n\n", j, objData[j], objData[j+1]);
  }//out of file
  fclose(input);
  return input;
}
