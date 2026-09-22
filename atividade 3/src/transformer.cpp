#include "../include/transformer.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;


// ============================================================
// FUNÇÕES AUXILIARES
// ============================================================

string minusculo(string texto)
{
    for (char& c : texto)
    {
        c = tolower(static_cast<unsigned char>(c));
    }

    return texto;
}


vector<double> multiplicar(
    const vector<double>& vetor,
    const vector<vector<double>>& matriz)
{
    vector<double> resultado(matriz[0].size(), 0.0);

    for (int j = 0; j < matriz[0].size(); j++)
    {
        for (int i = 0; i < vetor.size(); i++)
        {
            resultado[j] += vetor[i] * matriz[i][j];
        }
    }

    return resultado;
}


vector<double> aplicarSoftmax(const vector<double>& valores)
{
    vector<double> resultado;
    double maior = *max_element(valores.begin(), valores.end());
    double soma = 0.0;

    for (double valor : valores)
    {
        double e = exp(valor - maior);
        resultado.push_back(e);
        soma += e;
    }

    for (double& valor : resultado)
    {
        valor /= soma;
    }

    return resultado;
}


// ============================================================
// CONSTRUTOR
// ============================================================

TransformerSimulator::TransformerSimulator()
{
    proximoId = 1;
}


// ============================================================
// ETAPA 1 - ENTRADA
// ============================================================

string TransformerSimulator::receberFrase()
{
    string frase;

    cout << "Digite uma frase ou pergunta: ";
    getline(cin, frase);

    return frase;
}


// ============================================================
// ETAPA 2 - NORMALIZAÇÃO
// ============================================================

string TransformerSimulator::normalizarTexto(const string& texto)
{
    stringstream ss(texto);
    string palavra;
    string resultado;

    while (ss >> palavra)
    {
        if (!resultado.empty())
        {
            resultado += " ";
        }

        resultado += palavra;
    }

    return resultado;
}


// ============================================================
// ETAPA 3 - TOKENIZAÇÃO
// ============================================================

vector<string> TransformerSimulator::tokenizar(const string& texto)
{
    vector<string> tokens;
    string token;

    for (char c : texto)
    {
        if (c == ' ')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (
            c == '.' || c == ',' || c == '?' ||
            c == '!' || c == ';' || c == ':')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }

            tokens.push_back(string(1, c));
        }
        else
        {
            token += c;
        }
    }

    if (!token.empty())
    {
        tokens.push_back(token);
    }

    return tokens;
}


// ============================================================
// ETAPA 4 - TOKENS PARA IDs
// ============================================================

vector<int> TransformerSimulator::gerarIds(
    const vector<string>& tokens)
{
    vector<int> ids;

    for (const string& token : tokens)
    {
        if (vocabulario.find(token) == vocabulario.end())
        {
            vocabulario[token] = proximoId++;
        }

        ids.push_back(vocabulario[token]);
    }

    return ids;
}


// ============================================================
// ETAPA 5 - EMBEDDINGS DIDÁTICOS
// ============================================================

vector<vector<double>> TransformerSimulator::gerarEmbeddings(
    const vector<int>& ids)
{
    vector<vector<double>> embeddings;

    for (int id : ids)
    {
        embeddings.push_back({
            (id % 5 + 1) * 0.1,
            (id % 7 + 1) * 0.1,
            (id % 3 + 1) * 0.1
        });
    }

    return embeddings;
}


// ============================================================
// ETAPA 6 - INFORMAÇÃO DE POSIÇÃO
// ============================================================

vector<vector<double>> TransformerSimulator::adicionarPosicao(
    const vector<vector<double>>& embeddings)
{
    vector<vector<double>> resultado = embeddings;

    for (int i = 0; i < resultado.size(); i++)
    {
        for (int j = 0; j < resultado[i].size(); j++)
        {
            resultado[i][j] += (i + 1) * (j + 1) * 0.01;
        }
    }

    return resultado;
}


// ============================================================
// ETAPA 7 - QUERY
// ============================================================

vector<vector<double>> TransformerSimulator::gerarQuery(
    const vector<vector<double>>& entrada)
{
    vector<vector<double>> WQ = {
        {0.5, 0.2, 0.1},
        {0.1, 0.6, 0.2},
        {0.2, 0.1, 0.7}
    };

    vector<vector<double>> resultado;

    for (const vector<double>& vetor : entrada)
    {
        resultado.push_back(multiplicar(vetor, WQ));
    }

    return resultado;
}


// ============================================================
// ETAPA 7 - KEY
// ============================================================

vector<vector<double>> TransformerSimulator::gerarKey(
    const vector<vector<double>>& entrada)
{
    vector<vector<double>> WK = {
        {0.6, 0.1, 0.2},
        {0.2, 0.7, 0.1},
        {0.1, 0.2, 0.5}
    };

    vector<vector<double>> resultado;

    for (const vector<double>& vetor : entrada)
    {
        resultado.push_back(multiplicar(vetor, WK));
    }

    return resultado;
}


// ============================================================
// ETAPA 7 - VALUE
// ============================================================

vector<vector<double>> TransformerSimulator::gerarValue(
    const vector<vector<double>>& entrada)
{
    vector<vector<double>> WV = {
        {0.7, 0.2, 0.1},
        {0.1, 0.5, 0.3},
        {0.2, 0.3, 0.6}
    };

    vector<vector<double>> resultado;

    for (const vector<double>& vetor : entrada)
    {
        resultado.push_back(multiplicar(vetor, WV));
    }

    return resultado;
}


// ============================================================
// ETAPA 8 - PESOS DE ATENÇÃO
// softmax(QK^T / sqrt(dk))
// ============================================================

vector<vector<double>> TransformerSimulator::calcularAtencao(
    const vector<vector<double>>& query,
    const vector<vector<double>>& key)
{
    vector<vector<double>> pesos;

    if (query.empty() || key.empty())
    {
        return pesos;
    }

    double dk = key[0].size();

    for (int i = 0; i < query.size(); i++)
    {
        vector<double> pontuacoes;

        for (int j = 0; j < key.size(); j++)
        {
            double produto = 0.0;

            for (int k = 0; k < query[i].size(); k++)
            {
                produto += query[i][k] * key[j][k];
            }

            pontuacoes.push_back(produto / sqrt(dk));
        }

        pesos.push_back(aplicarSoftmax(pontuacoes));
    }

    return pesos;
}


// ============================================================
// ETAPA 9 - COMBINAÇÃO DOS VALUES
// Attention(Q,K,V) = pesos * V
// ============================================================

vector<vector<double>> TransformerSimulator::combinarValores(
    const vector<vector<double>>& pesos,
    const vector<vector<double>>& value)
{
    vector<vector<double>> resultado;

    if (pesos.empty() || value.empty())
    {
        return resultado;
    }

    for (int i = 0; i < pesos.size(); i++)
    {
        vector<double> vetor(value[0].size(), 0.0);

        for (int j = 0; j < value.size(); j++)
        {
            for (int k = 0; k < value[j].size(); k++)
            {
                vetor[k] += pesos[i][j] * value[j][k];
            }
        }

        resultado.push_back(vetor);
    }

    return resultado;
}


// ============================================================
// ETAPA 10 - SEGUNDA CABEÇA DE ATENÇÃO
// ============================================================

vector<vector<double>> TransformerSimulator::gerarSegundaCabeca(
    const vector<vector<double>>& entrada)
{
    vector<vector<double>> WQ = {
        {0.2, 0.7, 0.1},
        {0.6, 0.1, 0.3},
        {0.1, 0.4, 0.8}
    };

    vector<vector<double>> WK = {
        {0.3, 0.6, 0.2},
        {0.7, 0.2, 0.1},
        {0.2, 0.3, 0.7}
    };

    vector<vector<double>> WV = {
        {0.4, 0.3, 0.5},
        {0.2, 0.8, 0.1},
        {0.6, 0.2, 0.4}
    };

    vector<vector<double>> Q;
    vector<vector<double>> K;
    vector<vector<double>> V;

    for (const vector<double>& vetor : entrada)
    {
        Q.push_back(multiplicar(vetor, WQ));
        K.push_back(multiplicar(vetor, WK));
        V.push_back(multiplicar(vetor, WV));
    }

    return combinarValores(
        calcularAtencao(Q, K),
        V
    );
}


// ============================================================
// ETAPA 10 - COMBINAÇÃO DAS DUAS CABEÇAS
// ============================================================

vector<vector<double>> TransformerSimulator::combinarCabecas(
    const vector<vector<double>>& cabeca1,
    const vector<vector<double>>& cabeca2)
{
    if (cabeca1.empty())
    {
        return cabeca2;
    }

    if (cabeca2.empty())
    {
        return cabeca1;
    }

    vector<vector<double>> resultado = cabeca1;

    for (int i = 0; i < resultado.size(); i++)
    {
        for (int j = 0; j < resultado[i].size(); j++)
        {
            resultado[i][j] =
                (cabeca1[i][j] + cabeca2[i][j]) / 2.0;
        }
    }

    return resultado;
}


// ============================================================
// ETAPA 10 - TRÊS CAMADAS SIMULADAS
// ============================================================

vector<vector<vector<double>>> TransformerSimulator::processarCamadas(
    const vector<vector<double>>& entrada)
{
    vector<vector<vector<double>>> camadas;

    // Camada 1 - relações locais
    vector<vector<double>> camada1 = entrada;

    for (vector<double>& vetor : camada1)
    {
        for (double& valor : vetor)
        {
            valor *= 1.10;
        }
    }

    camadas.push_back(camada1);


    // Camada 2 - relações contextuais
    vector<vector<double>> camada2 = camada1;

    for (int i = 1; i < camada2.size(); i++)
    {
        for (int j = 0; j < camada2[i].size(); j++)
        {
            camada2[i][j] += camada1[i - 1][j] * 0.10;
        }
    }

    camadas.push_back(camada2);


    // Camada 3 - preparação da resposta
    vector<vector<double>> camada3 = camada2;

    for (vector<double>& vetor : camada3)
    {
        for (double& valor : vetor)
        {
            valor = valor * 0.90 + 0.05;
        }
    }

    camadas.push_back(camada3);

    return camadas;
}


// ============================================================
// RESPOSTAS DIDÁTICAS
// ============================================================

string TransformerSimulator::obterRespostaDidatica(
    const string& texto)
{
    string busca = minusculo(texto);

    if (
        busca.find("capital") != string::npos &&
        busca.find("brasil") != string::npos)
    {
        return "A capital do Brasil é Brasília.";
    }

    if (
        busca.find("2+2") != string::npos ||
        busca.find("2 + 2") != string::npos)
    {
        return "2 + 2 é igual a 4.";
    }

    if (busca.find("transformer") != string::npos)
    {
        return "Transformer é uma arquitetura baseada em mecanismos de atenção.";
    }

    return "Não tenho uma resposta cadastrada para essa pergunta.";
}


// ============================================================
// ETAPA 11 - PROBABILIDADES
// ============================================================

vector<pair<string, double>>
TransformerSimulator::calcularProbabilidades(
    const vector<string>& respostaTokens,
    int indiceAtual)
{
    if (
        indiceAtual < 0 ||
        indiceAtual >= respostaTokens.size())
    {
        return {{"<FIM>", 1.0}};
    }

    return {
        {respostaTokens[indiceAtual], 0.75},
        {"modelo", 0.10},
        {"contexto", 0.08},
        {"informação", 0.07}
    };
}


// ============================================================
// ETAPA 12 - SELEÇÃO GREEDY
// ============================================================

string TransformerSimulator::selecionarProximoToken(
    const vector<pair<string, double>>& probabilidades)
{
    if (probabilidades.empty())
    {
        return "<FIM>";
    }

    pair<string, double> melhor = probabilidades[0];

    for (const pair<string, double>& candidato : probabilidades)
    {
        if (candidato.second > melhor.second)
        {
            melhor = candidato;
        }
    }

    return melhor.first;
}


// ============================================================
// ETAPA 13 - GERAÇÃO PROGRESSIVA
// ============================================================

vector<string> TransformerSimulator::gerarRespostaProgressiva(
    const string& resposta)
{
    vector<string> respostaTokens = tokenizar(resposta);
    vector<string> etapas;

    string contexto;

    for (int i = 0; i < respostaTokens.size(); i++)
    {
        // Reprocessa o contexto disponível a cada novo token.
        if (!contexto.empty())
        {
            vector<string> tokensContexto = tokenizar(contexto);
            vector<int> ids = gerarIds(tokensContexto);

            vector<vector<double>> entrada =
                adicionarPosicao(gerarEmbeddings(ids));

            vector<vector<double>> Q = gerarQuery(entrada);
            vector<vector<double>> K = gerarKey(entrada);
            vector<vector<double>> V = gerarValue(entrada);

            vector<vector<double>> cabeca1 =
                combinarValores(calcularAtencao(Q, K), V);

            vector<vector<double>> cabeca2 =
                gerarSegundaCabeca(entrada);

            vector<vector<double>> multiHead =
                combinarCabecas(cabeca1, cabeca2);

            processarCamadas(multiHead);
        }

        vector<pair<string, double>> probs =
            calcularProbabilidades(respostaTokens, i);

        string token =
            selecionarProximoToken(probs);

        bool pontuacao =
            token == "." || token == "," ||
            token == "?" || token == "!" ||
            token == ";" || token == ":";

        if (!contexto.empty() && !pontuacao)
        {
            contexto += " ";
        }

        contexto += token;

        etapas.push_back(contexto);
    }

    return etapas;
}


// ============================================================
// EXIBIÇÃO DOS TOKENS
// ============================================================

void TransformerSimulator::mostrarTokens(
    const vector<string>& tokens)
{
    cout << "\nTOKENS\n";

    for (int i = 0; i < tokens.size(); i++)
    {
        cout << "[" << i << "] " << tokens[i] << "\n";
    }
}


// ============================================================
// EXIBIÇÃO DOS IDs
// ============================================================

void TransformerSimulator::mostrarIds(
    const vector<string>& tokens,
    const vector<int>& ids)
{
    cout << "\nTOKENS E IDs\n";
    cout << left << setw(20) << "Token" << "ID\n";

    for (int i = 0; i < tokens.size(); i++)
    {
        cout << left << setw(20)
             << tokens[i]
             << ids[i]
             << "\n";
    }
}


// ============================================================
// EXIBIÇÃO DOS VETORES
// ============================================================

void TransformerSimulator::mostrarVetores(
    const vector<string>& tokens,
    const vector<vector<double>>& vetores,
    const string& titulo)
{
    cout << "\n" << titulo << "\n";
    cout << fixed << setprecision(3);

    for (int i = 0; i < vetores.size(); i++)
    {
        cout << tokens[i] << " -> [";

        for (int j = 0; j < vetores[i].size(); j++)
        {
            cout << vetores[i][j];

            if (j + 1 < vetores[i].size())
            {
                cout << ", ";
            }
        }

        cout << "]\n";
    }
}


// ============================================================
// EXIBIÇÃO DA ATENÇÃO
// ============================================================

void TransformerSimulator::mostrarAtencao(
    const vector<string>& tokens,
    const vector<vector<double>>& pesos)
{
    cout << "\nPESOS DE ATENCAO\n";
    cout << fixed << setprecision(3);

    for (int i = 0; i < pesos.size(); i++)
    {
        cout << "\n" << tokens[i] << ":\n";

        double soma = 0.0;

        for (int j = 0; j < pesos[i].size(); j++)
        {
            cout << "  -> "
                 << tokens[j]
                 << ": "
                 << pesos[i][j]
                 << "\n";

            soma += pesos[i][j];
        }

        cout << "Soma: " << soma << "\n";
    }
}


// ============================================================
// EXIBIÇÃO DAS TRÊS CAMADAS
// ============================================================

void TransformerSimulator::mostrarCamadas(
    const vector<string>& tokens,
    const vector<vector<vector<double>>>& camadas)
{
    vector<string> nomes = {
        "CAMADA 1 - RELACOES LOCAIS",
        "CAMADA 2 - RELACOES CONTEXTUAIS",
        "CAMADA 3 - PREPARACAO DA RESPOSTA"
    };

    for (int i = 0; i < camadas.size(); i++)
    {
        mostrarVetores(tokens, camadas[i], nomes[i]);
    }
}


// ============================================================
// EXIBIÇÃO DAS PROBABILIDADES
// ============================================================

void TransformerSimulator::mostrarProbabilidades(
    const vector<pair<string, double>>& probabilidades)
{
    cout << "\nPROBABILIDADES DO PROXIMO TOKEN\n";
    cout << fixed << setprecision(2);

    for (const pair<string, double>& candidato : probabilidades)
    {
        cout << candidato.first
             << " -> "
             << candidato.second * 100
             << "%\n";
    }

    cout << "Estrategia: Greedy\n";
}


// ============================================================
// ETAPA 14 - RESUMO
// ============================================================

void TransformerSimulator::mostrarResumo(
    const string& pergunta,
    const vector<string>& tokensEntrada,
    const vector<string>& tokensGerados,
    const string& respostaFinal)
{
    cout << "\n========================================\n";
    cout << "RESUMO FINAL\n";
    cout << "========================================\n";

    cout << "Pergunta: " << pergunta << "\n";
    cout << "Tokens de entrada: "
         << tokensEntrada.size()
         << "\n";

    cout << "Tokens gerados: ";

    for (const string& token : tokensGerados)
    {
        cout << token << " ";
    }

    cout << "\nResposta final: "
         << respostaFinal
         << "\n";

    cout << "\nSimulacao educacional simplificada.\n";
    cout << "Embeddings, matrizes, camadas, probabilidades "
         << "e respostas sao didaticos.\n";

    cout << "Os valores nao representam parametros internos "
         << "de uma LLM comercial.\n";
}