#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TOKEN {
  int type;
  char value[50];
  struct TOKEN *prox;
} TOKEN;

typedef struct FILA {
  TOKEN *ini;
  TOKEN *fim;
} FILA;

enum types { word = 1, numero, operador, delimitador };

void inicializaFila(FILA *f) {
  f->ini = NULL;
  f->fim = NULL;
}

void enfileira(int type, char value[], FILA *f) {
  TOKEN *ptr = (TOKEN *)malloc(sizeof(TOKEN));
  if (ptr == NULL) {
    printf("Erro alocação. \n");
  } else {
    strncpy(ptr->value, value, 50);
    ptr->type = type;
    ptr->prox = NULL;
    if (f->ini == NULL) {
      f->ini = ptr;
    } else {
      f->fim->prox = ptr;
    }
    f->fim = ptr;
  }
}

TOKEN retornaToken(FILA *f) {
  TOKEN *ptr = f->ini;
  TOKEN dado;
  if (ptr != NULL) {
    f->ini = ptr->prox;
    ptr->prox = NULL;
    dado = *ptr;
    free(ptr);
    if (f->ini == NULL) {
      f->fim = NULL;
    }
    return dado;
  } else {
    printf("Programa Finalizado!\n");
    strncpy(dado.value, "Vázio", 50);
    return dado;
  }
}

void imprimeFila(FILA *f) {
  TOKEN *ptr = f->ini;
  if (ptr != NULL) {
    while (ptr != NULL) {
      printf("type:%d value:%s\n", ptr->type, ptr->value);
      ptr = ptr->prox;
    }
  } else
    printf("Fila vazia!\n");
}

bool operadores(char token) {
  if (token == '+' || token == '-' || token == '*' || token == '/' ||
      token == '=') {
    return true;
  }
  return false;
}

bool numeros(char token) {
  if (token == '0' || token == '1' || token == '2' || token == '3' ||
      token == '4' || token == '5' || token == '6' || token == '7' ||
      token == '8' || token == '9')
    return true;
  else
    return false;
}

char *subString(char *str, int pA, int pB) {
  int i;
  char *subStr = (char *)malloc(sizeof(char) * (pB - pA + 2));
  for (i = pA; i <= pB; i++) {
    subStr[i - pA] = str[i];
  }
  subStr[pB - pA + 1] = '\0';
  return (subStr);
}

bool separadores(char token) {
  if (token == '.')
    return true;
  else
    return false;
}

void lexical_analyzer(int *p, char token, char token_prox, char token_ant,
                      char *str, int *n, int *s, int *pA, int *pB, FILA *f1) {
  bool numb;
  bool sep;

  numb = numeros(token);
  sep = separadores(token);

  if (numb == true || sep == true) {
    *n = 0;
    (*pB)++;
    if (sep && numeros(token_prox) && numeros(token_ant))
      (*s)++;
  } else if (*n == 0) {
    (*pB)--;
    if (numeros(str[*pA]) && numeros(str[*pB]) && *s <= 1) {
      enfileira(2, subString(str, *pA, *pB), f1);
      *n = 1;
      (*pB)++;
      *pA = *pB;
      *s = 0;
    }
  }

  if (operadores(token)) {
    switch (token) {
    case '+': {
      char value[50];
      strncpy(value, "+", 50);
      enfileira(3, value, f1);
      break;
    }
    case '-': {
      char value[50];
      strncpy(value, "-", 50);
      enfileira(3, value, f1);
      break;
    }
    case '/': {
      char value[50];
      strncpy(value, "/", 50);
      enfileira(3, value, f1);
      break;
    }
    case '*': {
      if (token_prox == '*') {
        char value[50];
        strncpy(value, "**", 50);
        enfileira(3, value, f1);
        (*p)++;
      } else {
        char value[50];
        strncpy(value, "*", 50);
        enfileira(3, value, f1);
      }
      break;
    }
    }
  }
  if (*n == 1) {
    *pA = *pB + 1;
  }
}

FILA instanciar(FILA *f1, char *vector) {
  char exp[100];
  char c;
  int pA = 0, pB = 0, i = 0, s = 0, n = 1;

  i = 0;
  while (vector[i] != '\0') {
    pB = i;
    lexical_analyzer(&i, vector[i], vector[i + 1], vector[i - 1], vector, &n,
                     &s, &pA, &pB, f1);
    i++;
  }
  return *f1;
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

  FILA *program = (FILA *)malloc(sizeof(FILA));
  TOKEN *token = (TOKEN *)malloc(sizeof(TOKEN));
  inicializaFila(program);
  instanciar(program, vector);
  imprimeFila(program);
  return 0;
}