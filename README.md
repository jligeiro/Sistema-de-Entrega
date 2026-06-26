# Sistema de Entrega

Projeto de Estrutura de Dados II implementado em C.

## Estruturas usadas

- Grafo: representa os locais e as rotas entre pontos de entrega.
- Hash table: permite pesquisar clientes, entregadores e encomendas por ID.
- Fila: gere as encomendas pendentes por ordem de chegada.
- Lista ligada: usada nas colisões da hash table e na fila, para armazenar o id de todos os produtos disponiveis para encomenda.

## Perfis do sistema

O programa funciona pelo terminal e possui tres areas:

- Administrador / empresa: confirma cadastro de clientes, entregadores, consulta rotas, pesquisa, lista, guarda e carrega dados.
- Cliente: faz encomendas, consulta estado, lista as suas encomendas e cancela encomendas pendentes.
- Vendedor: cria produtos, ve entregas disponiveis, atualiza estado, consulta rota da entrega e altera disponibilidade.

## Compilacao

```bash
gcc src/*.c -Iinclude -Wall -Wextra -o sistema_entrega
```

## Execucao

```bash
./sistema_entrega
```

## Ficheiros gerados

Ao escolher a opcao de guardar, o programa cria:

- `clientes.txt`
- `entregadores.txt`
- `encomendas.txt`
- `rotas.txt`

Att:
    para acessar a area do administrador/empresa disque `1800` na pagina de login
    (Nome: admin)
    (Senha: admin123)
