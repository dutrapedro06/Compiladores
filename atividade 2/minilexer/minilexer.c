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

void imprimir_token(Token token, int *total_tokens, int *total_erros)
{
    // toda vez que um token é reconhecido, aumenta o total de tokens
    (*total_tokens)++;

    // se o token for do tipo erro, também aumenta o total de erros léxicos
    if (token.tipo == TOKEN_ERRO)
    {
        (*total_erros)++;
    }

    printf("Tipo: %d\n", token.tipo);
    printf("Lexema: %s\n", token.lexema);
    printf("Linha: %d\n", token.linha);
    printf("Coluna: %d\n", token.coluna);
    printf("\n");
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
    int total_tokens = 0;
    int total_erros = 0;

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
                int coluna_inicial = coluna;

                lexema[tamanho] = caractere;
                tamanho++;
                coluna++;

                // enquanto o próximo caractere for letra, número ou _, continua adicionando ao lexema
                while ((caractere = fgetc(arquivo)) != EOF && (isalpha(caractere) || isdigit(caractere) || caractere == '_'))
                {
                    if (tamanho < 63)
                    {
                        lexema[tamanho] = caractere;
                        tamanho++;
                    }

                    coluna++;
                }

                if (caractere != EOF)
                {
                    ungetc(caractere, arquivo); // se não fizer parte do identificador, coloca o caractere de volta para ser lido na próxima volta
                }

                lexema[tamanho] = '\0';

                Token token;

                token.linha = linha;
                token.coluna = coluna_inicial;

                strcpy(token.lexema, lexema);

                // verifica se o lexema encontrado é uma palavra reservada
                if (eh_palavra_reservada(lexema))
                {
                    token.tipo = TOKEN_PALAVRA_RESERVADA;
                }
                else
                {
                    token.tipo = TOKEN_IDENTIFICADOR;
                }

                imprimir_token(token, &total_tokens, &total_erros);
            }

            else if (isdigit(caractere))
            {
                char lexema[64];
                int tamanho = 0;
                int numero_real = 0;
                int coluna_inicial = coluna;

                lexema[tamanho] = caractere;
                tamanho++;
                coluna++;

                // enquanto o próximo caractere for um número, continua adicionando ao lexema
                while ((caractere = fgetc(arquivo)) != EOF && isdigit(caractere))
                {
                    if (tamanho < 63)
                    {
                        lexema[tamanho] = caractere;
                        tamanho++;
                    }

                    coluna++;
                }

                // se encontrar um ponto, verifica se o número pode ser real
                if (caractere == '.')
                {
                    numero_real = 1;

                    if (tamanho < 63)
                    {
                        lexema[tamanho] = caractere;
                        tamanho++;
                    }

                    coluna++;

                    while ((caractere = fgetc(arquivo)) != EOF && isdigit(caractere))
                    {
                        if (tamanho < 63)
                        {
                            lexema[tamanho] = caractere;
                            tamanho++;
                        }

                        coluna++;
                    }
                }

                if (caractere != EOF)
                {
                    // se encontrar um caractere que não faz parte do número, devolve para ser lido depois
                    ungetc(caractere, arquivo);
                }

                lexema[tamanho] = '\0';

                Token token;

                token.linha = linha;
                token.coluna = coluna_inicial;

                strcpy(token.lexema, lexema);

                if (numero_real)
                {
                    token.tipo = TOKEN_NUMERO_REAL;
                }
                else
                {
                    token.tipo = TOKEN_NUMERO_INTEIRO;
                }

                imprimir_token(token, &total_tokens, &total_erros);
            }

            else if (caractere == '\'')
            {
                char lexema[64];
                int tamanho = 0;
                int erro = 0;
                int coluna_inicial = coluna;

                // adiciona a primeira aspa simples ao lexema
                lexema[tamanho] = caractere;
                tamanho++;
                coluna++;

                // lê o próximo caractere após a aspa
                caractere = fgetc(arquivo);

                // verifica se o arquivo terminou ou se chegou ao final da linha
                if (caractere == EOF || caractere == '\n')
                {
                    erro = 1;
                }
                else
                {
                    // continua lendo até encontrar a aspa de fechamento, fim da linha ou fim do arquivo
                    while (caractere != EOF && caractere != '\n' && tamanho < 63)
                    {
                        lexema[tamanho] = caractere;
                        tamanho++;
                        coluna++;

                        // se encontrar uma aspa simples, encerra a leitura do literal
                        if (caractere == '\'')
                        {
                            break;
                        }

                        caractere = fgetc(arquivo);
                    }

                    // um literal válido deve ter exatamente: 'a'
                    if (tamanho != 3 || lexema[0] != '\'' || lexema[2] != '\'')
                    {
                        erro = 1;
                    }
                }

                lexema[tamanho] = '\0';

                Token token;

                token.linha = linha;
                token.coluna = coluna_inicial;

                strcpy(token.lexema, lexema);

                if (erro)
                {
                    token.tipo = TOKEN_ERRO;
                }
                else
                {
                    token.tipo = TOKEN_LITERAL_CARACTERE;
                }

                imprimir_token(token, &total_tokens, &total_erros);
            }

            else if (caractere == '=' || caractere == '!' || caractere == '<' || caractere == '>')
            {
                char lexema[3];
                int tamanho = 0;
                int coluna_inicial = coluna;

                lexema[tamanho] = caractere;
                tamanho++;
                coluna++;

                // lê o próximo caractere para verificar se temos um operador com dois caracteres
                int proximo = fgetc(arquivo);

                if (proximo == '=')
                {
                    lexema[tamanho] = proximo;
                    tamanho++;
                    coluna++;
                }
                else
                {
                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }

                lexema[tamanho] = '\0';

                Token token;

                token.tipo = TOKEN_OPERADOR;
                strcpy(token.lexema, lexema);
                token.linha = linha;
                token.coluna = coluna_inicial;

                imprimir_token(token, &total_tokens, &total_erros);
            }

            else if (caractere == '&')
            {
                int coluna_inicial = coluna;

                // lê o próximo caractere para verificar se temos o operador &&
                int proximo = fgetc(arquivo);

                if (proximo == '&')
                {
                    Token token;

                    token.tipo = TOKEN_OPERADOR;
                    strcpy(token.lexema, "&&");
                    token.linha = linha;
                    token.coluna = coluna_inicial;

                    coluna += 2;

                    imprimir_token(token, &total_tokens, &total_erros);
                }
                else
                {
                    // como & sozinho não forma um operador válido, ele é considerado um erro
                    Token token;

                    token.tipo = TOKEN_ERRO;
                    strcpy(token.lexema, "&");
                    token.linha = linha;
                    token.coluna = coluna_inicial;

                    coluna++;

                    imprimir_token(token, &total_tokens, &total_erros);

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '|')
            {
                int coluna_inicial = coluna;

                // lê o próximo caractere para verificar se temos o operador ||
                int proximo = fgetc(arquivo);

                if (proximo == '|')
                {
                    Token token;

                    token.tipo = TOKEN_OPERADOR;
                    strcpy(token.lexema, "||");
                    token.linha = linha;
                    token.coluna = coluna_inicial;

                    coluna += 2;

                    imprimir_token(token, &total_tokens, &total_erros);
                }
                else
                {
                    // como | sozinho não forma um operador válido, ele é considerado um erro
                    Token token;

                    token.tipo = TOKEN_ERRO;
                    strcpy(token.lexema, "|");
                    token.linha = linha;
                    token.coluna = coluna_inicial;

                    coluna++;

                    imprimir_token(token, &total_tokens, &total_erros);

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '/')
            {
                // lê o próximo caractere para verificar se temos o início de um comentário //
                int proximo = fgetc(arquivo);

                if (proximo == '/')
                {
                    // como encontramos //, ignora todos os caracteres do comentário
                    coluna += 2;

                    while ((caractere = fgetc(arquivo)) != EOF && caractere != '\n')
                    {
                        coluna++;
                    }

                    // se o comentário terminou com uma quebra de linha, atualiza a linha e reinicia a coluna
                    if (caractere == '\n')
                    {
                        linha++;
                        coluna = 1;
                    }
                }
                else
                {
                    // se não for comentário, / é reconhecido normalmente como operador
                    char lexema[2];
                    int coluna_inicial = coluna;

                    lexema[0] = '/';
                    lexema[1] = '\0';

                    Token token;

                    token.tipo = TOKEN_OPERADOR;
                    strcpy(token.lexema, lexema);
                    token.linha = linha;
                    token.coluna = coluna_inicial;

                    coluna++;

                    imprimir_token(token, &total_tokens, &total_erros);

                    // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                    if (proximo != EOF)
                    {
                        ungetc(proximo, arquivo);
                    }
                }
            }

            else if (caractere == '+' || caractere == '-' || caractere == '*')
            {
                char lexema[2];
                int coluna_inicial = coluna;

                lexema[0] = caractere;
                lexema[1] = '\0';

                Token token;

                token.tipo = TOKEN_OPERADOR;
                strcpy(token.lexema, lexema);
                token.linha = linha;
                token.coluna = coluna_inicial;

                coluna++;

                imprimir_token(token, &total_tokens, &total_erros);
            }

            else if (caractere == '(' || caractere == ')' || caractere == '{' || caractere == '}' || caractere == '[' || caractere == ']' || caractere == ';' || caractere == ',')
            {
                char lexema[2];
                int coluna_inicial = coluna;

                lexema[0] = caractere;
                lexema[1] = '\0';

                Token token;

                token.tipo = TOKEN_DELIMITADOR;
                strcpy(token.lexema, lexema);
                token.linha = linha;
                token.coluna = coluna_inicial;

                coluna++;

                imprimir_token(token, &total_tokens, &total_erros);
            }

            else
            {
                // se o caractere não pertence a nenhum tipo reconhecido pelo lexer, ele é considerado um erro
                char lexema[2];
                int coluna_inicial = coluna;

                lexema[0] = caractere;
                lexema[1] = '\0';

                Token token;

                token.tipo = TOKEN_ERRO;
                strcpy(token.lexema, lexema);
                token.linha = linha;
                token.coluna = coluna_inicial;

                coluna++;

                imprimir_token(token, &total_tokens, &total_erros);
            }
        }
    }

    printf("Total de tokens: %d\n", total_tokens);
    printf("Total de erros lexicos: %d\n", total_erros);

    fclose(arquivo);

    return 0;
}