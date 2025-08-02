# Guia de Testes - Ft_IRC

Este documento descreve como testar a funcionalidade do servidor IRC. Ele é dividido em duas seções: testes manuais, para interação direta com o servidor, e testes automatizados, que verificam a lógica interna dos comandos.

---

## Testes Automatizados (GTest)

O projeto utiliza o framework Google Test (GTest) para testes de unidade. Esses testes são projetados para verificar a lógica do `CommandHandler` de forma rápida e isolada, sem a necessidade de iniciar o servidor de rede completo.

### Estrutura

-   **Makefile para Testes:** O arquivo `Makefile_test` é dedicado a compilar e executar os testes.
-   **Código de Teste:** Os arquivos de teste estão localizados em `tests/unit/`.
-   **Isolamento com Mocks:** Para evitar a complexidade e a instabilidade dos testes de rede, usamos um objeto "mock". A classe `ServerMock` simula o comportamento de um servidor real (gerenciando canais e clientes em memória), permitindo que o `CommandHandler` seja testado de forma independente e confiável.

### Como Executar os Testes

Abra seu terminal na raiz do projeto e use os seguintes comandos:

1.  **Compilar e Executar Todos os Testes:**
    Este é o comando mais comum. Ele limpa builds antigos, compila e executa todo o conjunto de testes.
    ```bash
    make -f Makefile_test clean run
    ```

2.  **Apenas Compilar os Testes:**
    Se você quiser apenas verificar se os testes compilam sem executá-los.
    ```bash
    make -f Makefile_test all
    ```

3.  **Apenas Limpar os Arquivos de Build dos Testes:**
    Remove o diretório `build_tests/`.
    ```bash
    make -f Makefile_test clean
    ```

---

## Testes Manuais

Esta seção descreve o passo a passo para testar a funcionalidade do servidor IRC manualmente.

### Pré-requisitos

1.  **Servidor em Execução:** O servidor `ircserv` deve estar compilado e em execução.
    ```bash
    make
    ./ircserv <porta> <senha>
    ```
    Para este guia, vamos assumir que o servidor está rodando com:
    ```bash
    ./ircserv 6667 pass
    ```

2.  **Cliente IRC:** Você precisa de um cliente IRC. `nc` (netcat) é a opção mais simples para o terminal. Clientes mais completos como `irssi`, ou clientes gráficos como HexChat, também funcionam.

### 1. Autenticação e Registro

Para interagir com o servidor, um cliente deve primeiro se autenticar. A sequência correta é `PASS`, `NICK`, e `USER`.

**Passos:**

1.  **Compile e rode o servidor** em um terminal:
    ```bash
    make
    ./ircserv 6667 password
    ```

2.  **Em um segundo terminal, conecte-se** com `nc`:
    ```bash
    nc localhost 6667
    ```

3.  **Envie os comandos de autenticação** na ordem correta:

    ```
    PASS password
    NICK mynick
    USER myuser 0 * :My Real Name
    ```

**O que esperar:**

-   Após enviar os três comandos corretamente, o servidor deve enviar a mensagem de boas-vindas (`RPL_WELCOME`), confirmando que o registro foi bem-sucedido.

**Cenários de Erro:**

-   **Senha incorreta:**
    ```
    PASS wrongpassword
    ```
    *O servidor deve responder com `ERR_PASSWDMISMATCH`.*

-   **`NICK` ou `USER` antes de `PASS`:**
    ```
    NICK mynick
    ```
    *O servidor deve ignorar o comando ou enviar uma mensagem de erro indicando que a autenticação é necessária.*

-   **Apelido (nick) já em uso:**
    *Abra um terceiro terminal, conecte outro cliente e registre-o com `mynick`. Depois, no segundo terminal, tente usar o mesmo nick.*
    ```
    NICK mynick
    ```
    *O servidor deve responder com `ERR_NICKNAMEINUSE`.*

### 2. Comandos de Canal

#### JOIN - Entrar em um Canal

-   **Função:** Permite que um cliente entre em um canal. Se o canal não existir, ele é criado e o cliente se torna seu primeiro operador.
-   **Sintaxe:** `JOIN #<nome_do_canal>`

-   **Teste de Sucesso:**
    ```
    JOIN #test
    ```
    **O que esperar:**
    1.  O servidor envia uma mensagem para todos no canal (incluindo você) confirmando que você entrou.
    2.  O servidor envia o tópico do canal (`RPL_TOPIC`) ou uma mensagem de que não há tópico (`RPL_NOTOPIC`).
    3.  O servidor envia a lista de usuários no canal (`RPL_NAMREPLY`), prefixando operadores com `@`.
    4.  O servidor envia a mensagem de fim da lista (`RPL_ENDOFNAMES`).

-   **Cenários de Erro:**
    -   **Canal com nome inválido:**
        ```
        JOIN test
        ```
        *O servidor deve responder com `ERR_BADCHANMASK`.*
    -   **Comando sem argumentos:**
        ```
        JOIN
        ```
        *O servidor deve responder com `ERR_NEEDMOREPARAMS`.*

#### PRIVMSG - Enviar Mensagens

-   **Função:** Envia uma mensagem para um canal ou para um usuário específico.
-   **Sintaxe:**
    -   Para canal: `PRIVMSG #<nome_do_canal> :<sua mensagem>`
    -   Para usuário: `PRIVMSG <apelido_do_usuario> :<sua mensagem>`

-   **Teste de Sucesso (Canal):**
    *Conecte um segundo cliente (vamos chamá-lo de `friend_nick`) e faça-o entrar no mesmo canal (`JOIN #test`).*
    No terminal do primeiro cliente (`mynick`):
    ```
    PRIVMSG #test :Hello everyone!
    ```
    **O que esperar:**
    -   O cliente `friend_nick` deve receber a mensagem: `:mynick!myuser@localhost PRIVMSG #test :Hello everyone!`

-   **Teste de Sucesso (Usuário):**
    No terminal do primeiro cliente (`mynick`):
    ```
    PRIVMSG friend_nick :Hi there!
    ```
    **O que esperar:**
    -   O cliente `friend_nick` deve receber a mensagem privada: `:mynick!myuser@localhost PRIVMSG friend_nick :Hi there!`

-   **Cenários de Erro:**
    -   **Canal ou usuário não existe:**
        ```
        PRIVMSG #nonexistent :...
        PRIVMSG nonexistent_nick :...
        ```
        *O servidor deve responder com `ERR_NOSUCHCHANNEL` ou `ERR_NOSUCHNICK`.*
    -   **Não está no canal (para enviar mensagem):**
        *Tente enviar uma mensagem para um canal que você não entrou.*
        *O servidor deve responder com `ERR_CANNOTSENDTOCHAN`.*

#### TOPIC - Gerenciar Tópico do Canal

-   **Função:** Permite ver ou definir o tópico de um canal.
-   **Sintaxe:**
    -   Ver tópico: `TOPIC #<nome_do_canal>`
    -   Definir tópico: `TOPIC #<nome_do_canal> :<novo tópico>`

-   **Teste de Sucesso:**
    1.  **Definir o tópico (como operador):**
        ```
        TOPIC #test :This is a cool topic!
        ```
        **O que esperar:** O servidor informa a todos no canal que o tópico foi alterado.

    2.  **Ver o tópico:**
        ```
        TOPIC #test
        ```
        **O que esperar:** O servidor responde com `RPL_TOPIC`, mostrando o tópico atual.

-   **Cenários de Erro:**
    -   **Canal não existe:**
        ```
        TOPIC #nonexistent
        ```
        *O servidor deve responder com `ERR_NOSUCHCHANNEL`.*
    -   **Usuário não está no canal:**
        *Com um segundo cliente, tente ver o tópico de um canal no qual ele não está.*
        ```
        TOPIC #test
        ```
        *O servidor deve responder com `ERR_NOTONCHANNEL`.*
    -   **Permissão negada (modo `+t` ativo e usuário não é operador):**
        *Com um segundo cliente que não é operador, tente definir o tópico.*
        ```
        TOPIC #test :Trying to change the topic
        ```
        *O servidor deve responder com `ERR_CHANOPRIVSNEEDED`.*

### 3. Comando HELP

-   **Função:** Mostra informações de ajuda sobre os comandos disponíveis.
-   **Sintaxe:** `HELP`

-   **Teste de Sucesso:**
    ```
    HELP
    ```
    **O que esperar:**
    -   O servidor deve responder com uma ou mais mensagens `NOTICE`, listando os comandos suportados e uma breve descrição de cada um.
