#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct TOKEN{
  int type;
  char value[50];
  struct TOKEN *prox; 
}TOKEN;

typedef struct FILA{
  TOKEN *ini;
  TOKEN *fim;
}FILA;

enum types {word = 1, numero, operador, delimitador};

void inicializaFila(FILA *f){
  f->ini = NULL;
  f->fim = NULL;
}


void enfileira(int type, char value[], FILA *f){
  TOKEN *ptr = (TOKEN*) malloc(sizeof(TOKEN));
  if(ptr == NULL){
    printf("Erro alocação. \n");
  }else{
    strncpy(ptr->value, value, 50);
    ptr->type = type;
    ptr->prox = NULL;
    if(f->ini == NULL){
      f->ini = ptr;
    } else{
      f->fim->prox = ptr;
    }
    f->fim = ptr;
  }
}

TOKEN retornaToken(FILA *f){
  TOKEN *ptr = f->ini;
  TOKEN dado;
  if(ptr != NULL){
    f->ini = ptr->prox;
    ptr->prox = NULL;
    dado = *ptr;
    free(ptr);
    if(f->ini == NULL){
      f->fim = NULL;
    }
    return dado;
  }else {
    printf("Programa Finalizado!\n");
    strncpy(dado.value, "Vázio", 50); 
    return dado;
  }
}

void imprimeFila(FILA *f){
  TOKEN *ptr = f->ini;
  if(ptr != NULL){
    while(ptr != NULL){
      printf("type:%d value:%s\n", ptr->type,ptr->value);
      ptr = ptr->prox;
    }
  }else
    printf("Fila vazia!\n");
}

bool operadores(char token){
  if(token == '+' || token == '-' || token == '*' || token == '/' || token == '='){
    return true;
  }
  return false;
}


void lexical_analyzer(int *p,char token, char token_prox, char token_ant,char* str, int *pA, int *pB, FILA *f1){

  
  if(operadores(token)){
    switch (token){
      case '+':{
        char value[50];
        strncpy(value, "+", 50);
        enfileira(3,value, f1);
        break; 
      }
      case '-':{
        char value[50];
        strncpy(value, "-", 50);
        enfileira(3, value, f1);
        break; 
      }
      case '/':{
        char value[50];
        strncpy(value, "/", 50);
        enfileira(3, value, f1);
        break; 
      }
      case '*':{
        if(token_prox == '*'){
          char value[50];
          strncpy(value, "**", 50);
          enfileira(3, value, f1);
          (*p)++;
        }else{
          char value[50];
          strncpy(value, "*", 50);
          enfileira(3, value, f1);  
        }
        break;
      }
    }
    *pA = *pB + 1;
  }
}

FILA instanciar(FILA *f1, char *vector){
  char exp[100];
  char c;
  int  pA = 0, pB = 0,i = 0;

  i=0;
  while(vector[i] != '\0'){
    pB = i;
    lexical_analyzer(&i,vector[i],vector[i+1],vector[i-1],vector, &pA, &pB, f1);
    i++;
  }
  return *f1;
}

int main(int argc, const char * argv[]) {
  FILE *fp;
  char c;
  char vector [1000];
  int r = 0 ,i = 0;
  
  if (argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Usage: %s filename\n", argv[0]);
      return -1;
  }else{
    while ((c = getc(fp)) != EOF) {
      vector[r] = c;
      r++;
    }
  }

  FILA *program = (FILA*) malloc(sizeof(FILA));
  TOKEN *token = (TOKEN*) malloc(sizeof(TOKEN));
  inicializaFila(program);
  instanciar(program, vector);
  imprimeFila(program);
  
  // *token = retornaToken(program);
  // printf("%d", token->type);
  return 0;
}