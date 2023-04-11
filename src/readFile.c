#include <stdio.h>
#include <stdlib.h>
#include "readFile.h"

/* Ler o arquivo da seguinte maneira:
 *
 * 1a linha: símbolos do alfabeto de entrada (no maximo 10);
 * 2a linha: numero de Q estados (primeiro estado sempre q0);
 * 3a linha: numero de estados finais; 
 * 4a linha: lista dos estados finais, separado por espaco;
 * 5a linha: numero de transicoes do AFnD;
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
 */

int ler_arquivo_afnd(const char* nome_arquivo, char* alfabeto, int* tamanho_alfabeto, int* q_estados, int* f_estados, int* array_estados_finais ,int* n_transicoes, int*** matriz_transicoes, int* n_palavras, char** palavras)
{
	/* Declarar variaveis. */
  	FILE* arquivo;
  	char linha_atual[NUM_MAX_PALAVRA];
  	int num_linha = 0;
  	int count = 1;
	int contador_finais = 0;
  
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

    	/* Ler a linha caracter por caracter. Adquire o alfabeto. */
    	if (num_linha == 1)
    	{
      		*tamanho_alfabeto = 0;
      		for (int i = 0; i < sizeof(linha_atual); i++)
      		{
        		if (linha_atual[i] != '\0')
				{
					alfabeto[*tamanho_alfabeto] = linha_atual[i];
          			*tamanho_alfabeto = *tamanho_alfabeto + 1;
				}
				else if (linha_atual[i] == '\0')
				{
					break;
				}
				else if (i > NUM_MAX_ALFABETO)
				{
					return 1;
				}
      		}
      		*tamanho_alfabeto = *tamanho_alfabeto - 1;
    	}

    	/* Ler numero de Q estados. */
    	else if (num_linha == 2)
    	{
      		*q_estados = atoi(linha_atual);   

      		/* Aloca a matriz tridimensional que sera usada para visualizar todas as transicoes. */
      		matriz_transicoes = (int ***) malloc(*q_estados * sizeof(int **)); // numero de linhas = numero de estados
      		for (int i = 0; i < *q_estados; i++)
      		{
        		matriz_transicoes[i] = (int **) malloc(*tamanho_alfabeto * sizeof(int *)); // numero de colunas = numero de letras presentes no alfabeto
        		for (int j = 0; j < *tamanho_alfabeto; j++)
        		{
          			matriz_transicoes[i][j] = (int*) malloc(*q_estados * sizeof(int)); // definimos o numero de estados possiveis apos cada leitura = numero de estados do AFnD
        		}
      		}

      		/* Preenche a matriz com -1, futuramente sera preenchida com os valores adequados. */
      		for (int i = 0; i < *q_estados; i++)
			{
        		for (int j = 0; j < *tamanho_alfabeto; j++)
				{
          			for (int k = 0; k < *q_estados; k++)
					{
            			matriz_transicoes[i][j][k] = -1;
          			}
        		}
      		}
    	}

    	/* Ler numero de F estados. */
    	else if (num_linha == 3)
    	{
      		*f_estados = atoi(linha_atual);
    	}

		/* Ler inteiro por inteiro para obter os estados finais. */
		else if (num_linha == 4)
		{
		for (int i = 0; i < sizeof(linha_atual); i++)
		{
			if (linha_atual[i] != '\0' && linha_atual[i] != ' ')
					{
						array_estados_finais[contador_finais] = linha_atual[i] - 48;
						contador_finais++;
					}
					else if (linha_atual[i] == '\0')
					{
						break;
					}
				else if (i > NUM_MAX_ESTADOS)
					{
						return 1;
					}
		}
		}

		/* Ler numero de transicoes. */
		else if (num_linha == 5)
		{
			*n_transicoes = atoi(linha_atual);
		}

		/* Ler cada transicao linha por linha. */
		else if (num_linha > 5 && num_linha <= 5 + *n_transicoes)
		{

			int linha = linha_atual[0] - 48; // estado inicial da transicao
			int coluna = 0;

			/* Iterar sobre o alfabeto para encontrar a posicao da letra */      
			for (int i = 0; i < *tamanho_alfabeto; i++)
			{
				if (alfabeto[i] == linha_atual[2])
				{
					coluna = i;
					break;
				}
			}
		
			int vai_para = linha_atual[4] - 48;

			matriz_transicoes[linha][coluna][vai_para] = vai_para; // (ler estado que ele vai) [0, NULL, 2, ..., NULL]

			for (int i = 0; i < *q_estados; i++) 
			{
				printf("\nQ%d:\n", i);
				for (int j = 0; j < (*tamanho_alfabeto); j++) 
				{
					for (int k = 0; k < *q_estados; k++) 
					{
						printf("%d ", matriz_transicoes[i][j][k]);
					}
					printf(" | ");
				}
			}	
		}

		else if (num_linha == 5 + *n_transicoes + 1)
		{
			*n_palavras = atoi(linha_atual);
		}

		else if (num_linha > 5 + *n_transicoes + 1 && num_linha <= 5 + *n_transicoes + 1 + *n_palavras)
		{
			int resultado = 0;
			transicao_afnd(0, linha_atual, matriz_transicoes, alfabeto, tamanho_alfabeto, f_estados, q_estados, &resultado, array_estados_finais);
			if (resultado > 0)
			{
				printf("\n%d: %s OK", count, linha_atual);
			}
			else
			{
				printf("\n%d: %s not OK", count, linha_atual);
			}
			count++;
		}
    
  	}
	fclose(arquivo);
	return 0;
}

/*
* Recebe o estado atual, a linha que esta lendo, a matriz de transicao, alfabeto e estado final.
* Dependendo da letra lida na linha, a funcao ira chamar ela mesma e se 
* direcionar para o proximo estado. Se ler '\0' na palavra e o estado atual
* for estado final, retorna 0. Caso o contrario, retorna 1.
* */

void transicao_afnd(int estado_atual, char* linha_atual, int*** matriz_transicoes, char* alfabeto, int* tamanho_alfabeto, int* f_estados, int* q_estados, int* resultado, int* array_estados_finais)
{
	/* Definir variaveis. */
  	char letra_lida = *linha_atual;
  	int coluna;

  	/* Encontrar a letra passada no vetor alfabeto. */
  	for (int i = 0; i < *tamanho_alfabeto; i++)
  	{
    	if (letra_lida == alfabeto[i])
    	{
      		coluna = i;
      		break;
    	}
  	}

  	/* Se a palavra ainda nao terminou. */
  	if(*linha_atual != '\n')
  	{
    	/* Para cada elemento na transicao, chamar a funcao novamente. */
    	for (int i = 0; i < *q_estados; i++) 
    	{
      		/* Se encontrarmos uma possivel transicao do estado atual de acordo com a letra passada, chama a funcao novamente. */
      		if (matriz_transicoes[estado_atual][coluna][i] != -1)
      		{
        		linha_atual++;
          		transicao_afnd(matriz_transicoes[estado_atual][coluna][i], linha_atual, matriz_transicoes, alfabeto, tamanho_alfabeto, f_estados, q_estados, resultado, array_estados_finais);
      		}	
    	}
  	}
	/* Se a palavra chegou no fim. */
	/* Checar se o estado atual eh estado final. */
	for (int i = 0; i < *f_estados; i++)
	{
		/* Caso seja estado final, aumenta o resultado. */
		if (estado_atual == array_estados_finais[i])
		{
			*resultado = *resultado + 1;
		}
	}
  
  
}