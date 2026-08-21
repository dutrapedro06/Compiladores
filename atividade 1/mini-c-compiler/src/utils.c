#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"

/**
 * Reads the content of a text file into a dynamically allocated string.
 * The caller is responsible for freeing the returned buffer.
 */
char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r"); // usa fopen como o arquivo de leitura, se der NULL o arquivo não existe e já sai do programa(Nem chama o lexer)
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file); //descobrir o tamanho do arquivo com o fseek e ftell
    rewind(file);

    char* buffer = malloc(length + 1); // aloca o tamanho do vetor + 1 posição
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t read_size = fread(buffer, 1, length, file); // le o conteúdo
    buffer[read_size] = '\0';

    fclose(file);
    return buffer; // fecha o arquivo e manda devolve o ponteiro 
}
