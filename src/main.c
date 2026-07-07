/* ==============================================================
   FICHEIRO: main.c
   DESCRICAO: Funcao principal do sistema de gestao de entregas
   FUNCIONALIDADE: Inicializa o sistema, carrega dados, e gerencia 
                   o menu principal de login/criacao de conta
   ============================================================== */

#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "sistema.h"
#include "ficheiro.h"

/*
 * FUNCAO: main
 * RETORNO: int (0 se execucao bem-sucedida)
 * DESCRICAO: 
 *   - Inicializa a estrutura de dados do sistema
 *   - Carrega dados salvos em ficheiro
 *   - Loop principal que gerencia login, criacao de conta e saida
 *   - Salva dados antes de encerrar
 */
int main(void) {
    /* Declaracao de variaveis */
    Sistema sistema;           /* Estrutura que contem todos os dados do sistema */
    int     opcao;             /* Variavel para armazenar opcao do utilizador */

    /* Inicializa as estruturas de dados do sistema
       (hash tables, filas, listas, grafo) */
    sistema_inicializar(&sistema);
    
    /* Tenta carregar dados anteriormente salvos em ficheiro */
    if (ficheiro_carregar_dados(&sistema)) {
        printf("Dados carregados com sucesso.\n");
    } else {
        printf("Erro ao carregar dados.\n");
    }
    
    /* Loop principal do sistema */
    while (true){
        /* Menu de login/criacao de conta retorna opcao (1, 2 ou 3) */
        opcao = sistem_criarConta(&sistema);
        
        /* Opcao 2: Realizar login */
        if (opcao == 2){
            system("cls");                      /* Limpa a tela */
            sistema_login(&sistema);            /* Funcao de login */
            Sleep(700);                        /* Pausa de 2 segundos */
        }
        /* Opcao 3: Sair do sistema */
        else if (opcao == 3){
            system("cls");
            
            /* Salva os dados antes de encerrar */
            if (ficheiro_guardar_dados(&sistema)) {
                printf("Dados guardados com sucesso.\n");
            } 
            else {
                printf("Erro ao guardar dados.\n");
            }
            
            /* Libera a memoria alocada */
            sistema_liberar(&sistema);
            printf("A sair do sistema...\n");
            Sleep(700);
            break;                              /* Sai do loop principal */
        }
    }
    
    return 0;
}
