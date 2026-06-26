/*
 * Ficheiro: grafo.c
 * Implementa operações sobre um grafo de locais e rotas.
 * Permite adicionar locais, criar ligações e calcular caminhos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "grafo.h"
#include "utils.h"

/* Procura se um local já existe no grafo. */
int grafo_verificar_local(Grafo *grafo, char *local){
    int id = 0;
    char *grafo_local;

    local = ft_tolower(local);
    while (id < grafo->total){
        grafo_local = ft_tolower(grafo->locais[id].nome);
        if (strcmp(grafo_local, local) == 0)
            return (1);
        id++;
    }
    return (0);
}

/* Inicializa o grafo sem locais ou ligações. */
void grafo_inicializar(Grafo *grafo) {
    int i;
    grafo->total = 0;
    for (i = 0; i < MAX_LOCAIS; i++) {
        grafo->locais[i].id = -1;
        grafo->locais[i].nome[0] = '\0';
        grafo->locais[i].arestas = NULL;
    }
}

/* Adiciona um novo local ao grafo. */
int grafo_adicionar_local(Grafo *grafo, const char *nome) {
    int id;

    if (grafo->total >= MAX_LOCAIS) {
        return -1;
    }

    id = grafo->total;
    grafo->locais[id].id = id;
    strncpy(grafo->locais[id].nome, nome, sizeof(grafo->locais[id].nome) - 1);
    grafo->locais[id].nome[sizeof(grafo->locais[id].nome) - 1] = '\0';
    grafo->locais[id].arestas = NULL;
    grafo->total++;
    return id;
}

/* Cria uma ligação entre dois locais. */
int adicionar_aresta(Grafo *grafo, int origem, int destino, int distancia) {
    Aresta *nova;

    if (origem < 0 || origem >= grafo->total || destino < 0 || destino >= grafo->total || distancia <= 0) {
        return 0;
    }

    nova = malloc(sizeof(Aresta));
    if (nova == NULL) {
        return 0;
    }

    nova->destino = destino;
    nova->peso = distancia;
    nova->proximo = grafo->locais[origem].arestas;
    grafo->locais[origem].arestas = nova;
    return 1;
}

/* Adiciona uma rota nos dois sentidos. */
int grafo_adicionar_rota(Grafo *grafo, int origem, int destino, int distancia) {
    if (!adicionar_aresta(grafo, origem, destino, distancia)) {
        return 0;
    }

    if (!adicionar_aresta(grafo, destino, origem, distancia)) {
        return 0;
    }

    return 1;
}

/* Lista todos os locais e ligações. */
void grafo_listar(Grafo *grafo) {
    int i;
    Aresta *atual;

    if (grafo->total == 0) {
        printf("Nenhum local cadastrado.\n");
        return;
    }

    for (i = 0; i < grafo->total; i++) {
        printf("[%d] %s:", grafo->locais[i].id, grafo->locais[i].nome);
        atual = grafo->locais[i].arestas;
        while (atual != NULL) {
            printf(" -> %s(%dkm)", grafo->locais[atual->destino].nome, atual->peso);
            atual = atual->proximo;
        }
        printf("\n");
    }
    Sleep(3000);
}

/* Calcula o menor caminho entre dois locais. */
int grafo_menor_caminho(Grafo *grafo, int origem, int destino, int caminho[], int *tamanho) {
    int dist[MAX_LOCAIS];
    int visitado[MAX_LOCAIS];
    int anterior[MAX_LOCAIS];
    int i, j, atual, menor, totalCaminho;
    Aresta *aresta;

    if (origem < 0 || origem >= grafo->total || destino < 0 || destino >= grafo->total) {
        return -1;
    }

    for (i = 0; i < grafo->total; i++) {
        dist[i] = INF;
        visitado[i] = 0;
        anterior[i] = -1;
    }
    dist[origem] = 0;

    for (i = 0; i < grafo->total; i++) {
        atual = -1;
        menor = INF;

        for (j = 0; j < grafo->total; j++) {
            if (!visitado[j] && dist[j] < menor) {
                menor = dist[j];
                atual = j;
            }
        }

        if (atual == -1) {
            break;
        }

        visitado[atual] = 1;
        aresta = grafo->locais[atual].arestas;
        while (aresta != NULL) {
            if (dist[atual] + aresta->peso < dist[aresta->destino]) {
                dist[aresta->destino] = dist[atual] + aresta->peso;
                anterior[aresta->destino] = atual;
            }
            aresta = aresta->proximo;
        }
    }

    if (dist[destino] == INF) {
        return -1;
    }

    totalCaminho = 0;
    atual = destino;
    while (atual != -1) {
        caminho[totalCaminho++] = atual;
        atual = anterior[atual];
    }

    for (i = 0; i < totalCaminho / 2; i++) {
        int temp = caminho[i];
        caminho[i] = caminho[totalCaminho - 1 - i];
        caminho[totalCaminho - 1 - i] = temp;
    }

    *tamanho = totalCaminho;
    return dist[destino];
}

/* Liberta a memória das ligações do grafo. */
void grafo_liberar(Grafo *grafo) {
    int i;
    Aresta *atual;
    Aresta *proximo;

    for (i = 0; i < grafo->total; i++) {
        atual = grafo->locais[i].arestas;
        while (atual != NULL) {
            proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
        grafo->locais[i].arestas = NULL;
    }
}
