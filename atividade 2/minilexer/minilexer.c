#include <stdio.h>
#include <ctype.h>
#include <string.h>

//o enum nesse caso serve para enumerar os valores de cada token, para que seja mais fácil de identificar (Ex: 0 = token palavra reservada)
//e definir quais são os tipos de tokens que existem e o lexer vai reconhecer.
typedef enum { 
    TOKEN_PALAVRA_RESERVADA,
    TOKEN_IDENTIFICADOR,
    TOKEN_NUMERO_INTEIRO,
    TOKEN_NUMERO_REAL,
    TOKEN_LITERAL_CARACTERE,
    TOKEN_OPERADOR,
    TOKEN_DELIMITADOR,
    TOKEN_ERRO
} TipoToken;

typedef struct { //struct para agrupar as informações que queremos de cada token.
    TipoToken tipo; //armazena o tipo do token, que é definido pelo enum TipoToken
    char lexema[64];
    int linha;
    int coluna;
} Token;

int eh_palavra_reservada(char lexema[])
{
    if (strcmp(lexema, "int") == 0 ||
        strcmp(lexema, "float") == 0 ||
        strcmp(lexema, "char") == 0 ||
        strcmp(lexema, "if") == 0 ||
        strcmp(lexema, "else") == 0 ||
        strcmp(lexema, "while") == 0 ||
        strcmp(lexema, "return") == 0)
    {
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) //argc quantidade de argumentos e o argv os argumentos passados
{
    FILE *arquivo;

    if (argc != 2) 
    {
        printf("Uso correto: minilexer <arquivo-fonte>\n");
        return 1;
    }

    arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) 
    {
        printf("Erro: não foi possível abrir o arquivo, conteudo nulo!\n");
        return 1;
    }

    int caractere;
    int linha = 1;
    int coluna = 1;

    while ((caractere = fgetc(arquivo)) != EOF) // enquanto não chegar no final do arquivo (EOF) ele continua
    {
        if (caractere == '\n')
        {
            linha++;
            coluna = 1;
        }
        else if (caractere == ' ' || caractere == '\t') // se o caractere for espaço ou tabulação, apenas incrementa a coluna
        {
            coluna++;
        }
        else
        {
           if (isalpha(caractere) || caractere == '_')
            {
                char lexema[64];
                int tamanho = 0;

                lexema[tamanho] = caractere;
                tamanho++;

                // enquanto o próximo caractere for letra, número ou _, continua adicionando ao lexema
                while ((caractere = fgetc(arquivo)) != EOF &&
                    (isalpha(caractere) || isdigit(caractere) || caractere == '_'))
                {
                    if (tamanho < 63)
                    {
                        lexema[tamanho] = caractere;
                        tamanho++;
                    }
                }

                if (caractere != EOF)
                {
                    ungetc(caractere, arquivo); // se não fizer parte do identificador, coloca o caractere de volta para ser lido na próxima volta
                }

                lexema[tamanho] = '\0';

                // verifica se o lexema encontrado é uma palavra reservada
                if (eh_palavra_reservada(lexema))
                {
                    printf("Palavra reservada: %s\n", lexema);
                }
                else
                {
                    printf("Identificador: %s\n", lexema);
                }
            }

            else if (isdigit(caractere))
            {
                char lexema[64];
                int tamanho = 0;
                int numero_real = 0;

                lexema[tamanho] = caractere;
                tamanho++;

                // enquanto o próximo caractere for um número, continua adicionando ao lexema
                while ((caractere = fgetc(arquivo)) != EOF && isdigit(caractere))
                {
                    lexema[tamanho] = caractere;
                    tamanho++;
                }

                // se encontrar um ponto, verifica se o número pode ser real
                if (caractere == '.')
                {
                    numero_real = 1;
                    lexema[tamanho] = caractere;
                    tamanho++;

                    while ((caractere = fgetc(arquivo)) != EOF && isdigit(caractere))
                    {
                        lexema[tamanho] = caractere;
                        tamanho++;
                    }
                }

                if (caractere != EOF)
                {
                    // se encontrar um caractere que não faz parte do número, devolve para ser lido depois
                    ungetc(caractere, arquivo);
                }

                lexema[tamanho] = '\0';

                if (numero_real)
                {
                    printf("Numero real: %s\n", lexema);
                }
                else
                {
                    printf("Numero inteiro: %s\n", lexema);
                }
            }

            else if (caractere == '=')
            {
                // lê o próximo caractere para verificar se temos o operador ==
                int proximo = fgetc(arquivo);

                if (proximo == '=')
                {
                    printf("Operador: ==\n");
                }
                else
                {
                    printf("Operador: =\n");

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '!')
            {
                // lê o próximo caractere para verificar se temos o operador !=
                int proximo = fgetc(arquivo);

                if (proximo == '=')
                {
                    printf("Operador: !=\n");
                }
                else
                {
                    printf("Operador: !\n");

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '<')
            {
                // lê o próximo caractere para verificar se temos o operador <=
                int proximo = fgetc(arquivo);

                if (proximo == '=')
                {
                    printf("Operador: <=\n");
                }
                else
                {
                    printf("Operador: <\n");

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '>')
            {
                // lê o próximo caractere para verificar se temos o operador >=
                int proximo = fgetc(arquivo);

                if (proximo == '=')
                {
                    printf("Operador: >=\n");
                }
                else
                {
                    printf("Operador: >\n");

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '&')
            {
                // lê o próximo caractere para verificar se temos o operador &&
                int proximo = fgetc(arquivo);

                if (proximo == '&')
                {
                    printf("Operador: &&\n");
                }
                else
                {
                    // como & sozinho não forma um operador válido, mostra como outro caractere
                    printf("Outro caractere: &\n");

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '|')
            {
                // lê o próximo caractere para verificar se temos o operador ||
                int proximo = fgetc(arquivo);

                if (proximo == '|')
                {
                    printf("Operador: ||\n");
                }
                else
                {
                    // como | sozinho não forma um operador válido, mostra como outro caractere
                    printf("Outro caractere: |\n");

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '+' || caractere == '-' || caractere == '*' || caractere == '/')
            {
                printf("Operador: %c\n", caractere);
            }

            else if (caractere == '(' || caractere == ')' || caractere == '{' || caractere == '}' || caractere == '[' || caractere == ']' || caractere == ';' || caractere == ',')
            {
                printf("Delimitador: %c\n", caractere);
            }
            else
            {
                printf("Outro caractere: %c\n", caractere);
            }
        }
    }

    printf("\n");

    fclose(arquivo);

    return 0;
}