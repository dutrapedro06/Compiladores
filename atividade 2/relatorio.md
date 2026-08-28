# Relatório — Construção de um Analisador Léxico em C

## 1. Introdução

Este trabalho teve como objetivo implementar, em linguagem C, um analisador léxico para uma linguagem fictícia chamada MiniC.

A análise léxica é uma das primeiras etapas de um compilador. Nessa etapa, o código-fonte é lido caractere por caractere e seus elementos são agrupados em unidades chamadas tokens.

Cada token reconhecido pelo programa possui:

* linha em que começa;
* coluna em que começa;
* categoria;
* lexema correspondente.

O analisador desenvolvido reconhece as categorias definidas para a linguagem MiniC:

* palavras reservadas;
* identificadores;
* números inteiros;
* números reais;
* literais de caractere;
* operadores;
* delimitadores;
* erros léxicos.

O programa não realiza análise sintática, verificação de tipos ou execução do código MiniC, pois essas funcionalidades estão fora do escopo da atividade.

---

## 2. Estratégia utilizada

O analisador lê o arquivo-fonte caractere por caractere utilizando a função `fgetc`.

Durante a leitura, duas variáveis controlam a posição atual:

```text
linha
coluna
```

A numeração começa em linha 1 e coluna 1. Quando uma quebra de linha é encontrada, a variável `linha` é incrementada e a `coluna` volta para 1.

Antes de processar cada possível token, o programa armazena a posição inicial:

```c
int linha_inicial = linha;
int coluna_inicial = coluna;
```

Dessa forma, mesmo quando um token possui vários caracteres, a posição apresentada na saída corresponde ao local onde ele começou.

O programa possui funções auxiliares para organizar a implementação. Entre elas estão:

* `eh_palavra_reservada()`, que verifica se um lexema pertence às palavras reservadas da linguagem;
* `eh_inicio_identificador()`, que verifica se um caractere pode iniciar um identificador;
* `eh_parte_identificador()`, que verifica se um caractere pode fazer parte de um identificador;
* `eh_delimitador()`, que verifica se um caractere é um delimitador válido;
* `criar_token()`, que agrupa as informações de um token;
* `nome_do_token()`, que retorna o nome textual da categoria;
* `imprimir_token()`, que exibe o token e atualiza os contadores;
* `adicionar_caractere()`, que impede que caracteres sejam adicionados além do limite do vetor.

## Essa organização em funções tornou o código mais modular e facilitou a separação das responsabilidades de cada parte do analisador.

## 3. Reconhecimento dos tokens

Os tipos de tokens são definidos utilizando um `enum`:

```c
typedef enum {
    TOKEN_PALAVRA_RESERVADA,
    TOKEN_IDENTIFICADOR,
    TOKEN_NUMERO_INTEIRO,
    TOKEN_NUMERO_REAL,
    TOKEN_LITERAL_CARACTERE,
    TOKEN_OPERADOR,
    TOKEN_DELIMITADOR,
    TOKEN_ERRO
} TipoToken;
```

Cada token é armazenado em uma estrutura contendo seu tipo, lexema, linha e coluna.

### Palavras reservadas e identificadores

O programa reconhece as palavras reservadas:

```text
int
float
char
if
else
while
return
print
```

Identificadores podem começar com letras ou `_` e podem continuar com letras, números ou `_`.

Após ler toda a sequência, o programa verifica se o lexema é uma palavra reservada. Caso seja, ele recebe a categoria `PALAVRA_RESERVADA`; caso contrário, recebe a categoria `IDENTIFICADOR`.

Identificadores com mais de 31 caracteres são classificados como erro léxico, mas todo o identificador é consumido antes de continuar a análise.

### Números

O programa diferencia números inteiros e reais.

Exemplos:

```text
10      → NUMERO_INTEIRO
3.14    → NUMERO_REAL
0.25    → NUMERO_REAL
```

Também são tratados números malformados, como:

```text
10.
3..5
.5
12.3.4
```

Essas sequências são reconhecidas como um único erro léxico. A implementação consome os números e pontos relacionados à sequência antes de classificá-la como erro, evitando que um mesmo número malformado seja dividido em vários tokens.

### Literais de caractere

Um literal de caractere válido deve possuir exatamente um caractere entre aspas simples.

Exemplos válidos:

```text
'a'
'7'
'+'
```

Exemplos inválidos:

```text
''
'ab'
'x
```

O programa verifica se o literal foi fechado corretamente e se possui exatamente um caractere entre as aspas. Caso contrário, é gerado um erro léxico.

### Operadores

São reconhecidos operadores de um e dois caracteres.

Entre os operadores de dois caracteres estão:

```text
==
!=
<=
>=
&&
||
```

Também são reconhecidos operadores de um caractere, como:

```text
+
-
*
/
%
=
<
>
!
```

Em situações que exigem verificar o próximo caractere, o programa realiza uma leitura antecipada. Caso esse caractere não faça parte do token atual, ele é devolvido ao fluxo utilizando `ungetc`.

O caractere `/` recebe um tratamento específico, pois pode representar tanto o operador de divisão quanto o início de um comentário iniciado por `//`.

### Delimitadores

São reconhecidos os delimitadores:

```text
( ) { } [ ] ; ,
```

Cada delimitador é registrado como um token da categoria `DELIMITADOR`.

### Comentários, espaços e caracteres inválidos

Comentários iniciados por `//` são ignorados até o final da linha e não geram tokens.

Espaços, tabulações e quebras de linha também não aparecem como tokens, mas a contagem de linha e coluna continua sendo atualizada.

## Qualquer caractere que não pertença às categorias reconhecidas gera um token `ERRO_LEXICO`. Mesmo após encontrar um erro, o programa continua analisando o restante do arquivo.

## 4. Exemplo de entrada e saída

Considere a seguinte entrada:

```c
int numero = 10;

if(numero >= 5){
    numero = numero + 1;
}
```

O analisador identifica os elementos individualmente, apresentando a posição inicial, a categoria e o lexema:

```text
1:1 | PALAVRA_RESERVADA  | int
1:5 | IDENTIFICADOR      | numero
1:12 | OPERADOR           | =
1:14 | NUMERO_INTEIRO     | 10
1:16 | DELIMITADOR        | ;

3:1 | PALAVRA_RESERVADA  | if
3:3 | DELIMITADOR        | (
3:4 | IDENTIFICADOR      | numero
3:11 | OPERADOR          | >=
3:14 | NUMERO_INTEIRO    | 5
3:15 | DELIMITADOR       | )
3:16 | DELIMITADOR       | {

4:5 | IDENTIFICADOR      | numero
4:12 | OPERADOR          | =
4:14 | IDENTIFICADOR     | numero
4:21 | OPERADOR          | +
4:23 | NUMERO_INTEIRO    | 1
4:24 | DELIMITADOR       | ;

5:1 | DELIMITADOR        | }
```

Ao final da execução, o programa apresenta também o total de tokens e o total de erros léxicos encontrados.

---

## 5. Testes realizados

Foram criados 12 arquivos de teste.

A organização dos testes foi dividida entre os casos necessários para cumprir os requisitos obrigatórios definidos no enunciado e um teste adicional criado para verificar o funcionamento integrado do analisador.

### 5.1 Testes relacionados aos requisitos obrigatórios

Os seguintes testes foram criados para atender aos casos exigidos pelo professor:

| Arquivo                            | Objetivo                                                                                        |
| ---------------------------------- | ----------------------------------------------------------------------------------------------- |
| `01_palavras_identificadores.mc`   | Testar palavras reservadas, identificadores válidos e identificadores com mais de 31 caracteres |
| `02_numeros.mc`                    | Testar números inteiros e números reais válidos                                                 |
| `03_caracteres.mc`                 | Testar literais de caractere válidos e inválidos                                                |
| `04_operadores.mc`                 | Testar os operadores de um e dois caracteres                                                    |
| `05_delimitadores.mc`              | Testar todos os delimitadores                                                                   |
| `06_comentarios.mc`                | Testar comentários iniciados por `//`                                                           |
| `07_erros.mc`                      | Testar caracteres inválidos e a continuidade da análise após erros                              |
| `09_numeros_malformados.mc`        | Testar números reais malformados                                                                |
| `10_arquivo_vazio.mc`              | Testar um arquivo sem conteúdo                                                                  |
| `11_apenas_espacos_comentarios.mc` | Testar um arquivo contendo apenas espaços e comentários                                         |
| `12_tokens_sem_espacos.mc`         | Testar tokens escritos sem espaços, como `if(x>=10){x=x+1;}`                                    |

Esses testes correspondem aos casos mínimos solicitados pelo professor: palavras reservadas, identificadores, identificadores com mais de 31 caracteres, números inteiros e reais, números malformados, operadores, delimitadores, comentários, literais de caractere válidos e inválidos, caracteres inválidos, arquivo vazio, arquivo com espaços e comentários e tokens sem espaços.

### 5.2 Teste adicional

Além dos testes específicos exigidos, foi criado o arquivo:

```text
08_programa_completo.mc
```

Esse teste foi criado por iniciativa própria para verificar o funcionamento integrado do analisador em um programa MiniC mais completo.

O arquivo reúne diversos elementos reconhecidos pelo lexer, incluindo:

* declarações de variáveis;
* números inteiros;
* números reais;
* literais de caractere;
* palavras reservadas;
* identificadores;
* operadores;
* delimitadores;
* estruturas `while`;
* estruturas `if` e `else`;
* operadores relacionais e lógicos.

O objetivo desse teste foi verificar se as diferentes partes do analisador funcionavam corretamente juntas, e não apenas de forma isolada.

Dessa forma, além de testar individualmente cada requisito obrigatório, também foi realizado um teste mais próximo de uma situação real de utilização da linguagem MiniC.

---

## 6. Principais dificuldades encontradas

Uma das principais dificuldades encontradas foi o controle correto da posição dos tokens.

Como alguns tokens possuem mais de um caractere, foi necessário armazenar a linha e a coluna antes de iniciar a leitura completa do token. Assim, a posição exibida corresponde ao início do lexema.

Outra dificuldade foi a leitura antecipada de caracteres.

Em situações como `==`, `>=` e `//`, o analisador precisa verificar o próximo caractere para decidir qual token está sendo formado. Quando o caractere lido não pertence ao token atual, ele precisa ser devolvido ao fluxo de entrada com `ungetc`.

Também foi necessário tratar corretamente os números reais malformados. Inicialmente, sequências inválidas poderiam ser separadas em vários tokens. A solução adotada foi consumir toda a sequência formada por números e pontos antes de classificá-la como erro.

Além disso, foi necessário tomar cuidado com o tamanho dos vetores utilizados para armazenar os lexemas. Para evitar acessos fora dos limites, foi criada uma função específica para adicionar caracteres apenas enquanto houver espaço disponível.

---

# 7. Questões propostas

## 1. Por que palavras reservadas e identificadores podem começar sendo reconhecidos pela mesma regra?

Porque ambos seguem inicialmente a mesma estrutura: começam com uma letra ou `_` e podem continuar com letras, números ou `_`.

Por isso, o analisador primeiro lê toda a sequência como um possível identificador. Depois, verifica se o lexema corresponde a alguma palavra reservada da linguagem.

---

## 2. Por que operadores de dois caracteres devem ser verificados antes dos operadores de um caractere?

Porque alguns operadores de dois caracteres começam com símbolos que também podem formar operadores individuais.

Por exemplo, `==` começa com `=`.

Se o primeiro `=` fosse reconhecido imediatamente, o resultado seria dois tokens separados:

```text
=
=
```

Por isso, é necessário verificar primeiro se o próximo caractere forma um operador de dois caracteres.

---

## 3. Qual é a diferença entre um erro léxico e um erro sintático?

Um erro léxico ocorre quando uma sequência de caracteres não pode ser reconhecida como um token válido da linguagem.

Por exemplo:

```text
@
```

Um erro sintático ocorre quando os tokens são individualmente válidos, mas estão organizados de forma incorreta segundo as regras da linguagem.

Por exemplo:

```c
int = 10;
```

Nesse caso, os tokens podem ser reconhecidos pelo analisador léxico, mas a estrutura é sintaticamente inválida.

---

## 4. Por que o analisador deve continuar trabalhando depois de encontrar um símbolo inválido?

Porque podem existir outros erros no restante do arquivo.

Se a análise fosse interrompida no primeiro erro, seria necessário corrigir um problema por vez e executar novamente o programa.

Ao continuar a análise, o programa consegue identificar vários erros em uma única execução.

---

## 5. Qual é o risco de não verificar o limite do vetor utilizado para armazenar um lexema?

O programa pode escrever dados fora dos limites do vetor.

Isso pode causar comportamento indefinido, corrupção de memória ou encerramento inesperado do programa.

Por esse motivo, a implementação possui uma função específica que controla a quantidade de caracteres adicionados ao lexema.

---

## 6. Em qual etapa seria detectado o problema em `int = 10;`, considerando que todos os caracteres formam tokens válidos?

Esse problema seria detectado na etapa de análise sintática.

O analisador léxico reconheceria normalmente os tokens:

```text
PALAVRA_RESERVADA | int
OPERADOR           | =
NUMERO_INTEIRO     | 10
DELIMITADOR        | ;
```

Porém, a sequência não corresponde a uma declaração sintaticamente válida, pois seria esperado um identificador após `int`.

---

## 7. Qual seria a vantagem de armazenar os identificadores encontrados em uma tabela de símbolos?

Uma tabela de símbolos permite armazenar informações sobre os identificadores encontrados durante a análise do programa.

Ela poderia guardar, por exemplo:

* nome do identificador;
* tipo;
* escopo;
* posição de declaração.

Essas informações seriam úteis principalmente nas etapas posteriores de um compilador, como a análise semântica.

---

## 8. Conclusão

A atividade permitiu aplicar na prática conceitos relacionados à primeira etapa de um compilador: a análise léxica.

O analisador desenvolvido em C percorre o arquivo-fonte caractere por caractere, reconhece os tokens definidos para a linguagem MiniC e informa a linha, a coluna, a categoria e o lexema de cada elemento encontrado.

O programa também trata espaços, tabulações e comentários, reconhece operadores de um e dois caracteres, diferencia palavras reservadas de identificadores, identifica números inteiros e reais, reconhece literais de caractere e detecta erros léxicos sem interromper a análise.

Foram criados 12 arquivos de teste. A maior parte deles foi desenvolvida para atender aos casos obrigatórios definidos no enunciado, enquanto o arquivo `08_programa_completo.mc` foi criado como um teste adicional para verificar o funcionamento integrado do analisador em um programa mais completo.

A implementação também utiliza funções auxiliares para melhorar a organização e a legibilidade do código, além de realizar verificações para evitar acessos além dos limites dos vetores utilizados para armazenar os lexemas.

Dessa forma, o projeto atende aos principais requisitos da atividade e permitiu compreender, de forma prática, como funciona o processo de transformação de uma sequência de caracteres em tokens que poderão ser utilizados pelas etapas posteriores de um compilador.
