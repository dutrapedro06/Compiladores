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

// Recebe uma string e devolve essa mesma string com todos os caracteres em minúsculo.
string minusculo(string texto)
{
    // Para cada caractere c que está dentro de texto, faça...
    for (char& c : texto) // : percorre os elementos de texto. & faz com que c seja uma referência ao caractere original da string
    {
        c = tolower(static_cast<unsigned char>(c)); // transforma cada caractere em minúsculo
        // static_cast<unsigned char> converte temporariamente c para unsigned char antes de passar para tolower()
    }

    return texto;
}


// Multiplica um vetor por uma matriz.
// Essa operação é utilizada depois para gerar Query, Key e Value.
vector<double> multiplicar(const vector<double>& vetor, const vector<vector<double>>& matriz)
{
    // Cria o vetor resultado com a quantidade de colunas da matriz.
    // Todos os valores começam em 0.0.
    vector<double> resultado(matriz[0].size(), 0.0);

    for (int j = 0; j < matriz[0].size(); j++)
    {
        for (int i = 0; i < vetor.size(); i++)
        {
            // Multiplica cada valor do vetor pelo valor correspondente da matriz
            // e acumula o resultado na posição j.
            resultado[j] += vetor[i] * matriz[i][j];
        }
    }

    return resultado;
}


// Aplica Softmax em um vetor de valores.
// O Softmax transforma pontuações em valores normalizados cuja soma é aproximadamente 1.
vector<double> aplicarSoftmax(const vector<double>& valores)
{
    vector<double> resultado;

    double maior = *max_element(valores.begin(), valores.end());

    double soma = 0.0;

    // Para cada valor dentro do vetor valores...
    for (double valor : valores) // : percorre cada elemento do vetor
    {
        // exp calcula a função exponencial.
        // Subtrair o maior valor ajuda a evitar números excessivamente grandes.
        double e = exp(valor - maior);

        resultado.push_back(e); // push_back adiciona e ao final do vetor
        soma += e;
    }

    // Percorre os valores que acabaram de ser calculados.
    for (double& valor : resultado) // & permite alterar diretamente o valor que está dentro do vetor
    {
        // Divide cada valor pela soma total para normalizar os resultados.
        valor /= soma;
    }

    return resultado;
}


// ============================================================
// CONSTRUTOR
// ============================================================

// :: indica que TransformerSimulator() pertence à classe TransformerSimulator.
// Essa classe foi declarada no arquivo transformer.hpp.
TransformerSimulator::TransformerSimulator()
{
    // O primeiro token novo cadastrado no vocabulário receberá o ID 1.
    proximoId = 1;
}


// ============================================================
// ETAPA 1 - ENTRADA
// ============================================================

// Recebe a frase ou pergunta digitada pelo usuário.
string TransformerSimulator::receberFrase()
{
    string frase;

    cout << "Digite uma frase ou pergunta: ";

    // getline lê a linha inteira digitada pelo usuário, incluindo os espaços.
    getline(cin, frase);

    return frase;
}


// ============================================================
// ETAPA 2 - NORMALIZAÇÃO
// ============================================================

// Normaliza o texto removendo espaços extras entre as palavras.
string TransformerSimulator::normalizarTexto(const string& texto)
{
    // stringstream permite tratar uma string como um fluxo de dados.
    stringstream ss(texto);

    string palavra;
    string resultado;

    // ss >> palavra lê uma palavra por vez.
    // Espaços extras são ignorados automaticamente pelo operador >>.
    while (ss >> palavra)
    {
        // Se resultado já possui alguma palavra, adiciona um espaço antes da próxima.
        if (!resultado.empty()) // ! significa negação: entra se resultado NÃO estiver vazio
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

// Divide o texto em tokens.
vector<string> TransformerSimulator::tokenizar(const string& texto)
{
    vector<string> tokens;
    string token;

    // Percorre cada caractere c existente no texto.
    for (char c : texto) // : percorre os elementos de texto. Como não usamos &, c recebe uma cópia do caractere
    {
        // Se encontrou um espaço, significa que o token atual terminou.
        if (c == ' ')
        {
            if (!token.empty())
            {
                tokens.push_back(token); // adiciona o token ao vetor
                token.clear();           // limpa a variável para começar o próximo token
            }
        }

        // Verifica se o caractere atual é uma pontuação reconhecida pelo simulador.
        else if (c == '.' || c == ',' || c == '?' || c == '!' || c == ';' || c == ':')
        {
            // Se já havia uma palavra sendo formada, adiciona essa palavra primeiro.
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }

            // string(1, c) cria uma string contendo apenas 1 ocorrência do caractere c.
            // Assim, a pontuação também vira um token.
            tokens.push_back(string(1, c));
        }
        else
        {
            // Se não for espaço nem pontuação, adiciona o caractere ao token atual.
            token += c;
        }
    }

    // Se o texto terminou sem espaço ou pontuação, ainda pode existir um token pendente.
    if (!token.empty())
    {
        tokens.push_back(token);
    }

    return tokens;
}


// ============================================================
// ETAPA 4 - TOKENS PARA IDs
// ============================================================

// Associa cada token a um número inteiro.
// Se o token já estiver no vocabulário, reutiliza seu ID.
vector<int> TransformerSimulator::gerarIds(
    const vector<string>& tokens)
{
    vector<int> ids;

    // Para cada string token existente no vetor tokens...
    for (const string& token : tokens)

    {
        if (vocabulario.find(token) == vocabulario.end())
        {
            vocabulario[token] = proximoId++;
        }

        // Adiciona o ID correspondente ao vetor de IDs.
        ids.push_back(vocabulario[token]);
    }

    return ids;
}


// ============================================================
// ETAPA 5 - EMBEDDINGS DIDÁTICOS
// ============================================================

// Converte cada ID em um vetor numérico de três dimensões.
// Os valores são didáticos e não representam embeddings de uma LLM real.
vector<vector<double>> TransformerSimulator::gerarEmbeddings(
    const vector<int>& ids)
{
    vector<vector<double>> embeddings;

    // Para cada ID existente no vetor ids...
    for (int id : ids)
    {
        // Cria um embedding com três números calculados a partir do ID.
        embeddings.push_back({(id % 5 + 1) * 0.1,(id % 7 + 1) * 0.1,(id % 3 + 1) * 0.1});
    }

    return embeddings;
}


// ============================================================
// ETAPA 6 - INFORMAÇÃO DE POSIÇÃO
// ============================================================

// Adiciona informação de posição aos embeddings.
// Isso permite que tokens iguais em posições diferentes tenham representações diferentes.
vector<vector<double>> TransformerSimulator::adicionarPosicao(
    const vector<vector<double>>& embeddings)
{
    // Cria uma cópia dos embeddings para poder modificá-la.
    vector<vector<double>> resultado = embeddings;

    // i representa a posição do token.
    for (int i = 0; i < resultado.size(); i++)
    {
        // j percorre cada dimensão do embedding desse token.
        for (int j = 0; j < resultado[i].size(); j++)
        {
            // Adiciona um pequeno valor calculado usando a posição do token
            // e a posição dentro do próprio vetor.
            resultado[i][j] += (i + 1) * (j + 1) * 0.01;
        }
    }

    return resultado;
}


// ============================================================
// ETAPA 7 - QUERY
// ============================================================

// Gera os vetores Query.
// Query representa, de forma didática, o que cada token procura nos demais tokens.
vector<vector<double>> TransformerSimulator::gerarQuery(const vector<vector<double>>& entrada)
{
    // WQ é a matriz utilizada para transformar os vetores de entrada em Query.
    vector<vector<double>> WQ = {
        {0.5, 0.2, 0.1},
        {0.1, 0.6, 0.2},
        {0.2, 0.1, 0.7}
    };

    vector<vector<double>> resultado;

    // Para cada vetor presente na entrada...
    for (const vector<double>& vetor : entrada)
    {
        // Multiplica o vetor pela matriz WQ e guarda a Query produzida.
        resultado.push_back(multiplicar(vetor, WQ));
    }

    return resultado;
}


// ============================================================
// ETAPA 7 - KEY
// ============================================================

// Gera os vetores Key.
// Key representa a informação utilizada para verificar a relevância de cada token.
vector<vector<double>> TransformerSimulator::gerarKey(
    const vector<vector<double>>& entrada)
{
    // WK é a matriz utilizada para transformar a entrada em Key.
    vector<vector<double>> WK = {{0.6, 0.1, 0.2},{0.2, 0.7, 0.1},{0.1, 0.2, 0.5}};

    vector<vector<double>> resultado;

    for (const vector<double>& vetor : entrada)
    {
        // Multiplica cada vetor de entrada pela matriz WK.
        resultado.push_back(multiplicar(vetor, WK));
    }

    return resultado;
}


// ============================================================
// ETAPA 7 - VALUE
// ============================================================

// Gera os vetores Value.
// Value representa a informação que será combinada de acordo com os pesos de atenção.
vector<vector<double>> TransformerSimulator::gerarValue(
    const vector<vector<double>>& entrada)
{
    // WV é a matriz utilizada para transformar a entrada em Value.
    vector<vector<double>> WV = {{0.7, 0.2, 0.1},{0.1, 0.5, 0.3},{0.2, 0.3, 0.6}};

    vector<vector<double>> resultado;

    for (const vector<double>& vetor : entrada)
    {
        // Multiplica cada vetor de entrada pela matriz WV.
        resultado.push_back(multiplicar(vetor, WV));
    }

    return resultado;
}


// ============================================================
// ETAPA 8 - PESOS DE ATENÇÃO
// softmax(QK^T / sqrt(dk))
// ============================================================

// Calcula quanto cada token deve prestar atenção nos demais tokens.
vector<vector<double>> TransformerSimulator::calcularAtencao(const vector<vector<double>>& query,const vector<vector<double>>& key)
{
    vector<vector<double>> pesos;

    // Se Query ou Key estiverem vazios, não existe atenção para calcular.
    if (query.empty() || key.empty())
    {
        return pesos;
    }

    // dk representa a quantidade de dimensões dos vetores Key.
    double dk = key[0].size();

    // i percorre cada vetor Query.
    for (int i = 0; i < query.size(); i++)
    {
        vector<double> pontuacoes;

        // j percorre cada vetor Key.
        // Assim, cada Query será comparada com todas as Keys.
        for (int j = 0; j < key.size(); j++)
        {
            double produto = 0.0;

            // Calcula o produto escalar entre Query[i] e Key[j].
            for (int k = 0; k < query[i].size(); k++)
            {
                produto += query[i][k] * key[j][k];
            }

            // Divide o resultado pela raiz de dk, como na fórmula de atenção.
            pontuacoes.push_back(produto / sqrt(dk));
        }

        // Softmax transforma as pontuações em pesos normalizados.
        // A soma dos pesos será aproximadamente 1.
        pesos.push_back(aplicarSoftmax(pontuacoes));
    }

    return pesos;
}


// ============================================================
// ETAPA 9 - COMBINAÇÃO DOS VALUES
// Attention(Q,K,V) = pesos * V
// ============================================================

// Combina os vetores Value utilizando os pesos de atenção calculados.
vector<vector<double>> TransformerSimulator::combinarValores(const vector<vector<double>>& pesos,const vector<vector<double>>& value)
{
    vector<vector<double>> resultado;

    // Se não houver pesos ou Values, não existe combinação para realizar.
    if (pesos.empty() || value.empty())
    {
        return resultado;
    }

    // i percorre cada token analisado.
    for (int i = 0; i < pesos.size(); i++)
    {
        // Cria um vetor com o mesmo tamanho dos Values, inicialmente preenchido com 0.
        vector<double> vetor(value[0].size(), 0.0);

        // j percorre os tokens que receberam pesos de atenção.
        for (int j = 0; j < value.size(); j++)
        {
            // k percorre cada dimensão do vetor Value.
            for (int k = 0; k < value[j].size(); k++)
            {
                // Multiplica o Value pelo peso de atenção correspondente.
                // Quanto maior o peso, maior a influência desse Value no resultado.
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

// Simula uma segunda cabeça de atenção utilizando matrizes diferentes.
// Cabeças diferentes podem produzir relações diferentes entre os tokens.
vector<vector<double>> TransformerSimulator::gerarSegundaCabeca(const vector<vector<double>>& entrada)
{
    // Matrizes próprias da segunda cabeça.
    vector<vector<double>> WQ = {{0.2, 0.7, 0.1},{0.6, 0.1, 0.3},{0.1, 0.4, 0.8}};

    vector<vector<double>> WK = {{0.3, 0.6, 0.2},{0.7, 0.2, 0.1},{0.2, 0.3, 0.7}};

    vector<vector<double>> WV = {{0.4, 0.3, 0.5},{0.2, 0.8, 0.1},{0.6, 0.2, 0.4}};

    vector<vector<double>> Q;
    vector<vector<double>> K;
    vector<vector<double>> V;

    // Gera Q, K e V da segunda cabeça.
    for (const vector<double>& vetor : entrada)
    {
        Q.push_back(multiplicar(vetor, WQ));
        K.push_back(multiplicar(vetor, WK));
        V.push_back(multiplicar(vetor, WV));
    }

    // Primeiro calcula os pesos de atenção entre Q e K.
    // Depois combina esses pesos com V.
    return combinarValores(calcularAtencao(Q, K),V);
}


// ============================================================
// ETAPA 10 - COMBINAÇÃO DAS DUAS CABEÇAS
// ============================================================

// Combina os resultados das duas cabeças de atenção.
// Nesta simulação, a combinação é feita calculando a média entre elas.
vector<vector<double>> TransformerSimulator::combinarCabecas(const vector<vector<double>>& cabeca1,const vector<vector<double>>& cabeca2)
{
    if (cabeca1.empty())
    {
        return cabeca2;
    }

    if (cabeca2.empty())
    {
        return cabeca1;
    }

    // Começa resultado como uma cópia da primeira cabeça.
    vector<vector<double>> resultado = cabeca1;

    for (int i = 0; i < resultado.size(); i++)
    {
        for (int j = 0; j < resultado[i].size(); j++)
        {
            // Calcula a média entre o mesmo valor das duas cabeças.
            resultado[i][j] =
                (cabeca1[i][j] + cabeca2[i][j]) / 2.0;
        }
    }

    return resultado;
}


// ============================================================
// ETAPA 10 - TRÊS CAMADAS SIMULADAS
// ============================================================

// Faz a representação passar por três transformações didáticas.
// O retorno possui três níveis de vector:
// primeiro = camadas, segundo = tokens, terceiro = valores de cada vetor.
vector<vector<vector<double>>> TransformerSimulator::processarCamadas(const vector<vector<double>>& entrada)
{
    vector<vector<vector<double>>> camadas;

    // --------------------------------------------------------
    // CAMADA 1 - RELAÇÕES LOCAIS
    // --------------------------------------------------------

    // Começa com uma cópia da entrada.
    vector<vector<double>> camada1 = entrada;

    // Para cada vetor existente na camada1...
    for (vector<double>& vetor : camada1)
    {
        // Para cada número dentro desse vetor...
        for (double& valor : vetor)
        {
            // Aumenta o valor em 10% como transformação didática.
            valor *= 1.10;
        }
    }

    // Guarda o resultado da primeira camada.
    camadas.push_back(camada1);


    // --------------------------------------------------------
    // CAMADA 2 - RELAÇÕES CONTEXTUAIS
    // --------------------------------------------------------

    vector<vector<double>> camada2 = camada1;

    // Começa em 1 porque usamos i - 1 para acessar o token anterior.
    for (int i = 1; i < camada2.size(); i++)
    {
        for (int j = 0; j < camada2[i].size(); j++)
        {
            // Adiciona 10% do valor do token anterior ao token atual.
            // Isso simula uma influência contextual entre tokens.
            camada2[i][j] += camada1[i - 1][j] * 0.10;
        }
    }

    camadas.push_back(camada2);


    // --------------------------------------------------------
    // CAMADA 3 - PREPARAÇÃO DA RESPOSTA
    // --------------------------------------------------------

    vector<vector<double>> camada3 = camada2;

    for (vector<double>& vetor : camada3)
    {
        for (double& valor : vetor)
        {
            // Realiza mais uma transformação didática antes da geração.
            valor = valor * 0.90 + 0.05;
        }
    }

    camadas.push_back(camada3);

    return camadas;
}


// ============================================================
// RESPOSTAS DIDÁTICAS
// ============================================================

// Procura uma resposta cadastrada para a entrada do usuário.
// Esta parte é simulada: não existe uma LLM real criando a resposta.
string TransformerSimulator::obterRespostaDidatica(const string& texto)
{
    // Converte o texto para minúsculo para facilitar as comparações.
    string busca = minusculo(texto);

    // find procura uma sequência de caracteres dentro da string.
    // string::npos significa que o texto procurado NÃO foi encontrado.
    if (busca.find("capital") != string::npos && busca.find("brasil") != string::npos)
    {
        return "A capital do Brasil é Brasília.";
    }

    // || significa "OU": basta uma das condições ser verdadeira.
    if (busca.find("2+2") != string::npos || busca.find("2 + 2") != string::npos)
    {
        return "2 + 2 é igual a 4.";
    }

    if (busca.find("transformer") != string::npos)
    {
        return "Transformer é uma arquitetura baseada em mecanismos de atenção.";
    }

    // Caso nenhuma resposta cadastrada seja encontrada.
    return "Não tenho uma resposta cadastrada para essa pergunta.";
}


// ============================================================
// ETAPA 11 - PROBABILIDADES
// ============================================================

// Cria probabilidades didáticas para os possíveis próximos tokens.
// pair<string, double> guarda um par: o token e sua probabilidade.
vector<pair<string, double>>
TransformerSimulator::calcularProbabilidades(const vector<string>& respostaTokens,int indiceAtual)
{
    // Verifica se o índice está fora dos limites do vetor.
    if (indiceAtual < 0 || indiceAtual >= respostaTokens.size())
    {
        // Se não existe mais token para gerar, retorna o marcador de fim.
        return {{"<FIM>", 1.0}};
    }

    // O token correto da resposta recebe 75%.
    // Os demais são candidatos didáticos.
    // As probabilidades somam 1.0, ou seja, 100%.
    return {{respostaTokens[indiceAtual], 0.75},{"modelo", 0.10},{"contexto", 0.08},{"informação", 0.07}};
}


// ============================================================
// ETAPA 12 - SELEÇÃO GREEDY
// ============================================================

// Seleciona o próximo token usando a estratégia Greedy.
// Greedy sempre escolhe o candidato com a maior probabilidade.
string TransformerSimulator::selecionarProximoToken(const vector<pair<string, double>>& probabilidades)
{
    if (probabilidades.empty())
    {
        return "<FIM>";
    }

    // Começa considerando o primeiro candidato como o melhor.
    pair<string, double> melhor = probabilidades[0];

    // Para cada candidato existente nas probabilidades...
    for (const pair<string, double>& candidato : probabilidades)
    {
        // pair possui first e second.
        // first = string com o token.
        // second = double com a probabilidade.
        if (candidato.second > melhor.second)
        {
            melhor = candidato;
        }
    }

    // Retorna apenas o nome do token que possui a maior probabilidade.
    return melhor.first;
}


// ============================================================
// ETAPA 13 - GERAÇÃO PROGRESSIVA
// ============================================================

// Simula a geração autoregressiva da resposta.
// A resposta é construída progressivamente, um token por vez.
vector<string> TransformerSimulator::gerarRespostaProgressiva(
    const string& resposta)
{
    // Divide a resposta cadastrada em tokens.
    vector<string> respostaTokens = tokenizar(resposta);

    // Armazena cada estágio da resposta:
    // por exemplo: "A", depois "A capital", depois "A capital do"...
    vector<string> etapas;

    string contexto;

    for (int i = 0; i < respostaTokens.size(); i++)
    {
        // A cada novo token, reprocessa o contexto que já foi gerado.
        // Isso simula o comportamento autoregressivo de uma LLM.
        if (!contexto.empty())
        {
            // Tokeniza novamente o contexto atual.
            vector<string> tokensContexto = tokenizar(contexto);

            // Converte os tokens do contexto em IDs.
            vector<int> ids = gerarIds(tokensContexto);

            // Gera os embeddings e adiciona informação de posição.
            vector<vector<double>> entrada = adicionarPosicao(gerarEmbeddings(ids));

            // Gera Query, Key e Value novamente para o contexto atualizado.
            vector<vector<double>> Q = gerarQuery(entrada);
            vector<vector<double>> K = gerarKey(entrada);
            vector<vector<double>> V = gerarValue(entrada);

            // Calcula a primeira cabeça de atenção.
            vector<vector<double>> cabeca1 = combinarValores(calcularAtencao(Q, K), V);

            // Calcula a segunda cabeça de atenção.
            vector<vector<double>> cabeca2 = gerarSegundaCabeca(entrada);

            // Combina as duas cabeças.
            vector<vector<double>> multiHead = combinarCabecas(cabeca1, cabeca2);

            // Passa o resultado pelas três camadas simuladas.
            // O retorno não precisa ser armazenado aqui porque esta chamada
            // representa o reprocessamento didático do contexto.
            processarCamadas(multiHead);
        }

        // Calcula os candidatos e suas probabilidades para o próximo token.
        vector<pair<string, double>> probs = calcularProbabilidades(respostaTokens, i);

        // Greedy seleciona o candidato de maior probabilidade.
        string token = selecionarProximoToken(probs);

        // Verifica se o token escolhido é um sinal de pontuação.
        bool pontuacao = token == "." || token == "," || token == "?" || token == "!" || token == ";" || token == ":";

        // Adiciona espaço antes de palavras, mas não antes de pontuação.
        if (!contexto.empty() && !pontuacao)
        {
            contexto += " ";
        }

        // Acrescenta o novo token ao contexto.
        contexto += token;

        // Guarda a resposta parcial dessa etapa.
        etapas.push_back(contexto);
    }

    return etapas;
}


// ============================================================
// EXIBIÇÃO DOS TOKENS
// ============================================================

// Mostra na tela cada token e sua posição.
void TransformerSimulator::mostrarTokens(const vector<string>& tokens)
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

// Mostra uma tabela relacionando cada token ao seu ID.
void TransformerSimulator::mostrarIds(const vector<string>& tokens,const vector<int>& ids)
{
    cout << "\nTOKENS E IDs\n";

    // left alinha o conteúdo à esquerda.
    // setw(20) reserva uma largura de 20 caracteres para a próxima informação.
    cout << left << setw(20) << "Token" << "ID\n";

    for (int i = 0; i < tokens.size(); i++)
    {
        cout << left << setw(20)<< tokens[i]<< ids[i]<< "\n";
    }
}


// ============================================================
// EXIBIÇÃO DOS VETORES
// ============================================================

// Função reutilizada para mostrar embeddings, posição, Q, K, V,
// cabeças de atenção e resultados das camadas.
void TransformerSimulator::mostrarVetores(const vector<string>& tokens,const vector<vector<double>>& vetores,const string& titulo)
{
    cout << "\n" << titulo << "\n";

    // fixed mostra os números em formato decimal fixo.
    // setprecision(3) define três casas decimais.
    cout << fixed << setprecision(3);

    // i representa qual token/vetor está sendo mostrado.
    for (int i = 0; i < vetores.size(); i++)
    {
        cout << tokens[i] << " -> [";

        // j percorre os valores existentes dentro do vetor atual.
        for (int j = 0; j < vetores[i].size(); j++)
        {
            cout << vetores[i][j];

            // Coloca vírgula entre os números, mas não depois do último.
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

// Mostra quanto cada token prestou atenção nos demais tokens.
void TransformerSimulator::mostrarAtencao(const vector<string>& tokens,const vector<vector<double>>& pesos)
{
    cout << "\nPESOS DE ATENCAO\n";
    cout << fixed << setprecision(3);

    // i representa o token que está prestando atenção.
    for (int i = 0; i < pesos.size(); i++)
    {
        cout << "\n" << tokens[i] << ":\n";

        double soma = 0.0;

        // j representa cada token que pode receber atenção.
        for (int j = 0; j < pesos[i].size(); j++)
        {
            cout << "  -> " << tokens[j] << ": " << pesos[i][j] << "\n";

            // Soma os pesos para mostrar que o resultado do Softmax
            // fica aproximadamente igual a 1.
            soma += pesos[i][j];
        }

        cout << "Soma: " << soma << "\n";
    }
}


// ============================================================
// EXIBIÇÃO DAS TRÊS CAMADAS
// ============================================================

// Mostra separadamente o resultado de cada uma das três camadas simuladas.
void TransformerSimulator::mostrarCamadas(const vector<string>& tokens,const vector<vector<vector<double>>>& camadas)
{
    // Guarda o nome que será mostrado para cada camada.
    vector<string> nomes = {"CAMADA 1 - RELACOES LOCAIS","CAMADA 2 - RELACOES CONTEXTUAIS","CAMADA 3 - PREPARACAO DA RESPOSTA"};

    for (int i = 0; i < camadas.size(); i++)
    {
        // Reutiliza mostrarVetores para exibir os valores de cada camada.
        mostrarVetores(tokens, camadas[i], nomes[i]);
    }
}


// ============================================================
// EXIBIÇÃO DAS PROBABILIDADES
// ============================================================

// Mostra os possíveis próximos tokens e suas probabilidades.
void TransformerSimulator::mostrarProbabilidades(
    const vector<pair<string, double>>& probabilidades)
{
    cout << "\nPROBABILIDADES DO PROXIMO TOKEN\n";
    cout << fixed << setprecision(2);

    // Percorre cada pair contendo o token e sua probabilidade.
    for (const pair<string, double>& candidato : probabilidades)
    {
        // first contém o token.
        // second contém a probabilidade.
        // Multiplicamos por 100 para mostrar como porcentagem.
        cout << candidato.first << " -> " << candidato.second * 100 << "%\n";
    }

    cout << "Estrategia: Greedy\n";
}


// ============================================================
// ETAPA 14 - RESUMO
// ============================================================

// Apresenta as principais informações da simulação depois que o processamento termina.
void TransformerSimulator::mostrarResumo(const string& pergunta,const vector<string>& tokensEntrada,const vector<string>& tokensGerados,const string& respostaFinal)
{
    cout << "\n========================================\n";
    cout << "RESUMO FINAL\n";
    cout << "========================================\n";

    cout << "Pergunta: " << pergunta << "\n";

    // size() retorna a quantidade de elementos do vetor.
    cout << "Tokens de entrada: " << tokensEntrada.size() << "\n";

    cout << "Tokens gerados: ";

    // Percorre todos os tokens que foram gerados na resposta.
    for (const string& token : tokensGerados)
    {
        cout << token << " ";
    }

    cout << "\nResposta final: " << respostaFinal << "\n";

    // Deixa explícito que os valores apresentados são apenas didáticos.
    cout << "\nSimulacao educacional simplificada.\n";
    cout << "Embeddings, matrizes, camadas, probabilidades " << "e respostas sao didaticos.\n";

    cout << "Os valores nao representam parametros internos " << "de uma LLM comercial.\n";
}