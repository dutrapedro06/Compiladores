# Compiladores

Repositório destinado às atividades e projetos desenvolvidos durante a disciplina de **Compiladores**, ministrada pelo professor [Murillo Eduardo](https://github.com/murilloed).

O objetivo deste repositório é acompanhar o desenvolvimento das atividades práticas da disciplina, aplicando conceitos relacionados às diferentes etapas de construção de um compilador.
---

## 👨‍💻 Autor

**Pedro Pereira Dutra**

Estudante de Ciência da Computação.

---

## 📁 Estrutura do Repositório

O repositório está organizado por atividades:

```text
Compiladores/
│
├── atividade 1/
│
├── atividade 2/
│   ├── minilexer/
│   │   └── minilexer.c
│   ├── testes/
│   ├── README.md
│   └── RELATORIO.md
│
└── README.md
```

## 📊 Status das Atividades

- [x] Atividade 1 — Concluída
- [x] Atividade 2 — Concluída
- [ ] Próximas atividades

---

## 📚 Atividades

### Atividade 1

Atividade inicial desenvolvida durante a disciplina, contendo os arquivos e exercícios realizados conforme as orientações propostas.

📁 Acesse a pasta da **Atividade 1** para visualizar os arquivos desenvolvidos.

---

### Atividade 2 — Analisador Léxico para MiniC

Implementação de um **analisador léxico** para uma linguagem simplificada chamada **MiniC**, desenvolvido utilizando a linguagem C.

O programa recebe um arquivo de código-fonte como entrada e realiza uma análise caractere por caractere, identificando os tokens presentes no arquivo.

Entre os elementos reconhecidos pelo analisador estão:

- Palavras reservadas;
- Identificadores;
- Números inteiros;
- Números reais;
- Literais de caractere;
- Operadores;
- Delimitadores;
- Comentários de uma linha;
- Erros léxicos.

O analisador também registra a **linha e a coluna** onde cada token foi encontrado.

Além dos testes obrigatórios solicitados na atividade, também foram implementados testes adicionais para validar casos de fronteira, incluindo:

- Números malformados;
- Arquivo vazio;
- Arquivo contendo apenas espaços e comentários;
- Tokens sem espaços entre si.

📁 Acesse a pasta da **Atividade 2** para visualizar o código, os arquivos de teste, o README específico e o relatório da atividade.

---

## 🛠️ Tecnologias Utilizadas

- Linguagem C;
- GCC;
- Git;
- GitHub;
- Visual Studio Code;
- Ubuntu / WSL.

---

## ▶️ Como Executar

Cada atividade possui sua própria estrutura e pode conter instruções específicas de execução.

Para mais detalhes, consulte o `README.md` presente dentro da pasta de cada atividade.

### Exemplo — Atividade 2

Entre na pasta do analisador léxico:

```bash
cd "atividade 2/minilexer"
```

Compile o programa:

```bash
gcc -Wall -Wextra -pedantic -std=c11 minilexer.c -o minilexer
```

Execute o programa passando um arquivo MiniC como argumento:

```bash
./minilexer ../testes/08_programa_completo.mc