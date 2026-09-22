#include "../include/transformer.hpp"

#include <iostream>

using namespace std;

int main()
{
    // Cria o simulador
    TransformerSimulator simulador;

    cout << "=============================================\n";
    cout << " SIMULADOR EDUCACIONAL DE UMA LLM\n";
    cout << " ARQUITETURA TRANSFORMER\n";
    cout << "=============================================\n\n";

    cout << "Esta e uma simulacao educacional simplificada.\n";
    cout << "Os valores nao representam os parametros internos\n";
    cout << "de uma LLM comercial.\n\n";


    // ========================================================
    // ETAPA 1 - RECEBIMENTO DA FRASE
    // ========================================================

    string frase = simulador.receberFrase();

    // Verifica se o usuário digitou alguma coisa
    if (frase.empty())
    {
        cout << "\nErro: nenhuma frase foi digitada.\n";
        return 0;
    }


    // ========================================================
    // ETAPA 2 - NORMALIZAÇÃO
    // ========================================================

    string textoNormalizado =
        simulador.normalizarTexto(frase);

    // Verifica novamente porque uma entrada contendo
    // apenas espaços fica vazia depois da normalização.
    if (textoNormalizado.empty())
    {
        cout << "\nErro: o texto informado esta vazio.\n";
        return 0;
    }

    cout << "\n=============================================\n";
    cout << "ETAPA 2 - TEXTO NORMALIZADO\n";
    cout << "=============================================\n";

    cout << textoNormalizado << "\n";


    // ========================================================
    // ETAPA 3 - TOKENIZAÇÃO
    // ========================================================

    vector<string> tokens =
        simulador.tokenizar(textoNormalizado);

    cout << "\n=============================================\n";
    cout << "ETAPA 3 - TOKENIZACAO\n";
    cout << "=============================================\n";

    simulador.mostrarTokens(tokens);


    // ========================================================
    // ETAPA 4 - CONVERSÃO PARA IDs
    // ========================================================

    vector<int> ids =
        simulador.gerarIds(tokens);

    cout << "\n=============================================\n";
    cout << "ETAPA 4 - TOKENS E IDs\n";
    cout << "=============================================\n";

    simulador.mostrarIds(tokens, ids);


    // ========================================================
    // ETAPA 5 - EMBEDDINGS
    // ========================================================

    vector<vector<double>> embeddings =
        simulador.gerarEmbeddings(ids);

    cout << "\n=============================================\n";
    cout << "ETAPA 5 - EMBEDDINGS\n";
    cout << "=============================================\n";

    cout << "Embedding didatico simulado.\n";
    cout << "Os valores nao representam embeddings reais.\n";

    simulador.mostrarVetores(
        tokens,
        embeddings,
        "EMBEDDINGS DIDATICOS"
    );


    // ========================================================
    // ETAPA 6 - INFORMAÇÃO DE POSIÇÃO
    // ========================================================

    vector<vector<double>> entradaComPosicao =
        simulador.adicionarPosicao(embeddings);

    cout << "\n=============================================\n";
    cout << "ETAPA 6 - INFORMACAO DE POSICAO\n";
    cout << "=============================================\n";

    simulador.mostrarVetores(
        tokens,
        entradaComPosicao,
        "EMBEDDING + POSICAO"
    );


    // ========================================================
    // ETAPA 7 - QUERY, KEY E VALUE
    // ========================================================

    vector<vector<double>> query =
        simulador.gerarQuery(entradaComPosicao);

    vector<vector<double>> key =
        simulador.gerarKey(entradaComPosicao);

    vector<vector<double>> value =
        simulador.gerarValue(entradaComPosicao);

    cout << "\n=============================================\n";
    cout << "ETAPA 7 - QUERY, KEY E VALUE\n";
    cout << "=============================================\n";

    simulador.mostrarVetores(
        tokens,
        query,
        "QUERY (Q)"
    );

    simulador.mostrarVetores(
        tokens,
        key,
        "KEY (K)"
    );

    simulador.mostrarVetores(
        tokens,
        value,
        "VALUE (V)"
    );


    // ========================================================
    // ETAPA 8 - PESOS DE ATENÇÃO
    // ========================================================

    vector<vector<double>> pesosAtencao =
        simulador.calcularAtencao(
            query,
            key
        );

    cout << "\n=============================================\n";
    cout << "ETAPA 8 - PESOS DE ATENCAO\n";
    cout << "=============================================\n";

    simulador.mostrarAtencao(
        tokens,
        pesosAtencao
    );


    // ========================================================
    // ETAPA 9 - COMBINAÇÃO DOS VALUES
    // ========================================================

    vector<vector<double>> cabeca1 =
        simulador.combinarValores(
            pesosAtencao,
            value
        );

    cout << "\n=============================================\n";
    cout << "ETAPA 9 - COMBINACAO DOS VALUES\n";
    cout << "=============================================\n";

    simulador.mostrarVetores(
        tokens,
        cabeca1,
        "RESULTADO DA CABECA 1"
    );


    // ========================================================
    // ETAPA 10 - SEGUNDA CABEÇA DE ATENÇÃO
    // ========================================================

    vector<vector<double>> cabeca2 =
        simulador.gerarSegundaCabeca(
            entradaComPosicao
        );

    cout << "\n=============================================\n";
    cout << "ETAPA 10 - MULTI-HEAD ATTENTION\n";
    cout << "=============================================\n";

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


    // Combina as duas cabeças
    vector<vector<double>> cabecasCombinadas =
        simulador.combinarCabecas(
            cabeca1,
            cabeca2
        );

    simulador.mostrarVetores(
        tokens,
        cabecasCombinadas,
        "RESULTADO DAS CABECAS COMBINADAS"
    );


    // ========================================================
    // ETAPA 10 - TRÊS CAMADAS SIMULADAS
    // ========================================================

    vector<vector<vector<double>>> camadas =
        simulador.processarCamadas(
            cabecasCombinadas
        );

    cout << "\n=============================================\n";
    cout << "ETAPA 10 - CAMADAS DO TRANSFORMER\n";
    cout << "=============================================\n";

    simulador.mostrarCamadas(
        tokens,
        camadas
    );


    // ========================================================
    // ETAPA 11 - RESPOSTA DIDÁTICA
    // ========================================================

    string resposta =
        simulador.obterRespostaDidatica(
            textoNormalizado
        );

    vector<string> tokensResposta =
        simulador.tokenizar(resposta);

    cout << "\n=============================================\n";
    cout << "ETAPA 11 - PREVISAO DO PROXIMO TOKEN\n";
    cout << "=============================================\n";


    // ========================================================
    // ETAPAS 11, 12 E 13
    // PROBABILIDADES + GREEDY + GERAÇÃO AUTOREGRESSIVA
    // ========================================================

    string respostaConstruida;

    vector<string> tokensGerados;

    for (int i = 0; i < tokensResposta.size(); i++)
    {
        cout << "\n---------------------------------------------\n";
        cout << "PASSO DE GERACAO " << i + 1 << "\n";
        cout << "---------------------------------------------\n";

        // Calcula as probabilidades dos candidatos
        vector<pair<string, double>> probabilidades =
            simulador.calcularProbabilidades(
                tokensResposta,
                i
            );

        simulador.mostrarProbabilidades(
            probabilidades
        );

        // Estratégia Greedy:
        // seleciona o token com maior probabilidade
        string proximoToken =
            simulador.selecionarProximoToken(
                probabilidades
            );

        cout << "\nToken selecionado: "
             << proximoToken
             << "\n";

        tokensGerados.push_back(
            proximoToken
        );

        // Evita colocar espaço antes das pontuações
        if (
            proximoToken == "." ||
            proximoToken == "," ||
            proximoToken == "?" ||
            proximoToken == "!" ||
            proximoToken == ";" ||
            proximoToken == ":")
        {
            respostaConstruida += proximoToken;
        }
        else
        {
            if (!respostaConstruida.empty())
            {
                respostaConstruida += " ";
            }

            respostaConstruida += proximoToken;
        }

        cout << "Resposta parcial: "
             << respostaConstruida
             << "\n";
    }


    // ========================================================
    // ETAPA 14 - RESPOSTA FINAL
    // ========================================================

    cout << "\n=============================================\n";
    cout << "ETAPA 14 - RESPOSTA FINAL\n";
    cout << "=============================================\n";

    cout << respostaConstruida << "\n";


    // ========================================================
    // RESUMO FINAL DA SIMULAÇÃO
    // ========================================================

    simulador.mostrarResumo(
        frase,
        tokens,
        tokensGerados,
        respostaConstruida
    );


    return 0;
}