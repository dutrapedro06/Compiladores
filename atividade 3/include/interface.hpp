#ifndef INTERFACE_HPP
#define INTERFACE_HPP
#include "transformer.hpp"

using namespace std;

// Essa classe é responsável pela parte de interação
// entre o usuário e o simulador.
class Interface
{
private:

    // Esse objeto será utilizado pela Interface para chamar
    // as funções responsáveis pelo processamento do Transformer.
    TransformerSimulator simulador;

    void mostrarCabecalho();

    void mostrarAviso();

    void mostrarLinha();

    void processar();

public:

    void executar();
};

#endif