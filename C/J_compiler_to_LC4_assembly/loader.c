#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "LC4.h"

FILE* loadMe(char* name, int* counter){
  FILE* input = fopen(name, "r");
  char c = toupper(fgetc(input));
  while(c != EOF){
    while(c < 33 && c != EOF) c = fgetc(input);
    if(c == EOF) break;
    *counter = *counter + 1;
    if(c == ';'){
      while(c != 10) c = fgetc(input);
    }
    else{
      while(c >= 33) c =fgetc(input);
    }
  }
  return input;
}
