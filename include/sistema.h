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
    Fila pendentes;                  /* Fila de encomendas pendentes de atribuição */
    Fila confirmacoes_clientes;      /* Fila de confirmações de clientes */
    Fila confirmacoes_vendedor;      /* Fila de confirmações de vendedores */
    Lista id_encomendas;             /* Lista de IDs de todas as encomendas */
    Grafo rotas;                     /* Grafo contendo locais e rotas de entrega */
} Sistema;

/* Função auxiliar para imprimir dados de um cliente formatado na tela */
void imprimir_cliente(void *dado);

/* Função auxiliar para imprimir dados de um entregador/vendedor formatado na tela */
void imprimir_entregador(void *dado);

/* Função auxiliar para imprimir dados de uma encomenda formatado na tela */
void imprimir_encomenda(void *dado);

/* Função auxiliar para imprimir dados de uma encomenda do ponto de vista do cliente */
void imprimir_encomenda_cliente(void *dado);

/* Inicializa o sistema inteiro, preparando todas as estruturas de dados */
void sistema_inicializar(Sistema *sistema);

/* Verifica se um username já existe entre os clientes e valida o ID se fornecido */
/* Retorna 1 se válido/disponível, 0 se existe ou inválido */
int verificar_user_cliente(HashTable *cliente, char *username, int ID);

/* Verifica se um username já existe entre os vendedores e valida o ID se fornecido */
/* Retorna 1 se válido/disponível, 0 se existe ou inválido */
int verificar_user_vendedor(HashTable *vendedor, char *username, int ID);

/* Verifica se um username existe em qualquer categoria (cliente ou vendedor) do sistema */
/* Retorna 1 se existe, 0 se disponível */
int verificar_username(Sistema *sistema, char *username, int ID);

/* Cria um novo cliente interativamente com dados fornecidos pelo utilizador */
void cadastrar_cliente(Sistema *sistema);

/* Cria um novo vendedor/entregador interativamente com dados fornecidos pelo utilizador */
void cadastrar_vendedor(Sistema *sistema);

/* Cria uma nova encomenda de um vendedor específico */
void criar_encomenda(Sistema *sistema, Vendedor *vendedor);

/* Cria uma nova encomenda para um cliente específico (encomenda de compra) */
void criar_encomenda_cliente(Sistema *sistema, int idCliente);

/* Atribui encomendas pendentes a entregadores disponíveis */
void atribuir_entrega(Sistema *sistema);

/* Permite atualizar o estado de uma encomenda (pendente, em trânsito, entregue, cancelada) */
void atualizar_estado(Sistema *sistema);

/* Atualiza o estado de todas as encomendas atribuídas a um entregador específico */
void atualizar_estado_entregador(Sistema *sistema, int idEntregador);

/* Consulta e exibe a rota ótima entre dois locais do sistema */
void consultar_rota(Sistema *sistema);

/* Realiza uma busca genérica no sistema (por clientes, vendedores ou encomendas) */
void pesquisar(Sistema *sistema);

/* Lista todos os produtos disponíveis (encomendas) do sistema */
void listar_produtos(HashTable *encomendas, Lista *idencomendas);

/* Lista todas as encomendas de um cliente específico */
/* Retorna quantidade de encomendas encontradas */
int listar_encomendas_cliente(Sistema *sistema, int idCliente);

/* Lista todas as encomendas atribuídas a um entregador específico */
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

/* Processa confirmações de clientes de uma fila e as imprime */
void confirmado_cliente(HashTable *hash, Fila *confirmacoes);

/* Processa confirmações de vendedores de uma fila e as imprime */
void confirmado_vendedor(HashTable *hash, Fila *confirmacoes);

/* Registra e configura as rotas e locais no sistema */
void registar_rotas(Sistema *sistema);

/* Menu interativo principal para o administrador */
void menu_administrador(Sistema *sistema);

/* Menu interativo para um cliente logado no sistema */
void menu_cliente(Sistema *sistema, Cliente *cliente);

/* Menu interativo para um vendedor/entregador logado no sistema */
void menu_vendedor(Sistema *sistema, Vendedor *vendedor);

/* Verifica se um cliente específico tem confirmações pendentes */
/* Retorna 1 se confirmado, 0 caso contrário */
int confirmado_cliente2(HashTable *hash, int chave);

/* Verifica se um vendedor específico tem confirmações pendentes */
/* Retorna 1 se confirmado, 0 caso contrário */
int confirmado_vendedor2(HashTable *hash, int chave);

/* Cria uma nova conta de utilizador no sistema (cliente ou vendedor) */
/* Retorna 1 se bem-sucedido, 0 em caso de erro */
int sistem_criarConta(Sistema *sistema);

/* Realiza o login do administrador com verificação de credenciais */
void login_adm(Sistema *sistema);

/* Realiza o login de um cliente ou vendedor com verificação de credenciais */
void sistema_login(Sistema *sistema);

/* Libera toda a memória alocada pelo sistema, finalizando operações */
void sistema_liberar(Sistema *sistema);

#endif
