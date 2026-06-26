/*
 * Ficheiro: fila.c
 * Implementa uma estrutura de dados do tipo fila (FIFO).
 * É usada para gerir elementos pendentes por ordem de chegada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "fila.h"

/* Inicializa uma fila vazia. */
void fila_inicializar(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
}

/* Verifica se a fila não contém elementos. */
int fila_vazia(Fila *fila) {
    return fila->inicio == NULL;
}

/* Adiciona um elemento no fim da fila. */
int fila_enfileirar(Fila *fila, int idEncomenda) {
    NoFila *novo = malloc(sizeof(NoFila));
    if (novo == NULL) {
        return 0;
    }

    novo->id = idEncomenda;
    novo->proximo = NULL;

    if (fila_vazia(fila)) {
        fila->inicio = novo;
    } else {
        fila->fim->proximo = novo;
    }

    fila->fim = novo;
    fila->tamanho++;
    return 1;
}

/* Remove o elemento mais antigo da fila. */
int fila_desenfileirar(Fila *fila, int *idEncomenda) {
    NoFila *remover;

    if (fila_vazia(fila)) {
        return 0;
    }

    remover = fila->inicio;
    *idEncomenda = remover->id;
    fila->inicio = remover->proximo;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    free(remover);
    fila->tamanho--;
    return 1;
}

/* Mostra os elementos presentes na fila. */
void fila_listar(Fila *fila) {
    NoFila *atual = fila->inicio;

    if (fila_vazia(fila)) {
        printf("Nao ha encomendas pendentes na fila.\n");
        return;
    }

    printf("Fila de encomendas pendentes: ");
    while (atual != NULL) {
        printf("#%d ", atual->id);
        atual = atual->proximo;
    }
    printf("\n");
    Sleep(3000);
}

/* Liberta a memória ocupada pela fila. */
void fila_liberar(Fila *fila) {
    int id;
    while (fila_desenfileirar(fila, &id)) {
    }
}
