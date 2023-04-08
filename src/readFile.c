#include <stdio.h>
#include <stdlib.h>
#include "readFile.h"

#define false 0
#define true 1
#define NUM_MAX_ALFABETO 10
#define NUM_MAX_ESTADOS  20
#define NUM_MAX_PALAVRA 100

/* Ler o arquivo da seguinte maneira:
 *
 * 1a linha: símbolos do alfabeto de entrada (no maximo 10);
 * 2a linha: numero de Q estados (primeiro estado sempre q0);
 * 3a linha: numero de estados finais; 
 * 4a linha: lista dos estados finais, separado por espaco;
 * 5a linha: numero de transicoes do AFND;
 *
 * Nas N linhas subsequentes, as transicoes especificadas, separadas
 * por espaco em branco, no seguinte formato:
 *
 * <estado corrent> espaco <simbolo do alfabeto> espaco <estado chegada>
 *
 * Ao final das N linhas, ler linha com inteiro T especificando o numero
 * de palavras que deverao ser testadas no AFND. As palavras estarao
 * cada uma em uma linha, contendo somente os simbolos do alfabeto. As
 * palavras devem ter no maximo 100 caracteres.
 *
 * ERROS (Num -> Mensagem):
 * 1 -> Erro ao ler o arquivo;
 * 2 -> Quantidade de letras do alfabeto supera limite;
 *  
 * */
int ler_arquivo_afnd(const char* nome_arquivo, char* alfabeto, int q_estados, int f_estados, int n_transicoes, char** palavras)
{
	FILE* arquivo;
	char linha_atual[NUM_MAX_PALAVRA];
	int num_linha = 0;

	/* Abrir arquivo. */
	arquivo = fopen(nome_arquivo, "r");
	if (arquivo == NULL)
	{
		printf("Erro ao ler o arquivo.");
		return 1;
	}

	/* Ler o arquivo linha por linha. */
	while (fgets(linha_atual, sizeof(linha_atual), arquivo))
	{
        num_linha++;
        
        switch (num_linha) {
            case 1:
                {
                    /* Ler a linha caracter por caracter. Adquire o alfabeto. */
                    for (int i = 0; i < sizeof(linha_atual); i++)
                    {
                        if (linha_atual[i] != NULL)
						{
							*alfabeto = *linha_atual;
							alfabeto++;
						}
						if (i > NUM_MAX_ALFABETO)
						{
							printf("ERROR: Quantidade de letras do alfabeto supera o limite máximo (10).");
							return 2;
						}
                    }
                }
                break;
                
            default:
                break;
        }
	}

	return 0;
}
