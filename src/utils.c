/* ==============================================================
   FICHEIRO: utils.c
   DESCRIÇÃO: Funções utilitárias para manipulação de strings,
              leitura de entrada, e conversão de dados
   ============================================================== */

#include <stdio.h>
#include <string.h>
#include "utils.h"

/*
 * FUNÇÃO: ft_strlen
 * PARÂMETRO: char *str - string a medir
 * RETORNO: int - comprimento da string
 * DESCRIÇÃO: Calcula o comprimento de uma string
 *            (implementação manual, sem usar strlen padrão)
 */
int ft_strlen(char *str){
    int i = -1;
    while (str[++i]);      /* Incrementa i até encontrar '\0' */
    return (i);            /* Retorna o comprimento */
}

/*
 * FUNÇÃO: ft_tolower
 * PARÂMETRO: char *str - string a converter
 * RETORNO: char* - a mesma string em minúsculas
 * DESCRIÇÃO: Converte todos os caracteres de uma string
 *            de maiúsculas (ASCII 65-90) para minúsculas (ASCII 97-122)
 */
char *ft_tolower(char *str){
    int i = 0;
    while (str[i]){
        /* Verifica se é letra maiúscula (ASCII 65-90) */
        if (str[i] >= 65 && str[i] <= 90)
            str[i] = str[i] + 32;  /* Converte para minúscula */
        i++;
    }
    return (str);
}

/*
 * FUNÇÃO: is_numeric
 * PARÂMETRO: char *str - string a validar
 * RETORNO: int - 1 se é numérica, 0 se não é
 * DESCRIÇÃO: Verifica se uma string contém apenas dígitos numéricos (0-9)
 * NOTA: Há um BUG nesta função - falta fechar parêntesis: str[i > 57]
 */
int is_numeric(char *str){
    int     i = 0;
    while (str[i]){
        /* Verifica se caractere NÃO é dígito (ASCII 48-57 são '0'-'9') */
        if (str[i] < 48 || str[i > 57])     /* BUG: str[i > 57] deveria ser str[i] > 57 */
            return (0);                      /* Retorna 0 se não é numérico */
        i++;
    }
    return (1);                              /* Retorna 1 se todos são numéricos */
}

/*
 * FUNÇÃO: limpar_buffer
 * RETORNO: void
 * DESCRIÇÃO: Limpa o buffer de entrada standard (stdin)
 *            Útil após usar scanf para remover '\n' deixado para trás
 */
void limpar_buffer(void) {
    int c;
    /* Lê caracteres até encontrar newline ou fim de ficheiro */
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/*
 * FUNÇÃO: ler_texto
 * PARÂMETROS: 
 *   - const char *mensagem: texto a exibir antes de ler
 *   - char *destino: endereço onde guardar a string lida
 *   - int tamanho: tamanho máximo a ler (proteção contra overflow)
 * RETORNO: void
 * DESCRIÇÃO: Lê uma linha de texto do utilizador com segurança
 *            Remove automaticamente o '\n' do final
 */
void ler_texto(const char *mensagem, char *destino, int tamanho) {
    printf("%s", mensagem);                  /* Exibe a mensagem de prompt */
    
    if (fgets(destino, tamanho, stdin) != NULL) {
        /* Substitui o '\n' por '\0' (terminador de string) */
        destino[strcspn(destino, "\n")] = '\0';
    }
}

/*
 * FUNÇÃO: ler_inteiro
 * PARÂMETRO: const char *mensagem - texto a exibir antes de ler
 * RETORNO: int - valor inteiro lido do utilizador
 * DESCRIÇÃO: Lê um inteiro do utilizador com validação
 *            Se a entrada for inválida, pede novamente (recursão)
 */
int ler_inteiro(const char *mensagem) {
    int valor;
    int lido;

    printf("%s", mensagem);                  /* Exibe a mensagem de prompt */
    lido = scanf("%d", &valor);              /* Tenta ler um inteiro */
    limpar_buffer();                         /* Limpa caracteres restantes do buffer */

    /* Se scanf não leu um inteiro com sucesso */
    if (lido != 1) {
        printf("Entrada invalida. Tente novamente.\n");
        /* Chama-se recursivamente até obter entrada válida */
        return ler_inteiro(mensagem);
    }

    return valor;                            /* Retorna o inteiro lido */
}

/*
 * FUNÇÃO: pausar
 * RETORNO: void
 * DESCRIÇÃO: Pausa a execução até o utilizador pressionar ENTER
 *            Usado para dar tempo ao utilizador ler mensagens
 */
void pausar(void) {
    printf("\nPrima ENTER para continuar...");
    getchar();                               /* Aguarda uma tecla */
}

/*
 * FUNÇÃO: estado_para_texto
 * PARÂMETRO: int estado - código numérico do estado
 * RETORNO: const char* - string descritiva do estado
 * DESCRIÇÃO: Converte o código numérico de estado de encomenda
 *            em texto legível para exibição ao utilizador
 * MAPEAMENTO:
 *   0 = PENDENTE (aguardando processamento)
 *   1 = EM_TRANSITO (a caminho)
 *   2 = ENTREGUE (chegou ao destino)
 *   3 = CANCELADA (foi cancelada)
 */
const char *estado_para_texto(int estado) {
    switch (estado) {
        case 0:
            return "PENDENTE";
        case 1:
            return "EM_TRANSITO";
        case 2:
            return "ENTREGUE";
        case 3:
            return "CANCELADA";
        default:
            return "DESCONHECIDO";           /* Se código inválido */
    }
}
