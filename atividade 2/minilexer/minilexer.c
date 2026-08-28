#include <stdio.h>
#include <ctype.h>
#include <string.h>

//cada categoria recebe um valor inteiro automaticamente, facilitando a identificação e o tratamento dos tokens no programa.
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

// verifica se o lexema encontrado é uma das palavras reservadas da linguagem MiniC
int eh_palavra_reservada(const char lexema[])
{
    if (strcmp(lexema, "int") == 0 || strcmp(lexema, "float") == 0 || strcmp(lexema, "char") == 0 || strcmp(lexema, "if") == 0 || strcmp(lexema, "else") == 0 || strcmp(lexema, "while") == 0 || strcmp(lexema, "return") == 0 || strcmp(lexema, "print") == 0)
    {
        return 1;
    }

    return 0;
}

// verifica se um caractere pode iniciar um identificador
// identificadores podem começar com letras ou _, não pode começar com número
int eh_inicio_identificador(int caractere)
{
    return isalpha(caractere) || caractere == '_';
}

// verifica se um caractere pode fazer parte de um identificador
// depois do primeiro caractere também podem aparecer números
int eh_parte_identificador(int caractere)
{
    return isalpha(caractere) ||
    isdigit(caractere) ||
    caractere == '_';
}

// verifica se o caractere recebido é um delimitador válido
int eh_delimitador(int caractere)
{
return caractere == '(' || caractere == ')' || caractere == '{' || caractere == '}' || caractere == '[' || caractere == ']' || caractere == ';' || caractere == ',';
}

// cria um Token agrupando todas as informações necessárias, função pra não ter que ficar fazendo isso a mão na main
Token criar_token(TipoToken tipo, const char lexema[], int linha, int coluna)
{
    Token token;

    token.tipo = tipo;
    strncpy(token.lexema, lexema, sizeof(token.lexema) - 1);
    token.lexema[sizeof(token.lexema) - 1] = '\0';
    token.linha = linha;
    token.coluna = coluna;

    return token;

}

// retorna o nome da categoria do token para que a saída não precise mostrar apenas números
const char *nome_do_token(TipoToken tipo)
{
    switch (tipo)
    {
        case TOKEN_PALAVRA_RESERVADA:
        return "PALAVRA_RESERVADA";

            case TOKEN_IDENTIFICADOR:
                return "IDENTIFICADOR";

            case TOKEN_NUMERO_INTEIRO:
                return "NUMERO_INTEIRO";

            case TOKEN_NUMERO_REAL:
                return "NUMERO_REAL";

            case TOKEN_LITERAL_CARACTERE:
                return "LITERAL_CARACTERE";

            case TOKEN_OPERADOR:
                return "OPERADOR";

            case TOKEN_DELIMITADOR:
                return "DELIMITADOR";

            case TOKEN_ERRO:
                return "ERRO_LEXICO";

            default:
                return "DESCONHECIDO";
    }
}

// imprime o token encontrado e atualiza o contador de token ou erro
void imprimir_token(Token token, int *total_tokens, int *total_erros)
{
    printf("%d:%d | %-18s | %s\n", token.linha,token.coluna,nome_do_token(token.tipo),token.lexema);

    (*total_tokens)++;

    // se o token for um erro, também incrementa o contador de erros léxicos
    if (token.tipo == TOKEN_ERRO)
    {
        (*total_erros)++;
    }

}

// adiciona um caractere ao lexema sem ultrapassar o tamanho máximo do vetor
void adicionar_caractere(char lexema[], int *tamanho, int caractere)
{
// o vetor possui espaço para 64 caracteres, mas o codigo deixa só até 63 caracteres para colocar o \0 no final
    if (*tamanho < 63)
    {
        lexema[*tamanho] = (char)caractere;
        (*tamanho)++;
    }
}

int main(int argc, char *argv[]) //argc quantidade de argumentos e o argv os argumentos passados
{
    FILE *arquivo;

    if (argc != 2)
    {
        printf("Uso: minilexer <arquivo-fonte>\n");
        return 1;
    }

    arquivo = fopen(argv[1], "r");

    if (arquivo == NULL)
    {
        printf("Erro: não foi possível abrir o arquivo.\n");
        return 1;
    }

    int caractere;

    //antes de consumir os caracteres, eu salvo a posição inicial do para exibir depois onde ele começou
    int linha = 1;
    int coluna = 1;

    int total_tokens = 0;
    int total_erros = 0;

    // enquanto conseguir ler um caractere e não chegar no final do arquivo (EOF) ele continua
    while ((caractere = fgetc(arquivo)) != EOF)
    {
        // guarda a posição onde o token começou
        int linha_inicial = linha;
        int coluna_inicial = coluna;

        // quando encontra uma quebra de linha, passa para a próxima linha
        if (caractere == '\n')
        {
            linha++;
            coluna = 1;
        }

        // se o caractere for espaço ou tabulação, ele é ignorado, \r usado em quebras de linha do Windows
        else if (caractere == ' ' || caractere == '\t' || caractere == '\r')
        {
            coluna++;
        }

        // verifica se o caractere é o início de um identificador ou palavra reservada
        else if (eh_inicio_identificador(caractere))
        {
            char lexema[64];
            int tamanho = 0;
            int tamanho_total = 0; // se tiver mais de 31 caracteres ele da erro lexico

            adicionar_caractere(lexema, &tamanho, caractere);
            tamanho_total++;
            coluna++;

            // enquanto o próximo caractere for letra, número ou _, continua adicionando ao lexema
            while ((caractere = fgetc(arquivo)) != EOF && eh_parte_identificador(caractere))
            {
                adicionar_caractere(lexema, &tamanho, caractere);
                tamanho_total++;
                coluna++;
            }

            // se não fizer parte do identificador, coloca o caractere de volta para ser lido na próxima volta
            if (caractere != EOF)
            {
                ungetc(caractere, arquivo); //devolve o caractere para ser lido novamente
            }

            lexema[tamanho] = '\0';

            // se o identificador ultrapassar 31 caracteres, gera um erro léxico
            // mas todo o identificador já foi consumido antes de continuar a análise
            if (tamanho_total > 31)
            {
                Token token = criar_token(TOKEN_ERRO,lexema,linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }

            // verifica se o lexema encontrado é uma palavra reservada
            else if (eh_palavra_reservada(lexema))
            {
                Token token = criar_token(
                    TOKEN_PALAVRA_RESERVADA,
                    lexema,
                    linha_inicial,
                    coluna_inicial
                );

                imprimir_token(token, &total_tokens, &total_erros);
            }
            else //se não for uma palavra reservada é um identificador 
            {
                Token token = criar_token(TOKEN_IDENTIFICADOR,lexema,linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }
        }


        // verifica se o token começa com um número
        else if (isdigit(caractere))
        {
            char lexema[64];
            int tamanho = 0;
            int numero_real = 0;
            int erro = 0; //controla se o número encontrado é inválido

            adicionar_caractere(lexema, &tamanho, caractere);
            coluna++;

            // enquanto o próximo caractere for um número, continua adicionando ao lexema
            while ((caractere = fgetc(arquivo)) != EOF && isdigit(caractere))
            {
                adicionar_caractere(lexema, &tamanho, caractere);
                coluna++;
            }

            // se encontrar um ponto, verifica se o número pode ser real ou malformado
            if (caractere == '.')
            {
                numero_real = 1;

                // adiciona o ponto ao lexema
                adicionar_caractere(lexema, &tamanho, caractere);
                coluna++;

                // continua consumindo números e pontos para manter todo o número malformado em um único erro
                while ((caractere = fgetc(arquivo)) != EOF && (isdigit(caractere) || caractere == '.'))
                {
                    adicionar_caractere(lexema, &tamanho, caractere);
                    coluna++;
                }

                // se o último caractere não fizer parte do número, devolve para ser lido depois
                if (caractere != EOF)
                {
                    ungetc(caractere, arquivo);
                }

                // verifica quantos pontos existem e se existe pelo menos um algarismo depois do ponto
                int quantidade_pontos = 0;
                int algarismo_depois_do_ponto = 0;
                int encontrou_ponto = 0;

                for (int i = 0; lexema[i] != '\0'; i++)
                {
                    if (lexema[i] == '.')
                    {
                        quantidade_pontos++;
                        encontrou_ponto = 1;
                    }
                    else if (encontrou_ponto && isdigit(lexema[i]))
                    {
                        algarismo_depois_do_ponto = 1;
                    }
                }

                // um número é inválido se possuir mais de um ponto ou nenhum algarismo depois do ponto
                if (quantidade_pontos != 1 || !algarismo_depois_do_ponto)
                {
                    erro = 1;
                }
            }
            else
            {
                // se encontrar um caractere que não faz parte do número, devolve para ser lido depois
                if (caractere != EOF)
                {
                    ungetc(caractere, arquivo);
                }
            }

            lexema[tamanho] = '\0';

            if (erro)
            {
                Token token = criar_token( TOKEN_ERRO, lexema, linha_inicial, coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }
            else if (numero_real)
            {
                Token token = criar_token( TOKEN_NUMERO_REAL, lexema, linha_inicial, coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }
            else
            {
                Token token = criar_token(TOKEN_NUMERO_INTEIRO, lexema, linha_inicial, coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }
        }


        // verifica se o token começa com um ponto, como .5
        // números reais da linguagem MiniC precisam possuir uma parte inteira antes do ponto
        else if (caractere == '.')
        {
            char lexema[64];
            int tamanho = 0;

            adicionar_caractere(lexema, &tamanho, caractere);
            coluna++;

            // se depois do ponto existirem números ou outros pontos, consome toda a sequência
            while ((caractere = fgetc(arquivo)) != EOF && (isdigit(caractere) || caractere == '.'))
            {
                adicionar_caractere(lexema, &tamanho, caractere);
                coluna++;
            }

            // se encontrar outro caractere, devolve para ser analisado normalmente
            if (caractere != EOF)
            {
                ungetc(caractere, arquivo);
            }

            lexema[tamanho] = '\0';

            // .5 e outros formatos que começam com ponto são números inválidos
            Token token = criar_token(TOKEN_ERRO, lexema, linha_inicial, coluna_inicial);

            imprimir_token(token, &total_tokens, &total_erros);
        }


        // verifica se temos um literal de caractere iniciado por aspas simples
        else if (caractere == '\'')
        {
            char lexema[64];
            int tamanho = 0;
            int erro = 0;
            int fechado = 0;

            // adiciona a primeira aspa simples ao lexema
            adicionar_caractere(lexema, &tamanho, caractere);
            coluna++;

            // lê o conteúdo que está dentro das aspas simples
            while ((caractere = fgetc(arquivo)) != EOF)
            {
                // se encontrar uma quebra de linha antes de fechar, o literal é inválido
                if (caractere == '\n')
                {
                    erro = 1;
                    linha++;
                    coluna = 1;
                    break;
                }

                adicionar_caractere(lexema, &tamanho, caractere);
                coluna++;

                // se encontrar a aspa simples, terminou a leitura do literal
                if (caractere == '\'')
                {
                    fechado = 1;
                    break;
                }
            }

            // se chegou ao EOF ou à quebra de linha sem fechar as aspas, gera erro
            if (!fechado)
            {
                erro = 1;
            }

            lexema[tamanho] = '\0';

            // verifica se o literal possui exatamente três caracteres: 'a'
            // isso significa uma aspa, um caractere e outra aspa
            if (fechado && tamanho != 3)
            {
                erro = 1;
            }

            if (erro)
            {
                Token token = criar_token(
                    TOKEN_ERRO,
                    lexema,
                    linha_inicial,
                    coluna_inicial
                );

                imprimir_token(token, &total_tokens, &total_erros);
            }
            else
            {
                Token token = criar_token(
                    TOKEN_LITERAL_CARACTERE,
                    lexema,
                    linha_inicial,
                    coluna_inicial
                );

                imprimir_token(token, &total_tokens, &total_erros);
            }
        }


        // verifica operadores que podem possuir uma ou duas posições
        else if (caractere == '=' || caractere == '!' || caractere == '<' || caractere == '>')
        {
            char lexema[3]; // no maximo dois caracteres e o \0

            lexema[0] = (char)caractere;
            lexema[1] = '\0';

            coluna++;

            // lê o próximo caractere para verificar se temos um operador de dois caracteres
            int proximo = fgetc(arquivo);

            if (proximo == '=')
            {
                lexema[1] = '=';
                lexema[2] = '\0';
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

            Token token = criar_token(TOKEN_OPERADOR,lexema,linha_inicial,coluna_inicial);

            imprimir_token(token, &total_tokens, &total_erros);
        }


        // verifica se temos o operador lógico &&
        else if (caractere == '&')
        {
            int proximo = fgetc(arquivo);

            if (proximo == '&')
            {
                coluna += 2;

                Token token = criar_token(TOKEN_OPERADOR,"&&",linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }
            else
            {
                coluna++;

                // como & sozinho não forma um operador válido, gera um erro léxico
                Token token = criar_token(TOKEN_ERRO,"&",linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);

                // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                if (proximo != EOF)
                {
                    ungetc(proximo, arquivo);
                }
            }
        }


        // verifica se temos o operador lógico ||
        else if (caractere == '|')
        {
            int proximo = fgetc(arquivo);

            if (proximo == '|')
            {
                coluna += 2;

                Token token = criar_token(TOKEN_OPERADOR,"||",linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);
            }
            else
            {
                coluna++;

                // como | sozinho não forma um operador válido, gera um erro léxico
                Token token = criar_token(TOKEN_ERRO,"|",linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);

                // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                if (proximo != EOF)
                {
                    ungetc(proximo, arquivo);
                }
            }
        }


        // reconhece os operadores de apenas um caractere
        else if (caractere == '+' || caractere == '-' || caractere == '*' || caractere == '%')
        {
            char lexema[2];

            lexema[0] = (char)caractere;
            lexema[1] = '\0';

            coluna++;

            Token token = criar_token(TOKEN_OPERADOR,lexema,linha_inicial,coluna_inicial);

            imprimir_token(token, &total_tokens, &total_erros);
        }


        // a / pode representar divisão ou o início de um comentário //
        else if (caractere == '/')
        {
            int proximo = fgetc(arquivo);

            // se encontrarmos outra /, todo o restante da linha é um comentário
            if (proximo == '/')
            {
                coluna += 2;

                // ignora todos os caracteres até encontrar uma quebra de linha ou EOF
                while ((caractere = fgetc(arquivo)) != EOF && caractere != '\n')
                {
                    coluna++;
                }

                // se o comentário terminou com quebra de linha, atualiza a posição
                if (caractere == '\n')
                {
                    linha++;
                    coluna = 1;
                }
            }
            else
            {
                coluna++;

                Token token = criar_token(TOKEN_OPERADOR,"/",linha_inicial,coluna_inicial);

                imprimir_token(token, &total_tokens, &total_erros);

                // se o próximo caractere não fizer parte do operador, devolve para ser lido depois
                if (proximo != EOF)
                {
                    ungetc(proximo, arquivo);
                }
            }
        }


        // verifica se o caractere é um delimitador
        else if (eh_delimitador(caractere))
        {
            char lexema[2];

            lexema[0] = (char)caractere;
            lexema[1] = '\0';

            coluna++;

            Token token = criar_token(TOKEN_DELIMITADOR,lexema,linha_inicial,coluna_inicial);

            imprimir_token(token, &total_tokens, &total_erros);
        }


        // qualquer caractere que não pertence às categorias anteriores gera um erro léxico
        else
        {
            char lexema[2];

            lexema[0] = (char)caractere;
            lexema[1] = '\0';

            coluna++;

            Token token = criar_token(TOKEN_ERRO,lexema,linha_inicial,coluna_inicial);

            imprimir_token(token, &total_tokens, &total_erros);
        }
    }


    // ao final da análise mostra a quantidade de tokens encontrados e erros léxicos
    printf("\n");
    printf("Total de tokens: %d\n", total_tokens);
    printf("Total de erros lexicos: %d\n", total_erros);

    // fecha o arquivo antes de encerrar o programa
    fclose(arquivo);

    return 0;
}
