#ifndef FICHEIRO_H
#define FICHEIRO_H

#include "sistema.h"

/* Funcao para guardar dados de todos os clientes registados na hash table para um arquivo */
void guardar_clientes(HashTable *clientes, FILE *arquivo);

/* Funcao para guardar dados de todos os entregadores (vendedores) registados na hash table para um arquivo */
void guardar_entregadores(HashTable *entregadores, FILE *arquivo);

/* Funcao para guardar dados de todas as encomendas registadas na hash table para um arquivo */
void guardar_encomendas(HashTable *encomendas, FILE *arquivo);

/* Funcao para guardar todas as rotas do grafo para um arquivo */
void guardar_rotas(Grafo *grafo, FILE *arquivo);

/* Funcao principal para guardar todos os dados do sistema (clientes, entregadores, encomendas e rotas) */
/* Retorna 1 se bem-sucedido, 0 caso contrario */
int ficheiro_guardar_dados(Sistema *sistema);

/* Funcao para limpar dados previamente carregados do sistema antes de carregar novos dados */
void limpar_dados_carregaveis(Sistema *sistema);

/* Funcao para carregar os dados de clientes desde um arquivo para a hash table do sistema */
void carregar_clientes(Sistema *sistema);

/* Funcao para carregar os dados de entregadores desde um arquivo para a hash table do sistema */
void carregar_entregadores(Sistema *sistema);

/* Funcao para carregar os dados de encomendas desde um arquivo para a hash table do sistema */
void carregar_encomendas(Sistema *sistema);

/* Funcao para carregar os dados de rotas desde um arquivo para o grafo do sistema */
void carregar_rotas(Sistema *sistema);

/* Funcao principal para carregar todos os dados do sistema a partir dos arquivos persistidos */
/* Retorna 1 se bem-sucedido, 0 caso contrario */
int ficheiro_carregar_dados(Sistema *sistema);

#endif
