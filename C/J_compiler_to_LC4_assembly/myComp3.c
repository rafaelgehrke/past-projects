#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "LC4.h"
#include "loader.h"

int tokenType = 0;
short numberHi = 0;
short numberLo = 0;
short argNum = 0;
short ifTotalCount = 0;
// 1 -> comment
// 2 -> defun
// 3 -> positive or negative decimal or hexa
// 4 ->
void getArgNum2(char tok[][250], int num){
  int aux;
  for(aux = 0; tok[num][aux] != 0; aux++);
  short summer = 0;
  for(int i = aux - 1; i != 2; i--){
    summer += (tok[num][i] - 48)*pow(10, (aux - i-1));
  }

  //printf("summer is %hd, token is %s\n", summer, tok[num]);
  argNum = summer+2;
}

void charHelpMePrintThisPlz(FILE *input, char message){

  
  char output[250];
  for(int aux2 = 0; aux2 < 250; aux2++){
    output[aux2] = 0;
  }
  sprintf(output, "%c", message);
  
  int aux;
  //printf("\n\n\n CHAR IS: %c, now is %s \n\n\n", message, output);
    for (aux = 0; output[aux] != 0; aux++);
    fwrite(output, 1, aux, input);
    for(int aux2 = 0; aux2 < 250; aux2++){
    output[aux2] = 0;
  }
}

void writeNumber(FILE *input, short num){

  char output[250];
  for(int aux2 = 0; aux2 < 250; aux2++){
    output[aux2] = 0;
  }
  sprintf(output, "%hd", num);
  int aux;
    for (aux = 0; output[aux] != 0; aux++);
    fwrite(output, 1, aux, input);
    for(int aux2 = 0; aux2 < 250; aux2++){
    output[aux2] = 0;
  }
}

int checkFun(char tok[][250], int num){
  // we already know it has arg in the beginning. see if it is order of 1 or 10.
  
    for(int i = 1; tok[num][i] != 0; i++){
      if(!((tok[num][i] >= 48 && tok[num][i] <= 57) || (tok[num][i] >= 65 && tok[num][i] <= 90) || (tok[num][i] == 95) || (tok[num][i] >= 97 && tok[num][i] <= 122))){
        return 0;
      }
    }
      //if code is here, it is a valid function
    if (tokenType == 2) return 23;
    else return 24;
}

int checkArg(char tok[][250], int num){
  // we already know it has arg in the beginning. see if it is order of 1 or 10.
  if(tok[num][3] >= 48 && tok[num][3] <= 57 && tok[num][4] == 0){ // if in order of 1
      return 21;
  }
  else if(((tok[num][3] == '1') && (tok[num][4] >= 48) && (tok[num][4] <= 57) && (tok[num][5] == 0)) || ((tok[num][3] == '2') && (tok[num][4] == '0') && (tok[num][5] == 0))){ // if in order of 10
      return 22;
  }
  else{ // none of previous, CAN be function, see characters.
      for(int i = 3; tok[num][i] != 0; i++){
        if(!((tok[num][i] >= 48 && tok[num][i] <= 57) || (tok[num][i] >= 65 && tok[num][i] <= 90) || (tok[num][i] == 95) || (tok[num][i] >= 97 && tok[num][i] <= 122))){
          return 0;
        }
      }
      //if code is here, it is a valid function
      if (tokenType == 2) return 23;
      else return 24;
  }
}

int isNumberDecHex(char tok[][250], int num){
  if(tok[num][0] == '-'){   //negative
    //printf("\n\n\n NEGATIVE NUMBER\n\n\n");
      for(int i = 1; tok[num][i] != 0; i++){
        if(tok[num][i] < 48 || tok[num][i] > 57){
          return 0;
        }
      }
      int aux4 = 0;
      short summer = 0;
      for(aux4 = 1; tok[num][aux4] != 0; aux4++);
      for(int i = aux4 - 1; i != 0; i--){
        summer += (tok[num][i] - 48)*pow(10, (aux4 - i-1));
      }
      summer = -summer;
      //printf("%s is %d, has %d digits\n", tok[num], summer, aux4);
      numberLo = ((unsigned short)((short)(summer << 8)) >> 8);
      numberHi = ((unsigned short)((short)(summer << 0)) >> 8);
      return 1;
  }
  else if(tok[num][0] == '0' && tok[num][1] == 'x'){  //hexadecimal
      //TA said we could consider it valid hexa
      return 2;
  }
  else if(tok[num][0] >= 48 && tok[num][0] <= 57){  //decimal
      
      for(int i = 1; tok[num][i] != 0; i++){
        if(tok[num][i] < 48 || tok[num][i] > 57){
          return 0;
        }
      }
      int aux4;
      short summer = 0;
      for(aux4 = 0; tok[num][aux4] != 0; aux4++){

      }
      for(int i = aux4 - 1; i != -1; i--){
        summer += (tok[num][i] - 48)*pow(10, (aux4 - i-1));
      }
      //printf("%s is %d, has %d digits\n", tok[num], summer, aux4);
      numberLo = ((unsigned short)((short)(summer << 8)) >> 8);
      numberHi = ((unsigned short)((short)(summer << 0)) >> 8);
      //printf("Lo is %d, Hi is %d\n", numberLo, numberHi);
      return 1;
  }
  else{
    return 0;
  }
}

int isValidToken(char tok[][250], int num){

  if(tok[num][0] == ';'){
      tokenType = 1;
      return tokenType;
  }
  else if (tok[num][0] == '-' && tok[num][1] == 0){
    tokenType = 5;
    return tokenType;
  }
  else if (tok[num][0] == 'i' && tok[num][1] == 'f' && tok[num][2] == 0){
    tokenType = 1000;
    return tokenType;
  }
  else if (tok[num][0] == 'e' && tok[num][1] == 'l' && tok[num][2] == 's' && tok[num][3] == 'e' && tok[num][4] == 0){
    tokenType = 1001;
    return tokenType;
  }
  else if (tok[num][0] == 'e' && tok[num][1] == 'n' && tok[num][2] == 'd' && tok[num][3] == 'i' && tok[num][4] == 'f' && tok[num][5] == 0){
    tokenType = 1002;
    return tokenType;
  }
  else if(tok[num][0] == 'd' && tok[num][1] == 'e' && tok[num][2] == 'f' && tok[num][3] == 'u' && tok[num][4] == 'n' && tok[num][5] == 0){
    tokenType = 2;
    return tokenType;
  }
  else if (isNumberDecHex(tok, num) == 1){
    tokenType = 3;
    return tokenType;
  }
  else if (isNumberDecHex(tok, num) == 2){  //hexadec
    tokenType = 100;
    return tokenType;
  }
  //***********ARITH OPERATIONS
  else if (tok[num][0] == '+' && tok[num][1] == 0){
    tokenType = 4;
    return tokenType;
  }
  
  else if (tok[num][0] == '*' && tok[num][1] == 0){
    tokenType = 6;
    return tokenType;
  }
  else if (tok[num][0] == '/' && tok[num][1] == 0){
    tokenType = 7;
    return tokenType;
  }
  else if (tok[num][0] == '%' && tok[num][1] == 0){
    tokenType = 8;
    return tokenType;
  }
  //***********COMPARISONS
  else if (tok[num][0] == 'l' && tok[num][1] == 't' && tok[num][2] == 0){
    tokenType = 9;
    return tokenType;
  }
  else if (tok[num][0] == 'l' && tok[num][1] == 'e' && tok[num][2] == 0){
    tokenType = 10;
    return tokenType;
  }
  else if (tok[num][0] == 'e' && tok[num][1] == 'q' && tok[num][2] == 0){
    tokenType = 11;
    return tokenType;
  }
  else if (tok[num][0] == 'g' && tok[num][1] == 'e' && tok[num][2] == 0){
    tokenType = 12;
    return tokenType;
  }
  else if (tok[num][0] == 'g' && tok[num][1] == 't' && tok[num][2] == 0){
    tokenType = 13;
    return tokenType;
  }
  //***********LOGICAL OPERATIONS
  else if (tok[num][0] == 'a' && tok[num][1] == 'n' && tok[num][2] == 'd' && tok[num][3] == 0){
    tokenType = 14;
    return tokenType;
  }
  else if (tok[num][0] == 'o' && tok[num][1] == 'r' && tok[num][2] == 0){
    tokenType = 15;
    return tokenType;
  }
  else if (tok[num][0] == 'n' && tok[num][1] == 'o' && tok[num][2] == 't' && tok[num][3] == 0){
    tokenType = 16;
    return tokenType;
  }
  //*******STACK OPERATIONS
  else if (tok[num][0] == 'd' && tok[num][1] == 'r' && tok[num][2] == 'o' && tok[num][3] == 'p' && tok[num][4] == 0){
    tokenType = 17;
    return tokenType;
  }
  else if (tok[num][0] == 'd' && tok[num][1] == 'u' && tok[num][2] == 'p' && tok[num][3] == 0){
    tokenType = 18;
    return tokenType;
  }
  else if (tok[num][0] == 's' && tok[num][1] == 'w' && tok[num][2] == 'a' && tok[num][3] == 'p' && tok[num][4] == 0){
    tokenType = 19;
    return tokenType;
  }
  else if (tok[num][0] == 'r' && tok[num][1] == 'o' && tok[num][2] == 't' && tok[num][3] == 0){
    tokenType = 20;
    return tokenType;
  }
  else if (tok[num][0] == 'a' && tok[num][1] == 'r' && tok[num][2] == 'g'){
    tokenType = checkArg(tok, num);  // 21 -> valid order 1 number
                                     // 22 -> valid order 10 number
                                     // 23 -> a function definition
                                     // 24 -> a function call
    return tokenType;
  }
  else if (tok[num][0] == 'r' && tok[num][1] == 'e' && tok[num][2] == 't' && tok[num][3] == 'u' && tok[num][4] == 'r' && tok[num][5] == 'n' && tok[num][6] == 0){
    tokenType = 25;
    return tokenType;
  }
  else if((tok[num][0] >= 65 && tok[num][0] <= 90) || (tok[num][0] >= 97 && tok[num][0] <= 122)){
        tokenType = checkFun(tok, num);
        return tokenType;
  }
  else{
    tokenType = 0;
    
    return tokenType;
  }
}

void translateRange (FILE *input, char token[][250], int numStart, int numEnd, char* name){
  for(int parse = numStart; parse < numEnd; parse++){
  int type = 0;
  type = isValidToken(token, parse);
    if(type != 0) { //is valid
      if(type == 1){
        //do nothing, thats a comment
      }
      else if(type == 23){ //a function definition
        helpMePrintThisPlz(input, ".FALIGN\n");
        helpMePrintThisPlz(input, token[parse]);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "STR R7, R6, #-2\n");
        helpMePrintThisPlz(input, "STR R5, R6, #-3\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #-3\n");
        helpMePrintThisPlz(input, "ADD R5, R6, #0\n");
      }
      else if(type == 24){ //a function call
        helpMePrintThisPlz(input, "JSR ");
        helpMePrintThisPlz(input, token[parse]);
        helpMePrintThisPlz(input, "\n");
      }
      else if(type == 3){ //number token
        helpMePrintThisPlz(input, "ADD R6, R6, #-1\n");
        helpMePrintThisPlz(input, "CONST R3, ");
        isNumberDecHex(token, parse);
        writeNumber(input, numberLo);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "HICONST R3, ");
        isNumberDecHex(token, parse);
        writeNumber(input, numberHi);
        helpMePrintThisPlz(input, "\n");
        
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
      }
     else if(type == 4){  
        helpMePrintThisPlz(input, "LDR R3, R6, #1\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R2, R2, R3\n");
        helpMePrintThisPlz(input, "STR R2, R6, #0\n");
      }
      else if(type == 5){  
        //printf("\n\n\n ACHOU SUBTRAC\n\n\n");
        helpMePrintThisPlz(input, "LDR R3, R6, #1\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "SUB R2, R2, R3\n");
        helpMePrintThisPlz(input, "STR R2, R6, #0\n");
      }
      else if(type == 6){
        helpMePrintThisPlz(input, "LDR R3, R6, #1\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "MUL R2, R2, R3\n");
        helpMePrintThisPlz(input, "STR R2, R6, #0\n");
      }
      else if(type == 7){  
        helpMePrintThisPlz(input, "LDR R3, R6, #1\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "DIV R2, R2, R3\n");
        helpMePrintThisPlz(input, "STR R2, R6, #0\n");
      }
      else if(type == 8){  
        helpMePrintThisPlz(input, "LDR R3, R6, #1\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "MOD R2, R2, R3\n");
        helpMePrintThisPlz(input, "STR R2, R6, #0\n");
      }
      else if(type == 25){ //return
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R5, #3\n");
        helpMePrintThisPlz(input, "STR R1, R6, #-1\n");
        helpMePrintThisPlz(input, "LDR R5, R6, #-3\n");
        helpMePrintThisPlz(input, "LDR R7, R6, #-2\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #-1\n");
        helpMePrintThisPlz(input, "RET\n");
      }
      else if(type == 20){//rot
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "LDR R3, R6, #2\n");
        helpMePrintThisPlz(input, "STR R1, R6, #1\n");
        helpMePrintThisPlz(input, "STR R2, R6, #2\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
      }
      else if(type == 13){   //gt
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "CMP R1, R2\n");

        helpMePrintThisPlz(input, "BRp gt_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "BRnz gt_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "gt_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #1\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP gt_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "gt_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #0\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP gt_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "gt_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
      }
      else if(type == 9){   //lt
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "CMP R1, R2\n");

        helpMePrintThisPlz(input, "BRn lt_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "BRzp lt_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "lt_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #1\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP lt_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "lt_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #0\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP lt_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "lt_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
      }
      else if(type == 10){   //le
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "CMP R1, R2\n");

        helpMePrintThisPlz(input, "BRnz le_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "BRp le_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "le_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #1\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP le_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "le_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #0\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP le_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "le_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
      }
      else if(type == 11){   //eq
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "CMP R1, R2\n");

        helpMePrintThisPlz(input, "BRz eq_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "BRnp eq_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "eq_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #1\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP eq_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "eq_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #0\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP eq_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "eq_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
      }
      else if(type == 12){   //ge
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "CMP R1, R2\n");

        helpMePrintThisPlz(input, "BRzp ge_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "BRn ge_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "ge_func_branch_true");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #1\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP ge_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "ge_func_branch_false");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "CONST R3, #0\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
        helpMePrintThisPlz(input, "JMP ge_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "ge_func_branch_end");
        writeNumber(input, parse);
        helpMePrintThisPlz(input, "\n");
      }
      else if(type == 14){   //and
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "AND R1, R1, R2\n");
    helpMePrintThisPlz(input, "STR R1, R6, #0\n");
      }
      else if(type == 15){   //or
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
        helpMePrintThisPlz(input, "OR R1, R1, R2\n");
    helpMePrintThisPlz(input, "STR R1, R6, #0\n");
      }
      else if(type == 16){   //not
        //printf("\n\n\n hey \n\n\n");
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "NOT R1, R1\n");
    helpMePrintThisPlz(input, "STR R1, R6, #0\n");
      }
      else if(type == 21 || type == 22){ //argN
        getArgNum2(token, parse);
        //offset = offset;
        helpMePrintThisPlz(input, "LDR R1, R5, #");
        //printf("your offset is: %hd\n", offset);
        writeNumber(input, argNum);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #-1\n");
        helpMePrintThisPlz(input, "STR R1, R6, #0\n");
      }
      else if(type == 17){ //drop
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n");
      }
      else if(type == 18){ //dup
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #-1\n");
        helpMePrintThisPlz(input, "STR R1, R6, #0\n");
      }
      else if(type == 19){ //dup
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "LDR R2, R6, #1\n");
        helpMePrintThisPlz(input, "STR R1, R6, #1\n");
        helpMePrintThisPlz(input, "STR R2, R6, #0\n");
      }
      else if(type == 100){ //hexadec
        //printf("\n\n\nfoundAnhexa: %s\n\n\n", token[parse]);
        helpMePrintThisPlz(input, "ADD R6, R6, #-1\n");
        helpMePrintThisPlz(input, "CONST R3, 0x");
        charHelpMePrintThisPlz(input, token[parse][4]);
        charHelpMePrintThisPlz(input, token[parse][5]);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "HICONST R3, 0x");
        charHelpMePrintThisPlz(input, token[parse][2]);
        charHelpMePrintThisPlz(input, token[parse][3]);
        helpMePrintThisPlz(input, "\n");
        helpMePrintThisPlz(input, "STR R3, R6, #0\n");
      }
      else if(type == 1000){
        //printf("\n\n\nfoundAnIf\n\n\n");
        //thats an if token, see where the next else is
	int impropFlag = 0;
        int ifCounter = 1;
        int distIftoElse = 1;
        for(distIftoElse = 1; ifCounter != 0 && parse + distIftoElse < numEnd; distIftoElse++){
          type = isValidToken(token, parse + distIftoElse);
          if(type == 1000) ifCounter++;
          else if (type == 1001) ifCounter--;
        }
        if(parse + distIftoElse == numEnd) impropFlag = 1;
	if(impropFlag == 1){
		int aux871 = 0;
		ifCounter = 1;
          for(distIftoElse = 1; ifCounter != 0 && parse + distIftoElse < numEnd; distIftoElse++){
            type = isValidToken(token, parse + distIftoElse);
            if(type == 1000) ifCounter++;
            else if (type == 1002){
            	ifCounter--;
            }
          }
	  //printf("\n\n\n THATS IMPROPER \n\n\n");
	}
        //printf("\n\n\n MARKER, dist if to else: %d\n\n\n", distIftoElse-1);
        distIftoElse--;
        int endifCounter = 1;
        int distElsetoEndif = 1;
        if(impropFlag == 0){
		for(distElsetoEndif = 1; endifCounter != 0 ; distElsetoEndif++){
		  type = isValidToken(token, parse + distElsetoEndif + distIftoElse);
		  if(type == 1000) endifCounter++;
		  else if (type == 1002) endifCounter--;
		}
		//printf("\n\n\n MARKER, dist else to endif: %d\n\n\n", distElsetoEndif-1);
		distElsetoEndif--;
	}
        helpMePrintThisPlz(input, "LDR R1, R6, #0\n");
        helpMePrintThisPlz(input, "ADD R6, R6, #1\n"); //consuming element
        helpMePrintThisPlz(input, "CMPI R1, #0\n");
        ifTotalCount++;
        //i need to figure out the file name.
        int lengthName = 0;
        
        while(*name != '.'){
          lengthName++;
          name++;
        }
        name -= lengthName;
        char printMarker[lengthName];
        for(int aux999 = 0; aux999 < lengthName; aux999++){
          printMarker[aux999] = 0;
        }
        int aux99 = 0;
        while(aux99 < lengthName-1){
          printMarker[aux99] = *name;
          name++;
          aux99++;
        }
        //printf("\n\n\n MARKER name: %s, size is %d\n\n\n", printMarker, lengthName);
        name -= lengthName-1;
        int snapshot = ifTotalCount;
	if(impropFlag == 0){
        helpMePrintThisPlz(input, "BRz ");
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_FALSE");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "BRnp ");
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_TRUE");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");
	}
	else{
	    helpMePrintThisPlz(input, "BRz ");
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_END");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");

        helpMePrintThisPlz(input, "BRnp ");
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_TRUE");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");
	}

        //********** code true
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_TRUE");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");

        translateRange (input, token, parse + 1, parse + 1 + distIftoElse, name);

        helpMePrintThisPlz(input, "JMP ");
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_END");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");

        //********** code false
	if(impropFlag == 0){
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_FALSE");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");

        translateRange (input, token, parse + distIftoElse , parse + distIftoElse + distElsetoEndif, name);

        helpMePrintThisPlz(input, "JMP ");
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_END");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");
	}

        //********** end
        helpMePrintThisPlz(input, printMarker);
        helpMePrintThisPlz(input, "_END");
        writeNumber(input, snapshot);
        helpMePrintThisPlz(input, "\n");
	if(impropFlag == 0) parse += distIftoElse + distElsetoEndif;
	else parse += distIftoElse;
        //printf("\n\n\n OUT OF IF\n\n\n");
      }
      else{
        //printf("Not coded token: %s   %d\n", token[parse], type);
      }
    }
    else{
    	fclose(input);
    	printf("Invalid token: %s   %d\n", token[parse], type);
    	exit(1);
        
      //break;
    }
  }
}

int main (int argc, char *argv[])
{ //obj files start at index 2. target file 
  FILE *input;


  int i = 0;

  for(i = 0; argv[1][i] != '.'; i++){
  }
  char name[i+5];
  for(int aux913 = 0; aux913 < i+5; aux913++){
    name[aux913] = 0;
  }
  for(i = 0; argv[1][i] != '.'; i++){
    name[i] = argv[1][i];
  }
  name[i] = '.';
  name[i+1] = 'a';
  name[i+2] = 's';
  name[i+3] = 'm';
  name[i+4] = '\0';

  //printf("%s\n",name); //now name holds the name of the file .asm
  int counter = 0;
  input = loadMe(argv[1], &counter);
  
  rewind(input);
  char token[counter][250];

  char c;
  int aux;
  c = fgetc(input);
  counter = 0;
  while(c != EOF){
    while(c < 33 && c != EOF) c = fgetc(input);
    if(c == EOF) break;
    counter++;
    aux = 0;
    if(c == ';'){
      while(c != 10){
        token[counter - 1][aux] = c;
        c = fgetc(input);
        aux++;
        if(c == 10) token[counter - 1][aux] = '\0';
      }
    }
    else{
      while(c >= 33){
        token[counter - 1][aux] = c;
        c = fgetc(input);
        aux++;
        if(c < 33) token[counter - 1][aux] = '\0';
      }
    }
  }
  fclose(input);
  
  input = fopen(name, "w");
  //for(int lol = 0; lol < counter; lol++) printf("%s\n", token[lol]);

  helpMePrintThisPlz(input, ".CODE\n");
  
  translateRange(input, token, 0, counter, name);
  fclose(input);
  return 0;
}
