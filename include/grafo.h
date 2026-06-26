#ifndef GRAFO_H
#define GRAFO_H

/* Número máximo de locais (cidades/pontos de entrega) que podem ser adicionados ao grafo */
#define MAX_LOCAIS 30

/* Valor usado para representar distância infinita (impossível alcançar) */
#define INF 999999

/* Estrutura que representa uma aresta do grafo com destino, peso e próxima aresta */
typedef struct Aresta {
    int destino;                     /* ID do local de destino */
    int peso;                        /* Distância ou custo até o local de destino */
    struct Aresta *proximo;          /* Ponteiro para a próxima aresta (lista encadeada) */
} Aresta;

/* Estrutura que representa um local (vértice) do grafo */
typedef struct {
    int id;                          /* Identificador único do local */
    char nome[80];                   /* Nome descritivo do local (cidade, bairro, etc) */
    Aresta *arestas;                 /* Ponteiro para a lista encadeada de arestas saindo deste local */
} Local;

/* Estrutura principal do grafo que armazena todos os locais e suas ligações */
typedef struct {
    Local locais[MAX_LOCAIS];        /* Array de locais (vértices) do grafo */
    int total;                       /* Número de locais atualmente registados no grafo */
} Grafo;

/* Verifica se um local com um determinado nome já existe no grafo */
/* Retorna 1 se existe, 0 caso contrário */
int grafo_verificar_local(Grafo *grafo, char *local);

/* Inicializa um grafo vazio, preparando-o para receber locais e rotas */
void grafo_inicializar(Grafo *grafo);

/* Adiciona um novo local ao grafo com um nome descritivo */
/* Retorna o ID do novo local, ou -1 se falhar */
int grafo_adicionar_local(Grafo *grafo, const char *nome);

/* Adiciona uma aresta (rota) entre dois locais com uma distância específica */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int adicionar_aresta(Grafo *grafo, int origem, int destino, int distancia);

/* Adiciona uma rota bidirecional entre dois locais (função auxiliar) */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int grafo_adicionar_rota(Grafo *grafo, int origem, int destino, int distancia);

/* Exibe na tela todos os locais do grafo e suas conexões (arestas) */
void grafo_listar(Grafo *grafo);

/* Encontra o caminho mais curto entre dois locais usando algoritmo apropriado */
/* Armazena o caminho no array 'caminho' e retorna a quantidade de nós no array 'tamanho' */
/* Retorna 1 se encontrado, 0 se impossível chegar */
int grafo_menor_caminho(Grafo *grafo, int origem, int destino, int caminho[], int *tamanho);

/* Libera toda a memória alocada para o grafo e suas arestas */
void grafo_liberar(Grafo *grafo);

#endif
