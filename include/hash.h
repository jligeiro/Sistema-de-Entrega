#ifndef HASH_H
#define HASH_H

#include "modelos.h"

/* Tamanho da tabela hash (número de buckets) */
#define TAM_HASH 101

/* Enumeração dos tipos de dados que podem ser armazenados na hash table */
typedef enum {
    TIPO_CLIENTE,                    /* Tipo para dados de clientes */
    TIPO_VENDEDOR,                  /* Tipo para dados de vendedores/entregadores */
    TIPO_ENCOMENDA                  /* Tipo para dados de encomendas */
} TipoDado;

/* Nó da tabela hash que armazena uma chave, um dado genérico e próximo nó (encadeamento) */
typedef struct NoHash {
    int chave;                       /* Chave de busca (ID ou conversão de username) */
    void *dado;                      /* Ponteiro genérico que pode apontar para Cliente, Vendedor ou Encomenda */
    struct NoHash *proximo;          /* Ponteiro para próximo nó (resolução de colisões por encadeamento) */
} NoHash;

/* Estrutura da tabela hash que implementa uma hash table com encadeamento */
typedef struct {
    NoHash *tabela[TAM_HASH];        /* Array de buckets (cada bucket é uma lista encadeada) */
    TipoDado tipo;                   /* Tipo de dado que esta tabela armazena */
} HashTable;

/* Função de hash que converte uma chave inteira num índice válido da tabela */
/* Retorna um valor entre 0 e TAM_HASH-1 */
int funcao_hash(int chave);

/* Inicializa uma tabela hash vazia com um tipo específico de dado */
void hash_inicializar(HashTable *hash, TipoDado tipo);

/* Converte um username (string) num valor hash inteiro para ser usado como chave */
/* Retorna o valor numérico correspondente ao username */
int converter_username(char *username);

/* Insere um novo elemento na tabela hash com uma chave e um dado associado */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int hash_inserir(HashTable *hash, int chave, void *dado);

/* Busca e retorna o dado associado a uma chave específica na tabela hash */
/* Retorna ponteiro ao dado se encontrado, NULL caso não exista */
void *hash_buscar(HashTable *hash, int chave);

/* Remove um elemento da tabela hash identificado pela chave */
/* Retorna 1 se bem-sucedido, 0 se elemento não encontrado */
int hash_remover(HashTable *hash, int chave);

/* Percorre e exibe todos os elementos da tabela hash usando a função de impressão fornecida */
void hash_listar(HashTable *hash, void (*imprimir)(void *));

/* Libera toda a memória alocada para a tabela hash e seus elementos */
void hash_liberar(HashTable *hash);

#endif
