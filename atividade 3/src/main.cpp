#include "../include/transformer.hpp"

#include <iostream>

using namespace std;

int main()
{
    TransformerSimulator simulador;

    cout << "========================================\n";
    cout << " SIMULADOR EDUCACIONAL DE TRANSFORMER\n";
    cout << "========================================\n";
    cout << "Simulacao educacional simplificada.\n";
    cout << "Os valores nao representam uma LLM comercial.\n\n";

    // 1 e 2 - Entrada e normalizacao
    string frase = simulador.receberFrase();
    string texto = simulador.normalizarTexto(frase);

    if (texto.empty())
    {
        cout << "Erro: digite uma frase ou pergunta.\n";
        return 0;
    }

    cout << "\n1. FRASE RECEBIDA\n";
    cout << frase << "\n";

    cout << "\n2. TEXTO NORMALIZADO\n";
    cout << texto << "\n";


    // 3 e 4 - Tokens e IDs
    vector<string> tokens = simulador.tokenizar(texto);
    vector<int> ids = simulador.gerarIds(tokens);

    cout << "\n3. TOKENIZACAO\n";
    simulador.mostrarTokens(tokens);

    cout << "\n4. TOKENS PARA IDs\n";
    simulador.mostrarIds(tokens, ids);


    // 5 - Embeddings
    vector<vector<double>> embeddings =
        simulador.gerarEmbeddings(ids);

    cout << "\n5. EMBEDDINGS\n";
    cout << "Embedding didatico simulado.\n";

    simulador.mostrarVetores(
        tokens,
        embeddings,
        "EMBEDDINGS"
    );


    // 6 - Informacao de posicao
    vector<vector<double>> entrada =
        simulador.adicionarPosicao(embeddings);

    cout << "\n6. INFORMACAO DE POSICAO\n";

    simulador.mostrarVetores(
        tokens,
        entrada,
        "EMBEDDING + POSICAO"
    );


    // 7 - Query, Key e Value
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


    // 8 - Pesos de atencao
    vector<vector<double>> pesos =
        simulador.calcularAtencao(Q, K);

    cout << "\n8. PESOS DE ATENCAO\n";
    simulador.mostrarAtencao(tokens, pesos);


    // 9 - Combinacao dos Values
    vector<vector<double>> cabeca1 =
        simulador.combinarValores(pesos, V);

    cout << "\n9. COMBINACAO DOS VALUES\n";

    simulador.mostrarVetores(
        tokens,
        cabeca1,
        "SAIDA DA CABECA 1"
    );


    // 10 - Multi-head e camadas
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

    vector<vector<vector<double>>> camadas =
        simulador.processarCamadas(multiHead);

    simulador.mostrarCamadas(tokens, camadas);


    // Resposta didatica usada pela simulacao
    string resposta =
        simulador.obterRespostaDidatica(texto);

    vector<string> tokensResposta =
        simulador.tokenizar(resposta);


    // 11 e 12 - Probabilidades e selecao
    cout << "\n11. PROBABILIDADES DO PROXIMO TOKEN\n";

    vector<pair<string, double>> probs =
        simulador.calcularProbabilidades(
            tokensResposta,
            0
        );

    simulador.mostrarProbabilidades(probs);

    cout << "\n12. TOKEN SELECIONADO\n";
    cout << simulador.selecionarProximoToken(probs)
         << "\n";


    // 13 - Geracao progressiva
    cout << "\n13. GERACAO PROGRESSIVA\n";

    vector<string> etapas =
        simulador.gerarRespostaProgressiva(resposta);

    for (int i = 0; i < etapas.size(); i++)
    {
        cout << "Passo "
             << i + 1
             << ": "
             << etapas[i]
             << "\n";
    }


    // Tokens efetivamente gerados
    vector<string> tokensGerados =
        simulador.tokenizar(
            etapas.empty() ? "" : etapas.back()
        );

    string respostaFinal =
        etapas.empty() ? "" : etapas.back();


    // 14 - Resposta final
    cout << "\n14. RESPOSTA FINAL\n";
    cout << respostaFinal << "\n";


    // Resumo da simulacao
    simulador.mostrarResumo(
        frase,
        tokens,
        tokensGerados,
        respostaFinal
    );

    return 0;
}