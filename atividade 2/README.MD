### Analisador Léxico MiniC

### Autor

Pedro Dutra

## Descrição

Este projeto consiste na implementação de um analisador léxico, também chamado de lexer ou scanner, para uma linguagem fictícia chamada **MiniC**.

O programa foi desenvolvido em linguagem C e realiza a leitura de um arquivo-fonte caractere por caractere. Durante a análise, o lexer identifica e classifica os elementos encontrados em tokens.

Para cada token reconhecido, o programa informa:

* linha em que o token começa;
* coluna em que o token começa;
* categoria do token;
* lexema identificado.

Ao final da análise, também são exibidos:

* total de tokens encontrados;
* total de erros léxicos encontrados.

---

## Estrutura do projeto

```text
atividade 2/
├── minilexer/
│   └── minilexer.c
│
└── testes/
    ├── 01_palavras_identificadores.mc
    ├── 02_numeros.mc
    ├── 03_caracteres.mc
    ├── 04_operadores.mc
    ├── 05_delimitadores.mc
    ├── 06_comentarios.mc
    ├── 07_erros.mc
    ├── 08_programa_completo.mc
    ├── 09_numeros_malformados.mc
    ├── 10_arquivo_vazio.mc
    ├── 11_apenas_espacos_comentarios.mc
    └── 12_tokens_sem_espacos.mc
```

---

# Tokens reconhecidos

O analisador léxico reconhece as seguintes categorias de tokens.

## Palavras reservadas

As palavras reservadas da linguagem MiniC são:

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

Essas palavras são classificadas como:

```text
PALAVRA_RESERVADA
```

---

## Identificadores

Um identificador:

* deve começar com uma letra ou `_`;
* pode continuar com letras, números ou `_`;
* possui no máximo 31 caracteres;
* deve ser diferente das palavras reservadas.

Exemplos:

```text
idade
_media
aluno2
valor_total
```

Um identificador com mais de 31 caracteres é classificado como um erro léxico.

---

## Números inteiros

Números formados apenas por algarismos são classificados como:

```text
NUMERO_INTEIRO
```

Exemplos:

```text
0
15
2026
```

---

## Números reais

Um número real válido deve possuir:

* uma parte inteira;
* um ponto decimal;
* uma parte decimal.

Exemplos:

```text
1.5
0.25
100.0
```

Esses valores são classificados como:

```text
NUMERO_REAL
```

Números malformados, como:

```text
10.
3..5
.5
12.3.4
```

são classificados como:

```text
ERRO_LEXICO
```

---

## Literais de caractere

Um literal de caractere deve possuir exatamente um caractere entre aspas simples.

Exemplos válidos:

```text
'a'
'7'
'+'
```

Categoria:

```text
LITERAL_CARACTERE
```

Exemplos inválidos:

```text
''
'ab'
'x
```

Esses casos são classificados como erros léxicos.

---

## Operadores

O analisador reconhece os seguintes operadores.

### Operadores de um caractere

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

### Operadores de dois caracteres

```text
==
!=
<=
>=
&&
||
```

Todos são classificados como:

```text
OPERADOR
```

---

## Delimitadores

Os delimitadores reconhecidos são:

```text
(
)
{
}
[
]
;
,
```

Categoria:

```text
DELIMITADOR
```

---

## Comentários

O analisador reconhece comentários de uma linha iniciados por:

```text
//
```

Todo o conteúdo após `//` até o final da linha é ignorado e não gera tokens.

Exemplo:

```c
int idade; // variável que armazena a idade
```

Nesse caso, apenas os tokens referentes a:

```text
int
idade
;
```

são reconhecidos.

---

# Compilação

Para compilar o programa, entre na pasta onde está localizado o arquivo `minilexer.c`.

Execute:

```bash
gcc -Wall -Wextra -pedantic -std=c11 minilexer.c -o minilexer
```

Esse comando gera o executável:

```text
minilexer
```

---

# Execução

Para executar o programa, informe o arquivo `.mc` que será analisado.

Exemplo:

```bash
./minilexer ../testes/01_palavras_identificadores.mc
```

Outro exemplo:

```bash
./minilexer ../testes/08_programa_completo.mc
```

O programa também pode ser utilizado com qualquer outro arquivo `.mc`:

```bash
./minilexer caminho/do/arquivo.mc
```

---

# Formato da saída

Cada token é exibido no formato:

```text
LINHA:COLUNA | CATEGORIA | LEXEMA
```

Exemplo:

```text
1:1 | PALAVRA_RESERVADA  | int
1:5 | IDENTIFICADOR      | idade
1:11 | OPERADOR           | =
1:13 | NUMERO_INTEIRO     | 18
1:15 | DELIMITADOR        | ;
```

Ao final da análise, são apresentados os totais:

```text
Total de tokens: 5
Total de erros lexicos: 0
```

---

# Decisões de implementação

A implementação foi organizada utilizando funções auxiliares para separar as diferentes responsabilidades do analisador.

As principais decisões foram:

* utilização de um `enum` para representar os tipos de tokens;
* utilização de uma `struct Token` para armazenar o tipo, lexema, linha e coluna;
* leitura do arquivo caractere por caractere utilizando `fgetc`;
* utilização de `ungetc` quando um caractere lido antecipadamente não pertence ao token atual;
* separação da verificação de início e continuação de identificadores;
* criação de funções auxiliares para reconhecimento de palavras reservadas e delimitadores;
* criação de uma função para criar tokens;
* criação de uma função para imprimir os tokens e atualizar os contadores;
* controle da posição de linha e coluna durante toda a leitura;
* tratamento de erros léxicos sem interromper a análise do arquivo;
* controle do tamanho do lexema para evitar escrita fora dos limites do vetor.

Para números malformados, o analisador consome toda a sequência relacionada ao erro antes de continuar a análise. Dessa forma, entradas como `3..5` são tratadas como um único erro léxico.

---

# Limitações

O projeto implementa apenas os requisitos definidos para a atividade.

Portanto, não são reconhecidos:

* comentários de múltiplas linhas `/* ... */`;
* strings entre aspas duplas;
* sequências de escape em literais de caractere;
* números em notação científica;
* análise sintática;
* verificação de tipos;
* balanceamento de parênteses ou chaves;
* tabela de símbolos;
* geração de código.

Essas funcionalidades estão fora do escopo da atividade ou pertencem a outras etapas de um compilador.

---

# Testes realizados

Foram criados 12 arquivos de teste para verificar diferentes situações.

| Arquivo                            | Conteúdo testado                                                   |
| ---------------------------------- | ------------------------------------------------------------------ |
| `01_palavras_identificadores.mc`   | Palavras reservadas e identificadores válidos                      |
| `02_numeros.mc`                    | Números inteiros e números reais                                   |
| `03_caracteres.mc`                 | Literais de caractere válidos                                      |
| `04_operadores.mc`                 | Operadores de um e dois caracteres                                 |
| `05_delimitadores.mc`              | Todos os delimitadores                                             |
| `06_comentarios.mc`                | Comentários de uma linha e divisão                                 |
| `07_erros.mc`                      | Caracteres inválidos, identificadores grandes e literais inválidos |
| `08_programa_completo.mc`          | Programa combinando diferentes tipos de tokens                     |
| `09_numeros_malformados.mc`        | Números reais inválidos                                            |
| `10_arquivo_vazio.mc`              | Arquivo sem conteúdo                                               |
| `11_apenas_espacos_comentarios.mc` | Arquivo contendo apenas espaços e comentários                      |
| `12_tokens_sem_espacos.mc`         | Tokens sem espaços entre eles                                      |

Os testes incluem casos válidos, inválidos e casos de fronteira.

Para executar todos os testes de uma vez:

```bash
for arquivo in ../testes/*.mc; do
    echo "========================================"
    echo "TESTANDO: $arquivo"
    echo "========================================"
    ./minilexer "$arquivo"
    echo
done
```

---

# Tecnologias utilizadas

* Linguagem C
* GCC
* Biblioteca padrão da linguagem C

---
