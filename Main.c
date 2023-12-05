#include <stdio.h>
#include <stddef.h>
#include "memoryAllocation.h" // FUNÇÕES DE CONTROLE DE MEMORIA
#include "tableManipulation.h" // FUÇÕES DE MANIPULAÇÃO DE TABELA
#include "fileManipulation.h" // FUNÇÕES DE MANIPULAÇÃO DE ARQUIVO


//INICIO DAS DIRETIVAS DE PRE PROCESSAMENTO
//Inclui o windows.h quando esta sendo executado no windows
#ifdef _WIN32
#include <windows.h>
#endif
//limpa a tela
void limparTela() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}
//FIM DAS DIRETIVAS DE PRE PROCESSAMENTO


int main(){
    #ifdef _WIN32
    SetConsoleOutputCP(65001); // Se estiver no Windows, define a página de códigos para UTF-8
    #endif
    FILE *arquivo;
    arquivo = fopen("tabelas.itp", "a+"); //Garantindo existencia do arquivo
    fclose(arquivo);
    int opcao, colunas, chave;
    char nome[TAMANHO_MAX_NOME];
    bool executando = true, aviso = false, tabelaCarregada = false, salvo = false;
    Tabela *lambda = NULL;
    while (executando) {
        limparTela(); // Para Qualquer Sistema;

        // Exibe o menu principal
        printf("=======================================================================\n");
        printf("|                         MENU PRINCIPAL                              |\n");
        printf("=======================================================================\n");
        printf("| Criação e Seleção de Tabelas:         |  Autores:                   |\n");
        printf("| 1. Criar Nova Tabela                  |    - Geraldo Filho          |\n");
        printf("| 2. Selecionar Tabela                  |    - Miguel Arcanjo         |\n");
        printf("| 3. Listar Tabelas                     |                             |\n");
        printf("|---------------------------------------|                             |\n");
        printf("| Manipulação de Dados:                 |                             |\n");
        printf("| 4. Adicionar dados na tabela          |                             |\n");
        printf("| 5. Visualizar Tabela                  |                             |\n");
        printf("| 6. Remover Linha                      |                             |\n");
        printf("| 7. Pesquisar Valor                    |                             |\n");
        printf("|---------------------------------------|                             |\n");
        printf("| Gerenciamento de Tabela:              |                             |\n");
        printf("| 8. Salvar Tabela                      |                             |\n");
        printf("| 9. Sair                               |                             |\n");
        printf("=======================================================================");
        if (lambda != NULL)
        {
            printf("=> Tabela selecionada: %s\n", lambda->nome); //MOSTRA TABELA SELECIONADA
        }else if (!aviso) 
        {
        printf("=> Nenhuma tabela selecionada ou criada.\n"); 
        }
        if (aviso)
        {
            printf("Nenhuma tabela selecionada.\n");// AVISO DE NÃO TER TABELA SELECIONADA
            printf("Crie uma nova tabela ou carregue alguma existente.\n");
            aviso  = false;
        }if (tabelaCarregada)
        {
            printf("Tabela %s carregada com sucesso.\n", lambda->nome);//AVISO SOBRE TABELA TER SIDO CARREGADA COM SUCESSO
            tabelaCarregada = false;
        }if (salvo)
        {
            printf("Tabela %s salva com sucesso.\n", lambda->nome); // AVISO SOBRE TABELA TER SIDO SALVA COM SUCESSO
        }

        //PEGAR ESCOLHA
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer de entrada

        switch (opcao) {
            case 1: //CRIAR UMA TABELA
                limparTela();
                printf("Qual Nome da Tabela? \n");
                scanf(" %49s", nome);
                getchar();
                printf("Quantas Colunas você quer na sua tabela? \n");
                scanf("%d", &colunas);
                getchar();
                if (lambda != NULL) {
                    liberarTabela(lambda); // Libera a tabela anterior se existir
                }
                lambda = construtorTabela(1, colunas, nome);
                break;
            case 2: // CARREGAR TABELA
                //depois que criar o listar tabelas, pode colocar aqui em cima os nomes
                printf("Digite o nome de uma tabela: ");
                if (fgets(nome, sizeof(nome), stdin) != NULL) {
                    // Removendo o caractere de nova linha, se presente
                    nome[strcspn(nome, "\n")] = '\0';
                    tabelaCarregada = true;
                } else {
                    printf("Erro ao ler a entrada.\n");
                }
                if(lambda != NULL) {
                    liberarTabela(lambda); // Libera a tabela anterior se existir
                }
                lambda = carregarTabela(nome);
                break;
            case 3://LISTAR TABELAS EXISTENTES
                listarTabelas();
                break;
                
            case 4://ADICIONAR DADOS NA TABELA SELECIONADA
                if (lambda != NULL)
                {
                    limparTela();
                    printf("Tabela selecionada: %s\n", lambda->nome);
                    printf("Quando você não quiser mais adicionar dados a tabela digite -> Fim <-\n");
                    PegarDados(lambda);
                }else{
                    aviso = true;
                }
                break;
            case 5://MOSTRAR TABELA SELECIONADA
                if (lambda != NULL)
                {
                    printf("Tabela selecionada: %s\n", lambda->nome);
                    limparTela();
                    mostrarTabela(lambda);  
                }else{
                    aviso = true;
                }
                break;
                
            case 6://REMOVE LINHA POR CHAVE
                if (lambda != NULL)
                {
                    limparTela();
                    printf("Tabela selecionada: %s\n", lambda->nome);
                    printf("Qual a chave da linha que você quer apagar da tabela '%s': \n", lambda->nome);
                    scanf("%d", &chave);
                    removerLinhaPorChave(lambda, chave);
                }else{
                    aviso = true;
                }
                break;
            case 7://PESQUISA VALOR NA TABELA
                limparTela();
                if (lambda != NULL)
                {
                    pesquisaValor(lambda);
                }else{
                    aviso = true;
                }
                break;
            case 8://SALVA TABELA NO BANCO DE DADOS

                if (lambda != NULL) {
                    salvarArquivo(lambda);
                }else{aviso = true;}
                break;
            case 9://SAIR DO PROGRAMA
                executando = false;
                if (lambda != NULL) {
                    liberarTabela(lambda);
                }
                printf("Saindo do programa...\n");
                break;
            default:
                printf("Opção inválida. Pressione qualquer tecla para tentar novamente.\n");
                getchar(); // Aguarda o usuário pressionar uma tecla
        }
    }

    return 0;
}