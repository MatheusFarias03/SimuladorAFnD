/*
 * Integrantes (NOME - TIA):
 * Matheus Farias de Oliveira Matsumoto - 32138271
 * Leonardo Pinheiro de Souza - 32127391
 * */

#include <stdio.h>
#include "readFile.h"

int main(int argc, char* argv[])	
{
	/* Checar numero de parametros passados. */
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
          	return 1;
      	}
	
	/* Declaracao das variaveis. */
	char* nome_arquivo = argv[1];
	char* alfabeto;
	int q_estados, f_estados, n_transicoes = 0;
	char** palavras;

	ler_arquivo_afnd(nome_arquivo, alfabeto, q_estados, f_estados, n_transicoes, palavras);
	
	return 0;
}
