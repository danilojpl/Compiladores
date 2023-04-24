#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

//enum types { word = 1, numero, operador, delimitador };

/* gramatica
<Expressão> :== <Termo> | ε
<Termo> :== <Número> | <Número> + <Número>
<Número> :== Inteiro | Real
*/

//Funções para cada simbolo da gramática 
expr(FILA *f);
double term(TOKEN *token);
num(char** input);

expr(FILA *f){
  double result = 0.0, numLeft = 0.0, numRight = 0.0;
  TOKEN *left = (TOKEN*) malloc(sizeof(TOKEN));
  TOKEN *right = (TOKEN*) malloc(sizeof(TOKEN));
  TOKEN *op = (TOKEN*) malloc(sizeof(TOKEN));
  
   *left = retornaToken(f);
   numLeft = term(left);

   //printf("%f", numLeft);
  
   *op = retornaToken(f);
    result = numLeft;
  
  while(true){
    if(strcmp(op->value, "+") == 0){
       *right = retornaToken(f);
       numRight = term(right);
       result=result+numRight;
    }else break;
    *op = retornaToken(f);
  }

  printf("%f", result);

  free(left);
  free(right);
  free(op);
  
}

double term(TOKEN *token){
  double num = 0.0;
  if(token->type == numero){
    num = strtod(token->value, NULL);
    return num;
  }else{
    printf("Expressão errada!");
    exit(1);
  }
}

int main(int argc, const char *argv[]) {

  FILE *fp;
  char c;
  char vector[1000];
  int r = 1, i = 0;
  vector[0] = ' ';

  if (argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    return -1;
  } else {
    while ((c = getc(fp)) != EOF) {
      vector[r] = c;
      r++;
    }
  }

  vector[r] = ' ';
  
  FILA *program = (FILA*) malloc(sizeof(FILA));
  TOKEN *token = (TOKEN*) malloc(sizeof(TOKEN));


  inicializaFila(program);
  instanciar(program, vector);
  expr(program);

  free(program);
  free(token);
  //imprimeFila(program);
}