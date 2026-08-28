int contador = 0;
int limite = 10;
float valor = 1.5;
char letra = 'a';

// Incrementa o contador
while (contador < limite) {
    contador = contador + 1;
}

// Verifica o resultado
if (contador >= limite && valor > 0.0) {
    valor = valor * 2.5;
}
else {
    valor = valor / 2.0;
}

return contador;
