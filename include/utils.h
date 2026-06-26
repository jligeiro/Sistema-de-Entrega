#ifndef UTILS_H
#define UTILS_H

/* Calcula o comprimento (número de caracteres) de uma string */
/* Equivalente a função strlen da biblioteca padrão */
int ft_strlen(char *str);

/* Converte todos os caracteres de uma string para minúsculas */
/* Retorna a string convertida */
char *ft_tolower(char *str);

/* Verifica se uma string contém apenas dígitos numéricos (0-9) */
/* Retorna 1 se string é numérica, 0 caso contrário */
int is_numeric(char *str);

/* Limpa o buffer de entrada (stdin) removendo caracteres pendentes */
/* Útil para evitar problemas com input incorretos */
void limpar_buffer(void);

/* Lê um texto da entrada do utilizador e armazena num destino com limite de tamanho */
/* Exibe uma mensagem de prompt antes de ler */
void ler_texto(const char *mensagem, char *destino, int tamanho);

/* Lê um número inteiro da entrada do utilizador */
/* Exibe uma mensagem de prompt e valida que input é numérico */
int ler_inteiro(const char *mensagem);

/* Pausa a execução do programa até que o utilizador pressione uma tecla */
/* Util para criar pausas entre menus */
void pausar(void);

/* Converte um código numérico de estado de encomenda para texto descritivo */
/* Retorna string com o nome do estado (ex: "Pendente", "Em Trânsito", "Entregue") */
const char *estado_para_texto(int estado);

#endif
