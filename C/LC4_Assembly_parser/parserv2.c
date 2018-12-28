#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "LC4.h"
#include "loader.h"

short mem[64000];
short reg[8];
int PC = 0x8200;
short priv = 1;
short NZP = 2;
short exception = 0;

int main (int argc, char *argv[])
{ //obj files start at index 2. target file 
  clock_t t;
  t = clock();
  FILE *input;

  for(int i = 2; i < argc; i++){
    input = loadMe(argv[i], mem);
  }
  //figure out wrong code. code: 0~1FFF and 8000~9FFF
  char temp[12];
  char temp2[15];
  input = fopen(argv[1], "w");

	
  for(int count = 0; PC != 0x80FF && count < 1000000; ){
    //printf("%d\n", count);
    
    getCmd(input, &PC, reg, &priv, &NZP, &exception, mem[PC], mem);
    //for(int j = 0; j < 1000000000; j++);
    //PC++;
    if (exception != 0) break;
    ++count;
  }
  fclose(input);

/*############# Extra credit section #################*/
  
  if(exception == 0){
  	//create ppm file
  	input = fopen("image.ppm", "w");

  	char output[50];
  	sprintf(output, "P6 \n");
    short aux;
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);

    sprintf(output, "# Hello there \n");
   
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);

    sprintf(output, "128 124 \n");

    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);

    sprintf(output, "255 \n");
    
    for (aux = 0; output[aux] != '\0'; aux++);
    fwrite(output, 1, aux, input);
  	char out[8];
	for(int i = 0xC000; i < 0xFE00; i++){
      short cmd = mem[i];
      short r = ((unsigned short)((short)(cmd << 1)) >> 11) << 3;
      short g = ((unsigned short)((short)(cmd << 6)) >> 11) << 3;
      short b = ((unsigned short)((short)(cmd << 11)) >> 11) << 3;
      convertToStringImg(out, r);
      //if(out[1] != '0') printf("heyoou!!\n");
      sprintf(output, "%c%c%c%c%c%c%c%c ", out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
    
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);

      convertToStringImg(out, g);
      sprintf(output, "%c%c%c%c%c%c%c%c ", out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
      
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);

      convertToStringImg(out, b);
      sprintf(output, "%c%c%c%c%c%c%c%c \n", out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);
      
      for (aux = 0; output[aux] != '\0'; aux++);
      fwrite(output, 1, aux, input);
	}

  	fclose (input);
  }
  
  return 0;
}
