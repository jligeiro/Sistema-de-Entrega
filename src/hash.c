/*
 * Ficheiro: hash.c
 * Implementa uma tabela hash com listas ligadas para armazenar dados.
 * Inclui inserção, pesquisa, remoção e libertação de memória.
 */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "sistema.h"
#include "hash.h"

#define ll long int

/* Calcula o índice da tabela hash para uma chave. */
int funcao_hash(int chave) {
    if (chave < 0) {
        chave = -chave;
    }
    return chave % TAM_HASH;
}

/* Inicializa a tabela hash. */
void hash_inicializar(HashTable *hash, TipoDado tipo) {
    int i;
    hash->tipo = tipo;
    for (i = 0; i < TAM_HASH; i++) {
        hash->tabela[i] = NULL;
    }
}

/* Converte um nome de utilizador para um valor numérico. */
int converter_username(char *username){
    ll username_para_id = 0;
    int     contador = 0;

    while(username[contador]){
        username_para_id = username_para_id + username[contador];
        contador++;
    }
    return (username_para_id);
}

/* Insere um elemento na tabela hash. */
int hash_inserir(HashTable *hash, int chave, void *dado) {
    int indice = funcao_hash(chave);
    NoHash *atual = hash->tabela[indice];
    NoHash *novo;

    while (atual != NULL) {
        if (atual->chave == chave) {
            return 0;
        }
        atual = atual->proximo;
    }

    novo = malloc(sizeof(NoHash));
    if (novo == NULL) {
        return 0;
    }

    novo->chave = chave;
    novo->dado = dado;
    novo->proximo = hash->tabela[indice];
    hash->tabela[indice] = novo;
    return 1;
}

/* Procura um elemento pela chave. */
void *hash_buscar(HashTable *hash, int chave) {
    int indice = funcao_hash(chave);
    NoHash *atual = hash->tabela[indice];

    while (atual != NULL) {
        if (atual->chave == chave) {
            return atual->dado;
        }
        atual = atual->proximo;
    }

    return NULL;
}

/* Remove um elemento da tabela hash. */
int hash_remover(HashTable *hash, int chave) {
    int indice = funcao_hash(chave);
    NoHash *atual = hash->tabela[indice];
    NoHash *anterior = NULL;

    while (atual != NULL) {
        if (atual->chave == chave) {
            if (anterior == NULL) {
                hash->tabela[indice] = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }
            free(atual->dado);
            free(atual);
            return 1;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    return 0;
}

/* Lista todos os elementos armazenados. */
void hash_listar(HashTable *hash, void (*imprimir)(void *)) {
    int i;
    int encontrou = 0;
    NoHash *atual;

    for (i = 0; i < TAM_HASH; i++) {
        atual = hash->tabela[i];
        while (atual != NULL) {
            imprimir(atual->dado);
            atual = atual->proximo;
            encontrou = 1;
        }
    }

    if (!encontrou){
        printf("Sem resgistro.\n");
        return;
    }
    Sleep(3000);
}

/* Liberta toda a memória da tabela hash. */
void hash_liberar(HashTable *hash) {
    int i;
    NoHash *atual;
    NoHash *proximo;

    for (i = 0; i < TAM_HASH; i++) {
        atual = hash->tabela[i];
        while (atual != NULL) {
            proximo = atual->proximo;
            free(atual->dado);
            free(atual);
            atual = proximo;
        }
        hash->tabela[i] = NULL;
    }
}
