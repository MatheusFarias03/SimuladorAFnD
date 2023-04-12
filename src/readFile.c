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
      for (int i = 0; i < *q_estados; i++){
        for (int j = 0; j < *tamanho_alfabeto; j++){
          for (int k = 0; k < *q_estados; k++){
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
    else if (num_linha > 5 && num_linha <= 5 + *n_transicoes){

      int linha = linha_atual[0] - 48; // estado inicial da transicao
      int coluna = 0;

      /* Iterar sobre o alfabeto para encontrar a posicao da letra */      
      for (int i = 0; i < *tamanho_alfabeto; i++){
        if (alfabeto[i] == linha_atual[2]){
          coluna = i;
          break;
        }
      }
      
      int vai_para = linha_atual[4] - 48;

      matriz_transicoes[linha][coluna][vai_para] = vai_para; // (ler estado que ele vai) [0, NULL, 2, ..., NULL]
        
    }

    /* Ler o numero de palavras que serao avaliadas */
    else if (num_linha == 5 + *n_transicoes + 1){
      *n_palavras = atoi(linha_atual);
    }

    /* Faz a verificacao se a palavra eh aceita pelo AFnD ou nao */
    else if (num_linha > 5 + *n_transicoes + 1 && num_linha <= 5 + *n_transicoes + 1 + *n_palavras){
      int resultado = 0;
      int aux = 0;
      transicao_afnd(0, linha_atual, matriz_transicoes, alfabeto, tamanho_alfabeto, f_estados, q_estados, array_estados_finais, &resultado);
      if (resultado > 0) 
      {
        printf("\n%d: ", count);
        /* Printa um caractere por vez ate chegar em '\n' para que a saida fique melhor apresentada */
        while (linha_atual[aux] != '\n') printf("%c", linha_atual[aux++]);
        printf(" OK");
      }
      else{
        printf("\n%d: ", count);
        while (linha_atual[aux] != '\n') printf("%c", linha_atual[aux++]);
        printf(" not OK");
      }
      count++;
    }
    
  }
	
	fclose(arquivo);
	return 0;
}

/*
* Recebe o estado inicial, a linha que esta lendo, a matriz de transicao, alfabeto, o tamanho do alfabeto, a quantidade de estados finais, a quantidade de estados, o vetor contendo os estados finais e uma variavel resultado que assume 1 se a palavra foi aceita e 0 se foi rejeitada.
*/

void transicao_afnd(int estado_inicial, char* linha_atual, int*** matriz_transicoes, char* alfabeto, int* tamanho_alfabeto, int* f_estados, int* q_estados, int* array_estados_finais, int* resultado)
{
  char letra_lida; 
  char* palavra = linha_atual;
  int tamanho_palavra = 0; 
  int prox_estados[*q_estados];
  int estados_atuais[*q_estados];

  /* Define o tamanho da palavra que sera avaliada */
  for (int i = 0; i < sizeof(linha_atual); i++){
    if (linha_atual[i] != '\n') tamanho_palavra++;
    else break;
  }

  /* Inicializa os vetores com 0 indicando que nao estao sendo avaliados no momento */
  for (int i = 0; i < *q_estados; i++){
    prox_estados[i] = 0;
    estados_atuais[i] = 0;
  }

  /* Estado Inicial sera o primeiro a ser avaliado */
  estados_atuais[estado_inicial] = 1;

  /* Percorre a palavra com base no tamanho da mesma */
  while (tamanho_palavra > 0){
    letra_lida = *linha_atual;
    /* Percorre as linhas da matriz tridimensional, que se referem aos estados */
    for (int i = 0; i < *q_estados; i++){
      /* Se os estados tiverem valor = 1 significa que eles sao resultados de transicoes de estados passados */
      if (estados_atuais[i] == 1){
        /* Percorre as colunas da matriz tridimensional, que se referem aos simbolos do alfabeto */
        for (int j = 0; j < *tamanho_alfabeto; j++){
          /* Percorrem o vetor que se refere as transicoes de cada estado ao ler determinado simbolo */
          for (int k = 0; k < *q_estados; k++){
            /* Retorna o valor da coluna com base no simbolo */
            int col = coluna(alfabeto, tamanho_alfabeto, letra_lida);
            /* Se for diferente de -1 significa que ao ler o simbolo da coluna, o estado daquela linha vai para outro estado */
            if (matriz_transicoes[i][col][k] != -1){
              /* Caso a posicao referente a determinado estado seja igual a 1, significa que aquele estado sera verificado na proxima interacao */
              prox_estados[k] = 1;
            }
          }
        }
      }
    }

    /* Reinicia o vetor com os proximos estados */
    for (int i = 0; i < *q_estados; i++){  
      estados_atuais[i] = prox_estados[i];
      prox_estados[i] = 0;
    }
    linha_atual++;
    tamanho_palavra--;
  }

  /* Verifica se um dos estados que finalizaram a verificacao eh final */ 
  for (int i = 0; i < *q_estados; i++){
    for (int j = 0; j < *f_estados; j++){
      if (estados_atuais[i] == 1 && i == array_estados_finais[j]){
        /* Valor que indica que a palavra foi aceita */
        *resultado = 1; 
      }
    }
  }
}

/* Retorna o valor da coluna referente a determinado simbolo do alfabeto */
int coluna(char* alfabeto, int* tamanho_alfabeto, char letra_lida){ 
  for (int i = 0; i < *tamanho_alfabeto; i++)
  {
    if (letra_lida == alfabeto[i])
    {
      return i;
    }
  }
}
