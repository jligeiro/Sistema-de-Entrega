#ifndef MODELOS_H
#define MODELOS_H
#define ll long int
/* Tamanho maximo para nomes de clientes, vendedores e produtos */
#define TAM_NOME 80

/* Tamanho maximo para usernames e senhas */
#define TAM_USERNAME 50

/* Tamanho maximo para textos descritivos (enderecos, descricoes, etc) */
#define TAM_TEXTO 120

/* Enumeracao para o estado de atividade de clientes e vendedores */
typedef enum{
    ATIVO = 1,                       /* Utilizador esta ativo no sistema */
    DESATIVO = 0,                    /* Utilizador esta desativado */
    NEGADO = -1                      /* Utilizador foi negado/bloqueado */
} Atividade;

/* Enumeracao para a disponibilidade de vendedores/entregadores */
typedef enum {
    DISPONIVEL = 1,                  /* Entregador disponivel para receber encomendas */
    OCUPADO = 0                      /* Entregador esta ocupado/indisponivel */
} Disponibilidade;

/* Enumeracao para o estado de uma encomenda ao longo do seu ciclo de vida */
typedef enum {
    LIVRE = -1,                      /* Encomenda esta livre para compra*/
    PENDENTE = 0,                    /* Encomenda aguardando atribuicao a entregador */
    EM_TRANSITO = 1,                 /* Encomenda esta sendo transportada */
    ENTREGUE = 2,                    /* Encomenda foi entregue com sucesso */
    CANCELADA = 3                    /* Encomenda foi cancelada */
} EstadoEncomenda;

/* Estrutura que armazena dados de um cliente do sistema */
typedef struct {
    int id;                          /* Identificador unico do cliente */
    int idade;                       /* Idade do cliente */
    char nome[TAM_NOME];             /* Nome completo do cliente */
    char username[TAM_USERNAME];     /* Nome de utilizador para login */
    char senha[TAM_USERNAME];        /* Senha/password do cliente */
    char telefone[20];               /* Numero de telefone para contacto */
    char endereco[TAM_TEXTO];        /* Endereco de entrega do cliente */
    Atividade ativo;                 /* Estado de atividade do cliente */
} Cliente;

/* Estrutura que armazena dados de um vendedor/entregador do sistema */
typedef struct {
    int id;                          /* Identificador unico do vendedor */
    int idade;                       /* Idade do vendedor */
    char nome[TAM_NOME];             /* Nome completo do vendedor */
    char username[TAM_USERNAME];     /* Nome de utilizador para login */
    char senha[TAM_USERNAME];        /* Senha/password do vendedor */
    char telefone[20];               /* Numero de telefone para contacto */
    Disponibilidade disponibilidade; /* Estado de disponibilidade para entregas */
    Atividade ativo;                 /* Estado de atividade do vendedor */
} Vendedor;

/* Estrutura que armazena credenciais do administrador */
typedef struct{
    char nome[TAM_NOME];             /* Nome do administrador */
    char senha[TAM_NOME];            /* Senha de acesso administrativo */
} Admin;

/* Estrutura que armazena informacoes completas de uma encomenda */

typedef struct{
    ll qtd;
    int id_cliente;
}Produto_qtd;
typedef struct {
    char nome_produto[TAM_NOME];     /* Nome descritivo do produto na encomenda */
    char *telefone_vendedor;         /* telefone do vendedor*/
    char *telefone_cliente;          /* telefone do cliente*/
    int id;                          /* Identificador unico da encomenda */
    int idCliente;                   /* ID do cliente que encomendou; -1 para produtos em venda */
    int idProduto;                   /* ID do produto base associado ao pedido; -1 para produtos em venda */
    int idEntregador;                /* ID do vendedor/entregador atribuido */
    char descricao[TAM_TEXTO];       /* Descricao detalhada do produto */
    int origem;                      /* ID do local de origem (armazem) */
    int destino;                     /* ID do local de destino (cliente) */
    int prioridade;                  /* Nivel de prioridade da entrega (quanto menor, maior prioridade) */
    double preco;                    /* Preco da encomenda em Kwanza (moeda local) */
    int comprado;                    /* Flag indicando se a encomenda foi paga/confirmada */
    ll qtd;
    Produto_qtd qtd_cliente;                   
    EstadoEncomenda estado;          /* Estado atual da encomenda no sistema */
} Encomenda;

#endif
