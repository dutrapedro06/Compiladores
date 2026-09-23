#include "../include/interface.hpp"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Interface::mostrarLinha()
{
    cout << "============================================================\n";
}

void Interface::mostrarCabecalho()
{
    cout << "\n";
    mostrarLinha();
    cout << "        SIMULADOR EDUCACIONAL DE TRANSFORMER\n";
    mostrarLinha();
}

void Interface::mostrarAviso()
{
    cout << "\n";
    mostrarLinha();
    cout << "Esta é uma simulação educacional simplificada. "
         << "Os valores não representam os parâmetros internos de uma LLM comercial.\n";
    mostrarLinha();
}

void Interface::processar()
{
    simulador = TransformerSimulator();

    string texto;
    cout << "\nDigite uma frase ou pergunta:\n> ";
    getline(cin, texto);

    string normalizado = simulador.normalizarTexto(texto);

    if (normalizado.empty())
    {
        cout << "\nErro: digite uma frase ou pergunta.\n";
        return;
    }

    // Processamento da entrada
    vector<string> tokens = simulador.tokenizar(normalizado);
    vector<int> ids = simulador.gerarIds(tokens);
    vector<vector<double>> embeddings = simulador.gerarEmbeddings(ids);
    vector<vector<double>> posicao = simulador.adicionarPosicao(embeddings);

    // Query, Key e Value
    vector<vector<double>> query = simulador.gerarQuery(posicao);
    vector<vector<double>> key = simulador.gerarKey(posicao);
    vector<vector<double>> value = simulador.gerarValue(posicao);

    // Atenção, cabeças e camadas
    vector<vector<double>> atencao = simulador.calcularAtencao(query, key);
    vector<vector<double>> cabeca1 = simulador.combinarValores(atencao, value);
    vector<vector<double>> cabeca2 = simulador.gerarSegundaCabeca(posicao);
    vector<vector<double>> cabecas = simulador.combinarCabecas(cabeca1, cabeca2);
    vector<vector<vector<double>>> camadas = simulador.processarCamadas(cabecas);

    // Geração da resposta
    string resposta = simulador.obterRespostaDidatica(normalizado);
    vector<string> tokensResposta = simulador.tokenizar(resposta);
    vector<string> geracao = simulador.gerarRespostaProgressiva(resposta);

    // Entrada
    cout << "\n";
    mostrarLinha();
    cout << "1. ENTRADA E NORMALIZAÇÃO\n";
    mostrarLinha();
    cout << "\nEntrada original: " << texto;
    cout << "\nTexto normalizado: " << normalizado << "\n";

    // Tokens
    cout << "\n";
    mostrarLinha();
    cout << "2. TOKENS\n";
    mostrarLinha();
    cout << "\n";

    for (const string& token : tokens)
    {
        cout << "[ " << token << " ] ";
    }

    cout << "\n";

    // IDs
    cout << "\n";
    mostrarLinha();
    cout << "3. TOKENS E IDs\n";
    mostrarLinha();
    simulador.mostrarIds(tokens, ids);

    // Embeddings
    cout << "\n";
    mostrarLinha();
    cout << "4. EMBEDDINGS\n";
    mostrarLinha();
    cout << "\nEmbedding didático simulado.\n";
    simulador.mostrarVetores(tokens, embeddings, "EMBEDDINGS");

    // Posição
    cout << "\n";
    mostrarLinha();
    cout << "5. INFORMAÇÃO DE POSIÇÃO\n";
    mostrarLinha();
    simulador.mostrarVetores(tokens, posicao, "EMBEDDING + POSIÇÃO");

    // QKV
    cout << "\n";
    mostrarLinha();
    cout << "6. QUERY, KEY E VALUE\n";
    mostrarLinha();
    simulador.mostrarVetores(tokens, query, "QUERY (Q)");
    simulador.mostrarVetores(tokens, key, "KEY (K)");
    simulador.mostrarVetores(tokens, value, "VALUE (V)");

    // Atenção
    cout << "\n";
    mostrarLinha();
    cout << "7. PESOS DE ATENÇÃO\n";
    mostrarLinha();
    simulador.mostrarAtencao(tokens, atencao);

    // Multi-head
    cout << "\n";
    mostrarLinha();
    cout << "8. MULTI-HEAD ATTENTION\n";
    mostrarLinha();
    simulador.mostrarVetores(tokens, cabeca1, "CABEÇA 1");
    simulador.mostrarVetores(tokens, cabeca2, "CABEÇA 2");
    simulador.mostrarVetores(tokens, cabecas, "CABEÇAS COMBINADAS");

    // Camadas
    cout << "\n";
    mostrarLinha();
    cout << "9. CAMADAS DO TRANSFORMER\n";
    mostrarLinha();
    simulador.mostrarCamadas(tokens, camadas);

    // Geração autoregressiva
    cout << "\n";
    mostrarLinha();
    cout << "10. GERAÇÃO AUTOREGRESSIVA\n";
    mostrarLinha();

    for (int i = 0; i < tokensResposta.size(); i++)
    {
        cout << "\n------------------------------\n";
        cout << "PASSO " << i + 1 << "\n";
        cout << "------------------------------\n";

        vector<pair<string, double>> probabilidades =
            simulador.calcularProbabilidades(tokensResposta, i);

        simulador.mostrarProbabilidades(probabilidades);

        string selecionado =
            simulador.selecionarProximoToken(probabilidades);

        cout << "\nToken selecionado: " << selecionado;
        cout << "\nResposta parcial: " << geracao[i] << "\n";
    }

    // Resposta final
    cout << "\n";
    mostrarLinha();
    cout << "11. RESPOSTA FINAL\n";
    mostrarLinha();
    cout << "\n" << resposta << "\n";

    // Resumo
    cout << "\n";
    mostrarLinha();
    cout << "RESUMO DA SIMULAÇÃO\n";
    mostrarLinha();

    cout << "\nPergunta original: " << texto;
    cout << "\nQuantidade de tokens de entrada: " << tokens.size();
    cout << "\nQuantidade de tokens gerados: " << tokensResposta.size();
    cout << "\nEtapas principais executadas: 14";
    cout << "\nResposta final: " << resposta << "\n";

    cout << "\nPARTES SIMULADAS:\n";
    cout << "- Embeddings didáticos\n";
    cout << "- Matrizes WQ, WK e WV\n";
    cout << "- Cabeças de atenção\n";
    cout << "- Transformações das camadas\n";
    cout << "- Probabilidades dos próximos tokens\n";
    cout << "- Respostas cadastradas\n";

    mostrarAviso();
}

void Interface::executar()
{
    int opcao = -1;

    do
    {
        mostrarCabecalho();

        cout << "\n[1] Processar uma frase";
        cout << "\n[0] Sair";
        cout << "\n\nEscolha: ";

        if (!(cin >> opcao))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\nOpção inválida.\n";
            continue;
        }

        cin.ignore(10000, '\n');

        if (opcao == 1)
        {
            processar();
            cout << "\nPressione ENTER para voltar ao menu...";
            cin.get();
        }
        else if (opcao == 0)
        {
            cout << "\nSimulador encerrado.\n";
        }
        else
        {
            cout << "\nOpção inválida.\n";
        }
    }
    while (opcao != 0);
}