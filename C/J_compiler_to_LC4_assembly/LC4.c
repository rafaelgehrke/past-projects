#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void helpMePrintThisPlz(FILE *input, char* message){

  char output[250];
  for(int aux2 = 0; aux2 < 250; aux2++){
    output[aux2] = 0;
  }
  for(int aux2 = 0; *message != 0; message++){
    output[aux2] = *message;
    aux2++;
  }
  int aux;
  for (aux = 0; output[aux] != 0; aux++);
  fwrite(output, 1, aux, input);
  for(int aux2 = 0; aux2 < 250; aux2++){
    output[aux2] = 0;
  }
}