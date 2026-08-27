#include <stdio.h>

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