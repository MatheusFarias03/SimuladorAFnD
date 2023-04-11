/*
 * Integrantes (NOME - TIA):
 * Matheus Farias de Oliveira Matsumoto - 32138271
 * Leonardo Pinheiro de Souza - 32127391
 * */

#include <stdio.h>
#include <string.h>
#include "readFile.h"

int main(int argc, char* argv[])	
{	
	/* Declaracao das variaveis. */
	char* nome_arquivo = "InputAFnD.txt";
	char alfabeto[NUM_MAX_ALFABETO];
  	memset(alfabeto, '\0', 10);
	int tamanho_alfabeto, q_estados, f_estados, n_transicoes, n_palavras = 0;
  	int*** matriz_transicoes;
	char** palavras;
  	int array_estados_finais[NUM_MAX_ESTADOS];

	ler_arquivo_afnd(nome_arquivo, alfabeto, &tamanho_alfabeto, &q_estados, &f_estados,  array_estados_finais, &n_transicoes, matriz_transicoes, &n_palavras, palavras);
  // printf("\nAlfabeto: %s", alfabeto);
  // printf("\nNumero de Estados: %d", q_estados);
  // printf("\nNumero de Estados Finais: %d", f_estados);
  // printf("\nArray Estados Finais:");
  // for (int i = 0; i < f_estados; i++)
  // {
  //   printf(" %d ", array_estados_finais[i]);    
  // }
  // printf("\nNumero de Transicoes: %d", n_transicoes);
  // printf("\nNumero de palavras para verificar: %d", n_palavras);

	
	return 0;
}
