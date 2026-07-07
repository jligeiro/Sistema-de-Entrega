/* ==============================================================
   FICHEIRO: sistema.c
   DESCRICAO: Nucleo do sistema de gestao de entregas
   FUNCIONALIDADES: 
     - Gestao de clientes e vendedores
     - Criacao e rastreamento de encomendas
     - Autenticacao e login de utilizadores
     - Menus e operacoes especificas por tipo de utilizador
   ============================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "utils.h"
#include "ficheiro.h"
#include "sistema.h"
#define ll long int                           /* Define tipo long int abreviado como ll */

static Sistema *sistema_atual = NULL;

void sistema_definir_contexto(Sistema *sistema) {
    sistema_atual = sistema;
}

static int texto_valido(const char *texto) {
    return texto != NULL && texto[0] != '\0';
}

static int produto_disponivel(const Encomenda *produto) {
    return produto != NULL && produto->idCliente == -1 && produto->idProduto == -1 && produto->estado == LIVRE && produto->qtd > 0;
}

static int pedido_pendente(const Encomenda *pedido) {
    return pedido != NULL && pedido->idProduto != -1 && pedido->estado == PENDENTE;
}

static int local_valido(const Sistema *sistema, int id_local) {
    return id_local >= 0 && id_local < sistema->rotas.total;
}

static void preencher_nomes_encomenda(const Encomenda *encomenda, char *nomeCliente, char *nomeVendedor) {
    Cliente *cliente = NULL;
    Vendedor *vendedor = NULL;

    if (sistema_atual == NULL) {
        return;
    }

    if (encomenda->idCliente != -1) {
        cliente = hash_buscar(&sistema_atual->clientes, encomenda->idCliente);
        if (cliente != NULL) {
            strcpy(nomeCliente, cliente->nome);
        }
    }

    if (encomenda->idEntregador != -1) {
        vendedor = hash_buscar(&sistema_atual->vendedores, encomenda->idEntregador);
        if (vendedor != NULL) {
            strcpy(nomeVendedor, vendedor->nome);
        }
    }
}

/*
 * FUNCAO: imprimir_cliente
 * PARAMETRO: void *dado - apontador generico para um Cliente
 * RETORNO: void
 * DESCRICAO: Exibe informacoes de um cliente em formato tabular
 */
void imprimir_cliente(void *dado) {
    Cliente *cliente = dado;
    printf("ID: %d | Username: %s | Nome: %s | Idade: %d | Telefone: %s | Endereco: %s\n",
           cliente->id, cliente->username, cliente->nome, cliente->idade, cliente->telefone, cliente->endereco);
}

/*
 * FUNCAO: imprimir_entregador
 * PARAMETRO: void *dado - apontador generico para um Vendedor
 * RETORNO: void
 * DESCRICAO: Exibe informacoes de um vendedor/entregador
 *            Mostra status de disponibilidade (Disponivel/Ocupado)
 */
void imprimir_entregador(void *dado) {
    Vendedor *entregador = dado;
    printf("ID: %d | Username: %s | Nome: %s | Idade: %d | Telefone: %s | Estado: %s\n",
           entregador->id, entregador->username, entregador->nome, entregador->idade, entregador->telefone,
           entregador->disponibilidade == DISPONIVEL ? "Disponivel" : "Ocupado");
}

/*
 * FUNCAO: imprimir_encomenda
 * PARAMETRO: void *dado - apontador generico para uma Encomenda
 * RETORNO: void
 * DESCRICAO: Exibe informacoes detalhadas de uma encomenda
 *            Inclui: produto, preco, ids, origem, destino, prioridade e estado
 */
void imprimir_encomenda(void *dado) {
    Encomenda *encomenda = dado;
    int quantidade = (encomenda->idProduto == -1) ? (int)encomenda->qtd : (int)encomenda->qtd_cliente.qtd;
    char nomeCliente[TAM_NOME] = "N/A";
    char nomeVendedor[TAM_NOME] = "N/A";

    preencher_nomes_encomenda(encomenda, nomeCliente, nomeVendedor);

    printf("ID: %d | Produto: %s | Preco: %.2lf | Vendedor: %s (%d) | Cliente: %s (%d) | Origem: %d | Destino: %d | Prioridade: %d | Quantidade: %d | Estado: %s | %s\n\n",
           encomenda->id, encomenda->nome_produto, encomenda->preco, nomeVendedor, encomenda->idEntregador,
           nomeCliente, encomenda->idCliente, encomenda->origem, encomenda->destino, encomenda->prioridade,
           quantidade, estado_para_texto(encomenda->estado), encomenda->descricao);
}

/*
 * FUNCAO: imprimir_encomenda_cliente
 * PARAMETRO: void *dado - apontador generico para uma Encomenda
 * RETORNO: void
 * DESCRICAO: Exibe informacoes simplificadas de uma encomenda
 *            Versao para visualizacao por clientes (menos dados)
 */
void imprimir_encomenda_cliente(void *dado) {
    Encomenda *encomenda = dado;
    char nomeVendedor[TAM_NOME] = "N/A";
    long quantidade = (encomenda->idProduto == -1) ? encomenda->qtd : encomenda->qtd_cliente.qtd;

    preencher_nomes_encomenda(encomenda, (char[80]){"N/A"}, nomeVendedor);

    printf("ID: %d | Produto: %s | Preco: %.2lf | Vendedor: %s | Origem: %d | Disponivel: %ld | Estado: %s | %s\n",
           encomenda->id, encomenda->nome_produto, encomenda->preco, nomeVendedor,
           encomenda->origem, quantidade, estado_para_texto(encomenda->estado), encomenda->descricao);
}

/*
 * FUNCAO: imprimir_encomenda_vendedor
 * PARAMETRO: void *dado - apontador generico para uma Encomenda
 * RETORNO: void
 * DESCRICAO: Exibe informacoes simplificadas de uma encomenda
 *            Versao para visualizacao por vendedor (menos dados)
 */
void imprimir_encomenda_vendedor(void *dado) {
    Encomenda *encomenda = dado;
    printf("ID: %d | Produto: %s | Preco: %.2lf | Origem: %d | Stock: %ld | Estado: %s\n",
           encomenda->id, encomenda->nome_produto, encomenda->preco,
           encomenda->origem, encomenda->qtd, estado_para_texto(encomenda->estado));
}

/*
 * FUNCAO: sistema_inicializar
 * PARAMETRO: Sistema *sistema - apontador para estrutura Sistema
 * RETORNO: void
 * DESCRICAO: Inicializa todas as estruturas de dados do sistema
 */
void sistema_inicializar(Sistema *sistema) {
    sistema_definir_contexto(sistema);
    strcpy(sistema->admin.nome, "admin");
    strcpy(sistema->admin.senha, "admin123");
    hash_inicializar(&sistema->clientes, TIPO_CLIENTE);
    hash_inicializar(&sistema->vendedores, TIPO_VENDEDOR);
    hash_inicializar(&sistema->encomendas, TIPO_ENCOMENDA);
    fila_inicializar(&sistema->pendentes);
    fila_inicializar(&sistema->confirmacoes_clientes);
    fila_inicializar(&sistema->confirmacoes_vendedor);
    lista_inicializar(&sistema->id_encomendas);
    grafo_inicializar(&sistema->rotas);
}

/*
 * FUNCAO: sistema_carregar_exemplo
 * PARAMETRO: Sistema *sistema - apontador para estrutura Sistema
 * RETORNO: void
 * DESCRICAO: Carrega dados de exemplo no sistema (bairros de Luanda)
 */
void sistema_carregar_exemplo(Sistema *sistema) {
    int kilamba = grafo_adicionar_local(&sistema->rotas, "Kilamba");
    int benfica = grafo_adicionar_local(&sistema->rotas, "Benfica");
    int maianga = grafo_adicionar_local(&sistema->rotas, "Maianga");
    int mutamba = grafo_adicionar_local(&sistema->rotas, "Mutamba");
    int talatona = grafo_adicionar_local(&sistema->rotas, "Talatona");

    grafo_adicionar_rota(&sistema->rotas, kilamba, benfica, 12);
    grafo_adicionar_rota(&sistema->rotas, benfica, maianga, 8);
    grafo_adicionar_rota(&sistema->rotas, maianga, mutamba, 5);
    grafo_adicionar_rota(&sistema->rotas, kilamba, talatona, 10);
    grafo_adicionar_rota(&sistema->rotas, talatona, mutamba, 14);
}

/*
 * FUNCAO: verificar_user_cliente
 * PARAMETROS: HashTable, username, ID
 * RETORNO: int - 1 se encontra, 0 senao
 * DESCRICAO: Verifica se existe um cliente com o username especificado
 */
int verificar_user_cliente(HashTable *cliente, char *username, int ID){
    Cliente *clientes = hash_buscar(cliente, ID);
    if (!clientes)
        return (0);
    if (strcmp(clientes->username, username) == 0)
        return (1);
    return (0);
}

/*
 * FUNCAO: verificar_user_vendedor
 * PARAMETROS: HashTable, username, ID
 * RETORNO: int - 1 se encontra, 0 senao
 * DESCRICAO: Verifica se existe um vendedor com o username especificado
 */
int verificar_user_vendedor(HashTable *vendedor, char *username, int ID){
    Vendedor *vendedores = hash_buscar(vendedor, ID);
    if (!vendedores)
        return (0);
    if (strcmp(vendedores->username, username) == 0)
        return (1);
    return (0);
}

/*
 * FUNCAO: verificar_username
 * PARAMETROS: Sistema, username, ID
 * RETORNO: int - 1 se username ja existe, 0 senao
 * DESCRICAO: Verifica se um username ja esta registado no sistema
 */
int verificar_username(Sistema *sistema, char *username, int ID){
    Vendedor *vendedor = hash_buscar(&sistema->vendedores, ID);
    Cliente *cliente = hash_buscar(&sistema->clientes, ID);
    if (vendedor && cliente){
        if (strcmp(vendedor->username, username) == 0 || strcmp(cliente->username, username) == 0)
            return (1);
    }
    else{
        if (vendedor && strcmp(vendedor->username, username) == 0)
            return (1);
        else if (cliente && strcmp(cliente->username, username) == 0)
            return (1);
    }
    return (0);
}

/*
 * FUNCAO: cadastrar_cliente
 * PARAMETRO: Sistema *sistema - apontador para estrutura Sistema
 * RETORNO: void
 * DESCRICAO: Permite que um novo cliente se registe no sistema
 */
void cadastrar_cliente(Sistema *sistema) {
    Cliente *cliente = malloc(sizeof(Cliente));
    int local;
    if (cliente == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    memset(cliente, 0, sizeof(*cliente));

    ler_texto("Nome: ", cliente->nome, TAM_NOME);
    if (!texto_valido(cliente->nome)) {
        printf("Erro: o nome e obrigatorio.\n");
        free(cliente);
        return;
    }

    cliente->idade = ler_inteiro("Idade: ");
    if (cliente->idade <= 17) {
        printf("Erro: idade invalida.\n");
        free(cliente);
        return;
    }

    ler_texto("Telefone: ", cliente->telefone, sizeof(cliente->telefone));
    if (!texto_valido(cliente->telefone)) {
        printf("Erro: o telefone e obrigatorio.\n");
        free(cliente);
        return;
    }

    ler_texto("Endereco: ", cliente->endereco, TAM_TEXTO);
    if (!texto_valido(cliente->endereco)) {
        printf("Erro: o endereco e obrigatorio.\n");
        free(cliente);
        return;
    }

    if (!grafo_verificar_local(&sistema->rotas, cliente->endereco)){
        Grafo *grafo = &sistema->rotas;
        system("cls");
        printf("Este ponto nao faz parte dos nossos registos. Informe a distancia a um desses pontos...\n");
        while (true){
            for (int i = 0; i < grafo->total; i++) {
                printf("[%d] %s\n", grafo->locais[i].id, grafo->locais[i].nome);
            }
            int ponto = ler_inteiro("Ponto: ");
            if (ponto > (grafo->total - 1) || ponto < 0){
                printf("Ponto invalido.\n");
                Sleep(800);
            }
            else{
                while (true){
                    system("cls");
                    int distancia = ler_inteiro("Informe a distancia a esse ponto: ");
                    if (distancia <= 0){
                        printf("Distancia invalida.\n");
                        Sleep(800);
                    }
                    else{
                        local = grafo_adicionar_local(&sistema->rotas, cliente->endereco);
                        grafo_adicionar_rota(&sistema->rotas, local, ponto, distancia);
                        printf("Rota criada com sucesso.\n");
                        Sleep(800);
                        break;
                    }
                }
                break;
            }
        }
        system("cls");
    }

    ler_texto("Username: ", cliente->username, TAM_USERNAME);
    if (!texto_valido(cliente->username)) {
        printf("Erro: o username e obrigatorio.\n");
        free(cliente);
        return;
    }

    cliente->id = converter_username(cliente->username);
    ler_texto("Senha: ", cliente->senha, TAM_USERNAME);
    if (!texto_valido(cliente->senha)) {
        printf("Erro: a senha e obrigatoria.\n");
        free(cliente);
        return;
    }

    cliente->ativo = DESATIVO;

    if ((hash_buscar(&sistema->clientes, cliente->id) != NULL && verificar_username(sistema, cliente->username, cliente->id)) 
        || (hash_buscar(&sistema->vendedores, cliente->id) != NULL && verificar_username(sistema, cliente->username, cliente->id))) {
        printf("Erro: ja existe usuario com este username.\n");
        free(cliente);
        return;
    }

    if (hash_inserir(&sistema->clientes, cliente->id, cliente)) {
        printf("Cliente cadastrado com sucesso. Aguardando aprovacao.\n");
        fila_enfileirar(&sistema->confirmacoes_clientes, cliente->id);
    } else {
        printf("Erro ao cadastrar cliente.\n");
        free(cliente);
    }
}

/*
 * FUNCAO: cadastrar_vendedor
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Permite que um novo vendedor se registe no sistema
 */
void cadastrar_vendedor(Sistema *sistema) {
    Vendedor *vendedor = malloc(sizeof(Vendedor));
    if (vendedor == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    memset(vendedor, 0, sizeof(*vendedor));

    ler_texto("Nome: ", vendedor->nome, TAM_NOME);
    if (!texto_valido(vendedor->nome)) {
        printf("Erro: o nome e obrigatorio.\n");
        free(vendedor);
        return;
    }

    vendedor->idade = ler_inteiro("Idade: ");
    if (vendedor->idade <= 0 || vendedor->idade > 120) {
        printf("Erro: idade invalida.\n");
        free(vendedor);
        return;
    }

    ler_texto("Telefone: ", vendedor->telefone, sizeof(vendedor->telefone));
    if (!texto_valido(vendedor->telefone)) {
        printf("Erro: o telefone e obrigatorio.\n");
        free(vendedor);
        return;
    }
    vendedor->disponibilidade = DISPONIVEL;

    ler_texto("Username: ", vendedor->username, TAM_USERNAME);
    if (!texto_valido(vendedor->username)) {
        printf("Erro: o username e obrigatorio.\n");
        free(vendedor);
        return;
    }

    vendedor->id = converter_username(vendedor->username);
    ler_texto("Senha: ", vendedor->senha, TAM_USERNAME);
    if (!texto_valido(vendedor->senha)) {
        printf("Erro: a senha e obrigatoria.\n");
        free(vendedor);
        return;
    }

    if ((hash_buscar(&sistema->vendedores, vendedor->id) != NULL && verificar_username(sistema, vendedor->username, vendedor->id)) 
    || (hash_buscar(&sistema->clientes, vendedor->id) != NULL && verificar_username(sistema, vendedor->username, vendedor->id))) {
        printf("Erro: ja existe usuario com este username.\n");
        free(vendedor);
        return;
    }

    vendedor->ativo = DESATIVO;
    if (hash_inserir(&sistema->vendedores, vendedor->id, vendedor)) {
        printf("Vendedor cadastrado com sucesso. Aguardando aprovacao.\n");
        fila_enfileirar(&sistema->confirmacoes_vendedor, vendedor->id);
    } else {
        printf("Erro ao cadastrar entregador.\n");
        free(vendedor);
    }
}

/*
 * FUNCAO: criar_encomenda
 * PARAMETROS: Sistema *sistema, Vendedor *vendedor
 * RETORNO: void
 * DESCRICAO: Cria uma nova encomenda no sistema (por vendedor)
 */
void criar_encomenda(Sistema *sistema, Vendedor *vendedor) {
    Encomenda *encomenda = malloc(sizeof(Encomenda));
    if (encomenda == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    memset(encomenda, 0, sizeof(*encomenda));

    ler_texto("Produto: ", encomenda->nome_produto, TAM_NOME);
    if (!texto_valido(encomenda->nome_produto)) {
        printf("Erro: o nome do produto e obrigatorio.\n");
        free(encomenda);
        return;
    }

    encomenda->preco = (double)ler_inteiro("Preco: ");
    if (encomenda->preco <= 0) {
        printf("Erro: o preco deve ser maior que zero.\n");
        free(encomenda);
        return;
    }

    encomenda->qtd = ler_inteiro("Quantidade: ");
    if (encomenda->qtd <= 0) {
        printf("Erro: a quantidade deve ser maior que zero.\n");
        free(encomenda);
        return;
    }

    int next_id = 0;
    while (hash_buscar(&sistema->encomendas, next_id) != NULL) {
        next_id++;
    }
    encomenda->id = next_id;

    ler_texto("Descricao: ", encomenda->descricao, TAM_TEXTO);
    grafo_listar(&sistema->rotas);
    encomenda->origem = ler_inteiro("ID do local de origem: ");
    if (!local_valido(sistema, encomenda->origem)) {
        printf("Erro: local de origem invalido.\n");
        free(encomenda);
        return;
    }
    encomenda->idEntregador = vendedor->id;
    encomenda->estado = LIVRE;
    encomenda->qtd_cliente.qtd = 0;
    encomenda->qtd_cliente.id_cliente = -1;
    encomenda->idCliente = -1;
    encomenda->idProduto = -1;
    encomenda->prioridade = 0;
    encomenda->destino = -1;
    encomenda->telefone_vendedor = vendedor->telefone;
    encomenda->telefone_cliente = NULL;
    encomenda->comprado = 0;

    if (hash_inserir(&sistema->encomendas, encomenda->id, encomenda)) {
        lista_inserir(&sistema->id_encomendas, encomenda->id);
        printf("Produto registado com sucesso e disponivel para compra.\n");
    } else {
        printf("Erro ao criar encomenda.\n");
        free(encomenda);
    }
}

/*
 * FUNCAO: criar_encomenda_cliente
 * PARAMETROS: Sistema *sistema, int idCliente
 * RETORNO: void
 * DESCRICAO: Cliente cria uma encomenda a partir de um produto existente
 */
void criar_encomenda_cliente(Sistema *sistema, int idCliente) {
    int id;

    if (lista_vazia(&sistema->id_encomendas)){
        printf("Sem produtos a venda.\n");
        return;
    }
    listar_produtos(&sistema->encomendas, &sistema->id_encomendas);
    id = ler_inteiro("ID da encomenda: ");

    Encomenda *produto = hash_buscar(&sistema->encomendas, id);
    if (!produto){
        printf("Erro: Nao existe encomenda com este ID.\n");
        return;
    }

    if (!produto_disponivel(produto)){
        printf("Erro: este registo nao representa um produto disponivel.\n");
        return;
    }

    ll quantidade = ler_inteiro("Quantidade desejada: ");
    if (quantidade <= 0){
        printf("Erro: a quantidade deve ser maior que zero.\n");
        return;
    }

    if (quantidade > produto->qtd){
        printf("Stock insuficiente. Disponivel: %ld\n", produto->qtd);
        return;
    }

    int next_id = 0;
    while (hash_buscar(&sistema->encomendas, next_id) != NULL) {
        next_id++;
    }

    Encomenda *pedido = malloc(sizeof(Encomenda));
    if (pedido == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }

    memset(pedido, 0, sizeof(*pedido));
    pedido->id = next_id;
    strcpy(pedido->nome_produto, produto->nome_produto);
    pedido->preco = produto->preco;
    pedido->idCliente = idCliente;
    pedido->idProduto = produto->id;
    pedido->idEntregador = produto->idEntregador;
    strcpy(pedido->descricao, produto->descricao);
    pedido->origem = produto->origem;
    grafo_listar(&sistema->rotas);
    pedido->destino = ler_inteiro("ID do local de destino: ");
    if (!local_valido(sistema, pedido->destino)) {
        printf("Erro: local de destino invalido.\n");
        free(pedido);
        return;
    }
    pedido->prioridade = ler_inteiro("Prioridade (1 baixa, 2 media, 3 alta): ");
    if (pedido->prioridade < 1 || pedido->prioridade > 3) {
        printf("Erro: prioridade invalida. Use 1, 2 ou 3.\n");
        free(pedido);
        return;
    }
    pedido->estado = PENDENTE;
    pedido->qtd = quantidade;
    pedido->qtd_cliente.id_cliente = idCliente;
    pedido->qtd_cliente.qtd = quantidade;
    pedido->comprado = 1;
    pedido->telefone_vendedor = produto->telefone_vendedor;
    pedido->telefone_cliente = NULL;

    produto->qtd -= quantidade;

    if (hash_inserir(&sistema->encomendas, pedido->id, pedido) && fila_enfileirar(&sistema->pendentes, pedido->id)) {
        printf("Encomenda criada com sucesso. A sua compra foi registada e enviada para processamento.\n");
    } else {
        produto->qtd += quantidade;
        free(pedido);
        printf("Erro ao encomendar.\n");
    }
}

/*
 * FUNCAO: atribuir_entrega
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Atribui uma encomenda pendente a um entregador disponivel
 */
void atribuir_entrega(Sistema *sistema) {
    int idEncomenda;
    int idEntregador;
    Encomenda *encomenda;
    Vendedor *entregador;

    if (!fila_desenfileirar(&sistema->pendentes, &idEncomenda)) {
        printf("Nao existem encomendas pendentes.\n");
        return;
    }

    encomenda = hash_buscar(&sistema->encomendas, idEncomenda);
    if (encomenda == NULL || encomenda->estado != PENDENTE) {
        printf("A encomenda pendente ja nao esta disponivel.\n");
        return;
    }

    idEntregador = encomenda->idEntregador;
    entregador = hash_buscar(&sistema->vendedores, idEntregador);

    if (entregador == NULL) {
        printf("Erro: entregador nao encontrado. A encomenda volta para a fila.\n");
        fila_enfileirar(&sistema->pendentes, idEncomenda);
        return;
    }

    if (entregador->disponibilidade != DISPONIVEL) {
        printf("Erro: entregador ocupado. A encomenda volta para a fila.\n");
        fila_enfileirar(&sistema->pendentes, idEncomenda);
        return;
    }

    encomenda->estado = EM_TRANSITO;
    entregador->disponibilidade = OCUPADO;
}

/*
 * FUNCAO: atualizar_estado_entregador
 * PARAMETROS: Sistema *sistema, int idEntregador
 * RETORNO: void
 * DESCRICAO: Permite que vendedor atualize o estado de suas entregas
 */
void atualizar_estado_entregador(Sistema *sistema, int idEntregador) {

    if(!listar_encomendas_entregador(sistema, idEntregador)) return;

    int id = ler_inteiro("ID da encomenda: ");
    int estado;
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);
    Vendedor *entregador;

    if (encomenda == NULL) {
        printf("Erro: encomenda nao encontrada.\n");
        return;
    }

    if (encomenda->idEntregador != idEntregador) {
        printf("Erro: esta encomenda nao esta atribuida a este entregador.\n");
        return;
    }

    estado = ler_inteiro("Novo estado (1 em transito, 2 entregue): ");
    if (estado != EM_TRANSITO && estado != ENTREGUE) {
        printf("Erro: entregador so pode colocar em transito ou entregue.\n");
        return;
    }

    encomenda->estado = (EstadoEncomenda)estado;
    if (estado == ENTREGUE) {
        entregador = hash_buscar(&sistema->vendedores, idEntregador);
        if (entregador != NULL) {
            entregador->disponibilidade = DISPONIVEL;
        }
    }

    printf("Estado atualizado com sucesso.\n");
}

/*
 * FUNCAO: consultar_rota
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Encontra a menor rota entre dois locais usando grafo
 */
void consultar_rota(Sistema *sistema) {
    int origem;
    int destino;
    int caminho[MAX_LOCAIS];
    int tamanho = 0;
    int distancia;
    int i;

    grafo_listar(&sistema->rotas);
    origem = ler_inteiro("Origem: ");
    destino = ler_inteiro("Destino: ");

    distancia = grafo_menor_caminho(&sistema->rotas, origem, destino, caminho, &tamanho);
    if (distancia < 0) {
        printf("Nao existe rota entre os locais indicados.\n");
        return;
    }

    printf("Menor rota: ");
    for (i = 0; i < tamanho; i++) {
        printf("%s", sistema->rotas.locais[caminho[i]].nome);
        if (i + 1 < tamanho) {
            printf(" -> ");
        }
    }
    printf(" | Distancia total: %dkm\n", distancia);
}

/*
 * FUNCAO: pesquisar
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Permite pesquisar por clientes, vendedores ou encomendas
 */
void pesquisar(Sistema *sistema){
    int opcao;
    int id;
    char nome[TAM_NOME];
    void *resultado = NULL;

    printf("1. Cliente\n2. Entregador\n3. Encomenda\n");
    opcao = ler_inteiro("Pesquisar: ");
    
    if (opcao == 1 || opcao == 2){
        ler_texto("ID ou username: ", nome, TAM_NOME);
        if (is_numeric(nome))
            id = atoi(nome);
        else
            id = converter_username(nome);
    }
    else if (opcao == 3) id = ler_inteiro("ID: ");
    else {
        system("cls");
        printf("Opcao Invalida.\n"); 
        return;
    }
    system("cls");
    if (opcao == 1) {
        resultado = hash_buscar(&sistema->clientes, id);
        if (resultado != NULL) imprimir_cliente(resultado);
    } else if (opcao == 2) {
        resultado = hash_buscar(&sistema->vendedores, id);
        if (resultado != NULL) imprimir_entregador(resultado);
    } else if (opcao == 3) {
        resultado = hash_buscar(&sistema->encomendas, id);
        if (resultado != NULL) imprimir_encomenda(resultado);
    }

    if (resultado == NULL) {
        printf("Registo nao encontrado.\n");
        return;
    }
    Sleep(800);
}

/*
 * FUNCAO: listar_produtos
 * PARAMETROS: HashTable *encomendas, Lista *idencomendas
 * RETORNO: void
 * DESCRICAO: Lista todos os produtos/encomendas disponiveis para compra
 */
void listar_produtos(HashTable *encomendas, Lista *idencomendas){
    if (lista_vazia(idencomendas)){
        printf("Sem produtos a venda\n");
        return;
    }
    NoLista *aux = idencomendas->id_lista;
    while (aux){
        Encomenda *encomenda = hash_buscar(encomendas, aux->id);
        if (encomenda != NULL && encomenda->idCliente == -1 && encomenda->idProduto == -1 && encomenda->qtd > 0) {
            imprimir_encomenda_cliente(encomenda);
        }
        aux = aux->prox;
    }
}

/*
 * FUNCAO: listar_produtos_vendedor
 * PARAMETROS: HashTable *encomendas, Lista *idencomendas, int idvendedor
 * RETORNO: void
 * DESCRICAO: Lista todos os produtos a venda de um vendedor
 */
void listar_produtos_vendedor(HashTable *encomendas, Lista *idencomendas, int idvendedor){
    if (lista_vazia(idencomendas)){
        printf("Sem produtos a venda\n");
        return;
    }
    NoLista *aux = idencomendas->id_lista;
    while (aux){
        Encomenda *encomenda = hash_buscar(encomendas, aux->id);
        if (encomenda != NULL && encomenda->idCliente == -1 && encomenda->idProduto == -1 && encomenda->idEntregador == idvendedor) {
            imprimir_encomenda_vendedor(encomenda);
        }
        aux = aux->prox;
    }
}

/*
 * FUNCAO: listar_encomendas_cliente
 * PARAMETROS: Sistema *sistema, int idCliente
 * RETORNO: int - 1 se encontrou encomendas, 0 senao
 * DESCRICAO: Lista todas as encomendas de um cliente
 */
int listar_encomendas_cliente(Sistema *sistema, int idCliente) {
    int i;
    int encontrou = 0;
    NoHash *no;
    Encomenda *encomenda;

    for (i = 0; i < TAM_HASH; i++) {
        no = sistema->encomendas.tabela[i];
        while (no != NULL) {
            encomenda = no->dado;
            if (encomenda->idCliente == idCliente && encomenda->idProduto != -1) {
                imprimir_encomenda(encomenda);
                encontrou = 1;
            }
            no = no->proximo;
        }
    }

    if (!encontrou) {
        printf("Nao existem encomendas para este cliente.\n");
        return 0;
    }

    Sleep(800);
    return (1);
}

/*
 * FUNCAO: listar_encomendas_entregador
 * PARAMETROS: Sistema *sistema, int idEntregador
 * RETORNO: int - 1 se encontrou encomendas, 0 senao
 * DESCRICAO: Lista todas as encomendas de um vendedor/entregador
 */
int listar_encomendas_entregador(Sistema *sistema, int idEntregador) {
    int i;
    int encontrou = 0;
    NoHash *no;
    Encomenda *encomenda;

    for (i = 0; i < TAM_HASH; i++) {
        no = sistema->encomendas.tabela[i];
        while (no != NULL) {
            encomenda = no->dado;
            if (encomenda->idEntregador == idEntregador && encomenda->idProduto != -1) {
                if (encomenda->comprado == 1){
                    imprimir_encomenda(encomenda);
                    encontrou = 1;
                }
            }
            no = no->proximo;
        }
    }

    if (!encontrou) {
        printf("Nao existem entregas por fazer.\n");
        return(0);
    }
    
    Sleep(800);
    return (1);
}

/*
 * FUNCAO: consultar_estado_cliente
 * PARAMETROS: Sistema *sistema, int idCliente
 * RETORNO: void
 * DESCRICAO: Permite cliente consultar o estado de uma sua encomenda
 */
void consultar_estado_cliente(Sistema *sistema, int idCliente) {
    int id = ler_inteiro("ID da encomenda: ");
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);

    if (encomenda == NULL || encomenda->idCliente != idCliente) {
        printf("Encomenda nao encontrada para este cliente.\n");
        return;
    }

    imprimir_encomenda(encomenda);
    Sleep(800);
}

/*
 * FUNCAO: cancelar_encomenda_cliente
 * PARAMETROS: Sistema *sistema, int idCliente
 * RETORNO: void
 * DESCRICAO: Permite cliente cancelar uma encomenda pendente
 */
void cancelar_encomenda_cliente(Sistema *sistema, int idCliente) {

    if (!listar_encomendas_cliente(sistema, idCliente)) return;

    int id = ler_inteiro("ID da encomenda: ");
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);

    if (encomenda == NULL || encomenda->idCliente != idCliente || encomenda->idProduto == -1) {
        printf("Encomenda nao encontrada para este cliente.\n");
        return;
    }

    if (!pedido_pendente(encomenda)) {
        printf("Apenas encomendas pendentes podem ser canceladas pelo cliente.\n");
        return;
    }

    Encomenda *produto = hash_buscar(&sistema->encomendas, encomenda->idProduto);
    if (produto != NULL) {
        produto->qtd += encomenda->qtd;
        produto->estado = LIVRE;
    }

    encomenda->estado = CANCELADA;
    encomenda->comprado = 0;
    encomenda->qtd_cliente.qtd = 0;
    printf("Encomenda cancelada com sucesso. O stock do produto foi restaurado.\n");
}

/*
 * FUNCAO: consultar_rota_encomenda_entregador
 * PARAMETROS: Sistema *sistema, int idEntregador
 * RETORNO: void
 * DESCRICAO: Permite entregador ver a rota de uma sua encomenda
 */
void consultar_rota_encomenda_entregador(Sistema *sistema, int idEntregador) {
    int id = ler_inteiro("ID da encomenda: ");
    int caminho[MAX_LOCAIS];
    int tamanho = 0;
    int distancia;
    int i;
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);

    if (encomenda == NULL || encomenda->idEntregador != idEntregador || encomenda->idProduto == -1) {
        printf("Encomenda nao encontrada para este entregador.\n");
        return;
    }

    distancia = grafo_menor_caminho(&sistema->rotas, encomenda->origem, encomenda->destino, caminho, &tamanho);
    if (distancia < 0) {
        printf("Nao existe rota para esta encomenda.\n");
        return;
    }

    printf("Rota da encomenda #%d: ", encomenda->id);
    for (i = 0; i < tamanho; i++) {
        printf("%s", sistema->rotas.locais[caminho[i]].nome);
        if (i + 1 < tamanho) {
            printf(" -> ");
        }
    }
    printf(" | Distancia total: %dkm\n", distancia);
}

/*
 * FUNCAO: alterar_disponibilidade_entregador
 * PARAMETROS: Sistema *sistema, int idEntregador
 * RETORNO: void
 * DESCRICAO: Permite entregador mudar seu status (disponivel/ocupado)
 */
void alterar_disponibilidade_entregador(Sistema *sistema, int idEntregador) {
    int disponibilidade;
    Vendedor *entregador = hash_buscar(&sistema->vendedores, idEntregador);

    if (entregador == NULL) {
        printf("Entregador nao encontrado.\n");
        return;
    }

    disponibilidade = ler_inteiro("Disponibilidade (1 disponivel, 0 ocupado): ");
    if (disponibilidade != 0 && disponibilidade != 1) {
        printf("Valor invalido.\n");
        return;
    }

    entregador->disponibilidade = disponibilidade == 1 ? DISPONIVEL : OCUPADO;
    printf("Disponibilidade atualizada com sucesso.\n");
}

/*
 * FUNCAO: listar_dados
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Menu para admin listar clientes, vendedores, encomendas, rotas
 */
void listar_dados(Sistema *sistema) {
    int opcao;

    printf("1. Clientes\n2. Entregadores\n3. Encomendas\n4. Fila de pendentes\n5. Rotas\n");
    opcao = ler_inteiro("Listar: ");
    system("cls");
    if (opcao == 1) {
        hash_listar(&sistema->clientes, imprimir_cliente);
    } else if (opcao == 2) {
        hash_listar(&sistema->vendedores, imprimir_entregador);
    } else if (opcao == 3) {
        hash_listar(&sistema->encomendas, imprimir_encomenda);
    } else if (opcao == 4) {
        fila_listar(&sistema->pendentes);
    } else if (opcao == 5) {
        grafo_listar(&sistema->rotas);
    } else {
        printf("Opcao invalida.\n");
        return;
    }
}

/*
 * FUNCAO: confirmado_cliente
 * PARAMETROS: HashTable *hash, Fila *confirmacoes
 * RETORNO: void
 * DESCRICAO: Admin aprova/rejeita registos de clientes pendentes
 */
void confirmado_cliente(HashTable *hash, Fila *confirmacoes){
    if (fila_vazia(confirmacoes)){
        printf("Sem contas por verificar.\n");
        return;
    }
    while (!fila_vazia(confirmacoes)){
        int id = confirmacoes->inicio->id;
        Cliente *cliente = hash_buscar(hash, id);

        if (cliente == NULL || cliente->ativo != DESATIVO){
            int ign;
            fila_desenfileirar(confirmacoes, &ign);
            continue;
        }

        system("cls");
        imprimir_cliente(cliente);
        printf("\nDeseja validar cadastro?\n1. SIM - 2. NAO - 3 - SAIR\n");
        ll t = ler_inteiro("Opcao: ");
        system("cls");
        if (t == 1){
            cliente->ativo = ATIVO;
            int ign;
            fila_desenfileirar(confirmacoes, &ign);
            printf("Conta \"%s\" confirmada.\n", cliente->username);
            Sleep(700);
        } else if (t == 2){
            cliente->ativo = NEGADO;
            int ign;
            fila_desenfileirar(confirmacoes, &ign);
            printf("Conta \"%s\" negada.\n", cliente->username);
            Sleep(700);
        } else if (t == 3){
            return;
        } else {
            printf("Opcao invalida.\n");
            Sleep(700);
        }
    }
}

/*
 * FUNCAO: confirmado_vendedor
 * PARAMETROS: HashTable *hash, Fila *confirmacoes
 * RETORNO: void
 * DESCRICAO: Admin aprova/rejeita registos de vendedores pendentes
 */
void confirmado_vendedor(HashTable *hash, Fila *confirmacoes){
    if (fila_vazia(confirmacoes)){
        printf("Sem contas por verificar.\n");
        return;
    }
    while (!fila_vazia(confirmacoes)){
        int id = confirmacoes->inicio->id;
        Vendedor *vendedor = hash_buscar(hash, id);

        if (vendedor == NULL || vendedor->ativo != DESATIVO){
            int ign;
            fila_desenfileirar(confirmacoes, &ign);
            continue;
        }

        system("cls");
        imprimir_entregador(vendedor);
        printf("\nDeseja validar cadastro?\n1. SIM - 2. NAO - 3. SAIR\n");
        ll t = ler_inteiro("Opcao: ");
        system("cls");
        if (t == 1){
            vendedor->ativo = ATIVO;
            int ign;
            fila_desenfileirar(confirmacoes, &ign);
            printf("Conta \"%s\" confirmada.\n", vendedor->username);
            Sleep(700);
        } else if (t == 2){
            vendedor->ativo = NEGADO;
            int ign;
            fila_desenfileirar(confirmacoes, &ign);
            printf("Conta \"%s\" negada.\n", vendedor->username);
            Sleep(700);
        } else if (t == 3){
            return;
        } else {
            printf("Opcao invalida.\n");
            Sleep(700);
        }
    }
}

/*
 * FUNCAO: registar_rotas
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Admin adiciona novos locais ou rotas ao sistema
 */
void registar_rotas(Sistema *sistema){
    char endereco[TAM_TEXTO];
    ll opcao;
    int origem;
    int destino;
    Grafo *rotas = &sistema->rotas;
    for (int i = 0; i < rotas->total; i++) {
        printf("[%d] %s\n", rotas->locais[i].id, rotas->locais[i].nome);
    }

    printf("\n1. Adicionar novo local\n");
    printf("2. Registar nova rota\n");
    printf("0. Voltar\n");
    opcao = ler_inteiro("Opcao: ");
    system("cls");

    switch(opcao){
        case 1:
            ler_texto("local: ", endereco, TAM_TEXTO);
            if (!grafo_verificar_local(rotas, endereco)){
                system("cls");
                printf("Nos informe a distancia a um desses pontos...\n");
                while (true){
                    for (int i = 0; i < rotas->total; i++) {
                        printf("[%d] %s\n", rotas->locais[i].id, rotas->locais[i].nome);
                    }
                    int ponto = ler_inteiro("Ponto: ");
                    if (ponto > (rotas->total - 1) || ponto < 0){
                        printf("Ponto invalido.\n");
                        Sleep(800);
                    }
                    else{
                        while (true){
                            system("cls");
                            int distancia = ler_inteiro("Informe a distancia a esse ponto: ");
                            if (distancia <= 0){
                                printf("Distancia invalida.\n");
                                Sleep(800);
                            }
                            else{
                                int local = grafo_adicionar_local(rotas, endereco);
                                grafo_adicionar_rota(rotas, local, ponto, distancia);
                                printf("Rota criada com sucesso.\n");
                                Sleep(800);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            else{
                system("cls");
                printf("Este local ja faz parte dos nossos registos.\n"); 
            }
            break;
        case 2:{
            for (int i = 0; i < rotas->total; i++) {
                printf("[%d] %s\n", rotas->locais[i].id, rotas->locais[i].nome);
            }
            origem = ler_inteiro("Origem: ");
            destino = ler_inteiro("Destino: ");
            while (true){
                int distancia = ler_inteiro("Informe a distancia: ");
                if (distancia <= 0){
                    system("cls");
                    printf("Distancia invalida.\n");
                    Sleep(800);
                }
                else{
                    grafo_adicionar_rota(rotas, origem, destino, distancia);
                    printf("Rota criada com sucesso.\n");
                    Sleep(800);
                    break;
                }
            }
            break;
        }
        case 0:
            printf("Voltando...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        
    }

}

static void mostrar_cabecalho(const char *titulo, const char *subtitulo) {
    system("cls");
    printf("\n========================================\n");
    printf("%s\n", titulo);
    if (subtitulo != NULL && subtitulo[0] != '\0') {
        printf("%s\n", subtitulo);
    }
    printf("========================================\n");
}

/*
 * FUNCAO: menu_administrador
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Menu principal do administrador com opcoes de gestao
 */
void menu_administrador(Sistema *sistema){
    ll opcao;

    do {
        mostrar_cabecalho("PAINEL DA EMPRESA", "Gere cadastros, rotas e dados do sistema");
        printf("1. Confirmar cadastros de clientes\n");
        printf("2. Confirmar cadastros de vendedores\n");
        printf("3. Registar e gerir rotas\n");
        printf("4. Pesquisar utilizadores ou encomendas\n");
        printf("5. Listar clientes, vendedores e produtos\n");
        printf("6. Consultar a menor rota\n");
        printf("7. Guardar dados em ficheiros\n");
        printf("8. Carregar dados dos ficheiros\n");
        printf("0. Voltar ao login\n");
        opcao = ler_inteiro("Opcao: ");

        switch (opcao) {
            case 1:
                mostrar_cabecalho("CONFIRMACAO DE CLIENTES", "Aprova utilizadores pendentes");
                confirmado_cliente(&sistema->clientes, &sistema->confirmacoes_clientes);
                break;
            case 2:
                mostrar_cabecalho("CONFIRMACAO DE VENDEDORES", "Aprova entregadores pendentes");
                confirmado_vendedor(&sistema->vendedores, &sistema->confirmacoes_vendedor);
                break;
            case 3:
                mostrar_cabecalho("ROTAS", "Adicione novos locais ou ligacoes");
                registar_rotas(sistema);
                break;
            case 4:
                mostrar_cabecalho("PESQUISA", "Procure registos pelo ID");
                pesquisar(sistema);
                break;
            case 5:
                mostrar_cabecalho("LISTAGEM GERAL", "Consulta todos os dados registados");
                listar_dados(sistema);
                break;
            case 6:
                mostrar_cabecalho("ROTA MAIS CURTA", "Encontre a melhor ligacao entre locais");
                consultar_rota(sistema);
                break;
            case 7:
                mostrar_cabecalho("GUARDAR DADOS", "Grava o estado atual para ficheiros");
                if (ficheiro_guardar_dados(sistema)) {
                    printf("Dados guardados com sucesso.\n");
                } else {
                    printf("Erro ao guardar dados.\n");
                }
                break;
            case 8:
                mostrar_cabecalho("CARREGAR DADOS", "Recarrega os dados guardados");
                if (ficheiro_carregar_dados(sistema)) {
                    printf("Dados carregados com sucesso.\n");
                } else {
                    printf("Erro ao carregar dados.\n");
                }
                break;
            case 0:
                printf("A voltar ao ecra de login...\n");
                break;
            default:
                printf("Opcao invalida. Selecione uma opcao do menu.\n");
        }

        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0);
}

/*
 * FUNCAO: menu_cliente
 * PARAMETROS: Sistema *sistema, Cliente *cliente
 * RETORNO: void
 * DESCRICAO: Menu principal do cliente com suas opcoes
 */
void menu_cliente(Sistema *sistema, Cliente *cliente) {
    int opcao;
    ll idCliente = converter_username(cliente->username);

    do {
        mostrar_cabecalho("AREA DO CLIENTE", cliente->nome);
        printf("1. Fazer uma nova encomenda\n");
        printf("2. Consultar o estado de uma encomenda\n");
        printf("3. Ver as minhas encomendas\n");
        printf("4. Cancelar uma encomenda pendente\n");
        printf("0. Voltar ao login\n");
        opcao = ler_inteiro("Opcao: ");

        switch (opcao) {
            case 1:
                mostrar_cabecalho("NOVA ENCOMENDA", "Escolha um produto disponivel");
                criar_encomenda_cliente(sistema, idCliente);
                break;
            case 2:
                mostrar_cabecalho("CONSULTAR ESTADO", "Veja o estado da sua compra");
                consultar_estado_cliente(sistema, idCliente);
                break;
            case 3:
                mostrar_cabecalho("MINHAS ENCOMENDAS", "Historico das suas compras");
                listar_encomendas_cliente(sistema, idCliente);
                break;
            case 4:
                mostrar_cabecalho("CANCELAR ENCOMENDA", "Cancele uma compra ainda pendente");
                cancelar_encomenda_cliente(sistema, idCliente);
                break;
            case 0:
                printf("A voltar ao ecra de login...\n");
                break;
            default:
                printf("Opcao invalida. Selecione uma opcao do menu.\n");
        }

        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0);
}

/*
 * FUNCAO: menu_vendedor
 * PARAMETROS: Sistema *sistema, Vendedor *vendedor
 * RETORNO: void
 * DESCRICAO: Menu principal do vendedor com suas opcoes
 */
void menu_vendedor(Sistema *sistema, Vendedor *vendedor) {
    int opcao;
    ll idvendedor = converter_username(vendedor->username);
    const char *estadoTexto = vendedor->disponibilidade == DISPONIVEL ? "Disponivel" : "Ocupado";

    do {
        mostrar_cabecalho("AREA DO VENDEDOR", vendedor->nome);
        printf("Estado atual: %s\n\n", estadoTexto);
        printf("1. Criar um novo produto\n");
        printf("2. Ver os meus produtos em venda\n");
        printf("3. Ver as minhas entregas\n");
        printf("4. Atualizar o estado de uma entrega\n");
        printf("5. Consultar a rota de uma entrega\n");
        printf("6. Alterar a minha disponibilidade\n");
        printf("0. Voltar ao login\n");
        opcao = ler_inteiro("Opcao: ");

        switch (opcao) {
            case 1:
                mostrar_cabecalho("NOVO PRODUTO", "Registe um produto para venda");
                criar_encomenda(sistema, vendedor);
                break;
            case 2:
                mostrar_cabecalho("MEUS PRODUTOS", "Produtos disponiveis para compra");
                listar_produtos_vendedor(&sistema->encomendas, &sistema->id_encomendas, idvendedor);
                break;
            case 3:
                mostrar_cabecalho("MINHAS ENTREGAS", "Encomendas atribuidas a si");
                listar_encomendas_entregador(sistema, idvendedor);
                break;
            case 4:
                mostrar_cabecalho("ATUALIZAR ESTADO", "Altere o estado de uma entrega");
                atualizar_estado_entregador(sistema, idvendedor);
                break;
            case 5:
                mostrar_cabecalho("ROTA DE ENTREGA", "Consulte a rota da sua encomenda");
                consultar_rota_encomenda_entregador(sistema, idvendedor);
                break;
            case 6:
                mostrar_cabecalho("DISPONIBILIDADE", "Altere o seu estado de trabalho");
                alterar_disponibilidade_entregador(sistema, idvendedor);
                break;
            case 0:
                printf("A voltar ao ecra de login...\n");
                break;
            default:
                printf("Opcao invalida. Selecione uma opcao do menu.\n");
        }

        if (opcao != 0) {
            pausar();
        }
    } while (opcao != 0);
}

/*
 * FUNCAO: confirmado_cliente2
 * PARAMETROS: HashTable *hash, int chave
 * RETORNO: int - 1=ATIVO, 0=DESATIVO, -1=NEGADO
 * DESCRICAO: Verifica o estado de aprovacao de um cliente (para login)
 */
int confirmado_cliente2(HashTable *hash, int chave){
    int indice = funcao_hash(chave);
    NoHash *atual = hash->tabela[indice];

    while (atual){
        if (atual->chave == chave){
            Cliente *cliente = atual->dado;
            if (cliente->ativo == ATIVO){
                return (1);
            }
            else if (cliente->ativo == DESATIVO){
                return (0);
            }
            else if (cliente->ativo == NEGADO){
                return (-1);
            }
        }
        atual = atual->proximo;
    }
    return (0);
}

/*
 * FUNCAO: confirmado_vendedor2
 * PARAMETROS: HashTable *hash, int chave
 * RETORNO: int - 1=ATIVO, 0=DESATIVO, -1=NEGADO
 * DESCRICAO: Verifica o estado de aprovacao de um vendedor (para login)
 */
int confirmado_vendedor2(HashTable *hash, int chave){
    int indice = funcao_hash(chave);
    NoHash *atual = hash->tabela[indice];

    while (atual){
        if (atual->chave == chave){
            Vendedor *vendedor = atual->dado;
            if (vendedor->ativo == ATIVO)
                return (1);
            else if (vendedor->ativo == DESATIVO)
                return (0);
            else if (vendedor->ativo == NEGADO)
                return (-1);
        }
        atual = atual->proximo;
    }
    return (0);
}

/*
 * FUNCAO: sistem_criarConta
 * PARAMETRO: Sistema *sistema
 * RETORNO: int - opcao escolhida (1=criar, 2=login, 3=sair)
 * DESCRICAO: Menu principal do sistema antes de login
 *   EASTER EGG: Opcao 1800 = login de admin
 */
int sistem_criarConta(Sistema *sistema){
    ll opcao, opcao2;
    while (true){
        system("cls");
        printf("\n========================================\n");
        printf("     SISTEMA DE ENTREGAS E COMPRAS\n");
        printf("========================================\n");
        printf("1. Criar conta\n");
        printf("2. Iniciar sessao\n");
        printf("3. Sair\n");
        printf("========================================\n");
        opcao = ler_inteiro("Opcao: ");
        system("cls");
        if (opcao == 1){
            while (true){
                printf("\nEscolha o tipo de conta:\n");
                printf("1. Cliente\n");
                printf("2. Vendedor\n");
                printf("3. Voltar\n");
                opcao2 = ler_inteiro("Opcao: ");
                if (opcao2 == 1){
                    system("cls");
                    printf("\nCadastro de Cliente\n");
                    cadastrar_cliente(sistema);
                    Sleep(700);
                    break;
                }
                else if (opcao2 == 2){
                    system("cls");
                    printf("\nCadastro de Vendedor\n");
                    cadastrar_vendedor(sistema);
                    Sleep(700);
                    break;
                }
                else if (opcao2 == 3) break;
                else{
                    printf("Opcao invalida. Tente novamente.\n");
                    Sleep(700);
                }
            }
        }
        else if (opcao == 2) break;
        else if (opcao == 3) break;
        else if (opcao == 1800) login_adm(sistema);
        else {
            printf("Opcao invalida. Selecione uma opcao do menu.\n");
            Sleep(700);
        }
    }
    return ((int)opcao);
}

/*
 * FUNCAO: login_adm
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Autentica um administrador e acessa menu_administrador
 */
void login_adm(Sistema *sistema){
    Admin *ADMIN = &sistema->admin;
    char name[TAM_NOME], senha[TAM_NOME];

    system("cls");
    printf("\n========================================\n");
    printf("        LOGIN DE ADMINISTRADOR\n");
    printf("========================================\n");
    ler_texto("Nome: ", name, TAM_NOME);
    ler_texto("Senha: ", senha, TAM_NOME);
    system("cls");
    if (strcmp(ADMIN->nome, name) == 0 && strcmp(ADMIN->senha, senha) == 0){
        printf("Acesso concedido. Bem-vindo ao painel da empresa.\n");
        menu_administrador(sistema);
        return;
    }
    else {
        printf("Acesso negado. Credenciais invalidas.\n");
        Sleep(700);
    }
}

/*
 * FUNCAO: sistema_login
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Autentica clientes ou vendedores no sistema
 *   PROCESSO:
 *     1. Pede username e senha
 *     2. Tenta login como vendedor
 *     3. Se falhar, tenta login como cliente
 *     4. Verifica se conta esta aprovada
 *     5. Se aprovada, acessa o menu respectivo
 */
void sistema_login(Sistema *sistema){
    int ID_geral;
    char username[TAM_USERNAME];
    char senha[TAM_USERNAME];
    Vendedor *vendedor;

    system("cls");
    printf("\n========================================\n");
    printf("         LOGIN NO SISTEMA\n");
    printf("========================================\n");
    ler_texto("Username: ", username, TAM_USERNAME);
    ler_texto("Senha: ", senha, TAM_USERNAME);
    system("cls");
    ID_geral = converter_username(username);
    vendedor = hash_buscar(&sistema->vendedores, ID_geral);
    if (vendedor && verificar_user_vendedor(&sistema->vendedores, username, ID_geral)){
        if (strcmp(vendedor->senha, senha) == 0){
            int result = confirmado_vendedor2(&sistema->vendedores, ID_geral);
            if (result == 1){
                printf("Acesso concedido. Bem-vindo, vendedor.\n");
                menu_vendedor(sistema, vendedor);
                return;
            }
            else if (result == 0){
                printf("Conta ainda nao confirmada pela empresa.\n");
            }
            else if (result == -1){
                printf("O acesso foi negado.\n");
                return;
            }
        }
        else{
            printf("Username ou senha incorretos.\n");
        }
    }
    Cliente *cliente = hash_buscar(&sistema->clientes, ID_geral);
    if (cliente && verificar_user_cliente(&sistema->clientes, username, ID_geral)){
        if (strcmp(cliente->senha, senha) == 0){
            int result = confirmado_cliente2(&sistema->clientes, ID_geral);
            if (result == 1){
                printf("Acesso concedido. Bem-vindo, cliente.\n");
                menu_cliente(sistema, cliente);
                return;
            }
            else if (result == 0) {
                printf("Conta ainda nao confirmada pela empresa.\n");
            }
            else if (result == -1){
                printf("O acesso foi negado.\n");
            }
        }
        else{
            printf("Username ou senha incorretos.\n");
        }
    }
    else if (!vendedor && !cliente){
        printf("Usuario nao encontrado.\n");
        return;
    }
}

/*
 * FUNCAO: sistema_liberar
 * PARAMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRICAO: Liberta toda a memoria alocada dinamicamente
 *            Deve ser chamada antes de encerrar o programa
 */
void sistema_liberar(Sistema *sistema) {
    hash_liberar(&sistema->clientes);
    hash_liberar(&sistema->vendedores);
    hash_liberar(&sistema->encomendas);
    fila_liberar(&sistema->pendentes);
    fila_liberar(&sistema->confirmacoes_clientes);
    fila_liberar(&sistema->confirmacoes_vendedor);
    lista_liberar(&sistema->id_encomendas);
    grafo_liberar(&sistema->rotas);
}
