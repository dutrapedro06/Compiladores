#include <stdio.h>

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

int main(int argc, char *argv[]) { //argc quantidade de argumentos e o argv os argumentos passados
    FILE *arquivo;

    if (argc != 2) {
        printf("Uso correto: minilexer <arquivo-fonte>\n");
        return 1;
    }

    arquivo = fopen(argv[1], "r");

    if (arquivo == NULL) {
        printf("Erro: não foi possível abrir o arquivo, conteudo nulo!\n");
        return 1;
    }

    fclose(arquivo);

    return 0;
}