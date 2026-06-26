#ifndef LISTA_H
#define LISTA_H

/* Tamanho máximo de caracteres para uma mensagem SMS */
#define TAM_SMS 1000

/* Nó simples da lista que armazena apenas um ID de encomenda */
typedef struct nolista{
    int id;                          /* Identificador da encomenda */
    struct nolista *prox;            /* Ponteiro para o próximo nó da lista */
} NoLista;

/* Estrutura para armazenar mensagens com origem, destino e conteúdo */
typedef struct sms{
    int id;                          /* Identificador único da mensagem */
    int id_origem;                   /* ID do utilizador que originou a mensagem */
    int id_destino;                  /* ID do utilizador destinatário da mensagem */
    int total;                       /* Número de caracteres na mensagem */
    char mensagem[TAM_SMS];          /* Conteúdo da mensagem (até 1000 caracteres) */
    struct sms *prox;                /* Ponteiro para a próxima mensagem (lista encadeada) */
} Mensagens;

/* Estrutura da lista que armazena IDs de encomendas */
typedef struct lista{
    NoLista *id_lista;               /* Ponteiro para o primeiro nó da lista de IDs */
} Lista;

/* Inicializa uma lista vazia, preparando-a para receber elementos */
void lista_inicializar(Lista *lista);

/* Verifica se a lista está vazia */
/* Retorna 1 se vazia, 0 se contém elementos */
int lista_vazia(Lista *lista);

/* Insere um novo ID de encomenda na lista mantendo uma ordem apropriada */
/* Retorna a lista atualizada com o novo elemento */
Lista *lista_inserir(Lista *lista, int idEncomenda);

/* Remove um ID de encomenda específico da lista */
/* Retorna a lista atualizada sem o elemento removido */
Lista *lista_remover(Lista *lista, int idEncomenda);

#endif
