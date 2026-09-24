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
    // unordered_map armazena informações no formato chave -> valor.
    // Aqui a chave é uma string (o token) e o valor é um int (seu ID).
    // Exemplo: "Brasil" -> 5
    unordered_map<string, int> vocabulario;

    int proximoId;

public:
    // Construtor da classe.
    // É chamado quando um objeto TransformerSimulator é criado.
    // No transformer.cpp ele inicializa proximoId com 1.
    TransformerSimulator();

    // ========================================================
    // ETAPAS 1 A 6 - PREPARAÇÃO DA ENTRADA
    // ========================================================

    string receberFrase();

    // Recebe o texto e devolve sua versão normalizada.
    string normalizarTexto(const string& texto);

    // Cada string será um token.
    vector<string> tokenizar(const string& texto);

    // Recebe os tokens e retorna seus respectivos IDs.
    vector<int> gerarIds(const vector<string>& tokens);

    // Recebe os IDs e gera os embeddings didáticos.
    // Cada vetor interno representa o embedding de um token.
    vector<vector<double>> gerarEmbeddings(const vector<int>& ids);

    // Recebe os embeddings e adiciona informação de posição.
    vector<vector<double>> adicionarPosicao(const vector<vector<double>>& embeddings);


    // ========================================================
    // ETAPA 7 - QUERY, KEY E VALUE
    // ========================================================

    // Gera os vetores Query a partir da entrada.
    // Query representa, de forma didática, o que cada token procura
    // nas outras posições durante o cálculo da atenção.
    vector<vector<double>> gerarQuery(const vector<vector<double>>& entrada);

    // Gera os vetores Key.
    // Key é usado para comparar a relevância dos tokens em relação às Queries.
    vector<vector<double>> gerarKey(const vector<vector<double>>& entrada);

    // Gera os vetores Value.
    // Value contém as informações que serão combinadas
    // utilizando os pesos calculados pela atenção.
    vector<vector<double>> gerarValue(const vector<vector<double>>& entrada);


    // ========================================================
    // ETAPAS 8 E 9 - MECANISMO DE ATENÇÃO
    // ========================================================

    // Calcula os pesos de atenção comparando Query e Key.
    // O resultado é uma matriz de pesos.
    vector<vector<double>> calcularAtencao(const vector<vector<double>>& query,const vector<vector<double>>& key);

    // Combina os vetores Value de acordo com os pesos de atenção.
    // Tokens que receberam pesos maiores terão maior influência no resultado.
    vector<vector<double>> combinarValores(const vector<vector<double>>& pesos,const vector<vector<double>>& value);


    // ========================================================
    // ETAPA 10 - MÚLTIPLAS CABEÇAS E CAMADAS
    // ========================================================

    // Gera uma segunda cabeça de atenção utilizando
    // matrizes diferentes das utilizadas na primeira cabeça.
    vector<vector<double>> gerarSegundaCabeca(const vector<vector<double>>& entrada);

    // Recebe o resultado das duas cabeças de atenção
    // e combina os dois resultados.
    vector<vector<double>> combinarCabecas(const vector<vector<double>>& cabeca1,const vector<vector<double>>& cabeca2);

    // Simula a passagem da informação por três camadas.

    // Aqui existem três vector:
    // vector<vector<vector<double>>>
    //
    // O primeiro nível representa as CAMADAS.
    // O segundo nível representa os TOKENS dentro de cada camada.
    // O terceiro nível representa os VALORES do vetor de cada token.
    //
    // Exemplo simplificado:
    // camadas
    //   -> camada 1
    //        -> token 1 -> [0.1, 0.2, 0.3]
    //        -> token 2 -> [0.4, 0.5, 0.6]
    //   -> camada 2
    //        -> token 1 -> [...]
    vector<vector<vector<double>>> processarCamadas(const vector<vector<double>>& entrada);


    // ========================================================
    // ETAPAS 11 A 13 - GERAÇÃO DA RESPOSTA
    // ========================================================

    // Procura uma resposta didática cadastrada para o texto recebido.
    // No simulador, essa resposta não é produzida por uma LLM real.
    string obterRespostaDidatica(const string& texto);

    // Calcula os candidatos para o próximo token e suas probabilidades.
    //vetor contendo vários pares de token + probabilidade.
    vector<pair<string, double>> calcularProbabilidades(const vector<string>& respostaTokens,int indiceAtual);

    // Recebe os candidatos e seleciona o próximo token.
    // No transformer.cpp é utilizada a estratégia Greedy,
    // que escolhe sempre o candidato de maior probabilidade.
    string selecionarProximoToken(const vector<pair<string, double>>& probabilidades);

    // Gera a resposta progressivamente, token por token.
    // vector<string> guarda cada estágio da resposta.
    //
    // Exemplo:
    // "A"
    // "A capital"
    // "A capital do"
    // ...
    vector<string> gerarRespostaProgressiva(const string& resposta);


    // ========================================================
    // EXIBIÇÃO DOS RESULTADOS
    // ========================================================

    // Mostra os tokens encontrados.
    void mostrarTokens(const vector<string>& tokens);

    void mostrarIds(const vector<string>& tokens, const vector<int>& ids);

    // Mostra vetores numéricos na tela.
    // Essa função pode ser reutilizada para embeddings, Q, K, V,
    // cabeças de atenção e camadas.
    void mostrarVetores(const vector<string>& tokens, const vector<vector<double>>& vetores, const string& titulo);

    // Mostra os pesos de atenção calculados entre os tokens.
    void mostrarAtencao(const vector<string>& tokens, const vector<vector<double>>& pesos);

    // Mostra os resultados das três camadas simuladas.
    void mostrarCamadas(const vector<string>& tokens, const vector<vector<vector<double>>>& camadas);

    // Mostra os candidatos a próximo token e suas probabilidades.
    void mostrarProbabilidades(const vector<pair<string, double>>& probabilidades);


    // ========================================================
    // ETAPA 14 - RESUMO FINAL
    // ========================================================

    // Mostra as principais informações ao final da simulação:
    // pergunta original, tokens de entrada, tokens gerados
    // e resposta final.
    void mostrarResumo(const string& pergunta, const vector<string>& tokensEntrada, const vector<string>& tokensGerados, const string& respostaFinal);
};

#endif
