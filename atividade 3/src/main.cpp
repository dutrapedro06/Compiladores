#include "../include/transformer.hpp"

#include <iostream>

using namespace std;

int main()
{
    TransformerSimulator simulador;

    cout << "========================================\n";
    cout << " SIMULADOR EDUCACIONAL DE TRANSFORMER\n";
    cout << "========================================\n";
    cout << "Esta e uma simulacao educacional simplificada.\n";
    cout << "Os valores nao representam os parametros internos "
         << "de uma LLM comercial.\n";


    // ETAPAS 1 E 2 - ENTRADA E NORMALIZACAO

    string frase = simulador.receberFrase();
    string texto = simulador.normalizarTexto(frase);

    if (texto.empty())
    {
        cout << "\nErro: digite uma frase ou pergunta.\n";
        return 0;
    }

    cout << "\n1. FRASE RECEBIDA\n";
    cout << frase << "\n";

    cout << "\n2. TEXTO NORMALIZADO\n";
    cout << texto << "\n";


    // ETAPAS 3 E 4 - TOKENS E IDs

    vector<string> tokens = simulador.tokenizar(texto);
    vector<int> ids = simulador.gerarIds(tokens);

    cout << "\n3. TOKENIZACAO\n";
    simulador.mostrarTokens(tokens);

    cout << "\n4. TOKENS PARA IDs\n";
    simulador.mostrarIds(tokens, ids);


    // ETAPA 5 - EMBEDDINGS

    vector<vector<double>> embeddings =
        simulador.gerarEmbeddings(ids);

    cout << "\n5. EMBEDDINGS\n";
    cout << "Embedding didatico simulado.\n";

    simulador.mostrarVetores(
        tokens,
        embeddings,
        "EMBEDDINGS"
    );


    // ETAPA 6 - INFORMACAO DE POSICAO

    vector<vector<double>> entrada =
        simulador.adicionarPosicao(embeddings);

    cout << "\n6. INFORMACAO DE POSICAO\n";

    simulador.mostrarVetores(
        tokens,
        entrada,
        "EMBEDDING + POSICAO"
    );


    // ETAPA 7 - QUERY, KEY E VALUE

    vector<vector<double>> Q =
        simulador.gerarQuery(entrada);

    vector<vector<double>> K =
        simulador.gerarKey(entrada);

    vector<vector<double>> V =
        simulador.gerarValue(entrada);

    cout << "\n7. QUERY, KEY E VALUE\n";

    simulador.mostrarVetores(tokens, Q, "QUERY (Q)");
    simulador.mostrarVetores(tokens, K, "KEY (K)");
    simulador.mostrarVetores(tokens, V, "VALUE (V)");


    // ETAPA 8 - PESOS DE ATENCAO

    vector<vector<double>> pesos =
        simulador.calcularAtencao(Q, K);

    cout << "\n8. PESOS DE ATENCAO\n";

    simulador.mostrarAtencao(
        tokens,
        pesos
    );


    // ETAPA 9 - COMBINACAO DOS VALUES

    vector<vector<double>> cabeca1 =
        simulador.combinarValores(
            pesos,
            V
        );

    cout << "\n9. COMBINACAO DOS VALUES\n";

    simulador.mostrarVetores(
        tokens,
        cabeca1,
        "SAIDA DA CABECA 1"
    );


    // ETAPA 10 - MULTI-HEAD ATTENTION

    vector<vector<double>> cabeca2 =
        simulador.gerarSegundaCabeca(entrada);

    vector<vector<double>> multiHead =
        simulador.combinarCabecas(
            cabeca1,
            cabeca2
        );

    cout << "\n10. MULTI-HEAD ATTENTION\n";

    simulador.mostrarVetores(
        tokens,
        cabeca1,
        "CABECA 1"
    );

    simulador.mostrarVetores(
        tokens,
        cabeca2,
        "CABECA 2"
    );

    simulador.mostrarVetores(
        tokens,
        multiHead,
        "CABECAS COMBINADAS"
    );


    // TRES CAMADAS SIMULADAS

    vector<vector<vector<double>>> camadas =
        simulador.processarCamadas(multiHead);

    simulador.mostrarCamadas(
        tokens,
        camadas
    );


    // RESPOSTA DIDATICA

    string resposta =
        simulador.obterRespostaDidatica(texto);

    vector<string> tokensResposta =
        simulador.tokenizar(resposta);


    // ETAPAS 11, 12 E 13
    // PROBABILIDADES, SELECAO E GERACAO PROGRESSIVA

    cout << "\n========================================\n";
    cout << "11, 12 E 13. GERACAO AUTOREGRESSIVA\n";
    cout << "========================================\n";

    vector<string> etapas =
        simulador.gerarRespostaProgressiva(resposta);

    vector<string> tokensGerados;

    for (int i = 0; i < tokensResposta.size(); i++)
    {
        cout << "\n----------------------------------------\n";
        cout << "PASSO " << i + 1 << "\n";
        cout << "----------------------------------------\n";

        vector<pair<string, double>> probabilidades =
            simulador.calcularProbabilidades(
                tokensResposta,
                i
            );

        cout << "\n11. PROBABILIDADES DO PROXIMO TOKEN\n";

        simulador.mostrarProbabilidades(
            probabilidades
        );

        string tokenSelecionado =
            simulador.selecionarProximoToken(
                probabilidades
            );

        cout << "\n12. TOKEN SELECIONADO\n";
        cout << tokenSelecionado << "\n";

        tokensGerados.push_back(
            tokenSelecionado
        );

        cout << "\n13. RESPOSTA PARCIAL\n";

        if (i < etapas.size())
        {
            cout << etapas[i] << "\n";
        }
    }


    // ETAPA 14 - RESPOSTA FINAL

    string respostaFinal;

    if (!etapas.empty())
    {
        respostaFinal = etapas.back();
    }

    cout << "\n========================================\n";
    cout << "14. RESPOSTA FINAL\n";
    cout << "========================================\n";

    cout << respostaFinal << "\n";


    // RESUMO FINAL

    simulador.mostrarResumo(
        frase,
        tokens,
        tokensGerados,
        respostaFinal
    );

    return 0;
}