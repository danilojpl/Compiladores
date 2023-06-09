#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 256

typedef struct {
  unsigned char confirmation;
  unsigned char accumulator;
  unsigned char programCounter;
  unsigned int programSize;
  unsigned char *memory;
} InterpreterState;

void initializeInterpreterState(InterpreterState *state) {
  state->confirmation = 0;
  state->accumulator = 0;
  state->programCounter = 0;
  state->programSize = 0;
  state->memory = (unsigned char *)malloc(MEMORY_SIZE * sizeof(unsigned char));
  for (int i = 0; i < MEMORY_SIZE; i++) {
    state->memory[i] = 0;
  }
}

void loadProgramFromFile(const char *filename, InterpreterState *state) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    exit(1);
  }

  // Verificar o número de confirmação
  unsigned char confirmation;
  size_t a = fread(&confirmation, sizeof(unsigned char), 1, file);
  if (confirmation != 0b00101010) {
    printf("Arquivo inválido.\n");
    exit(1);
  }

  // Carregar o estado inicial do acumulador e program counter
  size_t b = fread(&(state->accumulator), sizeof(unsigned char), 1, file);
  size_t c = fread(&(state->programCounter), sizeof(unsigned char), 1, file);

  fseek(file, -3, SEEK_CUR);

  // Carregar o programa para a memória
  size_t bytesRead =
      fread(state->memory, sizeof(unsigned char), MEMORY_SIZE, file);

  state->programSize = bytesRead - 3;

  if (bytesRead >= MEMORY_SIZE) {
    printf("Arquivo inválido.\n");
    exit(1);
  }

  printf("Programa carregado com sucesso.\n");
  fclose(file);
}

void executeProgram(InterpreterState *state) {
  state->programCounter += 3;
  while (1) {
    // Buscar a próxima instrução no program counter
    unsigned char instruction = state->memory[state->programCounter];

    // Executar a instrução
    switch (instruction) {
    case 0x00:
      // NOP - Não faz nada
      break;

    case 0x10:
      // STA <endereço> - Armazena o valor do acumulador na memória
      state->memory[state->memory[state->programCounter + 1]] =
          state->accumulator;
      state->programCounter += 2;
      break;

    case 0x20:
      // LDA <endereço> - Carrega um valor da memória para o acumulador
      state->accumulator =
          state->memory[state->memory[state->programCounter + 1]];
      state->programCounter += 2;
      break;

    case 0x30:
      // ADD <endereço> - Soma o valor da memória ao acumulador
      state->accumulator +=
          state->memory[state->memory[state->programCounter + 1]];
      state->programCounter += 2;
      break;

    case 0x40:
      // OR <endereço> - Faz a operação OR entre o valor da memória e o
      // acumulador
      state->accumulator |=
          state->memory[state->memory[state->programCounter + 1]];
      state->programCounter += 2;
      break;

    case 0x50:
      // AND <endereço> - Faz a operação AND entre o valor da memória e o
      // acumulador
      state->accumulator &=
          state->memory[state->memory[state->programCounter + 1]];
      state->programCounter += 2;
      break;

    case 0x60:
      // NOT - Inverte os bits do acumulador
      state->accumulator = ~(state->accumulator);
      state->programCounter++;
      break;

    case 0x70:
      // JMP <endereço> - Salta para um endereço específico
      state->programCounter = state->memory[state->programCounter + 1];
      break;

    case 0x80:
      // JN <endereço> - Salta para um endereço específico se o acumulador for
      // negativo
      if (state->accumulator & 0x80) {
        state->programCounter = state->memory[state->programCounter + 1];
      } else {
        state->programCounter += 2;
      }
      break;

    case 0x90:
      // JZ <endereço> - Salta para um endereço específico se o acumulador for
      // zero
      if (state->accumulator == 0) {
        state->programCounter = state->memory[state->programCounter + 1];
      } else {
        state->programCounter += 2;
      }
      break;

    case 0xF0:
      // HLT - Termina a execução do programa
      printf("Programa terminado.\n");
      return;

    default:
      printf("Instrução inválida: %02X\n", instruction);
      exit(1);
    }

    // Verificar condição de parada
    if (state->programCounter >= state->programSize) {
      printf("Programa terminado.\n");
      return;
    }
  }
}

void printInterpreterState(const InterpreterState *state) {
  printf("Valor do Acumulador: %d\n", state->accumulator);
  printf("Valor do PC: %d\n", state->programCounter);
  // Imprimir as flags N e Z
  printf("Flag N: ...\n");
  printf("Flag Z: ...\n");
  // Imprimir o mapa de memória completo
  printf("Mapa de memória completo:\n");
  for (int i = 0; i < MEMORY_SIZE; i++) {
    printf("%d: %d\n", i, state->memory[i]);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Uso: %s <nome_do_arquivo>\n", argv[0]);
    return 0;
  }

  const char *filename = argv[1];
  InterpreterState state;
  initializeInterpreterState(&state);
  loadProgramFromFile(filename, &state);

  executeProgram(&state);
  printInterpreterState(&state);

  free(state.memory);
  return 0;
}