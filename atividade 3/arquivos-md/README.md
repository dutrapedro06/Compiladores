# Simulador Educacional de Arquitetura Transformer

Projeto desenvolvido para a disciplina de **Compiladores**, com o objetivo de demonstrar, de forma didática, as principais etapas envolvidas no processamento de uma entrada textual por uma arquitetura baseada em Transformer.

O simulador permite acompanhar o fluxo desde o recebimento da entrada até a geração progressiva da resposta, tornando visíveis operações que normalmente acontecem internamente em modelos de linguagem.

> **Esta é uma simulação educacional simplificada. Os valores não representam os parâmetros internos de uma LLM comercial.**

---

## Sobre o projeto

O projeto implementa uma representação simplificada do funcionamento de um Transformer utilizando **C++**.

A partir de uma frase ou pergunta informada pelo usuário, o simulador apresenta etapas como:

- normalização do texto;
- tokenização;
- conversão dos tokens para IDs;
- geração de embeddings;
- adição de informação posicional;
- criação de Query, Key e Value;
- cálculo dos pesos de atenção;
- combinação dos Values;
- múltiplas cabeças de atenção;
- processamento em camadas;
- probabilidades para o próximo token;
- seleção utilizando estratégia Greedy;
- geração autoregressiva;
- apresentação da resposta final.

O objetivo não é implementar ou reproduzir uma LLM comercial, mas representar os principais conceitos da arquitetura de maneira visual e compreensível.

---

## Tecnologias utilizadas

- **C++** — implementação do simulador;
- **STL** — estruturas como `vector`, `string`, `unordered_map` e `pair`;
- **g++** — compilação do projeto;
- **Linux / WSL** — ambiente de desenvolvimento e execução;
- **Git** — controle de versão;
- **GitHub** — armazenamento e documentação do projeto.

---

## Estrutura do projeto

```text
atividade 3/
├── arquivos-md/
│   ├── README.md
│   └── prompts.md
│
├── docs/
│
├── include/
│   ├── interface.hpp
│   └── transformer.hpp
│
├── src/
│   ├── interface.cpp
│   ├── main.cpp
│   └── transformer.cpp
│
└── tests/
    └── testes.md
```

### Organização dos arquivos

| Arquivo | Responsabilidade |
|---|---|
| `main.cpp` | Ponto de entrada da aplicação |
| `interface.cpp` | Controla a interação com o usuário e a apresentação dos resultados |
| `transformer.cpp` | Implementa as etapas da simulação do Transformer |
| `interface.hpp` | Declaração da classe responsável pela interface |
| `transformer.hpp` | Declaração da classe responsável pelo processamento |
| `testes.md` | Registro dos casos de teste e resultados |
| `prompts.md` | Registro do uso de IA durante o desenvolvimento |

---

# Execução

## Compilação

Dentro do diretório `atividade 3`, execute:

```bash
g++ src/main.cpp src/transformer.cpp src/interface.cpp -Iinclude -o simulador
```

## Inicialização

Após a compilação:

```bash
./simulador
```

O programa apresenta o menu:

```text
============================================================
        SIMULADOR EDUCACIONAL DE TRANSFORMER
============================================================

[1] Processar uma frase
[0] Sair

Escolha:
```

Ao selecionar a opção `1`, uma frase ou pergunta pode ser informada para iniciar a simulação.

Exemplo:

```text
Qual a capital do Brasil?
```

---

# Funcionamento do simulador

O processamento foi organizado de acordo com as principais etapas apresentadas na atividade.

## 1. Recebimento da entrada

O simulador recebe uma frase ou pergunta digitada pelo usuário.

```text
Qual a capital do Brasil?
```

---

## 2. Normalização

Antes do processamento, o texto passa por uma normalização simples para remover espaços desnecessários.

Essa etapa prepara a entrada para a tokenização.

---

## 3. Tokenização

O texto normalizado é dividido em tokens.

Palavras e os seguintes sinais de pontuação são tratados separadamente:

```text
. , ? ! ; :
```

Exemplo:

```text
Qual a capital do Brasil?
```

Resultado:

```text
[Qual] [a] [capital] [do] [Brasil] [?]
```

Um token não precisa necessariamente representar uma palavra completa. Neste projeto, foi utilizada uma tokenização simplificada para facilitar a visualização do processo.

---

## 4. Tokens para IDs

Cada token recebe um identificador numérico.

Exemplo:

```text
Qual       -> 1
a          -> 2
capital    -> 3
do         -> 4
Brasil     -> 5
?          -> 6
```

Dentro de uma mesma execução, tokens iguais recebem o mesmo ID.

Os IDs funcionam como identificadores para acessar as representações numéricas utilizadas nas etapas seguintes.

---

## 5. Embeddings

Após a geração dos IDs, cada token é convertido em um vetor numérico de três dimensões.

Exemplo:

```text
Qual -> [0.200, 0.200, 0.200]
```

Os valores são gerados de maneira determinística a partir do ID do token.

> **Embedding didático simulado.**

Os embeddings utilizados neste projeto não foram aprendidos durante treinamento e não representam relações semânticas reais entre palavras.

---

## 6. Informação de posição

Como a posição dos tokens é importante para o processamento da sequência, o simulador adiciona valores posicionais aos embeddings.

Exemplo:

```text
Embedding:
[0.200, 0.200, 0.200]

Embedding + posição:
[0.210, 0.220, 0.230]
```

Isso permite representar a posição de cada token dentro da entrada.

---

## 7. Query, Key e Value

A representação de cada token é utilizada para gerar três novos vetores:

- **Query (Q)**
- **Key (K)**
- **Value (V)**

No simulador, esses vetores são calculados utilizando matrizes fixas `WQ`, `WK` e `WV`.

De forma simplificada:

```text
Q = X × WQ
K = X × WK
V = X × WV
```

onde `X` representa a entrada contendo embeddings e informação posicional.

---

## 8. Mecanismo de atenção

A atenção permite representar quanto cada token considera os demais tokens durante o processamento.

O simulador utiliza o conceito de **Scaled Dot-Product Attention**:

```text
Attention(Q, K, V) = softmax(QKᵀ / √d_k)V
```

Primeiro são calculados os scores entre Query e Key. Em seguida, o Softmax transforma esses valores em pesos de atenção.

Para cada token, os pesos possuem soma aproximadamente igual a:

```text
1.000
```

Esses valores são exibidos durante a execução para permitir a visualização das relações de atenção.

---

## 9. Combinação dos Values

Após o cálculo dos pesos de atenção, os vetores Value são combinados utilizando esses pesos.

Essa operação gera uma nova representação para cada token considerando informações dos demais tokens da sequência.

---

## 10. Multi-Head Attention e camadas

O projeto implementa **duas cabeças de atenção didáticas**.

```text
Cabeça 1
        \
         → Combinação das cabeças
        /
Cabeça 2
```

As representações das duas cabeças são combinadas antes de seguir para as camadas seguintes.

O simulador possui três camadas:

```text
Camada 1 - Relações locais
Camada 2 - Relações contextuais
Camada 3 - Preparação da resposta
```

Cada camada realiza uma transformação sobre a saída da camada anterior.

Essas denominações possuem finalidade didática. Em Transformers reais, cabeças de atenção não possuem necessariamente funções fixas ou diretamente interpretáveis.

---

## 11. Probabilidades do próximo token

Durante a geração da resposta, o simulador apresenta possíveis próximos tokens acompanhados de probabilidades.

Exemplo:

```text
A           -> 75%
modelo      -> 10%
contexto    -> 8%
informação  -> 7%
```

Essas probabilidades são simuladas e servem para demonstrar o processo de seleção do próximo token.

---

## 12. Seleção do próximo token

A estratégia utilizada pelo projeto é **Greedy Decoding**.

Nessa estratégia, o token que apresenta a maior probabilidade é selecionado.

Exemplo:

```text
A           -> 75%  <- selecionado
modelo      -> 10%
contexto    -> 8%
informação  -> 7%
```

---

## 13. Geração autoregressiva

A resposta é construída progressivamente, adicionando um novo token a cada passo.

Exemplo:

```text
A
A capital
A capital do
A capital do Brasil
A capital do Brasil é
A capital do Brasil é Brasília
A capital do Brasil é Brasília.
```

A cada etapa, o próximo token é selecionado e acrescentado à resposta parcial.

Esse processo representa, de forma simplificada, o comportamento autoregressivo de modelos de linguagem.

---

## 14. Resposta final

Após a geração dos tokens, o simulador apresenta a resposta completa.

Exemplo:

```text
A capital do Brasil é Brasília.
```

Também é apresentado um resumo contendo informações da execução, como:

- pergunta original;
- quantidade de tokens de entrada;
- quantidade de tokens gerados;
- número de etapas executadas;
- resposta final;
- partes simuladas do projeto.

---

# Fluxo da arquitetura

O fluxo geral implementado pode ser representado da seguinte forma:

```text
Entrada textual
      ↓
Normalização
      ↓
Tokenização
      ↓
Tokens → IDs
      ↓
Embeddings
      ↓
Informação posicional
      ↓
Query / Key / Value
      ↓
Scaled Dot-Product Attention
      ↓
Combinação dos Values
      ↓
Multi-Head Attention
      ↓
Camada 1
      ↓
Camada 2
      ↓
Camada 3
      ↓
Probabilidades
      ↓
Greedy Decoding
      ↓
Geração autoregressiva
      ↓
Resposta final
```

---

# Respostas didáticas

O projeto não utiliza uma LLM treinada para produzir respostas.

Para possibilitar a demonstração completa da geração autoregressiva, algumas respostas foram previamente cadastradas.

Atualmente, existem respostas específicas relacionadas a:

- capital do Brasil;
- operação `2 + 2`;
- arquitetura Transformer.

Quando uma entrada não possui uma resposta cadastrada, o simulador utiliza a resposta padrão:

```text
Não tenho uma resposta cadastrada para essa pergunta.
```

Essa decisão permite testar tanto entradas conhecidas quanto entradas que não possuem uma resposta disponível.

---

# O que é real e o que é simulado

O projeto implementa matematicamente e conceitualmente algumas operações utilizadas em Transformers, mas utiliza valores simplificados para fins educacionais.

### Conceitos representados

- tokenização;
- conversão para IDs;
- embeddings;
- informação posicional;
- Query, Key e Value;
- produto entre Query e Key;
- escalonamento por `√d_k`;
- Softmax;
- combinação dos Values;
- múltiplas cabeças;
- processamento em camadas;
- probabilidades;
- Greedy Decoding;
- geração autoregressiva.

### Elementos didáticos ou simulados

- valores dos embeddings;
- matrizes `WQ`, `WK` e `WV`;
- segunda cabeça de atenção;
- transformações realizadas pelas três camadas;
- probabilidades dos próximos tokens;
- conjunto de respostas disponíveis.

Portanto, os números apresentados não devem ser interpretados como parâmetros de uma LLM comercial.

---

# Limitações

Por se tratar de uma implementação educacional, o simulador possui limitações quando comparado a um Transformer real:

- não existe treinamento de modelo;
- o vocabulário é criado durante a execução;
- os embeddings possuem apenas três dimensões;
- os embeddings não possuem significado semântico aprendido;
- as matrizes utilizadas são fixas;
- são utilizadas apenas duas cabeças didáticas;
- são utilizadas apenas três camadas simuladas;
- as probabilidades são definidas para demonstração;
- a estratégia de seleção implementada é Greedy;
- existe um conjunto limitado de respostas cadastradas;
- a tokenização é simplificada;
- não são implementados todos os componentes de uma arquitetura Transformer completa.

Essas simplificações são intencionais e permitem observar cada etapa sem a complexidade computacional de uma LLM real.

---

# Testes

O simulador foi validado utilizando os **10 cenários de teste definidos para a atividade**.

| # | Cenário | Resultado |
|---|---|---|
| 1 | Pergunta curta | Aprovado |
| 2 | Frase afirmativa | Aprovado |
| 3 | Entrada com pontuação | Aprovado |
| 4 | Palavra desconhecida | Aprovado |
| 5 | Palavras repetidas | Aprovado |
| 6 | Mesmas palavras em ordem diferente | Aprovado |
| 7 | Pergunta que o simulador consegue responder | Aprovado |
| 8 | Pergunta que o simulador não consegue responder | Aprovado |
| 9 | Entrada vazia | Aprovado |
| 10 | Caracteres especiais | Aprovado |

Os detalhes de cada teste, incluindo entrada, comportamento esperado, resultado observado e resposta gerada, estão disponíveis em:

```text
tests/testes.md
```

**Resultado geral: 10 de 10 testes aprovados.**

---

# Exemplo de execução

### Entrada

```text
Qual a capital do Brasil?
```

### Tokens

```text
[Qual] [a] [capital] [do] [Brasil] [?]
```

### Geração

```text
Passo 1: A
Passo 2: A capital
Passo 3: A capital do
Passo 4: A capital do Brasil
Passo 5: A capital do Brasil é
Passo 6: A capital do Brasil é Brasília
Passo 7: A capital do Brasil é Brasília.
```

### Resposta final

```text
A capital do Brasil é Brasília.
```

---

# Considerações finais

O projeto demonstra de forma simplificada o caminho percorrido por uma entrada textual em uma arquitetura baseada em Transformer.

A visualização das representações intermediárias permite acompanhar a transformação do texto em valores numéricos, o cálculo de atenção e o processo de escolha e geração dos tokens da resposta.

O foco da implementação é **educacional**, priorizando a compreensão do fluxo e dos conceitos apresentados na disciplina.

---

# Uso de Inteligência Artificial

Durante o desenvolvimento deste projeto, ferramentas de **Inteligência Artificial generativa** foram utilizadas como recurso de apoio.

O uso de IA ocorreu principalmente nas seguintes atividades:

- auxílio na criação e organização dos arquivos em **Markdown (`.md`)**;
- auxílio na elaboração e melhoria de **comentários no código**;
- apoio na organização da documentação e explicação das etapas implementadas.

A IA foi utilizada como ferramenta auxiliar durante o processo de desenvolvimento. A implementação foi testada e validada por meio dos casos de teste documentados no projeto.

Os prompts utilizados como apoio durante o desenvolvimento estão registrados no arquivo:

```text
arquivos-md/prompts.md
```

---

## Aviso

> **Esta é uma simulação educacional simplificada. Os valores não representam os parâmetros internos de uma LLM comercial.**