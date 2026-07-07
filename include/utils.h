#ifndef UTILS_H
#define UTILS_H

/* Calcula o comprimento (numero de caracteres) de uma string */
/* Equivalente a funcao strlen da biblioteca padrao */
int ft_strlen(char *str);

/* Converte todos os caracteres de uma string para minusculas */
/* Retorna a string convertida */
char *ft_tolower(char *str);

/* Verifica se uma string contem apenas digitos numericos (0-9) */
/* Retorna 1 se string e numerica, 0 caso contrario */
int is_numeric(char *str);

/* Limpa o buffer de entrada (stdin) removendo caracteres pendentes */
/* Util para evitar problemas com input incorretos */
void limpar_buffer(void);

/* Le um texto da entrada do utilizador e armazena num destino com limite de tamanho */
/* Exibe uma mensagem de prompt antes de ler */
void ler_texto(const char *mensagem, char *destino, int tamanho);

/* Le um numero inteiro da entrada do utilizador */
/* Exibe uma mensagem de prompt e valida que input e numerico */
int ler_inteiro(const char *mensagem);

/* Pausa a execucao do programa ate que o utilizador pressione uma tecla */
/* Util para criar pausas entre menus */
void pausar(void);

/* Converte um codigo numerico de estado de encomenda para texto descritivo */
/* Retorna string com o nome do estado (ex: "Pendente", "Em Transito", "Entregue") */
const char *estado_para_texto(int estado);

#endif
