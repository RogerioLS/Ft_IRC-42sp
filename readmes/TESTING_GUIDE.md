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

### Passo a Passo do Teste (usando `nc`)

1.  **Compile e rode o servidor** em um terminal:
    ```bash
    make
    ./ircserv 6667 password
    ```

2.  **Em um segundo terminal, conecte-se** com `nc`:
    ```bash
    nc localhost 6667
    ```

3.  **Registre-se no servidor.** Voc�� deve enviar os comandos `PASS`, `NICK` e `USER` nesta ordem. Digite cada linha e pressione Enter.
    ```
    PASS password
    NICK mynick
    USER myuser 0 * :My Real Name
    ```
    *Se tudo der certo, o servidor registrará você.*

4.  **Teste os comandos:**
    -   **`JOIN`**: Crie e entre em um canal.
        ```
        JOIN #test
        ```
    -   **`TOPIC`**: Defina um tópico para o canal.
        ```
        TOPIC #test :Hello from the topic!
        ```
    -   **`PRIVMSG`**: Envie uma mensagem para o canal.
        ```
        PRIVMSG #test :This is a message to the channel.
        ```
    -   **`HELP`**: Peça a lista de comandos.
        ```
        HELP
        ```
        *O servidor deve responder com mensagens `NOTICE` contendo a ajuda.*

Para testar mensagens privadas, você precisará de um terceiro terminal para conectar um segundo cliente com um apelido diferente.