#ifndef SISTEMA_H
#define SISTEMA_H
#include "fila.h"
#include "lista.h"
#include "grafo.h"
#include "hash.h"

/* Estrutura principal do sistema que integra todos os componentes */
typedef struct {
    Admin admin;                     /* Dados do administrador do sistema */
    HashTable clientes;              /* Tabela hash para armazenar clientes */
    HashTable vendedores;            /* Tabela hash para armazenar vendedores/entregadores */
    HashTable encomendas;            /* Tabela hash para armazenar encomendas */
    Fila pendentes;                  /* Fila de encomendas pendentes de atribuicao */
    Fila confirmacoes_clientes;      /* Fila de confirmacoes de clientes */
    Fila confirmacoes_vendedor;      /* Fila de confirmacoes de vendedores */
    Lista id_encomendas;             /* Lista de IDs de todas as encomendas */
    Grafo rotas;                     /* Grafo contendo locais e rotas de entrega */
} Sistema;

/* Funcao auxiliar para imprimir dados de um cliente formatado na tela */
void imprimir_cliente(void *dado);

/* Funcao auxiliar para imprimir dados de um entregador/vendedor formatado na tela */
void imprimir_entregador(void *dado);

/* Funcao auxiliar para imprimir dados de uma encomenda formatado na tela */
void imprimir_encomenda(void *dado);

/* Funcao auxiliar para imprimir dados de uma encomenda do ponto de vista do cliente */
void imprimir_encomenda_cliente(void *dado);

/* Funcao auxiliar para imprimir dados de uma encomenda do ponto de vista do vendedor */
void imprimir_encomenda_vendedor(void *dado);

/* Inicializa o sistema inteiro, preparando todas as estruturas de dados */
void sistema_inicializar(Sistema *sistema);

/* Verifica se um username ja existe entre os clientes e valida o ID se fornecido */
/* Retorna 1 se valido/disponivel, 0 se existe ou invalido */
int verificar_user_cliente(HashTable *cliente, char *username, int ID);

/* Verifica se um username ja existe entre os vendedores e valida o ID se fornecido */
/* Retorna 1 se valido/disponivel, 0 se existe ou invalido */
int verificar_user_vendedor(HashTable *vendedor, char *username, int ID);

/* Verifica se um username existe em qualquer categoria (cliente ou vendedor) do sistema */
/* Retorna 1 se existe, 0 se disponivel */
int verificar_username(Sistema *sistema, char *username, int ID);

/* Cria um novo cliente interativamente com dados fornecidos pelo utilizador */
void cadastrar_cliente(Sistema *sistema);

/* Cria um novo vendedor/entregador interativamente com dados fornecidos pelo utilizador */
void cadastrar_vendedor(Sistema *sistema);

/* Cria uma nova encomenda de um vendedor especifico */
void criar_encomenda(Sistema *sistema, Vendedor *vendedor);

/* Cria uma nova encomenda para um cliente especifico (encomenda de compra) */
void criar_encomenda_cliente(Sistema *sistema, int idCliente);

/* Atribui encomendas pendentes a entregadores disponiveis */
void atribuir_entrega(Sistema *sistema);

/* Permite atualizar o estado de uma encomenda (pendente, em transito, entregue, cancelada) */
void atualizar_estado(Sistema *sistema);

/* Atualiza o estado de todas as encomendas atribuidas a um entregador especifico */
void atualizar_estado_entregador(Sistema *sistema, int idEntregador);

/* Consulta e exibe a rota otima entre dois locais do sistema */
void consultar_rota(Sistema *sistema);

/* Realiza uma busca generica no sistema (por clientes, vendedores ou encomendas) */
void pesquisar(Sistema *sistema);

/* Lista todos os produtos disponiveis (encomendas) do sistema*/
void listar_produtos(HashTable *encomendas, Lista *idencomendas);

/* Lista todos os produtos disponiveis (encomendas) de um vendedor*/
void listar_produtos_vendedor(HashTable *encomendas, Lista *idencomendas, int idvendedor);

/* Lista todas as encomendas de um cliente especifico */
/* Retorna quantidade de encomendas encontradas */
int listar_encomendas_cliente(Sistema *sistema, int idCliente);

/* Lista todas as encomendas atribuidas a um entregador especifico */
/* Retorna quantidade de encomendas encontradas */
int listar_encomendas_entregador(Sistema *sistema, int idEntregador);

/* Permite a um cliente consultar o estado atual de suas encomendas */
void consultar_estado_cliente(Sistema *sistema, int idCliente);

/* Permite a um cliente cancelar uma encomenda pendente */
void cancelar_encomenda_cliente(Sistema *sistema, int idCliente);

/* Permite a um entregador consultar a rota de uma encomenda que deve entregar */
void consultar_rota_encomenda_entregador(Sistema *sistema, int idEntregador);

/* Permite a um entregador alterar seu estado de disponibilidade */
void alterar_disponibilidade_entregador(Sistema *sistema, int idEntregador);

/* Lista todos os dados no sistema (clientes, vendedores, encomendas, rotas) */
void listar_dados(Sistema *sistema);

/* Processa confirmacoes de clientes de uma fila e as imprime */
void confirmado_cliente(HashTable *hash, Fila *confirmacoes);

/* Processa confirmacoes de vendedores de uma fila e as imprime */
void confirmado_vendedor(HashTable *hash, Fila *confirmacoes);

/* Registra e configura as rotas e locais no sistema */
void registar_rotas(Sistema *sistema);

/* Menu interativo principal para o administrador */
void menu_administrador(Sistema *sistema);

/* Menu interativo para um cliente logado no sistema */
void menu_cliente(Sistema *sistema, Cliente *cliente);

/* Menu interativo para um vendedor/entregador logado no sistema */
void menu_vendedor(Sistema *sistema, Vendedor *vendedor);

/* Verifica se um cliente especifico tem confirmacoes pendentes */
/* Retorna 1 se confirmado, 0 caso contrario */
int confirmado_cliente2(HashTable *hash, int chave);

/* Verifica se um vendedor especifico tem confirmacoes pendentes */
/* Retorna 1 se confirmado, 0 caso contrario */
int confirmado_vendedor2(HashTable *hash, int chave);

/* Cria uma nova conta de utilizador no sistema (cliente ou vendedor) */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int sistem_criarConta(Sistema *sistema);

/* Realiza o login do administrador com verificacao de credenciais */
void login_adm(Sistema *sistema);

/* Realiza o login de um cliente ou vendedor com verificacao de credenciais */
void sistema_login(Sistema *sistema);

/* Libera toda a memoria alocada pelo sistema, finalizando operacoes */
void sistema_liberar(Sistema *sistema);

#endif
