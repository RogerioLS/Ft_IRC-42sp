# Guia de Testes - Ft_IRC

Este documento descreve o passo a passo para testar a funcionalidade do servidor IRC, incluindo a conexão, registro e a verificação dos comandos implementados (`JOIN`, `TOPIC`, `PRIVMSG`, `HELP`).

## Pré-requisitos

1.  **Servidor em Execução:** O servidor `ircserv` deve estar compilado e em execução.
    ```bash
    ./ircserv <porta> <senha>
    ```
    Para este guia, vamos assumir que o servidor está rodando com:
    ```bash
    ./ircserv 6667 pass
    ```

2.  **Cliente IRC:** Você precisa de um cliente IRC. O `irssi` é uma opção popular para terminais. Outros clientes gráficos como HexChat ou mIRC também funcionam.

---

## Passo a Passo do Teste

### 1. Conexão e Registro

1.  **Inicie seu cliente IRC.** Se estiver usando `irssi`, abra um novo terminal e digite `irssi`.

2.  **Conecte-se ao servidor:**
    *   **Comando:** `/connect 127.0.0.1 6667 pass`
    *   **Resultado Esperado:** O cliente deve estabelecer uma conexão com o servidor. Você verá mensagens do servidor confirmando a conexão.

3.  **Defina seu apelido (Nickname):**
    *   **Comando:** `/NICK tester1`
    *   **Resultado Esperado:** Seu apelido no servidor será definido como `tester1`.

4.  **Defina suas informações de usuário:**
    *   **Comando:** `/USER tester1 0 * :Test User One`
    *   **Resultado Esperado:** O servidor completará seu registro. Você estará oficialmente online.

### 2. Teste dos Comandos

#### a) Comando `JOIN`

1.  **Entrar em um novo canal:**
    *   **Comando:** `/JOIN #test-channel`
    *   **Resultado Esperado:**
        *   Você entrará no canal `#test-channel`.
        *   Como o canal não existia, ele será criado.
        *   Você se tornará um operador do canal.
        *   O servidor pode enviar respostas numéricas como `RPL_TOPIC` (informando que não há tópico) e `RPL_NAMREPLY` (listando os usuários no canal, que será apenas você).

#### b) Comando `TOPIC`

1.  **Verificar o tópico (inexistente):**
    *   **Comando:** `/TOPIC #test-channel`
    *   **Resultado Esperado:** O servidor deve responder com uma mensagem indicando que "No topic is set" (RPL_NOTOPIC 331).

2.  **Definir um novo tópico:**
    *   **Comando:** `/TOPIC #test-channel :Este é o primeiro tópico do canal!`
    *   **Resultado Esperado:**
        *   Você (e qualquer outra pessoa no canal) receberá uma mensagem `TOPIC` confirmando a alteração.
        *   O tópico do canal `#test-channel` agora está definido.

3.  **Verificar o tópico (existente):**
    *   **Comando:** `/TOPIC #test-channel`
    *   **Resultado Esperado:** O servidor deve responder com o tópico que você acabou de definir: "Este é o primeiro tópico do canal!" (RPL_TOPIC 332).

#### c) Comando `PRIVMSG`

1.  **Enviar mensagem para o canal:**
    *   **Comando:** `/MSG #test-channel Olá, mundo!`
    *   **Resultado Esperado:** A mensagem "Olá, mundo!" aparecerá na janela do canal `#test-channel`, enviada por `tester1`.

2.  **Enviar mensagem privada (requer um segundo cliente):**
    *   Abra um segundo cliente IRC, conecte-se e registre-se com o nick `tester2`.
    *   Faça o `tester2` entrar no canal: `/JOIN #test-channel`.
    *   No cliente `tester1`, envie uma mensagem privada para `tester2`:
    *   **Comando:** `/MSG tester2 Esta é uma mensagem privada.`
    *   **Resultado Esperado:** Apenas o cliente `tester2` receberá a mensagem "Esta é uma mensagem privada." de `tester1`. A mensagem não deve aparecer no canal `#test-channel`.

#### d) Comando `HELP`

1.  **Solicitar ajuda:**
    *   **Comando:** `/HELP`
    *   **Resultado Esperado:** O servidor enviará uma série de mensagens `NOTICE` diretamente para você, listando os comandos disponíveis e sua sintaxe, como implementado no `_handleHelp`:
        ```
        -irc.42sp- NOTICE tester1 :Available commands:
        -irc.42sp- NOTICE tester1 :  JOIN <#channel> - Join or create a channel.
        -irc.42sp- NOTICE tester1 :  PRIVMSG <target> :<message> - Send a private message to a user or channel.
        -irc.42sp- NOTICE tester1 :  TOPIC <#channel> [:<new_topic>] - View or set the topic of a channel.
        -irc.42sp- NOTICE tester1 :  HELP - Shows this help message.
        ```

---

Se todos os passos acima funcionarem como descrito, a implementação dos comandos pode ser considerada um sucesso.
