# Documentação da Task 4: Comandos Essenciais

Este documento detalha a implementação e o funcionamento dos comandos IRC essenciais: `JOIN`, `PRIVMSG`, `TOPIC`, e `HELP`, conforme a task 4 do projeto.

## Arquitetura

A lógica de todos os comandos é centralizada na classe `CommandHandler`. Esta classe é responsável por:
1.  Receber a mensagem bruta do cliente.
2.  Fazer o parsing para extrair o nome do comando e seus argumentos.
3.  Mapear o nome do comando para a função de tratamento correspondente.
4.  Executar a função, que por sua vez interage com o estado do servidor (clientes, canais, etc.).

### IServer.hpp: A Interface do Servidor

`IServer.hpp` define uma **interface** (uma classe abstrata pura em C++) que estabelece um contrato para as operações que qualquer implementação de servidor deve fornecer. Seus principais propósitos são:

*   **Abstração e Polimorfismo:** Permite que outras partes do código, como o `CommandHandler` e as novas classes de comando (`JoinCommand`, `PrivmsgCommand`, etc.), interajam com o servidor através de um conjunto comum de métodos, sem depender dos detalhes específicos da implementação da classe `Server`.
*   **Desacoplamento:** Reduz o acoplamento entre as classes. Alterações na implementação interna de `Server` não afetam as classes que usam `IServer`, desde que a interface permaneça a mesma.
*   **Testabilidade (Mocks):** Facilita a criação de objetos simulados (`ServerMock`) para testes de unidade. O `ServerMock` implementa `IServer`, permitindo que os testes simulem o comportamento do servidor de forma isolada, sem a necessidade de um servidor real ou recursos de rede.

---

## Comandos Implementados

### 1. JOIN
*   **Descrição:** O comando `JOIN` é usado por um cliente para começar a escutar as mensagens de um canal específico. Se o canal não existir, ele é criado e o cliente que o criou se torna um operador do canal.
*   **Uso:** `JOIN <#canal>`
*   **Lógica de Implementação:**
    1.  O `CommandHandler` recebe o comando e verifica se o argumento `<#canal>` foi fornecido.
    2.  Ele valida se o nome do canal começa com o caractere `#`.
    3.  O `Server` é consultado para verificar se um canal com o nome fornecido já existe.
    4.  **Se o canal existe:** O ID do cliente é adicionado à lista de membros do canal.
    5.  **Se o canal não existe:** Um novo objeto `Channel` é criado através do `Server`. O cliente que emitiu o comando é automaticamente definido como o primeiro membro e operador do novo canal.
    6.  *(Futuro)*: Respostas numéricas apropriadas, como `RPL_TOPIC` e `RPL_NAMREPLY`, serão enviadas ao cliente para confirmar a entrada no canal e informar sobre seu estado atual.
*   **Implementação Detalhada:**
    *   O comando `JOIN` é encapsulado na classe `JoinCommand` (`includes/command/JoinCommand.hpp` e `sources/command/JoinCommand.cpp`).
    *   Sua lógica reside no método estático `static void execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug)`.
    *   Este método recebe referências à interface `IServer` (para interagir com o servidor), ao objeto `Client` (o cliente que enviou o comando), aos argumentos do comando (`args`) e ao objeto `Debug` (para logging).
    *   Ele utiliza `server.getChannelByName()` para buscar canais existentes e `server.createChannel()` para criar novos canais, demonstrando a interação com a interface do servidor.
    *   O `CommandHandler` agora simplesmente despacha a chamada para `JoinCommand::execute`, tornando sua própria implementação mais limpa e focada no roteamento de comandos.

### 2. PRIVMSG
*   **Descrição:** O comando `PRIVMSG` é usado para enviar uma mensagem privada para um usuário específico ou para todos os membros de um canal.
*   **Uso:**
    *   Para usuários: `PRIVMSG <nickname> :<mensagem>`
    *   Para canais: `PRIVMSG <#canal> :<mensagem>`
*   **Lógica de Implementação:**
    1.  O `CommandHandler` extrai o alvo (nickname ou canal) e a mensagem.
    2.  Ele formata a mensagem no padrão IRC: `:<remetente>!<usuário>@<host> PRIVMSG <alvo> :<mensagem>`.
    3.  **Se o alvo é um canal (`#`):**
        *   O servidor busca o canal pelo nome.
        *   Se encontrado, a mensagem é enviada para todos os clientes no canal, exceto o remetente.
        *   Se não encontrado, um erro (ERR_NOSUCHCHANNEL) é registrado no debug.
    4.  **Se o alvo é um usuário:**
        *   O servidor busca o cliente pelo nickname.
        *   Se encontrado, a mensagem é enviada diretamente para o descritor de arquivo (socket) do cliente.
        *   Se não encontrado, um erro (ERR_NOSUCHNICK) é registrado no debug.
*   **Implementação Detalhada:**
    *   A lógica do `PRIVMSG` está na classe `PrivmsgCommand` (`includes/command/PrivmsgCommand.hpp` e `sources/command/PrivmsgCommand.cpp`).
    *   O método estático `static void execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug)` é responsável por sua execução.
    *   Ele interage com o servidor através de `IServer` para obter informações sobre canais (`server.getChannelByName()`) e clientes (`server.getClientByNickname()`, `server.getClientById()`).
    *   O envio de mensagens é realizado utilizando `server.sendMessage()`, garantindo que a comunicação passe pela interface do servidor, o que é vital para a testabilidade (como visto com o `ServerMock`).
    *   A formatação da mensagem IRC completa é feita dentro deste método, garantindo a conformidade com o protocolo.

### 3. TOPIC
*   **Descrição:** O comando `TOPIC` é usado para visualizar ou modificar o tópico de um canal.
*   **Uso:**
    *   Para visualizar: `TOPIC <#canal>`
    *   Para modificar: `TOPIC <#canal> :<novo_tópico>`
*   **Lógica de Implementação:**
    1.  O `CommandHandler` verifica se o canal existe.
    2.  **Se um novo tópico não for fornecido (visualização):**
        *   O servidor envia uma resposta numérica ao cliente. `RPL_TOPIC` (332) se o tópico existir, ou `RPL_NOTOPIC` (331) caso contrário.
    3.  **Se um novo tópico for fornecido (modificação):**
        *   O sistema verifica se o canal tem o modo `+t` (tópico restrito a operadores) ativo.
        *   Se o modo `+t` estiver ativo, ele verifica se o cliente é um operador do canal. Se não for, uma mensagem de erro (`ERR_CHANOPRIVSNEEDED`) é registrada no debug.
        *   Se o cliente tiver permissão, o novo tópico é definido no objeto `Channel`.
        *   Uma mensagem `TOPIC` é enviada para todos os membros do canal, informando sobre a alteração.
*   **Implementação Detalhada:**
    *   A classe `TopicCommand` (`includes/command/TopicCommand.hpp` e `sources/command/TopicCommand.cpp`) contém a lógica para este comando.
    *   O método estático `static void execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug)` gerencia a visualização e a modificação do tópico.
    *   Ele utiliza `server.getChannelByName()` para acessar o canal e `server.getServerName()` para formatar as respostas do servidor.
    *   O envio de mensagens de resposta (RPL_TOPIC, RPL_NOTOPIC) e de broadcast (TOPIC para todos os membros do canal) é feito através de `server.sendMessage()`, mantendo a consistência com a interface.
    *   A verificação de permissões de operador (`channel->getRestrictTopic()` e `channel->getOperatorsById()`) é realizada diretamente na lógica do comando.

### 4. HELP
*   **Descrição:** O comando `HELP` exibe uma lista de todos os comandos disponíveis e uma breve descrição de seu uso.
*   **Uso:** `HELP`
*   **Lógica de Implementação:**
    1.  Ao receber o comando `HELP`, o `CommandHandler` não requer argumentos adicionais.
    2.  Uma série de mensagens `NOTICE` é enviada diretamente ao cliente que solicitou a ajuda.
    3.  Cada mensagem `NOTICE` contém um comando e sua sintaxe, formatada para fácil leitura.
    4.  Isso fornece ao usuário uma referência rápida dentro do próprio cliente de IRC, sem a necessidade de consultar documentação externa.
*   **Implementação Detalhada:**
    *   A classe `HelpCommand` (`includes/command/HelpCommand.hpp` e `sources/command/HelpCommand.cpp`) é responsável por este comando.
    *   Seu método estático `static void execute(IServer& server, Client& client, const std::vector<std::string>& args, Debug& debug)` gera e envia as mensagens de ajuda.
    *   Ele obtém o nome do servidor (`server.getServerName()`) e o nickname do cliente (`client.getClientNickName()`) para personalizar as mensagens `NOTICE`.
    *   Todas as mensagens de ajuda são enviadas ao cliente através de múltiplas chamadas a `server.sendMessage()`, garantindo que a interface de comunicação do servidor seja utilizada.
    *   Este comando é um exemplo simples de como a lógica de apresentação de informações pode ser encapsulada em uma classe de comando dedicada.
