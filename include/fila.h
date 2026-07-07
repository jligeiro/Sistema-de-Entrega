#ifndef FILA_H
#define FILA_H

/* No da fila que armazena um ID de encomenda e aponta para o proximo no */
typedef struct NoFila {
    int id;                          /* Identificador da encomenda */
    struct NoFila *proximo;          /* Ponteiro para o proximo no da fila */
} NoFila;

/* Estrutura da fila com inicio, fim e tamanho para operacoes eficientes */
typedef struct {
    NoFila *inicio;                  /* Ponteiro para o primeiro no da fila (cabeca) */
    NoFila *fim;                     /* Ponteiro para o ultimo no da fila (cauda) */
    int tamanho;                     /* Quantidade de elementos presentes na fila */
} Fila;

/* Inicializa uma fila vazia, preparando-a para receber elementos */
void fila_inicializar(Fila *fila);

/* Verifica se a fila esta vazia */
/* Retorna 1 se vazia, 0 se contem elementos */
int fila_vazia(Fila *fila);

/* Adiciona um elemento (ID da encomenda) ao final da fila */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int fila_enfileirar(Fila *fila, int idEncomenda);

/* Remove e retorna o elemento da frente da fila */
/* Retorna 1 se bem-sucedido, 0 se fila vazia */
int fila_desenfileirar(Fila *fila, int *idEncomenda);

/* Exibe na tela todos os elementos presentes na fila */
void fila_listar(Fila *fila);

/* Libera toda a memoria alocada para a fila e seus nos */
void fila_liberar(Fila *fila);

#endif
