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

//REMOVE LINHA PELA POR CHAVE PRIMARIA
void removerLinhaPorChave(Tabela *tabela, int chavePrimaria) {
    int linhaParaRemover = -1;

    // Encontrar a linha com a chave primária especificada
    for (int i = 0; i < tabela->linhas; i++) {
        if (tabela->table[i][0].intVal == chavePrimaria) {
            linhaParaRemover = i;
            break;
        }
    }

    // Se a linha foi encontrada, removê-la
    if (linhaParaRemover != -1) {
        // Libere a memória da linha
        free(tabela->table[linhaParaRemover]);

        // Mova todas as linhas após a removida para "cima" no array
        for (int i = linhaParaRemover; i < tabela->linhas - 1; i++) {
            tabela->table[i] = tabela->table[i + 1];
        }

        // Realoque o array da tabela para ter uma linha a menos
        tabela->table = realloc(tabela->table, (tabela->linhas - 1) * sizeof(Celula*));
        tabela->linhas--;
    } else {
        printf("Chave primária não encontrada.\n");
    }
}

//PRINTA A TABELA NO FORMATO ASCII. AINDA ESTÁ INCOMPLETA
void mostrarTabela(Tabela *tabela) {
    // Verificar se a tabela está vazia
    if (tabela->linhas == 0) {
        printf("A tabela está vazia.\n");
        return;
    }

    // Cabeçalho da Tabela
    for (int i = 0; i < tabela->colunas; i++) {
        printf("| %s ", tabela->nomeColuna[i]);
    }
    printf("|\n");

    // Linha Separadora
    for (int i = 0; i < tabela->colunas; i++) {
        printf("+-----");
    }
    printf("+\n");

    // Dados da Tabela
    for (int i = 0; i < tabela->linhas; i++) {
        for (int j = 0; j < tabela->colunas; j++) {
            // Exemplo de exibição: ajuste conforme os tipos de dados e formatação
            if (j == 0) {  // Supondo que a primeira coluna seja de inteiros
                printf("| %d ", tabela->table[i][j].intVal);
            } else {  // Supondo que as outras colunas sejam strings
                printf("| %s ", tabela->table[i][j].strVal);
            }
        }
        printf("|\n");
    }

    // Linha Final
    for (int i = 0; i < tabela->colunas; i++) {
        printf("+-----");
    }
    printf("+\n");
}

int main(){
	

	return 0;
}