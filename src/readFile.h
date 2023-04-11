#ifndef READ_FILE_H
#define READ_FILE_H

#define false 0
#define true 1
#define NUM_MAX_ALFABETO 10
#define NUM_MAX_ESTADOS  20
#define NUM_MAX_PALAVRA 100

int ler_arquivo_afnd(const char* nome_arquivo, char* alfabeto, int* tamanho_alfabeto, int* q_estados, int* f_estados, int* array_estados_finais ,int* n_transicoes, int*** matriz_transicoes, int* n_palavras, char** palavras);

void transicao_afnd(int estado_atual, char* linha_atual, int*** matriz_transicoes, char* alfabeto, int* tamanho_alfabeto, int* f_estados, int* q_estados, int* resultado, int* array_estados_finais);

#endif