/*
 * Ficheiro: lista.c
 * Implementa uma lista ligada simples para guardar identificadores.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "sistema.h"
#include "utils.h"
#include "lista.h"
#define ll long int

/* Inicializa uma lista vazia. */
void lista_inicializar(Lista *lista) {
    lista->id_lista = NULL;
}

/* Verifica se a lista está vazia. */
int lista_vazia(Lista *lista){
    return (lista->id_lista == NULL);
}

/* Adiciona um elemento ao final da lista. */
Lista *lista_inserir(Lista *lista, int idEncomenda) {
    NoLista *novo = malloc(sizeof(NoLista));
    if (novo == NULL) {
        return NULL;
    }

    novo->id = idEncomenda;
    novo->prox = NULL;

    if (lista_vazia(lista)) {
        lista->id_lista = novo;
    } else {
        NoLista *aux = lista->id_lista;
        while (aux->prox)
            aux = aux->prox;
        aux->prox = novo;
    }

    return lista;
}

/* Remove um elemento da lista pelo identificador. */
Lista *lista_remover(Lista *lista, int idEncomenda) {
    NoLista *aux = NULL;
    NoLista *remover = lista->id_lista;

    while (remover && remover->id != idEncomenda){
        aux = remover;
        remover = remover->prox;
    }

    if (!remover) return lista;
    if (!aux) lista->id_lista = remover->prox;
    else aux->prox = remover->prox;

    free(remover);
    return lista;
}