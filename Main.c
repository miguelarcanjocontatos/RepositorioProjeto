#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAMANHO_MAX_NOME 50
//ESTRUTURA DA TABELA
struct Tabela
{

	char **nomeColuna;
	Celula **table;
	//QUANTIDADE DE LINHAS E COLUNAS QUE A TABELA VAI TER, ASSIM CADA TABELA VAI "CONHECER" SUAS PROPRIAS DIMENSÕES
	int linhas, colunas;
};

//CELULA DA TABELA: GUARDA TIPOS DE DADOS DISTINTOS
typedef union 
{
	int intVal;
	char strVal[TAMANHO_MAX_NOME];
	float floatVal;

}Celula;

//AQUI É O CONSTRUTOR DA TABELA
Tabela criarTabela(int colunas, int linhas)
{
	Tabela newTable;
	//ALOCANDO MEMORIA PARA OS NOMES DAS COLUNAS
	newTable.nomeColuna = malloc(colunas*sizeof(char*));
	for(int i = 0; i < colunas; i++)
	{
		newTable.nomeColuna[i] = malloc(TAMANHO_MAX_NOME*sizeof(char));
	}
	newTable.table = malloc(linhas*sizeof(Celula*));

	//ALOCANDO ESPAÇO PARA AS CELULAS DA PLANILHA
	for (int i = 0; i < linhas; i++)
	{
		newTable.table[i] = malloc(colunas*sizeof(Celula));
		// Inicializando a chave primária na primeira coluna
        newTable.table[i][0].intVal = i + 1;  // Começando de 1
	}
	//PASSANDO PARA A TABELA SUAS DIMENSÕES
	newTable.linhas = linhas;
	newTable.colunas = colunas;
	return newTable;
}

//AUMENTA A TABELA QUANDO NECESSARIO, AUMENTA UMA LINHA DE CADA VEZ
void aumentarTabela(Tabela *tabela)
{
	int novoTamanho = tabela->linhas+1;
	Celula **novaTable = realloc(tabela->table, novoTamanho*sizeof(Celula*));
	if (novaTable == NULL)
	{
		printf("Erro ao adicionar nova linha\n");
		return;
	}
	tabela->table = novaTable;
	tabela->table[tabela->linhas] = malloc(tabela->colunas*sizeof(Celula));
	tabela->table[novoTamanho][0] = novoTamanho;
	tabela->linhas = novoTamanho;
}

//LIBERA A MEMORIA DE TABELAS
void liberarTabela(Tabela *tabela) {
    // Primeiro, liberar a memória de cada nome de coluna
    for (int i = 0; i < tabela->colunas; i++) {
        free(tabela->nomeColuna[i]);
    }
    // Agora, liberar o array de nomes de colunas
    free(tabela->nomeColuna);

    // Em seguida, liberar a memória de cada linha da tabela
    for (int i = 0; i < tabela->linhas; i++) {
        free(tabela->table[i]);
    // Agora, liberar o array da tabela
    free(tabela->table);

    free(tabela);
}

int main(){
	

	return 0;
}