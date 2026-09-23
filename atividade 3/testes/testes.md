# Testes do Simulador Educacional de Transformer

Este documento apresenta os testes realizados no Simulador Educacional de Transformer.

O objetivo dos testes é verificar o funcionamento das principais etapas do simulador, incluindo normalização, tokenização, geração de IDs, embeddings, informação posicional, atenção, múltiplas cabeças, camadas e geração progressiva da resposta.

Foram realizados os 10 casos de teste solicitados na atividade.

---

## Resumo dos Testes

| # | Caso de Teste | Entrada | Resultado | Status |
|---|---|---|---|---|
| 1 | Pergunta curta | `Transformer?` | Processamento completo e resposta cadastrada | Passou |
| 2 | Frase afirmativa | `Hoje estou estudando compiladores.` | Processamento completo e resposta padrão | Passou |
| 3 | Pontuação | `Olá, Transformer! Tudo bem?` | Pontuação separada corretamente | Passou |
| 4 | Palavra desconhecida | `hipopotomonstrosesquipedaliofobia` | Novo token recebeu ID e foi processado | Passou |
| 5 | Palavras repetidas | `modelo modelo modelo` | Tokens repetidos receberam o mesmo ID | Passou |
| 6 | Mesmas palavras em ordem diferente | `Brasil capital` / `capital Brasil` | Ordem dos tokens foi preservada | Passou |
| 7 | Pergunta respondível | `Quanto é 2+2?` | Resposta cadastrada gerada corretamente | Passou |
| 8 | Pergunta não respondível | `Qual é a distância entre a Terra e a Lua?` | Resposta padrão utilizada | Passou |
| 9 | Texto vazio | Entrada vazia | Entrada inválida detectada | Passou |
| 10 | Caracteres especiais | `IA @ 2026 #teste $100 %legal & dados!` | Caracteres processados sem erro | Passou |

---

# Teste 1 - Pergunta curta

**Entrada:**

`Transformer?`

**Tokens obtidos:**

- `Transformer`
- `?`

**Resultado esperado:**

O simulador deve separar a palavra da pontuação, executar todas as etapas do processamento e gerar uma resposta progressivamente.

**Resultado observado:**

A entrada foi dividida em 2 tokens. Os tokens receberam IDs e embeddings, passaram pelo cálculo de Q, K e V, atenção, múltiplas cabeças e três camadas.

A resposta foi gerada token por token utilizando a estratégia Greedy.

**Resposta final:**

`Transformer é uma arquitetura baseada em mecanismos de atenção.`

**Status:** Passou.

---

# Teste 2 - Frase afirmativa

**Entrada:**

`Hoje estou estudando compiladores.`

**Tokens obtidos:**

- `Hoje`
- `estou`
- `estudando`
- `compiladores`
- `.`

**Resultado esperado:**

O simulador deve processar normalmente uma frase afirmativa e utilizar a resposta padrão caso não exista uma resposta específica cadastrada.

**Resultado observado:**

Foram identificados 5 tokens e a pontuação foi separada corretamente. Todas as etapas do pipeline foram executadas normalmente.

**Resposta final:**

`Não tenho uma resposta cadastrada para essa pergunta.`

**Status:** Passou.

---

# Teste 3 - Pontuação

**Entrada:**

`Olá, Transformer! Tudo bem?`

**Tokens obtidos:**

- `Olá`
- `,`
- `Transformer`
- `!`
- `Tudo`
- `bem`
- `?`

**Resultado esperado:**

O simulador deve reconhecer e separar os sinais de pontuação definidos no tokenizador.

**Resultado observado:**

Os sinais `,`, `!` e `?` foram separados das palavras e tratados como tokens independentes.

O restante do pipeline foi executado normalmente.

**Resposta final:**

`Transformer é uma arquitetura baseada em mecanismos de atenção.`

**Status:** Passou.

---

# Teste 4 - Palavra desconhecida

**Entrada:**

`hipopotomonstrosesquipedaliofobia`

**Tokens obtidos:**

- `hipopotomonstrosesquipedaliofobia`

**Resultado esperado:**

O simulador deve conseguir receber uma palavra ainda não existente no vocabulário da execução, atribuir um novo ID e continuar o processamento.

**Resultado observado:**

A palavra foi reconhecida como um único token e recebeu um novo ID.

Como havia somente um token, o peso de atenção foi `1.000`, com soma total igual a `1.000`.

O programa continuou normalmente pelas demais etapas.

**Resposta final:**

`Não tenho uma resposta cadastrada para essa pergunta.`

**Status:** Passou.

---

# Teste 5 - Palavras repetidas

**Entrada:**

`modelo modelo modelo`

**Tokens obtidos:**

- `modelo`
- `modelo`
- `modelo`

**IDs obtidos:**

- `modelo` -> 1
- `modelo` -> 1
- `modelo` -> 1

**Resultado esperado:**

Palavras iguais devem receber o mesmo ID e o mesmo embedding inicial.

A informação de posição deve permitir diferenciar as ocorrências na sequência.

**Resultado observado:**

As três ocorrências receberam o mesmo ID e o mesmo embedding inicial:

`[0.200, 0.200, 0.200]`

Após a adição da informação de posição, foram obtidos vetores diferentes:

- Posição 1: `[0.210, 0.220, 0.230]`
- Posição 2: `[0.220, 0.240, 0.260]`
- Posição 3: `[0.230, 0.260, 0.290]`

Isso demonstra a função da informação posicional no simulador.

**Resposta final:**

`Não tenho uma resposta cadastrada para essa pergunta.`

**Status:** Passou.

---

# Teste 6 - Mesmas palavras em ordem diferente

Foram realizadas duas execuções.

## Execução A

**Entrada:**

`Brasil capital`

**Tokens:**

- `Brasil`
- `capital`

**IDs:**

- `Brasil` -> 1
- `capital` -> 2

## Execução B

**Entrada:**

`capital Brasil`

**Tokens:**

- `capital`
- `Brasil`

**IDs:**

- `capital` -> 1
- `Brasil` -> 2

**Resultado esperado:**

O simulador deve preservar a ordem dos tokens e aplicar a informação de posição de acordo com a posição de cada elemento na sequência.

**Resultado observado:**

A ordem foi preservada nas duas execuções.

Como o vocabulário didático é criado novamente a cada execução, os IDs são atribuídos conforme os tokens aparecem pela primeira vez.

As duas entradas contêm as palavras `capital` e `Brasil`, portanto a regra de resposta cadastrada foi identificada nos dois casos.

**Resposta final nas duas execuções:**

`A capital do Brasil é Brasília.`

**Status:** Passou.

---

# Teste 7 - Pergunta respondível

**Entrada:**

`Quanto é 2+2?`

**Tokens obtidos:**

- `Quanto`
- `é`
- `2+2`
- `?`

**Resultado esperado:**

O simulador deve reconhecer uma pergunta para a qual existe uma resposta cadastrada e gerar a resposta progressivamente.

**Resultado observado:**

A entrada foi processada em 4 tokens.

O tokenizador manteve `2+2` como um único token na entrada e separou o caractere `?`.

A resposta foi gerada em 8 passos utilizando a estratégia Greedy.

**Resposta progressiva:**

1. `2`
2. `2 +`
3. `2 + 2`
4. `2 + 2 é`
5. `2 + 2 é igual`
6. `2 + 2 é igual a`
7. `2 + 2 é igual a 4`
8. `2 + 2 é igual a 4.`

**Resposta final:**

`2 + 2 é igual a 4.`

**Status:** Passou.

---

# Teste 8 - Pergunta não respondível

**Entrada:**

`Qual é a distância entre a Terra e a Lua?`

**Tokens obtidos:**

- `Qual`
- `é`
- `a`
- `distância`
- `entre`
- `a`
- `Terra`
- `e`
- `a`
- `Lua`
- `?`

**Resultado esperado:**

O simulador deve executar normalmente todas as etapas e utilizar a resposta padrão quando não existir uma resposta específica cadastrada.

**Resultado observado:**

Foram identificados 11 tokens.

As ocorrências repetidas de `a` receberam o mesmo ID dentro da execução.

Os pesos de atenção foram calculados e suas linhas apresentaram soma igual a aproximadamente `1.000`.

Como não havia uma resposta cadastrada para a pergunta, o simulador utilizou a resposta padrão.

**Resposta final:**

`Não tenho uma resposta cadastrada para essa pergunta.`

**Status:** Passou.

---

# Teste 9 - Texto vazio

**Entrada:**

Entrada vazia, pressionando apenas `Enter`.

**Resultado esperado:**

O programa deve impedir o processamento de uma entrada vazia e informar o erro sem encerrar de maneira inesperada.

**Resultado observado:**

O simulador apresentou:

`Erro: digite uma frase ou pergunta.`

Nenhuma etapa posterior do processamento foi executada.

**Status:** Passou.

---

# Teste 10 - Caracteres especiais

**Entrada:**

`IA @ 2026 #teste $100 %legal & dados!`

**Tokens obtidos:**

- `IA`
- `@`
- `2026`
- `#teste`
- `$100`
- `%legal`
- `&`
- `dados`
- `!`

**Resultado esperado:**

O simulador deve conseguir processar uma entrada contendo caracteres especiais sem apresentar erro ou travamento.

**Resultado observado:**

Foram identificados 9 tokens.

Os caracteres e grupos de caracteres especiais foram preservados de acordo com as regras atuais do tokenizador.

O caractere `!` foi separado como um token de pontuação.

Todos os tokens receberam IDs e embeddings e passaram normalmente pelas etapas seguintes do simulador.

Como não havia resposta específica cadastrada, foi utilizada a resposta padrão.

**Resposta final:**

`Não tenho uma resposta cadastrada para essa pergunta.`

**Status:** Passou.

---

# Resultado Final

Foram executados os 10 casos de teste obrigatórios.

**Resultado: 10 de 10 testes aprovados.**

Os testes verificaram:

- processamento de perguntas curtas;
- processamento de frases afirmativas;
- separação de pontuação;
- tratamento de palavras desconhecidas;
- tratamento de palavras repetidas;
- influência da ordem dos tokens;
- perguntas com resposta cadastrada;
- perguntas sem resposta cadastrada;
- validação de entrada vazia;
- processamento de caracteres especiais.

Os resultados demonstram que o simulador executa corretamente o fluxo didático implementado, desde a entrada e tokenização até a geração progressiva da resposta.

É importante destacar que o projeto representa uma simulação educacional simplificada. Os embeddings, matrizes, camadas, probabilidades e respostas utilizados são didáticos e não representam os parâmetros internos de uma LLM comercial.