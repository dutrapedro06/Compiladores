#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "transformer.hpp"

using namespace std;

class Interface
{
private:
    TransformerSimulator simulador;

    void mostrarCabecalho();
    void mostrarAviso();
    void mostrarLinha();
    void processar();

public:
    void executar();
};

#endif