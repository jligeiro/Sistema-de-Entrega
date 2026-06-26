/*
 * Ficheiro: ficheiro.c
 * Contém funções responsáveis por guardar e carregar dados do sistema
 * em ficheiros de texto, incluindo clientes, entregadores, encomendas e rotas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "ficheiro.h"

/* Guarda todos os clientes existentes da tabela hash num ficheiro. */
void guardar_clientes(HashTable *clientes, FILE *arquivo) {
    int i;
    NoHash *no;
    Cliente *cliente;

    for (i = 0; i < TAM_HASH; i++) {
        no = clientes->tabela[i];
        while (no != NULL) {
            cliente = no->dado;
            fprintf(arquivo, "%d;%s;%s;%s;%d;%s;%s;%d\n", cliente->id, cliente->nome,
                    cliente->username, cliente->senha, cliente->idade, cliente->telefone, cliente->endereco, cliente->ativo);
            no = no->proximo;
        }
    }
}

/* Guarda todos os entregadores existentes num ficheiro. */
void guardar_entregadores(HashTable *entregadores, FILE *arquivo) {
    int i;
    NoHash *no;
    Vendedor *entregador;

    for (i = 0; i < TAM_HASH; i++) {
        no = entregadores->tabela[i];
        while (no != NULL) {
            entregador = no->dado;
            fprintf(arquivo, "%d;%s;%s;%s;%d;%s;%d;%d\n", entregador->id, entregador->nome,
                    entregador->username, entregador->senha, entregador->idade, entregador->telefone, entregador->disponibilidade, entregador->ativo);
            no = no->proximo;
        }
    }
}

/* Guarda todas as encomendas armazenadas num ficheiro. */
void guardar_encomendas(HashTable *encomendas, FILE *arquivo) {
    int i;
    NoHash *no;
    Encomenda *encomenda;

    for (i = 0; i < TAM_HASH; i++) {
        no = encomendas->tabela[i];
        while (no != NULL) {
            encomenda = no->dado;
            fprintf(arquivo, "%s;%d;%d;%d;%s;%d;%d;%d;%lf;%d\n",
                    encomenda->nome_produto, encomenda->id, encomenda->idCliente, encomenda->idEntregador,
                    encomenda->descricao, encomenda->origem, encomenda->destino,
                    encomenda->prioridade, encomenda->preco, encomenda->estado);
            no = no->proximo;
        }
    }
}

/* Guarda a informação do grafo de rotas. */
void guardar_rotas(Grafo *grafo, FILE *arquivo){
    Grafo *aux = grafo;
    Aresta *aresta;

    for(int i = 0; i < aux->total; i++){
        aresta = aux->locais[i].arestas;
        fprintf(arquivo, "%d;%s", aux->locais[i].id, aux->locais[i].nome);
        while (aresta){
            fprintf(arquivo, ";%d;%d", aresta->destino, aresta->peso);
            aresta = aresta->proximo;
        }
        fprintf(arquivo, "\n");
    }
}

/* Abre ficheiros e executa o processo completo de gravação. */
int ficheiro_guardar_dados(Sistema *sistema) {
    FILE *clientes = fopen("clientes.txt", "w");
    FILE *vendedores = fopen("entregadores.txt", "w");
    FILE *encomendas = fopen("encomendas.txt", "w");
    FILE *rotas = fopen("rotas.txt", "w");

    if (clientes == NULL || vendedores == NULL || encomendas == NULL || rotas == NULL) {
        if (clientes == NULL) printf("Falha ao abrir clientes.txt: %s\n", strerror(errno));
        if (vendedores == NULL) printf("Falha ao abrir entregadores.txt: %s\n", strerror(errno));
        if (encomendas == NULL) printf("Falha ao abrir encomendas.txt: %s\n", strerror(errno));
        if (rotas == NULL) printf("Falha ao abrir rotas.txt: %s\n", strerror(errno));
        if (clientes != NULL) fclose(clientes);
        if (vendedores != NULL) fclose(vendedores);
        if (encomendas != NULL) fclose(encomendas);
        if (rotas != NULL) fclose(rotas);
        return 0;
    }

    guardar_clientes(&sistema->clientes, clientes);
    guardar_entregadores(&sistema->vendedores, vendedores);
    guardar_encomendas(&sistema->encomendas, encomendas);
    guardar_rotas(&sistema->rotas, rotas);

    fclose(clientes);
    fclose(vendedores);
    fclose(encomendas);
    fclose(rotas);
    return 1;
}

/* Reinicializa as estruturas usadas pelo sistema. */
void limpar_dados_carregaveis(Sistema *sistema) {
    hash_liberar(&sistema->clientes);
    hash_liberar(&sistema->vendedores);
    hash_liberar(&sistema->encomendas);
    fila_liberar(&sistema->pendentes);
    
    hash_inicializar(&sistema->clientes, TIPO_CLIENTE);
    hash_inicializar(&sistema->vendedores, TIPO_VENDEDOR);
    hash_inicializar(&sistema->encomendas, TIPO_ENCOMENDA);
    fila_inicializar(&sistema->pendentes);
}

/* Carrega clientes a partir do ficheiro de dados. */
void carregar_clientes(Sistema *sistema) {
    FILE *arquivo = fopen("clientes.txt", "r");
    char linha[300];
    int atividade;
    Cliente *cliente;

    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        cliente = malloc(sizeof(Cliente));
        if (cliente == NULL) {
            break;
        }
        if (sscanf(linha, "%d;%79[^;];%79[^;];%79[^;];%d;%79[^;];%79[^;];%d",
                   &cliente->id, cliente->nome, cliente->username, cliente->senha, 
                   &cliente->idade, cliente->telefone, cliente->endereco, &atividade) == 8) {
            cliente->ativo = (Atividade)atividade;
            hash_inserir(&sistema->clientes, cliente->id, cliente);
            if (cliente->ativo == DESATIVO) 
                fila_enfileirar(&sistema->confirmacoes_clientes, cliente->id);
        } else {
            free(cliente);
        }
    }

    fclose(arquivo);
}

/* Carrega entregadores a partir do ficheiro. */
void carregar_entregadores(Sistema *sistema) {
    FILE *arquivo = fopen("entregadores.txt", "r");
    char linha[250];
    int disponibilidade;
    int atividade;
    Vendedor *vendedores;

    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        vendedores = malloc(sizeof(Vendedor));
        if (vendedores == NULL) {
            break;
        }

        if (sscanf(linha, "%d;%79[^;];%79[^;];%79[^;];%d;%19[^;];%d;%d",
                   &vendedores->id, vendedores->nome, vendedores->username, 
                   vendedores->senha, &vendedores->idade, vendedores->telefone, &disponibilidade, &atividade) == 8) {
            vendedores->disponibilidade = (Disponibilidade)disponibilidade;
            vendedores->ativo = (Atividade)atividade;
            hash_inserir(&sistema->vendedores, vendedores->id, vendedores);
            if (vendedores->ativo == DESATIVO)
                fila_enfileirar(&sistema->confirmacoes_vendedor, vendedores->id);
        } else {
            free(vendedores);
        }
    }

    fclose(arquivo);
}

/* Carrega encomendas a partir do ficheiro. */
void carregar_encomendas(Sistema *sistema) {
    FILE *arquivo = fopen("encomendas.txt", "r");
    char linha[350];
    int estado;
    Encomenda *encomenda;

    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        encomenda = malloc(sizeof(Encomenda));
        if (encomenda == NULL) {
            break;
        }

        if (sscanf(linha, "%79[^;];%d;%d;%d;%119[^;];%d;%d;%d;%lf;%d",
                   encomenda->nome_produto, &encomenda->id, &encomenda->idCliente, &encomenda->idEntregador,
                   encomenda->descricao, &encomenda->origem, &encomenda->destino,
                   &encomenda->prioridade, &encomenda->preco, &estado) == 8) {
            encomenda->estado = (EstadoEncomenda)estado;
            hash_inserir(&sistema->encomendas, encomenda->id, encomenda);
            if (encomenda->estado == PENDENTE) {
                fila_enfileirar(&sistema->pendentes, encomenda->id);
            }
        } else {
            free(encomenda);
        }
    }

    fclose(arquivo);
}

/* Carrega as rotas do grafo. */
void carregar_rotas(Sistema *sistema){
    FILE *arquivo = fopen("rotas.txt", "r");
    char linha[350];

    while (fgets(linha, sizeof(linha), arquivo) != NULL){
        char *token = strtok(linha, ";");
        int id = atoi(token);

        token = strtok(NULL, ";");
        char nome[80];
        strcpy(nome, token);

        grafo_adicionar_local(&sistema->rotas, nome);
        while (true){
            token = strtok(NULL, ";");
            if (token == NULL) break;
            int destino = atoi(token);

            token = strtok(NULL, ";");
            if (token == NULL) break;
            int peso = atoi(token);

            grafo_adicionar_rota(&sistema->rotas, id, destino, peso);
        }

    }
    fclose(arquivo);
}

/* Executa o carregamento completo dos dados. */
int ficheiro_carregar_dados(Sistema *sistema) {
    limpar_dados_carregaveis(sistema);
    carregar_entregadores(sistema);
    carregar_clientes(sistema);
    carregar_encomendas(sistema);
    carregar_rotas(sistema);
    return 1;
}
