#ifndef MODELOS_H
#define MODELOS_H

/* Tamanho máximo para nomes de clientes, vendedores e produtos */
#define TAM_NOME 80

/* Tamanho máximo para usernames e senhas */
#define TAM_USERNAME 50

/* Tamanho máximo para textos descritivos (endereços, descrições, etc) */
#define TAM_TEXTO 120

/* Enumeração para o estado de atividade de clientes e vendedores */
typedef enum{
    ATIVO = 1,                       /* Utilizador está ativo no sistema */
    DESATIVO = 0,                    /* Utilizador está desativado */
    NEGADO = -1                      /* Utilizador foi negado/bloqueado */
} Atividade;

/* Enumeração para a disponibilidade de vendedores/entregadores */
typedef enum {
    DISPONIVEL = 1,                  /* Entregador disponível para receber encomendas */
    OCUPADO = 0                      /* Entregador está ocupado/indisponível */
} Disponibilidade;

/* Enumeração para o estado de uma encomenda ao longo do seu ciclo de vida */
typedef enum {
    PENDENTE = 0,                    /* Encomenda aguardando atribuição a entregador */
    EM_TRANSITO = 1,                 /* Encomenda está sendo transportada */
    ENTREGUE = 2,                    /* Encomenda foi entregue com sucesso */
    CANCELADA = 3                    /* Encomenda foi cancelada */
} EstadoEncomenda;

/* Estrutura que armazena dados de um cliente do sistema */
typedef struct {
    int id;                          /* Identificador único do cliente */
    int idade;                       /* Idade do cliente */
    char nome[TAM_NOME];             /* Nome completo do cliente */
    char username[TAM_USERNAME];     /* Nome de utilizador para login */
    char senha[TAM_USERNAME];        /* Senha/password do cliente */
    char telefone[20];               /* Número de telefone para contacto */
    char endereco[TAM_TEXTO];        /* Endereço de entrega do cliente */
    Atividade ativo;                 /* Estado de atividade do cliente */
} Cliente;

/* Estrutura que armazena dados de um vendedor/entregador do sistema */
typedef struct {
    int id;                          /* Identificador único do vendedor */
    int idade;                       /* Idade do vendedor */
    char nome[TAM_NOME];             /* Nome completo do vendedor */
    char username[TAM_USERNAME];     /* Nome de utilizador para login */
    char senha[TAM_USERNAME];        /* Senha/password do vendedor */
    char telefone[20];               /* Número de telefone para contacto */
    Disponibilidade disponibilidade; /* Estado de disponibilidade para entregas */
    Atividade ativo;                 /* Estado de atividade do vendedor */
} Vendedor;

/* Estrutura que armazena credenciais do administrador */
typedef struct{
    char nome[TAM_NOME];             /* Nome do administrador */
    char senha[TAM_NOME];            /* Senha de acesso administrativo */
} Admin;

/* Estrutura que armazena informações completas de uma encomenda */
typedef struct {
    char nome_produto[TAM_NOME];     /* Nome descritivo do produto na encomenda */
    int id;                          /* Identificador único da encomenda */
    int idCliente;                   /* ID do cliente que encomendou */
    int idEntregador;                /* ID do vendedor/entregador atribuído */
    char descricao[TAM_TEXTO];       /* Descrição detalhada do produto */
    int origem;                      /* ID do local de origem (armazém) */
    int destino;                     /* ID do local de destino (cliente) */
    int prioridade;                  /* Nível de prioridade da entrega (quanto menor, maior prioridade) */
    double preco;                    /* Preço da encomenda em Kwanza (moeda local) */
    int comprado;                    /* Flag indicando se a encomenda foi paga/confirmada */
    EstadoEncomenda estado;          /* Estado atual da encomenda no sistema */
} Encomenda;

#endif
