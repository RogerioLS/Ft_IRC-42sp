✅ Checklist Inicial (regras do subject)
| Item                     | Descrição                                      | Status                     |
|--------------------------|------------------------------------------------|----------------------------|
| 🔒 `Estabilidade`          | Programa não pode crashar em nenhuma circunstância | ⚠️ Monitorar durante os testes |
| 🛠️ `Makefile`             | Deve conter $(NAME), all, clean, fclean, re    | ✅ Será configurado         |
| 🧱 `Flags de compilação`    | -Wall -Wextra -Werror e -std=c++98             | ✅ Definiremos no Makefile  |
| 📦 `Normas C++98`          | Código deve ser compatível com C++98           | ⚠️ Atenção especial (sem C++11/14/17) |
| 🚫 `Bibliotecas externas`   | Proibido uso de Boost ou libs externas         | ✅ Garantiremos isso        |

---

✅ C++ features	Preferir versões em C++ (ex: <cstring> ao invés de <string.h>)	⚠️ Reforçar durante o desenvolvimento

---

✅ Objetivo do Projeto ft_irc
Construir um servidor IRC que:

Usa C++98, sem bibliotecas externas.
```bash
Roda via ./ircserv <port> <password>.
```

Usa poll() ou equivalente para gerenciar I/O (não pode usar fork(), nem chamadas de bloqueio direta como recv() ou send() fora do poll).

Implementa comandos básicos e de operadores (NICK, USER, PRIVMSG, JOIN, KICK, INVITE, TOPIC, MODE...).

🧠 Arquitetura de Alto Nível
📌 Componentes principais
```pgsql
+--------------------+
|     IRC Server     |
+--------------------+
        |
        | listens on <port>
        V
+--------------------------+
|  poll() event loop       |
|--------------------------|
| - Accept connections     |
| - Read/write sockets     |
| - Handle timeouts        |
+--------------------------+
        |
        V
+-------------------------------+
|   Connection Manager          |
|-------------------------------|
| - Autenticação (pass)         |
| - Parsing de comandos         |
| - Criação de Client objects   |
| - Dispatch para Handlers      |
+-------------------------------+
```

📦 Módulos principais:
Módulo	Responsabilidades
| Módulo                | Responsabilidades                                |
|-----------------------|--------------------------------------------------|
| main.cpp              | Inicializa o servidor, inicia o loop de eventos |
| Server.hpp/cpp        | Gerencia socket, poll, aceita conexões          |
| Client.hpp/cpp        | Armazena info de cada cliente conectado         |
| Parser.hpp/cpp        | Divide entrada de texto em comandos IRC         |
| CommandHandler.hpp/cpp| Executa lógica para cada comando (JOIN, PRIVMSG, etc.) |
| Channel.hpp/cpp       | Representa um canal IRC, com membros, modos, etc. |
| Utils.hpp/cpp         | Funções auxiliares (ex: validação de nickname)  |

---

🧱 Plano de Features (dividido por etapas)

🎯 Etapa 1: Setup e Infra:
```
Criar Makefile com regras básicas ($(NAME), all, clean, fclean, re)
Criar main.cpp que lê os argumentos <port> <password>
Criar classe Server que:
    - Cria socket TCP
    - Seta O_NONBLOCK
    - Faz bind, listen
Inicia o poll()
```

🎯 Etapa 2: Conexões e Loop:
```
Criar estrutura de pollfd com vector<pollfd>
Implementar aceitação de múltiplos clientes (accept)
Armazenar Client com fd, IP, etc.
```

🎯 Etapa 3: Autenticação e Identidade:
```
Implementar PASS, NICK e USER
Criar mapa fd => Client
Validar nickname único, nome de usuário, etc.
```

🎯 Etapa 4: Comandos obrigatórios:
```
Comando	Descrição
JOIN	Entra em um canal (cria se não existir)
PRIVMSG	Envia msg privada a usuário ou canal
KICK	(op) Expulsa cliente do canal
INVITE	(op) Convida usuário a entrar
TOPIC	(op) Define ou vê o tópico do canal
MODE	(op) Altera modos: i, t, k, o, l
```

🎯 Etapa 5: Channels:
```
Classe Channel com
Nome, senha, topic, limit, modos, operadores, convidados
Mapa de membros e operadores
Métodos: addUser, removeUser, broadcast, setMode, etc.
```

🎯 Etapa 6: Testes e Robustez:
```
Teste com nc (ex: nc -C 127.0.0.1 6667)
Teste de entrada fracionada (com, man, d\n separados)
Clientes reais: WeeChat, irssi, HexChat, etc.
```

---

🧩 Organização dos Arquivos
```bash
ft_IRC-42sp/
├── includes/              # Arquivos .hpp (cabeçalhos)
│   ├── Server.hpp
│   ├── Client.hpp
│   ├── Channel.hpp
│   ├── Parser.hpp
│   ├── CommandHandler.hpp
│   └── Utils.hpp          
│                
├── sorces/                # Arquivos .cpp (código fonte principal)
│   ├── main.cpp
│   ├── Server.cpp
│   ├── Client.cpp
│   ├── Channel.cpp
│   ├── Parser.cpp
│   ├── CommandHandler.cpp
│   └── Utils.cpp
│   └── main.cpp
├── Makefile               # Compilador e regras
├── README.md              # Instruções do projeto
├── tests/                 # Testes manuais e automáticos
│   └── test_clients/
└── .gitignore
```
---

📋 Sugestão de Cards no GitHub Projects
Você pode montar os seguintes cards iniciais no GitHub Projects:
🔧 Infraestrutura e Inicialização
| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| Criação do Makefile padrão            | Criar Makefile com regras NAME, all, clean, fclean, re.                   | infra, compilation  |
| Parse dos argumentos `<port> <password>` | Garantir que o programa só inicie com os dois argumentos corretos e válidos. | infra, parser       |
| Criação da classe Server com socket não bloqueante | Classe Server para iniciar o socket, configurar O_NONBLOCK, e escutar na porta. | infra, socket       |
| Configuração do `poll()`              | Implementar estrutura de `poll()` com `vector<pollfd>` para todos os FDs. | infra, poll         |

---

👥 Gerenciamento de Conexões

| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| Aceitar múltiplos clientes via `poll()` | Usar `poll()` para detectar conexões e adicionar ao vetor de fds.         | networking, poll    |
| Criação da classe Client              | Armazenar info de um cliente (fd, nickname, hostname, auth, etc.).        | networking, client  |
| Desconectar clientes com erro         | Detectar desconexão ou erro e liberar recursos do Client.                 | networking, error   |

---

🔐 Autenticação e Identidade

| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| Implementar comando PASS              | Validar a senha enviada pelo cliente.                                     | auth, command       |
| Implementar comando NICK              | Permitir definição e validação de nicknames únicos.                       | auth, command       |
| Implementar comando USER              | Armazenar username e realname do cliente.                                 | auth, command       |

---

💬 Comandos IRC Essenciais

| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| JOIN - Entrar ou criar canal          | Criar canal se não existir, adicionar cliente à lista de membros.         | command, channel    |
| PRIVMSG - Enviar mensagem             | Enviar mensagem privada a um usuário ou a todos de um canal.             | command, messaging  |
| TOPIC - Definir ou ver tópico do canal | Comando com permissões dependendo do modo `t`.                           | command, channel    |

---

👮‍♂️ Comandos de Operador

| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| KICK - Expulsar cliente do canal      | Só operadores podem usar. Atualizar lista de membros.                     | command, op, channel|
| INVITE - Convidar cliente             | Adiciona cliente à lista de convidados de um canal com modo `i`.          | command, op, channel|
| MODE - Modificar modos do canal       | Implementar modos `i`, `t`, `k`, `l`, `o` com regras.                     | command, op, channel|

---

🧪 Testes e Robustez

| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| Testar recebimento parcial de comandos (split) | Simular envio de comandos por partes como `com`, `man`, `d\n`.            | test, robustness    |
| Testar múltiplos clientes simultâneos | Validar comportamento com vários clientes conectados e comandos em paralelo. | test, scalability |
| Conectar cliente IRC real (HexChat / WeeChat) | Verificar se cliente consegue autenticar, mandar mensagens, etc.          | test, integration   |
| Testar canais privados e modos        | Criar canais com senha, limite e modos para verificar regras.             | test, mode          |

---

📎 Auxiliares e Organização

| Título                                | Descrição                                                                 | Labels              |
|---------------------------------------|---------------------------------------------------------------------------|---------------------|
| Parser - Dividir input em comandos IRC | Separar buffer recebido em comandos completos usando `\r\n`.              | utils, parser       |
| Logger ou debug printer opcional      | Ferramenta de log para debug em tempo real (usado só durante dev).         | utils, debug        |
| Documentação das funções e classes    | Comentários do tipo Doxygen e organização do `.hpp` vs `.cpp`.            | doc, refactor       |
