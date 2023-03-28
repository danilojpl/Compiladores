#include <stdio.h>
#include <string.h>

void tokenizar(char token);

int main(int argc, const char * argv[]) {
  FILE *fp;
  char c;

  if (argc != 2 || (fp = fopen(argv[1], "r")) == NULL) {
      fprintf(stderr, "Usage: %s filename\n", argv[0]);
      return -1;
  }else{
    while ((c = getc(fp)) != EOF) {
      if(c != ' '){
        tokenizar(c);
      }
    }
  }
  return 0;
}

void tokenizar(char token){
  
  char t[2];
  t[0] = token;
  t[1] = '\0';
  
  char dictionary[4][2][15]={
  {"+","Soma"},
  {"-","Subtração"},
  {"*","Multiplicação"},
  {"/","Divisão"}
  };

  int c = 0;
  for(int i = 0;i < 4; i++){
      if(strcmp(t,dictionary[i][0])==0){
          printf("Operador - %s, valor(%s)\n",dictionary[i][1], dictionary[i][0]);
      }
      else c++;
  }
  if(c == 4){
    printf("Indeterminado, valor(%s)\n", t);
  }
  
}
