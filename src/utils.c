/* ==============================================================
   FICHEIRO: utils.c
   DESCRICAO: Funcoes utilitarias para manipulacao de strings,
              leitura de entrada, e conversao de dados
   ============================================================== */

#include <stdio.h>
#include <string.h>
#include "utils.h"

/*
 * FUNCAO: ft_strlen
 * PARAMETRO: char *str - string a medir
 * RETORNO: int - comprimento da string
 * DESCRICAO: Calcula o comprimento de uma string
 *            (implementacao manual, sem usar strlen padrao)
 */
int ft_strlen(char *str){
    int i = -1;
    while (str[++i]);      /* Incrementa i ate encontrar '\0' */
    return (i);            /* Retorna o comprimento */
}

/*
 * FUNCAO: ft_tolower
 * PARAMETRO: char *str - string a converter
 * RETORNO: char* - a mesma string em minusculas
 * DESCRICAO: Converte todos os caracteres de uma string
 *            de maiusculas (ASCII 65-90) para minusculas (ASCII 97-122)
 */
char *ft_tolower(char *str){
    int i = 0;
    while (str[i]){
        /* Verifica se e letra maiuscula (ASCII 65-90) */
        if (str[i] >= 65 && str[i] <= 90)
            str[i] = str[i] + 32;  /* Converte para minuscula */
        i++;
    }
    return (str);
}

/*
 * FUNCAO: is_numeric
 * PARAMETRO: char *str - string a validar
 * RETORNO: int - 1 se e numerica, 0 se nao e
 * DESCRICAO: Verifica se uma string contem apenas digitos numericos (0-9)
 * NOTA: Ha um BUG nesta funcao - falta fechar parentesis: str[i > 57]
 */
int is_numeric(char *str){
    int     i = 0;
    while (str[i]){
        /* Verifica se caractere NAO e digito (ASCII 48-57 sao '0'-'9') */
        if (str[i] < 48 || str[i] > 57)
            return (0);                      /* Retorna 0 se nao e numerico */
        i++;
    }
    return (1);                              /* Retorna 1 se todos sao numericos */
}

/*
 * FUNCAO: limpar_buffer
 * RETORNO: void
 * DESCRICAO: Limpa o buffer de entrada standard (stdin)
 *            Util apos usar scanf para remover '\n' deixado para tras
 */
void limpar_buffer(void) {
    int c;
    /* Le caracteres ate encontrar newline ou fim de ficheiro */
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/*
 * FUNCAO: ler_texto
 * PARAMETROS: 
 *   - const char *mensagem: texto a exibir antes de ler
 *   - char *destino: endereco onde guardar a string lida
 *   - int tamanho: tamanho maximo a ler (protecao contra overflow)
 * RETORNO: void
 * DESCRICAO: Le uma linha de texto do utilizador com seguranca
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
 * FUNCAO: ler_inteiro
 * PARAMETRO: const char *mensagem - texto a exibir antes de ler
 * RETORNO: int - valor inteiro lido do utilizador
 * DESCRICAO: Le um inteiro do utilizador com validacao
 *            Se a entrada for invalida, pede novamente (recursao)
 */
int ler_inteiro(const char *mensagem) {
    int valor;
    int lido;

    printf("%s", mensagem);                  /* Exibe a mensagem de prompt */
    lido = scanf("%d", &valor);              /* Tenta ler um inteiro */
    limpar_buffer();                         /* Limpa caracteres restantes do buffer */

    /* Se scanf nao leu um inteiro com sucesso */
    if (lido != 1) {
        printf("Entrada invalida. Tente novamente.\n");
        /* Chama-se recursivamente ate obter entrada valida */
        return ler_inteiro(mensagem);
    }

    return valor;                            /* Retorna o inteiro lido */
}

/*
 * FUNCAO: pausar
 * RETORNO: void
 * DESCRICAO: Pausa a execucao ate o utilizador pressionar ENTER
 *            Usado para dar tempo ao utilizador ler mensagens
 */
void pausar(void) {
    printf("\nPrima ENTER para continuar...");
    getchar();                               /* Aguarda uma tecla */
}

/*
 * FUNCAO: estado_para_texto
 * PARAMETRO: int estado - codigo numerico do estado
 * RETORNO: const char* - string descritiva do estado
 * DESCRICAO: Converte o codigo numerico de estado de encomenda
 *            em texto legivel para exibicao ao utilizador
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
            return "DESCONHECIDO";           /* Se codigo invalido */
    }
}
