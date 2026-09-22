#ifndef TRANSFORMER_HPP
#define TRANSFORMER_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

class TransformerSimulator
{
private:
    unordered_map<string, int> vocabulario;
    int proximoId;

public:
    TransformerSimulator();

    // Etapas 1 a 6 - preparação da entrada
    string receberFrase();
    string normalizarTexto(const string& texto);
    vector<string> tokenizar(const string& texto);
    vector<int> gerarIds(const vector<string>& tokens);
    vector<vector<double>> gerarEmbeddings(const vector<int>& ids);
    vector<vector<double>> adicionarPosicao(
        const vector<vector<double>>& embeddings
    );

    // Etapa 7 - Query, Key e Value
    vector<vector<double>> gerarQuery(
        const vector<vector<double>>& entrada
    );

    vector<vector<double>> gerarKey(
        const vector<vector<double>>& entrada
    );

    vector<vector<double>> gerarValue(
        const vector<vector<double>>& entrada
    );

    // Etapas 8 e 9 - mecanismo de atenção
    vector<vector<double>> calcularAtencao(
        const vector<vector<double>>& query,
        const vector<vector<double>>& key
    );

    vector<vector<double>> combinarValores(
        const vector<vector<double>>& pesos,
        const vector<vector<double>>& value
    );

    // Etapa 10 - múltiplas cabeças e camadas
    vector<vector<double>> gerarSegundaCabeca(
        const vector<vector<double>>& entrada
    );

    vector<vector<double>> combinarCabecas(
        const vector<vector<double>>& cabeca1,
        const vector<vector<double>>& cabeca2
    );

    vector<vector<vector<double>>> processarCamadas(
        const vector<vector<double>>& entrada
    );

    // Etapas 11 a 13 - geração da resposta
    string obterRespostaDidatica(const string& texto);

    vector<pair<string, double>> calcularProbabilidades(
        const vector<string>& respostaTokens,
        int indiceAtual
    );

    string selecionarProximoToken(
        const vector<pair<string, double>>& probabilidades
    );

    vector<string> gerarRespostaProgressiva(
        const string& resposta
    );

    // Exibição dos resultados
    void mostrarTokens(const vector<string>& tokens);

    void mostrarIds(
        const vector<string>& tokens,
        const vector<int>& ids
    );

    void mostrarVetores(
        const vector<string>& tokens,
        const vector<vector<double>>& vetores,
        const string& titulo
    );

    void mostrarAtencao(
        const vector<string>& tokens,
        const vector<vector<double>>& pesos
    );

    void mostrarCamadas(
        const vector<string>& tokens,
        const vector<vector<vector<double>>>& camadas
    );

    void mostrarProbabilidades(
        const vector<pair<string, double>>& probabilidades
    );

    // Etapa 14 - resumo final
    void mostrarResumo(
        const string& pergunta,
        const vector<string>& tokensEntrada,
        const vector<string>& tokensGerados,
        const string& respostaFinal
    );
};

#endif