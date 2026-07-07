#ifndef GRAFO_H
#define GRAFO_H

/* Numero maximo de locais (cidades/pontos de entrega) que podem ser adicionados ao grafo */
#define MAX_LOCAIS 30

/* Valor usado para representar distancia infinita (impossivel alcancar) */
#define INF 999999

/* Estrutura que representa uma aresta do grafo com destino, peso e proxima aresta */
typedef struct Aresta {
    int destino;                     /* ID do local de destino */
    int peso;                        /* Distancia ou custo ate o local de destino */
    struct Aresta *proximo;          /* Ponteiro para a proxima aresta (lista encadeada) */
} Aresta;

/* Estrutura que representa um local (vertice) do grafo */
typedef struct {
    int id;                          /* Identificador unico do local */
    char nome[80];                   /* Nome descritivo do local (cidade, bairro, etc) */
    Aresta *arestas;                 /* Ponteiro para a lista encadeada de arestas saindo deste local */
} Local;

/* Estrutura principal do grafo que armazena todos os locais e suas ligacoes */
typedef struct {
    Local locais[MAX_LOCAIS];        /* Array de locais (vertices) do grafo */
    int total;                       /* Numero de locais atualmente registados no grafo */
} Grafo;

/* Verifica se um local com um determinado nome ja existe no grafo */
/* Retorna 1 se existe, 0 caso contrario */
int grafo_verificar_local(Grafo *grafo, char *local);

/* Inicializa um grafo vazio, preparando-o para receber locais e rotas */
void grafo_inicializar(Grafo *grafo);

/* Adiciona um novo local ao grafo com um nome descritivo */
/* Retorna o ID do novo local, ou -1 se falhar */
int grafo_adicionar_local(Grafo *grafo, const char *nome);

/* Adiciona uma aresta (rota) entre dois locais com uma distancia especifica */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int adicionar_aresta(Grafo *grafo, int origem, int destino, int distancia);

/* Adiciona uma rota bidirecional entre dois locais (funcao auxiliar) */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int grafo_adicionar_rota(Grafo *grafo, int origem, int destino, int distancia);

/* Exibe na tela todos os locais do grafo e suas conexoes (arestas) */
void grafo_listar(Grafo *grafo);

/* Encontra o caminho mais curto entre dois locais usando algoritmo apropriado */
/* Armazena o caminho no array 'caminho' e retorna a quantidade de nos no array 'tamanho' */
/* Retorna 1 se encontrado, 0 se impossivel chegar */
int grafo_menor_caminho(Grafo *grafo, int origem, int destino, int caminho[], int *tamanho);

/* Libera toda a memoria alocada para o grafo e suas arestas */
void grafo_liberar(Grafo *grafo);

#endif
