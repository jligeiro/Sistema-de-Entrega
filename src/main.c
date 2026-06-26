/* ==============================================================
   FICHEIRO: main.c
   DESCRIÇÃO: Função principal do sistema de gestão de entregas
   FUNCIONALIDADE: Inicializa o sistema, carrega dados, e gerencia 
                   o menu principal de login/criação de conta
   ============================================================== */

#include <stdbool.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "sistema.h"
#include "ficheiro.h"

/*
 * FUNÇÃO: main
 * RETORNO: int (0 se execução bem-sucedida)
 * DESCRIÇÃO: 
 *   - Inicializa a estrutura de dados do sistema
 *   - Carrega dados salvos em ficheiro
 *   - Loop principal que gerencia login, criação de conta e saída
 *   - Salva dados antes de encerrar
 */
int main(void) {
    /* Declaração de variáveis */
    Sistema sistema;           /* Estrutura que contém todos os dados do sistema */
    int     opcao;             /* Variável para armazenar opção do utilizador */

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
        /* Menu de login/criação de conta retorna opção (1, 2 ou 3) */
        opcao = sistem_criarConta(&sistema);
        
        /* Opção 2: Realizar login */
        if (opcao == 2){
            system("cls");                      /* Limpa a tela */
            sistema_login(&sistema);            /* Função de login */
            Sleep(2000);                        /* Pausa de 2 segundos */
        }
        /* Opção 3: Sair do sistema */
        else if (opcao == 3){
            system("cls");
            
            /* Salva os dados antes de encerrar */
            if (ficheiro_guardar_dados(&sistema)) {
                printf("Dados guardados com sucesso.\n");
            } 
            else {
                printf("Erro ao guardar dados.\n");
            }
            
            /* Libera a memória alocada */
            sistema_liberar(&sistema);
            printf("A sair do sistema...\n");
            Sleep(2000);
            break;                              /* Sai do loop principal */
        }
    }
    
    return 0;
}
