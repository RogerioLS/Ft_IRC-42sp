### Checklist do Projeto `ft_irc`

#### 1. Requisitos Fundamentais (Conforme o Subject)
- **✅ Compilação e Normas:**
  - O `Makefile` principal compila o projeto com as flags `-Wall -Wextra -Werror -std=c++98`.
  - O projeto não utiliza bibliotecas externas proibidas.
- **✅ Estrutura do Servidor:**
  - O executável `ircserv` aceita os argumentos `<port>` e `<password>`.
  - O servidor utiliza `epoll` (equivalente a `poll`) para I/O não-bloqueante.
  - O servidor é capaz de lidar com múltiplas conexões de clientes sem usar `fork`.
- **✅ Autenticação e Comandos Básicos:**
  - **`PASS`**: Implementado e funcional para autenticação.
  - **`NICK`**: Implementado, com validação de unicidade.
  - **`USER`**: Implementado, completa o registro do cliente.
  - **`JOIN`**: Implementado, permite entrar e criar canais.
  - **`PRIVMSG`**: Implementado, para mensagens a usuários e canais.
  - **`HELP`**: Implementado, para exibir ajuda ao usuário.

#### 2. Comandos de Operador de Canal
- **✅ `TOPIC`**: Implementado, permite visualizar e alterar o tópico do canal.
- **✅ `KICK`**: Implementado, permite a um operador expulsar um usuário.
- **✅ `INVITE`**: Implementado, permite a um operador convidar um usuário para um canal.
- **✅ `MODE`**: Implementado, com suporte para as seguintes flags de canal:
  - **`i`**: Canal apenas para convidados.
  - **`t`**: Tópico restrito a operadores.
  - **`k`**: Chave (senha) do canal.
  - **`o`**: Conceder/remover privilégio de operador.
  - **`l`**: Limite de usuários no canal.

#### 3. Testes e Robustez (Conforme `issues.txt` e `TESTING_GUIDE.md`)
- **✅ Testes de Unidade (GTest):**
  - A estrutura de testes com Google Test está funcional.
  - Os testes para os comandos básicos (`PASS`, `NICK`, `USER`, `JOIN`, `PRIVMSG`, `TOPIC`, `HELP`) e para o `Parser` estão passando.
- **⚠️ Testes de Robustez (A verificar):**
  - **Recebimento parcial de comandos:** A lógica do `Parser` e do buffer do cliente parece lidar com isso, mas precisa de testes manuais mais rigorosos para garantir 100% de robustez.
  - **Múltiplos clientes simultâneos:** A arquitetura com `epoll` suporta isso, mas o comportamento em cenários de alta concorrência precisa ser validado manualmente.
- **🔲 Validação com Cliente Real:**
  - Conectar com um cliente IRC como HexChat, irssi, etc., para validar a experiência do usuário e a compatibilidade das respostas do servidor. Este é um passo manual importante que ainda precisa ser feito.

#### 4. Qualidade e Organização
- **✅ Modularidade:** O código está bem organizado, com a lógica de cada comando separada em sua própria classe (`JoinCommand`, `KickCommand`, etc.).
- **✅ Utilitários:** A classe `Debug` está implementada e o `Parser` lida com a formatação das mensagens.
- **⚠️ Documentação:** O código possui uma boa estrutura, mas a documentação interna (comentários Doxygen, por exemplo) pode ser expandida para detalhar melhor as funções e classes.
