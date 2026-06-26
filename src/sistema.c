/* ==============================================================
   FICHEIRO: sistema.c
   DESCRIÇÃO: Núcleo do sistema de gestão de entregas
   FUNCIONALIDADES: 
     - Gestão de clientes e vendedores
     - Criação e rastreamento de encomendas
     - Autenticação e login de utilizadores
     - Menus e operações específicas por tipo de utilizador
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

/*
 * FUNÇÃO: imprimir_cliente
 * PARÂMETRO: void *dado - apontador genérico para um Cliente
 * RETORNO: void
 * DESCRIÇÃO: Exibe informações de um cliente em formato tabular
 */
void imprimir_cliente(void *dado) {
    Cliente *cliente = dado;
    printf("ID: %d | Username: %s | Nome: %s | Idade: %d | Telefone: %s | Endereco: %s\n",
           cliente->id, cliente->username, cliente->nome, cliente->idade, cliente->telefone, cliente->endereco);
}

/*
 * FUNÇÃO: imprimir_entregador
 * PARÂMETRO: void *dado - apontador genérico para um Vendedor
 * RETORNO: void
 * DESCRIÇÃO: Exibe informações de um vendedor/entregador
 *            Mostra status de disponibilidade (Disponível/Ocupado)
 */
void imprimir_entregador(void *dado) {
    Vendedor *entregador = dado;
    printf("ID: %d | Username: %s | Nome: %s | Idade: %d | Telefone: %s | Estado: %s\n",
           entregador->id, entregador->username, entregador->nome, entregador->idade, entregador->telefone,
           entregador->disponibilidade == DISPONIVEL ? "Disponivel" : "Ocupado");
}

/*
 * FUNÇÃO: imprimir_encomenda
 * PARÂMETRO: void *dado - apontador genérico para uma Encomenda
 * RETORNO: void
 * DESCRIÇÃO: Exibe informações detalhadas de uma encomenda
 *            Inclui: produto, preço, ids, origem, destino, prioridade e estado
 */
void imprimir_encomenda(void *dado) {
    Encomenda *encomenda = dado;
    printf("ID: %d | Produto: %s | Preco: %.2lf | Vendedor: %d | Cliente: %d | Origem: %d | Destino: %d | Prioridade: %d| Estado: %s | %s\n\n",
           encomenda->id, encomenda->nome_produto, encomenda->preco, encomenda->idEntregador,
           encomenda->idCliente, encomenda->origem, encomenda->destino, encomenda->prioridade, estado_para_texto(encomenda->estado), encomenda->descricao);
}

/*
 * FUNÇÃO: imprimir_encomenda_cliente
 * PARÂMETRO: void *dado - apontador genérico para uma Encomenda
 * RETORNO: void
 * DESCRIÇÃO: Exibe informações simplificadas de uma encomenda
 *            Versão para visualização por clientes (menos dados)
 */
void imprimir_encomenda_cliente(void *dado) {
    Encomenda *encomenda = dado;
    printf("ID: %d | Produto: %s | Preco: %lf |Vendedor: %d | Origem: %d | Estado: %s | %s\n",
           encomenda->id, encomenda->nome_produto, encomenda->preco, encomenda->idEntregador,
           encomenda->origem,estado_para_texto(encomenda->estado), encomenda->descricao);
}

/*
 * FUNÇÃO: sistema_inicializar
 * PARÂMETRO: Sistema *sistema - apontador para estrutura Sistema
 * RETORNO: void
 * DESCRIÇÃO: Inicializa todas as estruturas de dados do sistema
 */
void sistema_inicializar(Sistema *sistema) {
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
 * FUNÇÃO: sistema_carregar_exemplo
 * PARÂMETRO: Sistema *sistema - apontador para estrutura Sistema
 * RETORNO: void
 * DESCRIÇÃO: Carrega dados de exemplo no sistema (bairros de Luanda)
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
 * FUNÇÃO: verificar_user_cliente
 * PARÂMETROS: HashTable, username, ID
 * RETORNO: int - 1 se encontra, 0 senão
 * DESCRIÇÃO: Verifica se existe um cliente com o username especificado
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
 * FUNÇÃO: verificar_user_vendedor
 * PARÂMETROS: HashTable, username, ID
 * RETORNO: int - 1 se encontra, 0 senão
 * DESCRIÇÃO: Verifica se existe um vendedor com o username especificado
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
 * FUNÇÃO: verificar_username
 * PARÂMETROS: Sistema, username, ID
 * RETORNO: int - 1 se username já existe, 0 senão
 * DESCRIÇÃO: Verifica se um username já está registado no sistema
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
 * FUNÇÃO: cadastrar_cliente
 * PARÂMETRO: Sistema *sistema - apontador para estrutura Sistema
 * RETORNO: void
 * DESCRIÇÃO: Permite que um novo cliente se registe no sistema
 */
void cadastrar_cliente(Sistema *sistema) {
    Cliente *cliente = malloc(sizeof(Cliente));
    int local;
    if (cliente == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }
    ler_texto("Nome: ", cliente->nome, TAM_NOME);
    cliente->idade = ler_inteiro("Idade: ");
    ler_texto("Telefone: ", cliente->telefone, sizeof(cliente->telefone));
    ler_texto("Endereco: ", cliente->endereco, TAM_TEXTO);

    if (!grafo_verificar_local(&sistema->rotas, cliente->endereco)){
        Grafo *grafo = &sistema->rotas;
        system("cls");
        printf("Este ponto nao faz parte dos nossos registos, nos informe a distancia a um desses pontos...\n");
        while (true){
            for (int i = 0; i < grafo->total; i++) {
                printf("[%d] %s\n", grafo->locais[i].id, grafo->locais[i].nome);
            }
            int ponto = ler_inteiro("Ponto: ");
            if (ponto > (grafo->total - 1) || ponto < 0){
                printf("Ponto invalido.\n");
                Sleep(3000);
            }
            else{
                while (true){
                    system("cls");
                    int distancia = ler_inteiro("Informe a distancia a esse ponto: ");
                    if (distancia <= 0){
                        printf("Distancia invalida.\n");
                        Sleep(3000);
                    }
                    else{
                        local = grafo_adicionar_local(&sistema->rotas, cliente->endereco);
                        grafo_adicionar_rota(&sistema->rotas, local, ponto, distancia);
                        printf("Rota criada com sucesso.\n");
                        Sleep(3000);
                        break;
                    }
                }
                break;
            }
        }
        system("cls");
    }

    ler_texto("Username: ", cliente->username, TAM_USERNAME);
    cliente->id = converter_username(cliente->username);
    ler_texto("Senha: ", cliente->senha, TAM_USERNAME);
    cliente->ativo = DESATIVO;

    if ((hash_buscar(&sistema->clientes, cliente->id) != NULL && verificar_username(sistema, cliente->username, cliente->id)) 
        || (hash_buscar(&sistema->vendedores, cliente->id) != NULL && verificar_username(sistema, cliente->username, cliente->id))) {
        printf("Erro: ja existe usuario com este username.\n");
        free(cliente);
        return;
    }

    if (hash_inserir(&sistema->clientes, cliente->id, cliente)) {
        printf("Cliente cadastrado com sucesso.\n");
        fila_enfileirar(&sistema->confirmacoes_clientes, cliente->id);
    } else {
        printf("Erro ao cadastrar cliente.\n");
        free(cliente);
    }
}

/*
 * FUNÇÃO: cadastrar_vendedor
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Permite que um novo vendedor se registe no sistema
 */
void cadastrar_vendedor(Sistema *sistema) {
    Vendedor *vendedor = malloc(sizeof(Vendedor));
    if (vendedor == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }

    ler_texto("Nome: ", vendedor->nome, TAM_NOME);
    vendedor->idade = ler_inteiro("Idade: ");
    ler_texto("Telefone: ", vendedor->telefone, sizeof(vendedor->telefone));
    vendedor->disponibilidade = DISPONIVEL;

    ler_texto("Username: ", vendedor->username, TAM_USERNAME);
    vendedor->id = converter_username(vendedor->username);    
    ler_texto("Senha: ", vendedor->senha, TAM_USERNAME);

    if ((hash_buscar(&sistema->vendedores, vendedor->id) != NULL && verificar_username(sistema, vendedor->username, vendedor->id)) 
    || (hash_buscar(&sistema->clientes, vendedor->id) != NULL && verificar_username(sistema, vendedor->username, vendedor->id))) {
        printf("Erro: ja existe usuario com este username.\n");
        free(vendedor);
        return;
    }

    vendedor->ativo = DESATIVO;
    if (hash_inserir(&sistema->vendedores, vendedor->id, vendedor)) {
        printf("Vendedor cadastrado com sucesso.\n");
        fila_enfileirar(&sistema->confirmacoes_vendedor, vendedor->id);
    } else {
        printf("Erro ao cadastrar entregador.\n");
        free(vendedor);
    }
}

/*
 * FUNÇÃO: criar_encomenda
 * PARÂMETROS: Sistema *sistema, Vendedor *vendedor
 * RETORNO: void
 * DESCRIÇÃO: Cria uma nova encomenda no sistema (por vendedor)
 */
void criar_encomenda(Sistema *sistema, Vendedor *vendedor) {
    srand(time(NULL));

    Encomenda *encomenda = malloc(sizeof(Encomenda));
    if (encomenda == NULL) {
        printf("Erro: memoria insuficiente.\n");
        return;
    }

    ler_texto("Produto: ", encomenda->nome_produto, TAM_NOME);
    encomenda->preco = (double)ler_inteiro("Preco: ");

    while (true){
        encomenda->id = rand()%TAM_HASH;
        if (hash_buscar(&sistema->encomendas, encomenda->id) == NULL) {
            break;
        }
    }

    ler_texto("Descricao: ", encomenda->descricao, TAM_TEXTO);
    grafo_listar(&sistema->rotas);
    encomenda->origem = ler_inteiro("ID do local de origem: ");
    encomenda->idEntregador = vendedor->id;
    encomenda->comprado = 0;

    if (hash_inserir(&sistema->encomendas, encomenda->id, encomenda)) {
        fila_enfileirar(&sistema->pendentes, encomenda->id);
        lista_inserir(&sistema->id_encomendas, encomenda->id);
        printf("Encomenda criada e colocada na fila de pendentes.\n");
    } else {
        printf("Erro ao criar encomenda.\n");
        free(encomenda);
    }
}

/*
 * FUNÇÃO: criar_encomenda_cliente
 * PARÂMETROS: Sistema *sistema, int idCliente
 * RETORNO: void
 * DESCRIÇÃO: Cliente cria uma encomenda a partir de um produto existente
 */
void criar_encomenda_cliente(Sistema *sistema, int idCliente) {
    int id;

    if (lista_vazia(&sistema->id_encomendas)){
        printf("Sem produtos a venda.\n");
        return;
    }
    listar_produtos(&sistema->encomendas, &sistema->id_encomendas);
    id = ler_inteiro("ID da encomenda: ");

    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);
    if (!encomenda){
        printf("Erro: Nao existe encomenda com este ID.\n");
        return;
    }
    encomenda->idCliente = idCliente;
    grafo_listar(&sistema->rotas);
    encomenda->destino = ler_inteiro("ID do local de destino: ");
    encomenda->prioridade = ler_inteiro("Prioridade (1 baixa, 2 media, 3 alta): ");
    encomenda->estado = PENDENTE;
    encomenda->comprado = 1;

    if (fila_enfileirar(&sistema->pendentes, encomenda->id)) {
        printf("Encomenda criada com sucesso e enviada para a fila da empresa.\n");
    } else {
        printf("Erro ao encomendar.\n");
        free(encomenda);
    }
}

/*
 * FUNÇÃO: atribuir_entrega
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Atribui uma encomenda pendente a um entregador disponível
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
 * FUNÇÃO: atualizar_estado_entregador
 * PARÂMETROS: Sistema *sistema, int idEntregador
 * RETORNO: void
 * DESCRIÇÃO: Permite que vendedor atualize o estado de suas entregas
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
 * FUNÇÃO: consultar_rota
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Encontra a menor rota entre dois locais usando grafo
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
 * FUNÇÃO: pesquisar
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Permite pesquisar por clientes, vendedores ou encomendas
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
    Sleep(3000);
}

/*
 * FUNÇÃO: listar_produtos
 * PARÂMETROS: HashTable *encomendas, Lista *idencomendas
 * RETORNO: void
 * DESCRIÇÃO: Lista todos os produtos/encomendas disponíveis para compra
 */
void listar_produtos(HashTable *encomendas, Lista *idencomendas){
    if (!idencomendas->id_lista){
        printf("Sem produtos a venda\n");
        return;
    }
    NoLista *aux = idencomendas->id_lista;
    while (aux){
        int indice = funcao_hash(aux->id);
        NoHash *atual = encomendas->tabela[indice];
        while (atual){
            imprimir_encomenda_cliente(atual->dado);
            atual = atual->proximo;
        }
        aux = aux->prox;
    }
}

/*
 * FUNÇÃO: listar_encomendas_cliente
 * PARÂMETROS: Sistema *sistema, int idCliente
 * RETORNO: int - 1 se encontrou encomendas, 0 senão
 * DESCRIÇÃO: Lista todas as encomendas de um cliente
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
            if (encomenda->idCliente == idCliente) {
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

    Sleep(3000);
    return (1);
}

/*
 * FUNÇÃO: listar_encomendas_entregador
 * PARÂMETROS: Sistema *sistema, int idEntregador
 * RETORNO: int - 1 se encontrou encomendas, 0 senão
 * DESCRIÇÃO: Lista todas as encomendas de um vendedor/entregador
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
            if (encomenda->idEntregador == idEntregador) {
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
    
    Sleep(3000);
    return (1);
}

/*
 * FUNÇÃO: consultar_estado_cliente
 * PARÂMETROS: Sistema *sistema, int idCliente
 * RETORNO: void
 * DESCRIÇÃO: Permite cliente consultar o estado de uma sua encomenda
 */
void consultar_estado_cliente(Sistema *sistema, int idCliente) {
    int id = ler_inteiro("ID da encomenda: ");
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);

    if (encomenda == NULL || encomenda->idCliente != idCliente) {
        printf("Encomenda nao encontrada para este cliente.\n");
        return;
    }

    imprimir_encomenda(encomenda);
    Sleep(3000);
}

/*
 * FUNÇÃO: cancelar_encomenda_cliente
 * PARÂMETROS: Sistema *sistema, int idCliente
 * RETORNO: void
 * DESCRIÇÃO: Permite cliente cancelar uma encomenda pendente
 */
void cancelar_encomenda_cliente(Sistema *sistema, int idCliente) {

    if (!listar_encomendas_cliente(sistema, idCliente)) return;

    int id = ler_inteiro("ID da encomenda: ");
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);

    if (encomenda == NULL || encomenda->idCliente != idCliente) {
        printf("Encomenda nao encontrada para este cliente.\n");
        return;
    }

    if (encomenda->estado != PENDENTE) {
        printf("Apenas encomendas pendentes podem ser canceladas pelo cliente.\n");
        return;
    }

    encomenda->estado = CANCELADA;
    printf("Encomenda cancelada com sucesso.\n");
}

/*
 * FUNÇÃO: consultar_rota_encomenda_entregador
 * PARÂMETROS: Sistema *sistema, int idEntregador
 * RETORNO: void
 * DESCRIÇÃO: Permite entregador ver a rota de uma sua encomenda
 */
void consultar_rota_encomenda_entregador(Sistema *sistema, int idEntregador) {
    int id = ler_inteiro("ID da encomenda: ");
    int caminho[MAX_LOCAIS];
    int tamanho = 0;
    int distancia;
    int i;
    Encomenda *encomenda = hash_buscar(&sistema->encomendas, id);

    if (encomenda == NULL || encomenda->idEntregador != idEntregador) {
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
 * FUNÇÃO: alterar_disponibilidade_entregador
 * PARÂMETROS: Sistema *sistema, int idEntregador
 * RETORNO: void
 * DESCRIÇÃO: Permite entregador mudar seu status (disponível/ocupado)
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
 * FUNÇÃO: listar_dados
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Menu para admin listar clientes, vendedores, encomendas, rotas
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
 * FUNÇÃO: confirmado_cliente
 * PARÂMETROS: HashTable *hash, Fila *confirmacoes
 * RETORNO: void
 * DESCRIÇÃO: Admin aprova/rejeita registos de clientes pendentes
 */
void confirmado_cliente(HashTable *hash, Fila *confirmacoes){
    NoFila *aux = confirmacoes->inicio;
    if (!aux){
        printf("Sem contas por verificar.\n");
        return;
    }
    while (aux){
        int indice = funcao_hash(aux->id);
        NoHash *atual = hash->tabela[indice];
        while (atual){
            Cliente *cliente = atual->dado;
            if (cliente->ativo == DESATIVO){
                system("cls");
                imprimir_cliente((void *)cliente);
                printf("\nDeseja validar cadastro?\n1. SIM - 2. NAO - 3 - SAIR\n");
                ll t = ler_inteiro("Opcao: ");
                system("cls");
                if (t == 1){
                    cliente->ativo = ATIVO;
                    fila_desenfileirar(confirmacoes, &aux->id);
                    printf("Conta \"%s\" confirmada.\n", cliente->username);
                    Sleep(2000);
                }else if (t == 2){
                    cliente->ativo = -1;
                    fila_desenfileirar(confirmacoes, &aux->id);
                    printf("Conta \"%s\" negada.\n", cliente->username);
                    Sleep(2000);
                }
                else if (t == 3){
                    return;
                }
                else{
                    printf("Opcao invalida.\n");
                    Sleep(2000);
                }
            }
            atual = atual->proximo;
        }
        aux = aux->proximo;
    }
}

/*
 * FUNÇÃO: confirmado_vendedor
 * PARÂMETROS: HashTable *hash, Fila *confirmacoes
 * RETORNO: void
 * DESCRIÇÃO: Admin aprova/rejeita registos de vendedores pendentes
 */
void confirmado_vendedor(HashTable *hash, Fila *confirmacoes){
    NoFila *aux = confirmacoes->inicio;
    if (!aux){
        printf("Sem contas por verificar.\n");
        return;
    }
    while (aux){
        int indice = funcao_hash(aux->id);
        NoHash *atual = hash->tabela[indice];
        while (atual){
            Vendedor *vendedor = atual->dado;
            if (vendedor->ativo == DESATIVO){
                system("cls");
                imprimir_entregador((void *)vendedor);
                printf("\nDeseja validar cadastro?\n1. SIM - 2. NAO - 3. SAIR\n");
                ll t = ler_inteiro("Opcao: ");
                system("cls");
                if (t == 1){
                    vendedor->ativo = ATIVO;
                    fila_desenfileirar(confirmacoes, &aux->id);
                    printf("Conta \"%s\" confirmada.\n", vendedor->username);
                    Sleep(2000);
                }else if (t == 2){
                    vendedor->ativo = -1;
                    fila_desenfileirar(confirmacoes, &aux->id);
                    printf("Conta \"%s\" negado.\n", vendedor->username);
                    Sleep(2000);
                }
                else if (t == 3){
                    free(vendedor);
                    return;
                }
                else{
                    printf("Opcao invalida.\n");
                    Sleep(2000);
                }
            }
            atual = atual->proximo;
        }
        aux = aux->proximo;
    }
}

/*
 * FUNÇÃO: registar_rotas
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Admin adiciona novos locais ou rotas ao sistema
 */
void registar_rotas(Sistema *sistema){
    ll opcao;
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
            char endereco[TAM_TEXTO];
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
                        Sleep(3000);
                    }
                    else{
                        while (true){
                            system("cls");
                            int distancia = ler_inteiro("Informe a distancia a esse ponto: ");
                            if (distancia <= 0){
                                printf("Distancia invalida.\n");
                                Sleep(3000);
                            }
                            else{
                                int local = grafo_adicionar_local(rotas, endereco);
                                grafo_adicionar_rota(rotas, local, ponto, distancia);
                                printf("Rota criada com sucesso.\n");
                                Sleep(3000);
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
        case 2:
            for (int i = 0; i < rotas->total; i++) {
                printf("[%d] %s\n", rotas->locais[i].id, rotas->locais[i].nome);
            }
            int origem = ler_inteiro("Origem: ");
            int destino = ler_inteiro("Destino: ");
            while (true){
                int distancia = ler_inteiro("Informe a distancia: ");
                if (distancia <= 0){
                    system("cls");
                    printf("Distancia invalida.\n");
                    Sleep(3000);
                }
                else{
                    grafo_adicionar_rota(rotas, origem, destino, distancia);
                    printf("Rota criada com sucesso.\n");
                    Sleep(3000);
                    break;
                }
            }
            break;
        case 0:
            printf("Voltando...\n");
            break;
        default:
            printf("Opcao invalida.\n");
        
    }

}

/*
 * FUNÇÃO: menu_administrador
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Menu principal do administrador com opções de gestão
 */
void menu_administrador(Sistema *sistema){
    ll opcao;

    do {
        Sleep(2000);
        system("cls");
        printf("===== ADMINISTRADOR / EMPRESA =====\n");
        printf("1. Confirmar Cadastro do cliente\n");
        printf("2. Confirmar Cadastro do vendedor\n");
        printf("3. Registar rotas\n");
        printf("4. Pesquisar por ID\n");
        printf("5. Listar dados\n");
        printf("6. Consultar menor rota\n");
        printf("7. Guardar dados em ficheiros\n");
        printf("8. Carregar dados dos ficheiros\n");
        printf("0. Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        system("cls");

        switch (opcao) {
            case 1:
                confirmado_cliente(&sistema->clientes, &sistema->confirmacoes_clientes);
                break;
            case 2:
                confirmado_vendedor(&sistema->vendedores, &sistema->confirmacoes_vendedor);
                break;
            case 3:
                registar_rotas(sistema);
                break;
            case 4:
                pesquisar(sistema);
                break;
            case 5:
                listar_dados(sistema);
                break;
            case 6:
                consultar_rota(sistema);
                Sleep(3000);                
                break;
            case 7:
                if (ficheiro_guardar_dados(sistema)) {
                    printf("Dados guardados com sucesso.\n");
                } else {
                    printf("Erro ao guardar dados.\n");
                }
                break;
            case 8:
                if (ficheiro_carregar_dados(sistema)) {
                    printf("Dados carregados com sucesso.\n");
                } else {
                    printf("Erro ao carregar dados.\n");
                }
                break;
            case 0:
                printf("A voltar a tela de login...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/*
 * FUNÇÃO: menu_cliente
 * PARÂMETROS: Sistema *sistema, Cliente *cliente
 * RETORNO: void
 * DESCRIÇÃO: Menu principal do cliente com suas opções
 */
void menu_cliente(Sistema *sistema, Cliente *cliente) {
    int opcao;
    ll idCliente = converter_username(cliente->username);;

    do {
        Sleep(2000);
        system("cls");
        printf("===== AREA DO CLIENTE: %s =====\n", cliente->nome);
        printf("1. Fazer encomenda\n");
        printf("2. Consultar estado de uma encomenda\n");
        printf("3. Listar minhas encomendas\n");
        printf("4. Cancelar encomenda pendente\n");
        printf("0. Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        system("cls");
        switch (opcao) {
            case 1:
                criar_encomenda_cliente(sistema, idCliente);
                break;
            case 2:
                consultar_estado_cliente(sistema, idCliente);
                break;
            case 3:
                listar_encomendas_cliente(sistema, idCliente);
                break;
            case 4:
                cancelar_encomenda_cliente(sistema, idCliente);
                break;
            case 0:
                printf("A voltar a tela de login...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/*
 * FUNÇÃO: menu_vendedor
 * PARÂMETROS: Sistema *sistema, Vendedor *vendedor
 * RETORNO: void
 * DESCRIÇÃO: Menu principal do vendedor com suas opções
 */
void menu_vendedor(Sistema *sistema, Vendedor *vendedor) {
    int opcao;
    ll idvendedor = converter_username(vendedor->username);

    do {
        Sleep(2000);
        system("cls");
        printf("===== AREA DO VENDEDOR: %s =====\n", vendedor->nome);
        printf("1. Criar produto\n");
        printf("2. Ver minhas entregas\n");
        printf("3. Atualizar estado da entrega\n");
        printf("4. Consultar rota de uma entrega\n");
        printf("5. Alterar disponibilidade\n");
        printf("0. Voltar\n");
        opcao = ler_inteiro("Opcao: ");
        system("cls");
        switch (opcao) {
            case 1:
                criar_encomenda(sistema, vendedor);
                break;
            case 2:
                listar_encomendas_entregador(sistema, idvendedor);
                break;
            case 3:
                atualizar_estado_entregador(sistema, idvendedor);
                break;
            case 4:
                consultar_rota_encomenda_entregador(sistema, idvendedor);
                Sleep(3000);
                break;
            case 5:
                alterar_disponibilidade_entregador(sistema, idvendedor);
                break;
            case 0:
                printf("A voltar a tela de login...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
}

/*
 * FUNÇÃO: confirmado_cliente2
 * PARÂMETROS: HashTable *hash, int chave
 * RETORNO: int - 1=ATIVO, 0=DESATIVO, -1=NEGADO
 * DESCRIÇÃO: Verifica o estado de aprovação de um cliente (para login)
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
 * FUNÇÃO: confirmado_vendedor2
 * PARÂMETROS: HashTable *hash, int chave
 * RETORNO: int - 1=ATIVO, 0=DESATIVO, -1=NEGADO
 * DESCRIÇÃO: Verifica o estado de aprovação de um vendedor (para login)
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
 * FUNÇÃO: sistem_criarConta
 * PARÂMETRO: Sistema *sistema
 * RETORNO: int - opção escolhida (1=criar, 2=login, 3=sair)
 * DESCRIÇÃO: Menu principal do sistema antes de login
 *   EASTER EGG: Opção 1800 = login de admin
 */
int sistem_criarConta(Sistema *sistema){
    ll opcao, opcao2;
    while (true){
        system("cls");
        printf("1. Criar Conta\n2. Login\n3. Sair\n");
        opcao = ler_inteiro("Opcao: ");
        system("cls");
        if (opcao == 1){
            while (true){
                printf("1. CLiente\n2. Vendedor\n3. Voltar\n");
                opcao2 = ler_inteiro("Opcao: ");
                if (opcao2 == 1){
                    system("cls");                    
                    cadastrar_cliente(sistema);
                    Sleep(2000);
                    break;
                }
                else if (opcao2 == 2){
                    system("cls");                    
                    cadastrar_vendedor(sistema);
                    Sleep(2000);
                    break;
                }
                else if (opcao2 == 3) break;
                else{
                    printf("Opcao invalida.\n");
                    Sleep(2000);
                }
            }
        }
        else if (opcao == 2) break;
        else if (opcao == 3) break;
        else if (opcao == 1800) login_adm(sistema);
        else {
            printf("Opcao invalida.\n");
            Sleep(2000);            
        }
    }
    return ((int)opcao);
}

/*
 * FUNÇÃO: login_adm
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Autentica um administrador e acessa menu_administrador
 */
void login_adm(Sistema *sistema){
    Admin *ADMIN = &sistema->admin;
    char name[TAM_NOME], senha[TAM_NOME];
    ler_texto("Nome: ", name, TAM_NOME);
    ler_texto("Senha: ", senha, TAM_NOME);
    system("cls");
    if (strcmp(ADMIN->nome, name) == 0 && strcmp(ADMIN->senha, senha) == 0){
        printf("Acesso Concedido.\n");
        menu_administrador(sistema);
        return;
    }
    else {
        printf("Acesso Negado.\n");
        Sleep(2000);
    }
}

/*
 * FUNÇÃO: sistema_login
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Autentica clientes ou vendedores no sistema
 *   PROCESSO:
 *     1. Pede username e senha
 *     2. Tenta login como vendedor
 *     3. Se falhar, tenta login como cliente
 *     4. Verifica se conta está aprovada
 *     5. Se aprovada, acessa o menu respectivo
 */
void sistema_login(Sistema *sistema){
    int ID_geral;
    char username[TAM_USERNAME];
    char senha[TAM_USERNAME];
    Vendedor *vendedor;
    
    ler_texto("Username: ", username, TAM_USERNAME);
    ler_texto("Senha: ", senha, TAM_USERNAME);
    system("cls");   
    ID_geral = converter_username(username);
    vendedor = hash_buscar(&sistema->vendedores, ID_geral);
    if (vendedor && verificar_user_vendedor(&sistema->vendedores, username, ID_geral)){
        if (strcmp(vendedor->senha, senha) == 0){
            int result = confirmado_vendedor2(&sistema->vendedores, ID_geral);
            if (result == 1){
                printf("Acesso concedido.\n");
                menu_vendedor(sistema, vendedor);
                return;
            }
            else if (result == 0){
                printf("Conta nao confirmada.\n");
            }
            else if (result == -1){
                printf("O acesso foi negado.\n");
                return;
            }
        }
        else{
            printf("Usuario ou senha incorreta.\n");
        }
    }
    Cliente *cliente = hash_buscar(&sistema->clientes, ID_geral);
    if (cliente && verificar_user_cliente(&sistema->clientes, username, ID_geral)){
        if (strcmp(cliente->senha, senha) == 0){
            int result = confirmado_cliente2(&sistema->clientes, ID_geral);
            if (result == 1){
                printf("Acesso concedido.\n");
                menu_cliente(sistema, cliente);
                return;
            }
            else if (result == 0) {
                printf("Conta nao confirmada.\n");
            }
            else if (result == -1){
                printf("O acesso foi negado.\n");
            }
        }
        else{
            printf("Usuario ou senha incorreta.\n");
        }
    }
    else if (!vendedor && !cliente){
        printf("Usuario nao encontrado.\n");
        return;
    }
}

/*
 * FUNÇÃO: sistema_liberar
 * PARÂMETRO: Sistema *sistema
 * RETORNO: void
 * DESCRIÇÃO: Liberta toda a memória alocada dinamicamente
 *            Deve ser chamada antes de encerrar o programa
 */
void sistema_liberar(Sistema *sistema) {
    hash_liberar(&sistema->clientes);
    hash_liberar(&sistema->vendedores);
    hash_liberar(&sistema->encomendas);
    fila_liberar(&sistema->pendentes);
    grafo_liberar(&sistema->rotas);
}
