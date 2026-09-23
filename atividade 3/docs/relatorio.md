# Relatório Técnico — Simulador Educacional de Arquitetura Transformer

## 1. Introdução

Este projeto foi desenvolvido para a disciplina de **Compiladores** com o objetivo de construir um simulador educacional capaz de representar, de forma simplificada, as principais etapas envolvidas no processamento de texto por uma arquitetura baseada em **Transformer**.

O sistema foi implementado em **C++** e permite que o usuário informe uma frase ou pergunta e acompanhe as transformações realizadas desde a entrada textual até a geração progressiva de uma resposta.

O objetivo principal não é reproduzir integralmente uma LLM real, mas tornar visíveis conceitos que normalmente são executados internamente nesses modelos, como tokenização, embeddings, Query, Key e Value, mecanismo de atenção, múltiplas cabeças, camadas e geração autoregressiva.

---

## 2. Objetivo

O objetivo do projeto é demonstrar didaticamente o fluxo de processamento utilizado por modelos baseados na arquitetura Transformer.

Para isso, o simulador apresenta as diferentes representações pelas quais uma entrada passa durante o processamento.

O fluxo geral desenvolvido foi:

```text
Entrada
  ↓
Normalização
  ↓
Tokenização
  ↓
Tokens para IDs
  ↓
Embeddings
  ↓
Informação Posicional
  ↓
Query, Key e Value
  ↓
Mecanismo de Atenção
  ↓
Combinação dos Values
  ↓
Multi-Head Attention
  ↓
Camadas do Transformer
  ↓
Probabilidades
  ↓
Seleção do próximo token
  ↓
Geração Autoregressiva
  ↓
Resposta Final
```

---

## 3. Organização da implementação

A implementação foi dividida principalmente entre as classes `TransformerSimulator` e `Interface`.

### TransformerSimulator

A classe `TransformerSimulator` concentra a lógica utilizada para representar as etapas internas do Transformer.

Entre suas responsabilidades estão:

- normalização;
- tokenização;
- geração de IDs;
- geração de embeddings;
- informação posicional;
- geração de Query, Key e Value;
- cálculo da atenção;
- combinação dos Values;
- segunda cabeça de atenção;
- combinação das cabeças;
- processamento das camadas;
- cálculo das probabilidades;
- seleção do próximo token;
- geração progressiva da resposta.

### Interface

A classe `Interface` é responsável pela comunicação entre o usuário e o simulador.

Ela recebe a entrada, executa as etapas do processamento e apresenta os resultados de maneira organizada no terminal.

---

# 4. Etapas do processamento

## 4.1 Recebimento da entrada

O processamento começa com uma frase ou pergunta fornecida pelo usuário.

Por exemplo:

```text
Qual a capital do Brasil?
```

Essa entrada é armazenada e enviada para as etapas seguintes.

---

## 4.2 Normalização

A entrada passa por uma normalização simples antes da tokenização.

O objetivo dessa etapa é remover espaços desnecessários e deixar o texto em uma forma adequada para o processamento.

Essa normalização evita que diferentes quantidades de espaços interfiram na separação dos tokens.

---

## 4.3 Tokenização

Após a normalização, o texto é dividido em tokens.

Na implementação desenvolvida, palavras e alguns sinais de pontuação são separados.

Para a entrada:

```text
Qual a capital do Brasil?
```

são obtidos os tokens:

```text
[Qual] [a] [capital] [do] [Brasil] [?]
```

Os sinais considerados separadamente incluem:

```text
. , ? ! ; :
```

A tokenização utilizada é propositalmente simplificada. Tokenizadores utilizados por LLMs reais podem dividir palavras em unidades menores, como partes de palavras ou subpalavras.

---

## 4.4 Conversão dos tokens para IDs

Depois da tokenização, cada token recebe um identificador numérico.

Exemplo:

```text
Qual       -> 1
a          -> 2
capital    -> 3
do         -> 4
Brasil     -> 5
?          -> 6
```

Um vocabulário é mantido durante o processamento.

Caso um token apareça novamente dentro da mesma execução, ele recebe o mesmo ID.

Por exemplo:

```text
modelo modelo modelo
```

pode ser representado como:

```text
1 1 1
```

Essa etapa representa o princípio de que modelos computacionais trabalham com representações numéricas, e não diretamente com palavras.

---

## 4.5 Embeddings

Os IDs são utilizados para gerar vetores numéricos chamados embeddings.

Neste projeto, cada token recebe um vetor com três dimensões.

Exemplo:

```text
Token: Qual
Embedding: [0.200, 0.200, 0.200]
```

Os valores são gerados de maneira determinística a partir do ID.

Essa implementação permite demonstrar a transformação de uma representação discreta, o ID, em uma representação vetorial.

> **Embedding didático simulado.**

Os embeddings utilizados não foram aprendidos por treinamento e não representam relações semânticas reais entre palavras.

---

## 4.6 Informação posicional

Além do significado representado pelo embedding, Transformers precisam considerar a posição dos tokens na sequência.

Por esse motivo, o simulador adiciona informação posicional aos embeddings.

Exemplo simplificado:

```text
Embedding:
[0.200, 0.200, 0.200]

Posição adicionada:
[0.210, 0.220, 0.230]
```

Dessa maneira, tokens iguais localizados em posições diferentes podem produzir representações diferentes.

Essa característica também pôde ser observada durante os testes com palavras repetidas e com palavras apresentadas em ordens diferentes.

---

## 4.7 Query, Key e Value

Depois da adição da informação posicional, são produzidas três representações para cada token:

- **Query (Q)**
- **Key (K)**
- **Value (V)**

A implementação utiliza matrizes fixas para representar `WQ`, `WK` e `WV`.

De maneira simplificada:

```text
Q = X × WQ
K = X × WK
V = X × WV
```

onde `X` representa a entrada vetorial resultante das etapas anteriores.

As matrizes utilizadas no simulador possuem finalidade didática e não foram obtidas por treinamento.

---

## 4.8 Mecanismo de atenção

O mecanismo de atenção permite determinar quanto cada token considera os outros tokens da sequência durante o processamento.

A implementação representa a fórmula:

```text
Attention(Q, K, V) = softmax(QKᵀ / √d_k)V
```

Primeiramente, é calculada a similaridade entre os vetores Query e Key.

O resultado é dividido por:

```text
√d_k
```

Em seguida, é aplicada a função **Softmax**, transformando os scores em pesos de atenção.

Os valores de cada linha são normalizados para que sua soma seja aproximadamente:

```text
1.000
```

A matriz resultante é apresentada ao usuário durante a execução.

---

## 4.9 Combinação dos Values

Depois do cálculo dos pesos de atenção, esses pesos são utilizados para combinar os vetores Value.

De maneira simplificada:

```text
Saída = Pesos de Atenção × V
```

Essa etapa gera uma nova representação para cada token, agora levando em consideração informações presentes nos demais tokens da sequência.

---

## 4.10 Multi-Head Attention

Transformers utilizam múltiplas cabeças de atenção.

Para representar esse conceito, o simulador implementa **duas cabeças de atenção didáticas**.

O fluxo pode ser representado como:

```text
                 ┌─ Cabeça 1 ─┐
Entrada ─────────┤             ├─→ Combinação
                 └─ Cabeça 2 ─┘
```

Cada cabeça produz uma representação e, posteriormente, seus resultados são combinados.

O objetivo é demonstrar que diferentes cabeças podem processar diferentes relações da entrada simultaneamente.

As cabeças deste simulador são simplificadas. Em modelos reais, não é correto assumir que cada cabeça possui necessariamente uma função fixa ou diretamente interpretável.

---

## 4.11 Camadas do Transformer

Depois da combinação das cabeças, a informação passa por três camadas simuladas.

Foram utilizadas:

```text
Camada 1 — Relações locais
Camada 2 — Relações contextuais
Camada 3 — Preparação da resposta
```

Cada camada realiza uma transformação sobre a saída da etapa anterior.

A primeira camada aplica uma transformação numérica inicial.

A segunda também considera informações da representação anterior, simulando a incorporação de contexto.

A terceira realiza uma nova transformação antes da etapa de geração.

Essas camadas são representações didáticas e não correspondem integralmente aos blocos utilizados em um Transformer real.

---

## 4.12 Probabilidades do próximo token

Depois do processamento, inicia-se a etapa de geração.

O simulador apresenta possíveis próximos tokens acompanhados de suas probabilidades.

Exemplo:

```text
A           -> 75%
modelo      -> 10%
contexto    -> 8%
informação  -> 7%
```

Esses valores permitem visualizar o princípio utilizado por modelos de linguagem para escolher o próximo elemento da sequência.

As probabilidades utilizadas neste projeto são simuladas e não são produzidas por um modelo treinado.

---

## 4.13 Seleção do próximo token

A estratégia utilizada para selecionar o próximo token é **Greedy Decoding**.

Nesse método, é escolhido o token que possui a maior probabilidade.

Por exemplo:

```text
A           -> 75%  <- selecionado
modelo      -> 10%
contexto    -> 8%
informação  -> 7%
```

Portanto:

```text
Próximo token = A
```

A estratégia foi escolhida por ser simples e adequada ao objetivo educacional do projeto.

---

## 4.14 Geração autoregressiva

Após selecionar um token, ele é adicionado à resposta.

O processo continua até que a resposta seja concluída.

Por exemplo:

```text
A

A capital

A capital do

A capital do Brasil

A capital do Brasil é

A capital do Brasil é Brasília

A capital do Brasil é Brasília.
```

Esse comportamento representa o conceito de geração autoregressiva, no qual a sequência é construída progressivamente.

Durante a execução do simulador, as probabilidades, o token selecionado e a resposta parcial são apresentados em cada passo.

---

# 5. Geração das respostas

O simulador não possui um modelo de linguagem treinado.

Para permitir a demonstração completa do processo de geração, foram cadastradas respostas específicas para alguns tipos de entrada.

Entre elas estão entradas relacionadas a:

```text
capital do Brasil
2 + 2
Transformer
```

Por exemplo:

```text
Entrada:
Qual a capital do Brasil?

Resposta:
A capital do Brasil é Brasília.
```

Caso nenhuma resposta cadastrada corresponda à entrada, o simulador utiliza:

```text
Não tenho uma resposta cadastrada para essa pergunta.
```

Essa abordagem permite demonstrar o fluxo de geração sem afirmar que o programa possui conhecimento equivalente ao de uma LLM.

---

# 6. Diagrama da arquitetura

O fluxo implementado pelo simulador também foi representado por meio de um diagrama de arquitetura.

O arquivo encontra-se em:

```text
docs/arquitetura-transformer.png
```

O diagrama apresenta visualmente a sequência entre entrada, tokenização, representações vetoriais, mecanismo de atenção, múltiplas cabeças, camadas e geração da resposta.

---

# 7. Testes realizados

Foram realizados os dez cenários previstos para validação do simulador.

| Teste | Cenário | Resultado |
|---|---|---|
| 1 | Pergunta curta | Aprovado |
| 2 | Frase afirmativa | Aprovado |
| 3 | Entrada com pontuação | Aprovado |
| 4 | Palavra desconhecida | Aprovado |
| 5 | Palavras repetidas | Aprovado |
| 6 | Mesmas palavras em ordem diferente | Aprovado |
| 7 | Pergunta que pode ser respondida | Aprovado |
| 8 | Pergunta sem resposta cadastrada | Aprovado |
| 9 | Entrada vazia | Aprovado |
| 10 | Caracteres especiais | Aprovado |

**Resultado geral: 10 de 10 testes aprovados.**

Os resultados completos encontram-se em:

```text
tests/testes.md
```

---

# 8. Componentes reais e simulados

O projeto utiliza conceitos reais da arquitetura Transformer, porém diversos valores foram simplificados para permitir a visualização do funcionamento.

## Conceitos representados

O simulador representa conceitos como:

- tokenização;
- identificação numérica dos tokens;
- embeddings;
- informação posicional;
- Query, Key e Value;
- Scaled Dot-Product Attention;
- Softmax;
- combinação dos Values;
- múltiplas cabeças;
- processamento em camadas;
- probabilidades do próximo token;
- Greedy Decoding;
- geração autoregressiva.

## Componentes simulados

São considerados didáticos ou simulados:

- valores dos embeddings;
- matrizes `WQ`, `WK` e `WV`;
- segunda cabeça de atenção;
- transformações utilizadas nas três camadas;
- probabilidades utilizadas na geração;
- respostas previamente cadastradas.

Esses componentes foram implementados para representar o fluxo conceitual sem utilizar parâmetros internos de uma LLM real.

---

# 9. Limitações

O simulador possui limitações intencionais devido ao seu objetivo educacional.

Entre elas:

- não existe treinamento de modelo;
- não é utilizada uma rede neural treinada;
- o vocabulário é criado durante a execução;
- os embeddings possuem apenas três dimensões;
- os embeddings não representam significado semântico aprendido;
- as matrizes de transformação possuem valores fixos;
- são utilizadas duas cabeças didáticas;
- são utilizadas três camadas simplificadas;
- as probabilidades são simuladas;
- a geração depende de respostas cadastradas;
- a tokenização é simplificada;
- não são implementados todos os componentes existentes em um Transformer completo.

Portanto, o projeto deve ser interpretado como uma representação didática do fluxo de processamento, e não como uma implementação completa de uma LLM.

---

# 10. Resultados

A implementação permitiu visualizar todas as principais etapas propostas para o simulador.

A partir de uma entrada textual, foi possível observar:

```text
Texto
↓
Tokens
↓
IDs
↓
Vetores
↓
Q / K / V
↓
Atenção
↓
Multi-Head
↓
Camadas
↓
Probabilidades
↓
Tokens gerados
↓
Resposta
```

Além disso, a execução dos dez casos de teste permitiu verificar situações diferentes, incluindo pontuação, palavras repetidas, palavras desconhecidas, caracteres especiais, perguntas conhecidas, perguntas sem resposta cadastrada e entrada vazia.

---

# 11. Conclusão

O desenvolvimento do simulador permitiu representar de maneira prática conceitos fundamentais da arquitetura Transformer.

A implementação em C++ tornou possível acompanhar cada etapa individualmente e observar como uma entrada textual pode ser convertida em representações numéricas até chegar ao processo de geração de uma resposta.

Apesar das simplificações adotadas, o projeto demonstra conceitos importantes como tokenização, embeddings, informação posicional, Query, Key e Value, atenção, múltiplas cabeças, processamento em camadas, probabilidades e geração autoregressiva.

A separação entre os componentes reais da arquitetura e os valores simulados também é importante para deixar claro o caráter educacional da aplicação.

O resultado final é um simulador que prioriza a visualização e a compreensão do fluxo de processamento de uma arquitetura baseada em Transformer.

---

# 12. Uso de Inteligência Artificial

Durante o desenvolvimento do projeto, ferramentas de **Inteligência Artificial generativa** foram utilizadas como recurso de apoio.

O uso ocorreu principalmente para:

- auxílio na criação e organização dos arquivos Markdown;
- auxílio na elaboração e revisão de comentários no código;
- apoio na organização da documentação;
- auxílio na explicação dos componentes implementados.

A utilização da IA ocorreu como ferramenta auxiliar ao processo de desenvolvimento. O funcionamento da implementação foi posteriormente verificado por meio da compilação, execução e realização dos casos de teste documentados no projeto.

Os prompts utilizados durante o desenvolvimento são registrados separadamente no arquivo:

```text
arquivos-md/prompts.md
```

---

## Aviso

> **Esta é uma simulação educacional simplificada. Os valores não representam os parâmetros internos de uma LLM comercial.**

> **Foi utilizada Inteligência Artificial como ferramenta de apoio na criação dos arquivos Markdown e no auxílio à elaboração de comentários no código.**