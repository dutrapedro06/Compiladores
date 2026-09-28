#include "../include/interface.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void Interface::mostrarLinha()
{
    cout << "============================================================\n";
}

// Mostra o título principal do simulador.
void Interface::mostrarCabecalho()
{
    cout << "\n";

    // Chama a função mostrarLinha() para colocar uma linha acima do título.
    mostrarLinha();

    cout << "        SIMULADOR EDUCACIONAL DE TRANSFORMER\n";

    // Coloca outra linha abaixo do título.
    mostrarLinha();
}

// Mostra uma mensagem informando que os valores utilizados
// são apenas uma simulação educacional.
void Interface::mostrarAviso()
{
    cout << "\n";

    mostrarLinha();

    cout << "Esta é uma simulação educacional simplificada. " << "Os valores não representam os parâmetros internos de uma LLM comercial.\n";

    mostrarLinha();
}


// ============================================================
// PROCESSAMENTO PRINCIPAL
// ============================================================

// chama as funções do TransformerSimulator na ordem
// em que as etapas da simulação devem acontecer.
void Interface::processar()
{
    // Isso reinicia o simulador antes de processar uma nova entrada.
    simulador = TransformerSimulator();

    string texto;

    cout << "\nDigite uma frase ou pergunta:\n> ";

    // getline lê a linha inteira digitada pelo usuário.
    getline(cin, texto);

    // Chama a função normalizarTexto() do objeto simulador.
    // O resultado é armazenado em normalizado.
    string normalizado = simulador.normalizarTexto(texto);


    // Verifica se o texto está vazio depois da normalização.
    if (normalizado.empty())
    {
        cout << "\nErro: digite uma frase ou pergunta.\n";

        // return encerra a execução da função processar().
        return;
    }

    // Divide o texto normalizado em tokens.
    vector<string> tokens = simulador.tokenizar(normalizado);

    // Converte cada token em um ID numérico.
    vector<int> ids = simulador.gerarIds(tokens);

    // Gera um embedding didático para cada ID.
    vector<vector<double>> embeddings = simulador.gerarEmbeddings(ids);

    // Adiciona informação de posição aos embeddings.
    vector<vector<double>> posicao = simulador.adicionarPosicao(embeddings);


    // ========================================================
    // QUERY, KEY E VALUE
    // ========================================================

    // Gera os vetores Query a partir dos embeddings com posição.
    vector<vector<double>> query = simulador.gerarQuery(posicao);

    // Gera os vetores Key.
    vector<vector<double>> key = simulador.gerarKey(posicao);

    // Gera os vetores Value.
    vector<vector<double>> value = simulador.gerarValue(posicao);

    // Calcula os pesos de atenção comparando Query e Key.
    vector<vector<double>> atencao = simulador.calcularAtencao(query, key);

    // Utiliza os pesos de atenção para combinar os valores de Value.
    // Esse resultado representa a primeira cabeça de atenção.
    vector<vector<double>> cabeca1 = simulador.combinarValores(atencao, value);

    // Gera uma segunda cabeça de atenção com matrizes diferentes.
    vector<vector<double>> cabeca2 = simulador.gerarSegundaCabeca(posicao);

    // Combina os resultados das duas cabeças.
    vector<vector<double>> cabecas = simulador.combinarCabecas(cabeca1, cabeca2);

    // vector<vector<vector<double>>> possui três níveis:
    // 1º -> camadas
    // 2º -> tokens
    // 3º -> valores dos vetores
    vector<vector<vector<double>>> camadas = simulador.processarCamadas(cabecas);

    // Procura uma resposta didática cadastrada para a pergunta.
    string resposta = simulador.obterRespostaDidatica(normalizado);

    // Divide a resposta cadastrada em tokens.
    vector<string> tokensResposta = simulador.tokenizar(resposta);

    // Gera a resposta progressivamente, um token por vez.
    vector<string> geracao = simulador.gerarRespostaProgressiva(resposta);

    cout << "\n";

    mostrarLinha();

    cout << "1. ENTRADA E NORMALIZAÇÃO\n";

    mostrarLinha();

    // Mostra o texto exatamente como foi digitado.
    cout << "\nEntrada original: " << texto;

    // Mostra o texto depois da normalização.
    cout << "\nTexto normalizado: " << normalizado << "\n";

    // 2. TOKENS

    cout << "\n";

    mostrarLinha();

    cout << "2. TOKENS\n";

    mostrarLinha();

    cout << "\n";

    // Para cada token existente no vetor tokens, faça...
    for (const string& token : tokens)
    // : percorre os elementos do vetor tokens.
    // const impede alterar o token.
    // & faz token ser uma referência ao elemento original,
    // evitando criar uma cópia da string.
    {
        cout << "[ " << token << " ] ";
    }

    cout << "\n";

    // 3. TOKENS E IDs

    cout << "\n";

    mostrarLinha();

    cout << "3. TOKENS E IDs\n";

    mostrarLinha();

    // Chama a função que mostra cada token junto com seu ID.
    simulador.mostrarIds(tokens, ids);

    // 4. EMBEDDINGS

    cout << "\n";

    mostrarLinha();

    cout << "4. EMBEDDINGS\n";

    mostrarLinha();

    cout << "\nEmbedding didático simulado.\n";

    // Mostra os vetores de embedding associados aos tokens.
    simulador.mostrarVetores(tokens, embeddings, "EMBEDDINGS");

    // 5. INFORMAÇÃO DE POSIÇÃO

    cout << "\n";

    mostrarLinha();

    cout << "5. INFORMAÇÃO DE POSIÇÃO\n";

    mostrarLinha();

    // Mostra os embeddings depois que a informação de posição foi adicionada.
    simulador.mostrarVetores(tokens, posicao, "EMBEDDING + POSIÇÃO");

    // 6. QUERY, KEY E VALUE

    cout << "\n";

    mostrarLinha();

    cout << "6. QUERY, KEY E VALUE\n";

    mostrarLinha();

    // Mostra os vetores Query.
    simulador.mostrarVetores(tokens, query, "QUERY (Q)");

    // Mostra os vetores Key.
    simulador.mostrarVetores(tokens, key, "KEY (K)");

    // Mostra os vetores Value.
    simulador.mostrarVetores(tokens, value, "VALUE (V)");

    // 7. PESOS DE ATENÇÃO

    cout << "\n";

    mostrarLinha();

    cout << "7. PESOS DE ATENÇÃO\n";

    mostrarLinha();

    // Mostra quanto cada token prestou atenção nos outros tokens.
    simulador.mostrarAtencao(tokens, atencao);

    // 8. MULTI-HEAD ATTENTION

    cout << "\n";

    mostrarLinha();

    cout << "8. MULTI-HEAD ATTENTION\n";

    mostrarLinha();

    // Mostra o resultado da primeira cabeça.
    simulador.mostrarVetores( tokens, cabeca1, "CABEÇA 1");

    // Mostra o resultado da segunda cabeça.
    simulador.mostrarVetores(tokens, cabeca2, "CABEÇA 2");

    // Mostra o resultado depois da combinação das duas cabeças.
    simulador.mostrarVetores(tokens, cabecas, "CABEÇAS COMBINADAS");

    // 9. CAMADAS DO TRANSFORMER

    cout << "\n";

    mostrarLinha();

    cout << "9. CAMADAS DO TRANSFORMER\n";

    mostrarLinha();

    // Mostra os resultados das três camadas simuladas.
    simulador.mostrarCamadas(tokens, camadas);

    // 10. GERAÇÃO AUTOREGRESSIVA

    cout << "\n";

    mostrarLinha();

    cout << "10. GERAÇÃO AUTOREGRESSIVA\n";

    mostrarLinha();


    // Percorre todos os tokens que fazem parte da resposta.
    for (int i = 0; i < tokensResposta.size(); i++)
    {
        cout << "\n------------------------------\n";

        // i começa em 0, por isso usamos i + 1
        // para mostrar o número do passo começando em 1.
        cout << "PASSO " << i + 1 << "\n";

        cout << "------------------------------\n";


        // Calcula as probabilidades dos candidatos para o token atual.
        vector<pair<string, double>> probabilidades = simulador.calcularProbabilidades(tokensResposta, i);

        // Mostra os candidatos e suas probabilidades.
        simulador.mostrarProbabilidades(probabilidades);


        // Seleciona o token que possui a maior probabilidade.
        // A função utiliza a estratégia Greedy.
        string selecionado = simulador.selecionarProximoToken(probabilidades);

        // Mostra qual token foi escolhido.
        cout << "\nToken selecionado: " << selecionado;

        // Mostra como está a resposta naquele momento.
        cout << "\nResposta parcial: " << geracao[i] << "\n";
    }

    // 11. RESPOSTA FINAL

    cout << "\n";

    mostrarLinha();

    cout << "11. RESPOSTA FINAL\n";

    mostrarLinha();

    // Mostra a resposta completa cadastrada para aquela entrada.
    cout << "\n" << resposta << "\n";

    // RESUMO

    cout << "\n";

    mostrarLinha();

    cout << "RESUMO DA SIMULAÇÃO\n";

    mostrarLinha();


    // Mostra a pergunta exatamente como foi digitada.
    cout << "\nPergunta original: " << texto;

    // size() retorna a quantidade de elementos existentes no vetor.
    cout << "\nQuantidade de tokens de entrada: " << tokens.size();

    // Mostra quantos tokens existem na resposta.
    cout << "\nQuantidade de tokens gerados: " << tokensResposta.size();

    // Mostra a quantidade de etapas principais informadas pelo simulador.
    cout << "\nEtapas principais executadas: 14";

    // Mostra a resposta final.
    cout << "\nResposta final: " << resposta << "\n";


    // Mostra quais partes do Transformer são simuladas pelo programa.
    cout << "\nPARTES SIMULADAS:\n";
    cout << "- Embeddings didáticos\n";
    cout << "- Matrizes WQ, WK e WV\n";
    cout << "- Cabeças de atenção\n";
    cout << "- Transformações das camadas\n";
    cout << "- Probabilidades dos próximos tokens\n";
    cout << "- Respostas cadastradas\n";


    // Mostra novamente o aviso de que os valores são didáticos.
    mostrarAviso();
}

// MENU PRINCIPAL

// Controla o menu inicial do programa.
// O usuário pode escolher processar uma frase ou sair.
void Interface::executar()
{
    // Guarda a opção escolhida pelo usuário.
    // Começamos com -1 para garantir que o menu seja executado.
    int opcao = -1;


    // do...while executa o bloco pelo menos uma vez
    // e depois continua enquanto a condição for verdadeira.
    do
    {
        // Mostra o cabeçalho do programa.
        mostrarCabecalho();


        // Mostra as opções disponíveis.
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