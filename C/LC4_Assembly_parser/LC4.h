#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convertToStringImg(char out[], short cmd);
void convertToString(char out[], short cmd);
void getCmd(FILE* input, int* PC, short reg[], short* priv, short* NZP, short* exception, short cmd, short memo[]);
