#ifndef LISTA_H
#define LISTA_H

/* Tamanho maximo de caracteres para uma mensagem SMS */
#define TAM_SMS 1000

/* No simples da lista que armazena apenas um ID de encomenda */
typedef struct nolista{
    int id;                          /* Identificador da encomenda */
    struct nolista *prox;            /* Ponteiro para o proximo no da lista */
} NoLista;

/* Estrutura da lista que armazena IDs de encomendas */
typedef struct lista{
    NoLista *id_lista;               /* Ponteiro para o primeiro no da lista de IDs */
    int total;
} Lista;

/* Inicializa uma lista vazia, preparando-a para receber elementos */
void lista_inicializar(Lista *lista);

/* Verifica se a lista esta vazia */
/* Retorna 1 se vazia, 0 se contem elementos */
int lista_vazia(Lista *lista);

/* Insere um novo ID de encomenda na lista mantendo uma ordem apropriada */
/* Retorna a lista atualizada com o novo elemento */
Lista *lista_inserir(Lista *lista, int idEncomenda);

/* Remove um ID de encomenda especifico da lista */
/* Retorna a lista atualizada sem o elemento removido */
Lista *lista_remover(Lista *lista, int idEncomenda);

/* Libera toda a memoria alocada para a lista e seus nos */
void lista_liberar(Lista *lista);

#endif
