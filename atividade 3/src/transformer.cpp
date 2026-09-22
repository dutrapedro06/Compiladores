#include "../include/transformer.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;


// ============================================================
// FUNÇÕES AUXILIARES INTERNAS
// ============================================================

// Converte uma string para letras minúsculas.
// É utilizada apenas para identificar respostas cadastradas.
string converterMinusculo(string texto)
{
    for (char& caractere : texto)
    {
        caractere = tolower(static_cast<unsigned char>(caractere));
    }

    return texto;
}


// Multiplica um vetor por uma matriz.
// É utilizada para gerar Query, Key e Value.
vector<double> multiplicarVetorMatriz(
    const vector<double>& vetor,
    const vector<vector<double>>& matriz)
{
    vector<double> resultado(matriz[0].size(), 0.0);

    for (int coluna = 0; coluna < matriz[0].size(); coluna++)
    {
        for (int linha = 0; linha < vetor.size(); linha++)
        {
            resultado[coluna] += vetor[linha] * matriz[linha][coluna];
        }
    }

    return resultado;
}


// Softmax transforma valores numéricos em uma distribuição
// normalizada, cuja soma é aproximadamente igual a 1.
vector<double> softmax(const vector<double>& valores)
{
    vector<double> resultado;

    if (valores.empty())
    {
        return resultado;
    }

    // Evita problemas numéricos com valores muito grandes.
    double maiorValor = *max_element(valores.begin(), valores.end());

    double soma = 0.0;

    for (double valor : valores)
    {
        double exponencial = exp(valor - maiorValor);

        resultado.push_back(exponencial);
        soma += exponencial;
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
    // O primeiro token criado receberá ID 1.
    proximoId = 1;
}


// ============================================================
// ETAPA 1 - RECEBIMENTO DA FRASE
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
    stringstream fluxo(texto);

    string palavra;
    string textoNormalizado;

    // Remove espaços extras entre as palavras.
    while (fluxo >> palavra)
    {
        if (!textoNormalizado.empty())
        {
            textoNormalizado += " ";
        }

        textoNormalizado += palavra;
    }

    return textoNormalizado;
}


// ============================================================
// ETAPA 3 - TOKENIZAÇÃO
// ============================================================

vector<string> TransformerSimulator::tokenizar(const string& texto)
{
    vector<string> tokens;

    string token;

    for (char caractere : texto)
    {
        // Um espaço encerra o token atual.
        if (caractere == ' ')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }

        // Pontuações são transformadas em tokens separados.
        else if (
            caractere == '.' ||
            caractere == ',' ||
            caractere == '?' ||
            caractere == '!' ||
            caractere == ';' ||
            caractere == ':')
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }

            tokens.push_back(string(1, caractere));
        }

        // Os demais caracteres formam o token atual.
        else
        {
            token += caractere;
        }
    }

    // Adiciona o último token caso ele ainda exista.
    if (!token.empty())
    {
        tokens.push_back(token);
    }

    return tokens;
}


// ============================================================
// ETAPA 4 - CONVERSÃO DOS TOKENS EM IDs
// ============================================================

vector<int> TransformerSimulator::gerarIds(
    const vector<string>& tokens)
{
    vector<int> ids;

    for (const string& token : tokens)
    {
        // Se o token ainda não estiver no vocabulário,
        // cria um novo ID para ele.
        if (vocabulario.find(token) == vocabulario.end())
        {
            vocabulario[token] = proximoId;
            proximoId++;
        }

        // Tokens iguais recebem o mesmo ID.
        ids.push_back(vocabulario[token]);
    }

    return ids;
}


// ============================================================
// ETAPA 5 - EMBEDDINGS
// ============================================================

// Cada ID é transformado em um vetor numérico de 3 dimensões.
//
// IMPORTANTE:
// Estes valores são didáticos e simulados.
// Eles NÃO representam embeddings reais de uma LLM.
vector<vector<double>> TransformerSimulator::gerarEmbeddings(
    const vector<int>& ids)
{
    vector<vector<double>> embeddings;

    for (int id : ids)
    {
        vector<double> embedding;

        // O mesmo ID sempre gera o mesmo embedding.
        embedding.push_back((id % 5 + 1) * 0.1);
        embedding.push_back((id % 7 + 1) * 0.1);
        embedding.push_back((id % 3 + 1) * 0.1);

        embeddings.push_back(embedding);
    }

    return embeddings;
}


// ============================================================
// ETAPA 6 - INFORMAÇÃO DE POSIÇÃO
// ============================================================

vector<vector<double>> TransformerSimulator::adicionarPosicao(
    const vector<vector<double>>& embeddings)
{
    // Fazemos uma cópia para manter os embeddings originais.
    vector<vector<double>> vetoresComPosicao = embeddings;

    for (int posicao = 0;
         posicao < vetoresComPosicao.size();
         posicao++)
    {
        for (int dimensao = 0;
             dimensao < vetoresComPosicao[posicao].size();
             dimensao++)
        {
            // Representação posicional didática simplificada.
            //
            // Tokens em posições diferentes recebem
            // valores diferentes.
            vetoresComPosicao[posicao][dimensao] +=
                (posicao + 1) *
                (dimensao + 1) *
                0.01;
        }
    }

    return vetoresComPosicao;
}


// ============================================================
// ETAPA 7 - QUERY
// ============================================================

vector<vector<double>> TransformerSimulator::gerarQuery(
    const vector<vector<double>>& entrada)
{
    // Matriz WQ didática e simulada.
    vector<vector<double>> WQ = {
        {0.5, 0.2, 0.1},
        {0.1, 0.6, 0.2},
        {0.2, 0.1, 0.7}
    };

    vector<vector<double>> query;

    // Q = X * WQ
    for (const vector<double>& vetor : entrada)
    {
        query.push_back(
            multiplicarVetorMatriz(vetor, WQ)
        );
    }

    return query;
}


// ============================================================
// ETAPA 7 - KEY
// ============================================================

vector<vector<double>> TransformerSimulator::gerarKey(
    const vector<vector<double>>& entrada)
{
    // Matriz WK didática e simulada.
    vector<vector<double>> WK = {
        {0.6, 0.1, 0.2},
        {0.2, 0.7, 0.1},
        {0.1, 0.2, 0.5}
    };

    vector<vector<double>> key;

    // K = X * WK
    for (const vector<double>& vetor : entrada)
    {
        key.push_back(
            multiplicarVetorMatriz(vetor, WK)
        );
    }

    return key;
}


// ============================================================
// ETAPA 7 - VALUE
// ============================================================

vector<vector<double>> TransformerSimulator::gerarValue(
    const vector<vector<double>>& entrada)
{
    // Matriz WV didática e simulada.
    vector<vector<double>> WV = {
        {0.7, 0.2, 0.1},
        {0.1, 0.5, 0.3},
        {0.2, 0.3, 0.6}
    };

    vector<vector<double>> value;

    // V = X * WV
    for (const vector<double>& vetor : entrada)
    {
        value.push_back(
            multiplicarVetorMatriz(vetor, WV)
        );
    }

    return value;
}


// ============================================================
// ETAPA 8 - CÁLCULO DOS PESOS DE ATENÇÃO
// ============================================================

// Calcula:
//
// softmax(Q * K^T / sqrt(dk))
//
// O resultado é uma matriz em que cada linha representa
// os pesos de atenção de um token em relação aos demais.
vector<vector<double>> TransformerSimulator::calcularAtencao(
    const vector<vector<double>>& query,
    const vector<vector<double>>& key)
{
    vector<vector<double>> pesosAtencao;

    if (query.empty() || key.empty())
    {
        return pesosAtencao;
    }

    // Número de dimensões dos vetores Key.
    double dk = key[0].size();

    for (int i = 0; i < query.size(); i++)
    {
        vector<double> pontuacoes;

        for (int j = 0; j < key.size(); j++)
        {
            double produto = 0.0;

            // Produto escalar entre Query e Key.
            for (int k = 0; k < query[i].size(); k++)
            {
                produto += query[i][k] * key[j][k];
            }

            // Scaled Dot-Product Attention.
            produto /= sqrt(dk);

            pontuacoes.push_back(produto);
        }

        // Transforma as pontuações em pesos cuja
        // soma é aproximadamente igual a 1.
        pesosAtencao.push_back(
            softmax(pontuacoes)
        );
    }

    return pesosAtencao;
}


// ============================================================
// ETAPA 9 - COMBINAÇÃO DOS VALUES
// ============================================================

// Depois de calcular os pesos de atenção,
// combinamos os vetores Value:
//
// Attention(Q,K,V) = pesos * V
vector<vector<double>> TransformerSimulator::combinarValores(
    const vector<vector<double>>& pesosAtencao,
    const vector<vector<double>>& value)
{
    vector<vector<double>> resultado;

    if (pesosAtencao.empty() || value.empty())
    {
        return resultado;
    }

    int dimensoes = value[0].size();

    for (int i = 0; i < pesosAtencao.size(); i++)
    {
        vector<double> vetorContextual(
            dimensoes,
            0.0
        );

        for (int j = 0; j < value.size(); j++)
        {
            for (int d = 0; d < dimensoes; d++)
            {
                vetorContextual[d] +=
                    pesosAtencao[i][j] *
                    value[j][d];
            }
        }

        resultado.push_back(vetorContextual);
    }

    return resultado;
}


// ============================================================
// ETAPA 10 - SEGUNDA CABEÇA DE ATENÇÃO
// ============================================================

// Um Transformer utiliza várias cabeças de atenção.
//
// Para a simulação, usamos uma segunda cabeça com matrizes
// diferentes da primeira.
//
// As funções das cabeças são apenas didáticas.
// Em uma LLM real, elas são aprendidas durante o treinamento.
vector<vector<double>> TransformerSimulator::gerarSegundaCabeca(
    const vector<vector<double>>& entrada)
{
    vector<vector<double>> WQ2 = {
        {0.2, 0.7, 0.1},
        {0.6, 0.1, 0.3},
        {0.1, 0.4, 0.8}
    };

    vector<vector<double>> WK2 = {
        {0.3, 0.6, 0.2},
        {0.7, 0.2, 0.1},
        {0.2, 0.3, 0.7}
    };

    vector<vector<double>> WV2 = {
        {0.4, 0.3, 0.5},
        {0.2, 0.8, 0.1},
        {0.6, 0.2, 0.4}
    };

    vector<vector<double>> query2;
    vector<vector<double>> key2;
    vector<vector<double>> value2;

    for (const vector<double>& vetor : entrada)
    {
        query2.push_back(
            multiplicarVetorMatriz(vetor, WQ2)
        );

        key2.push_back(
            multiplicarVetorMatriz(vetor, WK2)
        );

        value2.push_back(
            multiplicarVetorMatriz(vetor, WV2)
        );
    }

    // Calcula os pesos da segunda cabeça.
    vector<vector<double>> pesos2 =
        calcularAtencao(query2, key2);

    // Combina os Values da segunda cabeça.
    return combinarValores(
        pesos2,
        value2
    );
}


// ============================================================
// ETAPA 10 - COMBINAÇÃO DAS CABEÇAS
// ============================================================

// Combina os resultados das duas cabeças.
//
// Em uma arquitetura real existem operações adicionais.
// Aqui usamos a média para manter a simulação compreensível.
vector<vector<double>> TransformerSimulator::combinarCabecas(
    const vector<vector<double>>& cabeca1,
    const vector<vector<double>>& cabeca2)
{
    vector<vector<double>> resultado;

    if (cabeca1.empty())
    {
        return cabeca2;
    }

    if (cabeca2.empty())
    {
        return cabeca1;
    }

    for (int i = 0; i < cabeca1.size(); i++)
    {
        vector<double> vetor;

        for (int j = 0; j < cabeca1[i].size(); j++)
        {
            double valor =
                (cabeca1[i][j] + cabeca2[i][j]) / 2.0;

            vetor.push_back(valor);
        }

        resultado.push_back(vetor);
    }

    return resultado;
}


// ============================================================
// ETAPA 10 - MÚLTIPLAS CAMADAS
// ============================================================

// O desafio exige pelo menos três camadas simuladas.
//
// Cada camada recebe o resultado da camada anterior
// e realiza uma nova transformação.
vector<vector<vector<double>>> TransformerSimulator::processarCamadas(
    const vector<vector<double>>& entrada)
{
    vector<vector<vector<double>>> camadas;

    // --------------------------------------------------------
    // CAMADA 1 - Relações locais
    // --------------------------------------------------------

    vector<vector<double>> camada1 = entrada;

    for (int i = 0; i < camada1.size(); i++)
    {
        for (int j = 0; j < camada1[i].size(); j++)
        {
            // Pequena transformação didática.
            camada1[i][j] *= 1.10;
        }
    }

    camadas.push_back(camada1);


    // --------------------------------------------------------
    // CAMADA 2 - Relações contextuais
    // --------------------------------------------------------

    // A camada 2 recebe a saída da camada 1.
    vector<vector<double>> camada2 = camada1;

    for (int i = 0; i < camada2.size(); i++)
    {
        for (int j = 0; j < camada2[i].size(); j++)
        {
            // Mistura parte da representação do token anterior.
            if (i > 0)
            {
                camada2[i][j] +=
                    camada1[i - 1][j] * 0.10;
            }
        }
    }

    camadas.push_back(camada2);


    // --------------------------------------------------------
    // CAMADA 3 - Preparação da resposta
    // --------------------------------------------------------

    // A camada 3 recebe a saída da camada 2.
    vector<vector<double>> camada3 = camada2;

    for (int i = 0; i < camada3.size(); i++)
    {
        for (int j = 0; j < camada3[i].size(); j++)
        {
            // Última transformação antes da previsão.
            camada3[i][j] =
                camada2[i][j] * 0.90 + 0.05;
        }
    }

    camadas.push_back(camada3);

    return camadas;
}


// ============================================================
// ETAPA 11 - RESPOSTA DIDÁTICA
// ============================================================

// Como não estamos treinando uma LLM real,
// utilizamos um pequeno conjunto de respostas cadastradas.
//
// Isso é permitido na modalidade de simulação didática.
string TransformerSimulator::obterRespostaDidatica(
    const string& texto)
{
    string textoBusca = converterMinusculo(texto);

    // Exemplo principal utilizado no enunciado.
    if (
        textoBusca.find("capital") != string::npos &&
        textoBusca.find("brasil") != string::npos)
    {
        return "A capital do Brasil é Brasília.";
    }

    if (
        textoBusca.find("2 + 2") != string::npos ||
        textoBusca.find("2+2") != string::npos)
    {
        return "2 + 2 é igual a 4.";
    }

    if (
        textoBusca.find("transformer") != string::npos)
    {
        return "Transformer é uma arquitetura baseada em mecanismos de atenção.";
    }

    // Caso o simulador não possua uma resposta cadastrada.
    return "Não tenho uma resposta cadastrada para essa pergunta.";
}


// ============================================================
// ETAPA 11 - PROBABILIDADES DO PRÓXIMO TOKEN
// ============================================================

// Gera probabilidades simuladas para alguns possíveis
// próximos tokens.
//
// A estratégia utilizada será Greedy:
// sempre selecionar o token de maior probabilidade.
vector<pair<string, double>>
TransformerSimulator::calcularProbabilidades(
    const vector<string>& respostaTokens,
    int indiceAtual)
{
    vector<pair<string, double>> probabilidades;

    if (
        indiceAtual < 0 ||
        indiceAtual >= respostaTokens.size())
    {
        probabilidades.push_back(
            {"<FIM>", 1.0}
        );

        return probabilidades;
    }

    string tokenCorreto =
        respostaTokens[indiceAtual];

    // Token esperado recebe a maior probabilidade.
    probabilidades.push_back(
        {tokenCorreto, 0.75}
    );

    // Alternativas simuladas.
    probabilidades.push_back(
        {"modelo", 0.10}
    );

    probabilidades.push_back(
        {"contexto", 0.08}
    );

    probabilidades.push_back(
        {"informação", 0.07}
    );

    return probabilidades;
}


// ============================================================
// ETAPA 12 - SELEÇÃO DO PRÓXIMO TOKEN
// ============================================================

// Estratégia Greedy:
// escolhe sempre o token com maior probabilidade.
string TransformerSimulator::selecionarProximoToken(
    const vector<pair<string, double>>& probabilidades)
{
    if (probabilidades.empty())
    {
        return "<FIM>";
    }

    pair<string, double> melhor =
        probabilidades[0];

    for (const pair<string, double>& candidato :
         probabilidades)
    {
        if (candidato.second > melhor.second)
        {
            melhor = candidato;
        }
    }

    return melhor.first;
}


// ============================================================
// ETAPA 13 - GERAÇÃO AUTOREGRESSIVA
// ============================================================

// Simula a construção da resposta token por token.
//
// A cada passo:
// 1. calcula probabilidades;
// 2. escolhe o próximo token;
// 3. adiciona ao contexto da resposta;
// 4. repete até terminar.
vector<string> TransformerSimulator::gerarRespostaProgressiva(
    const string& resposta)
{
    vector<string> tokensResposta =
        tokenizar(resposta);

    vector<string> etapas;

    string respostaAtual;

    for (int i = 0; i < tokensResposta.size(); i++)
    {
        vector<pair<string, double>> probabilidades =
            calcularProbabilidades(
                tokensResposta,
                i
            );

        string proximoToken =
            selecionarProximoToken(
                probabilidades
            );

        // Pontuação não recebe espaço antes.
        if (
            proximoToken == "." ||
            proximoToken == "," ||
            proximoToken == "?" ||
            proximoToken == "!" ||
            proximoToken == ";" ||
            proximoToken == ":")
        {
            respostaAtual += proximoToken;
        }
        else
        {
            if (!respostaAtual.empty())
            {
                respostaAtual += " ";
            }

            respostaAtual += proximoToken;
        }

        // Salva cada estado da geração.
        etapas.push_back(respostaAtual);
    }

    return etapas;
}


// ============================================================
// FUNÇÕES DE EXIBIÇÃO
// ============================================================

void TransformerSimulator::mostrarTokens(
    const vector<string>& tokens)
{
    cout << "\nTOKENS\n";

    if (tokens.empty())
    {
        cout << "Nenhum token encontrado.\n";
        return;
    }

    for (int i = 0; i < tokens.size(); i++)
    {
        cout
            << "[" << i << "] "
            << tokens[i]
            << "\n";
    }
}


void TransformerSimulator::mostrarIds(
    const vector<string>& tokens,
    const vector<int>& ids)
{
    cout << "\nTOKENS E IDs\n";

    cout
        << left
        << setw(20)
        << "Token"
        << "ID\n";

    cout << "-------------------------\n";

    for (
        int i = 0;
        i < tokens.size() && i < ids.size();
        i++)
    {
        cout
            << left
            << setw(20)
            << tokens[i]
            << ids[i]
            << "\n";
    }
}


void TransformerSimulator::mostrarVetores(
    const vector<string>& tokens,
    const vector<vector<double>>& vetores,
    const string& titulo)
{
    cout << "\n" << titulo << "\n";

    cout << fixed << setprecision(3);

    for (
        int i = 0;
        i < vetores.size();
        i++)
    {
        if (i < tokens.size())
        {
            cout << tokens[i] << " -> ";
        }
        else
        {
            cout << "Token " << i << " -> ";
        }

        cout << "[";

        for (
            int j = 0;
            j < vetores[i].size();
            j++)
        {
            cout << vetores[i][j];

            if (j < vetores[i].size() - 1)
            {
                cout << ", ";
            }
        }

        cout << "]\n";
    }
}


// ============================================================
// EXIBIÇÃO DA MATRIZ DE ATENÇÃO
// ============================================================

void TransformerSimulator::mostrarAtencao(
    const vector<string>& tokens,
    const vector<vector<double>>& pesos)
{
    cout << "\nPESOS DE ATENCAO\n";

    cout << fixed << setprecision(3);

    for (int i = 0; i < pesos.size(); i++)
    {
        cout
            << "\nToken analisado: "
            << tokens[i]
            << "\n";

        double soma = 0.0;

        for (int j = 0; j < pesos[i].size(); j++)
        {
            cout
                << "  -> "
                << tokens[j]
                << ": "
                << pesos[i][j]
                << "\n";

            soma += pesos[i][j];
        }

        cout
            << "Soma dos pesos: "
            << soma
            << "\n";
    }
}


// ============================================================
// EXIBIÇÃO DAS CAMADAS
// ============================================================

void TransformerSimulator::mostrarCamadas(
    const vector<string>& tokens,
    const vector<vector<vector<double>>>& camadas)
{
    for (int i = 0; i < camadas.size(); i++)
    {
        string titulo;

        if (i == 0)
        {
            titulo =
                "CAMADA 1 - RELACOES LOCAIS";
        }
        else if (i == 1)
        {
            titulo =
                "CAMADA 2 - RELACOES CONTEXTUAIS";
        }
        else
        {
            titulo =
                "CAMADA 3 - PREPARACAO DA RESPOSTA";
        }

        mostrarVetores(
            tokens,
            camadas[i],
            titulo
        );
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

    for (const pair<string, double>& candidato :
         probabilidades)
    {
        cout
            << candidato.first
            << " -> "
            << candidato.second * 100
            << "%\n";
    }

    cout
        << "Estrategia de selecao: Greedy "
        << "(maior probabilidade)\n";
}


// ============================================================
// ETAPA 14 - RESUMO FINAL
// ============================================================

void TransformerSimulator::mostrarResumo(
    const string& pergunta,
    const vector<string>& tokensEntrada,
    const vector<string>& tokensGerados,
    const string& respostaFinal)
{
    cout << "\n========================================\n";
    cout << "RESUMO FINAL DA SIMULACAO\n";
    cout << "========================================\n";

    cout
        << "Pergunta original: "
        << pergunta
        << "\n";

    cout
        << "Quantidade de tokens de entrada: "
        << tokensEntrada.size()
        << "\n";

    cout << "Tokens gerados: ";

    for (const string& token : tokensGerados)
    {
        cout << token << " ";
    }

    cout << "\n";

    cout
        << "Etapas principais executadas: 14\n";

    cout
        << "Resposta final: "
        << respostaFinal
        << "\n";

    cout << "\nPARTES SIMULADAS:\n";
    cout << "- Embeddings didaticos\n";
    cout << "- Matrizes WQ, WK e WV\n";
    cout << "- Segunda cabeca de atencao\n";
    cout << "- Transformacoes das camadas\n";
    cout << "- Probabilidades dos proximos tokens\n";
    cout << "- Respostas cadastradas\n";

    cout << "\nPARTES BASEADAS NA ARQUITETURA TRANSFORMER:\n";
    cout << "- Tokenizacao\n";
    cout << "- IDs\n";
    cout << "- Embeddings\n";
    cout << "- Informacao de posicao\n";
    cout << "- Query, Key e Value\n";
    cout << "- Scaled Dot-Product Attention\n";
    cout << "- Softmax\n";
    cout << "- Multiplas cabecas\n";
    cout << "- Multiplas camadas\n";
    cout << "- Previsao do proximo token\n";
    cout << "- Geracao autoregressiva\n";

    cout << "\n";
    cout
        << "Esta e uma simulacao educacional simplificada. "
        << "Os valores nao representam os parametros internos "
        << "de uma LLM comercial.\n";
}