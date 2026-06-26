#ifndef FILA_H
#define FILA_H

/* Nó da fila que armazena um ID de encomenda e aponta para o próximo nó */
typedef struct NoFila {
    int id;                          /* Identificador da encomenda */
    struct NoFila *proximo;          /* Ponteiro para o próximo nó da fila */
} NoFila;

/* Estrutura da fila com início, fim e tamanho para operações eficientes */
typedef struct {
    NoFila *inicio;                  /* Ponteiro para o primeiro nó da fila (cabeça) */
    NoFila *fim;                     /* Ponteiro para o último nó da fila (cauda) */
    int tamanho;                     /* Quantidade de elementos presentes na fila */
} Fila;

/* Inicializa uma fila vazia, preparando-a para receber elementos */
void fila_inicializar(Fila *fila);

/* Verifica se a fila está vazia */
/* Retorna 1 se vazia, 0 se contém elementos */
int fila_vazia(Fila *fila);

/* Adiciona um elemento (ID da encomenda) ao final da fila */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int fila_enfileirar(Fila *fila, int idEncomenda);

/* Remove e retorna o elemento da frente da fila */
/* Retorna 1 se bem-sucedido, 0 se fila vazia */
int fila_desenfileirar(Fila *fila, int *idEncomenda);

/* Exibe na tela todos os elementos presentes na fila */
void fila_listar(Fila *fila);

/* Libera toda a memória alocada para a fila e seus nós */
void fila_liberar(Fila *fila);

#endif
