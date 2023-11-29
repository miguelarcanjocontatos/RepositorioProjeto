#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAMANHO_MAX_NOME 50
//CELULA DA TABELA: GUARDA TIPOS DE DADOS DISTINTOS
typedef union 
{
    int intVal;
    char strVal[TAMANHO_MAX_NOME];
    float floatVal;
}Celula;

//TIPOS DE REGISTRO TIPO DE CADA CELULA
typedef enum {
    INT_TYPE,
    STRING_TYPE,
    FLOAT_TYPE
} DataType;

//CELULAS COM REGISTROS DE CADA TIPO
typedef struct {
    DataType tipo; // Adiciona um campo para o tipo
    Celula dado;
} CelulaComTipo;

typedef struct {
    char nome[TAMANHO_MAX_NOME];
    char **nomeColuna;
    CelulaComTipo *tiposColuna; // Primeira linha com tipos e dados
    Celula **table; // Restante das linhas com apenas dados
    int linhas, colunas;
} Tabela;


//AQUI É O CONSTRUTOR DA TABELA
Tabela criarTabela(int colunas, int linhas, char nome[])
{
    Tabela newTable;
    //ALOCANDO MEMORIA PARA OS NOMES DAS COLUNAS
    newTable.nomeColuna = malloc(colunas*sizeof(char*));
    for(int i = 0; i < colunas; i++)
    {
        newTable.nomeColuna[i] = malloc(TAMANHO_MAX_NOME*sizeof(char));
    }
    newTable.table = malloc(linhas*sizeof(CelulaComTipo*));

    //MEMORIA PARA A PRIMEIRA LINHA, ONDE ESTARÁ O TIPO DE CADA COLUNA
    newTable.tiposColuna = malloc(colunas * sizeof(CelulaComTipo));

    //ALOCANDO ESPAÇO PARA AS CELULAS RESTANTES DA PLANILHA
    newTable.table = malloc((linhas - 1) * sizeof(Celula*));
    for (int i = 0; i < linhas - 1; i++) {
        newTable.table[i] = malloc(colunas * sizeof(Celula));
    }
    //PASSANDO PARA A TABELA SUAS DIMENSÕES
    newTable.linhas = linhas;
    newTable.colunas = colunas;
    strcpy(newTable.nome, nome);

    char nomeArquivo[TAMANHO_MAX_NOME];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.csv", nome); // Cria o nome do arquivo com extensão .csv
    FILE *file = fopen(nomeArquivo, "w"); // Abre o arquivo para escrita
    if (file == NULL) {
        perror("Erro ao criar o arquivo da tabela");
    } else {
        fclose(file); // Fecha o arquivo
    }

    return newTable;
}

//AUMENTA A TABELA QUANDO NECESSARIO, AUMENTA UMA LINHA DE CADA VEZ
void aumentarTabela(Tabela *tabela)
{
    int novoTamanho = tabela->linhas;
    Celula **novaTable = realloc(tabela->table, novoTamanho*sizeof(Celula*));
    if (novaTable == NULL)
    {
        printf("Erro ao adicionar nova linha\n");
        return;
    }
    tabela->table = novaTable;
    tabela->table[tabela->linhas] = malloc(tabela->colunas*sizeof(Celula));
    tabela->linhas = novoTamanho+1;
}

//LIBERA A MEMORIA DE TABELAS
void liberarTabela(Tabela *tabela) {
    // Primeiro, liberar a memória de cada nome de coluna
    for (int i = 0; i < tabela->colunas; i++) {
        free(tabela->nomeColuna[i]);
    }
    // Agora, liberar o array de nomes de colunas
    free(tabela->nomeColuna);
    free(tabela->tiposColuna);

    // Em seguida, liberar a memória de cada célula da tabela
    for (int i = 0; i < tabela->linhas - 1; i++) {
        free(tabela->table[i]);
    }
    free(tabela->table);
}

//REMOVE LINHA PELA POR CHAVE PRIMARIA
void removerLinhaPorChave(Tabela *tabela, int chavePrimaria) {
    int linhaParaRemover = -1;

    // Encontrar a linha com a chave primária especificada
    // A chave primária está na primeira coluna de cada linha
    for (int i = 0; i < tabela->linhas - 1; i++) {
        if (tabela->table[i][0].intVal == chavePrimaria) {
            linhaParaRemover = i;
            break;
        }
    }

    // Se a linha foi encontrada, removê-la
    if (linhaParaRemover != -1) {
        // Libere a memória da linha específica
        free(tabela->table[linhaParaRemover]);

        // Mova todas as linhas após a removida para "cima" no array
        for (int i = linhaParaRemover; i < tabela->linhas - 2; i++) {
            tabela->table[i] = tabela->table[i + 1];
        }

        // Realoque o array da tabela para ter uma linha a menos
        tabela->table = realloc(tabela->table, (tabela->linhas - 2) * sizeof(Celula*));
        tabela->linhas--;
    } else {
        printf("Chave primária não encontrada.\n");
    }
}

//PRINTA A TABELA NO FORMATO ASCII. AINDA ESTÁ INCOMPLETA
void mostrarTabela(Tabela *tabela) {
    if (tabela->linhas == 0) {
        printf("A tabela está vazia.\n");
        return;
    }

    // Exibindo o cabeçalho da tabela
    for (int i = 0; i < tabela->colunas; i++) {
        printf("| %s ", tabela->nomeColuna[i]);
    }
    printf("|\n");

    // Linha separadora
    for (int i = 0; i < tabela->colunas; i++) {
        printf("+-----");
    }
    printf("+\n");

    // Exibindo os dados da tabela
    for (int i = 0; i < tabela->linhas; i++) { // Começa da linha 1, já que linha 0 é para tipos
        for (int j = 0; j < tabela->colunas; j++) {
            if (i == 0)
            {
                switch (tabela->tiposColuna[j].tipo) {
                case INT_TYPE:
                    printf("| %d ", tabela->tiposColuna[j].dado.intVal);
                    break;
                case STRING_TYPE:
                    printf("| %s ", tabela->tiposColuna[j].dado.strVal);
                    break;
                case FLOAT_TYPE:
                    printf("| %f ", tabela->tiposColuna[j].dado.floatVal);
                    break;
            }}else
            {
                switch (tabela->tiposColuna[j].tipo) {
                case INT_TYPE:
                    printf("| %d ", tabela->table[i-1][j].intVal);
                    break;
                case STRING_TYPE:
                    printf("| %s ", tabela->table[i-1][j].strVal);
                    break;
                case FLOAT_TYPE:
                    printf("| %f ", tabela->table[i-1][j].floatVal);
                    break;
            }
            }
            }
        }
        printf("|\n");
    }

//PEGAR ENTRADA DO USUARIO
void PegarDados(Tabela *tabela) {
    char buffer[TAMANHO_MAX_NOME];
    for (int i = 0; i < tabela->linhas; i++) {
        printf("Linha %d:\n", i + 1);
        for (int j = 0; j < tabela->colunas; j++) {
            printf("Digite o valor para a coluna '%s': ", tabela->nomeColuna[j]);
            if (i == 0)
            //AQUI ELE PEGA OS DADOS PARA A PRIMEIRA FILEIRA, JÁ QUE ELA VAI TER UMA ESTRUTURA DIFERENTE
            {
                switch (tabela->tiposColuna[j].tipo) {
                case INT_TYPE://PEGA CASO A COLUNA SEJA DO TIPO INT
                    scanf("%d", &tabela->tiposColuna[j].dado.intVal);
                    break;
                case STRING_TYPE://PEGA CASO A COLUNA SEJA DO TIPO STRING
                    scanf(" %49s", buffer); // Limitando a leitura para evitar overflow
                    strncpy(tabela->tiposColuna[j].dado.strVal, buffer, TAMANHO_MAX_NOME);
                    break;
                case FLOAT_TYPE://PEGA CASO A COLUNA SEJA DO TIPO FLOAT
                    scanf("%f", &tabela->tiposColuna[j].dado.floatVal);
                    break;
            }}else
            {
                //AQUI ELE PEGA PARA O RESTANTE DA TABELA
                switch (tabela->tiposColuna[j].tipo) {
                case INT_TYPE://PEGA CASO A COLUNA SEJA DO TIPO INT
                    scanf("%d", &tabela->table[i-1][j].intVal);
                    break;
                case STRING_TYPE://PEGA CASO A COLUNA SEJA DO TIPO STRING
                    scanf(" %49s", buffer); // Limitando a leitura para evitar overflow
                    strncpy(tabela->table[i-1][j].strVal, buffer, TAMANHO_MAX_NOME);
                    break;
                case FLOAT_TYPE://PEGA CASO A COLUNA SEJA DO TIPO FLOAT
                    scanf("%f", &tabela->table[i-1][j].floatVal);
                    break;
            }
            }
            //PEGAR ALBUNS BUGS DE LINHA
            getchar();
        }
    }
}
//CONVERTE O TIPO DE DADO DAS CELULAS PARA TEXTO LEGIVEL
const char* tipoParaString(DataType tipo) {
    switch (tipo) {
        case INT_TYPE:
            return "Inteiro";
        case STRING_TYPE:
            return "String";
        case FLOAT_TYPE:
            return "Flutuante";
        default:
            return "Desconhecido";
    }
}
//SALVA OS DADOS DE UMA TABELA EM UM ARQUIVO
void salvarArquivo(Tabela *tabela) {
    if (tabela == NULL) {
        printf("A tabela é nula. Não é possível salvar.\n");
        return;
    }

    char nomeArquivo[100];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.csv", tabela->nome);
    FILE *file = fopen(nomeArquivo, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para salvar a tabela");
        return;
    }

    // Escrevendo os tipos e os dados da primeira linha
    for (int j = 0; j < tabela->colunas; j++) {
        fprintf(file, "%s:", tipoParaString(tabela->tiposColuna[j].tipo)); // Escreve o tipo da coluna
        // Escreve o dado da primeira linha
        switch (tabela->tiposColuna[j].tipo) {
            case INT_TYPE:
                fprintf(file, "%d", tabela->tiposColuna[j].dado.intVal);
                break;
            case STRING_TYPE:
                fprintf(file, "\"%s\"", tabela->tiposColuna[j].dado.strVal);
                break;
            case FLOAT_TYPE:
                fprintf(file, "%f", tabela->tiposColuna[j].dado.floatVal);
                break;
        }
        if (j < tabela->colunas - 1) fprintf(file, ","); // Separador
    }
    fprintf(file, "\n");

    // Escrevendo os dados das demais linhas
    for (int i = 0; i < tabela->linhas - 1; i++) {
        for (int j = 0; j < tabela->colunas; j++) {
            switch (tabela->tiposColuna[j].tipo) {
                case INT_TYPE:
                    fprintf(file, "%d", tabela->table[i][j].intVal);
                    break;
                case STRING_TYPE:
                    fprintf(file, "\"%s\"", tabela->table[i][j].strVal);
                    break;
                case FLOAT_TYPE:
                    fprintf(file, "%f", tabela->table[i][j].floatVal);
                    break;
            }
            if (j < tabela->colunas - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Tabela '%s' salva com sucesso.\n", tabela->nome);
}

int main(){
    

    return 0;
}