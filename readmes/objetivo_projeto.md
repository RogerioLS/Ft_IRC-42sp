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

🛠 Infraestrutura
```bash
Setup inicial do projeto
Criar Makefile conforme normas 42
Parse dos argumentos <port> <password>
Criar loop com poll()
```

👥 Conexões
```bash
Aceitar conexões TCP não bloqueantes
Criar objeto Client
Encerrar conexões com erro
```

🔐 Autenticação
```bash
Comando PASS
Comando NICK
Comando USER
```

💬 IRC Commands
```bash
JOIN
PRIVMSG
TOPIC
INVITE
MODE (i, t, k, o, l)
KICK
```

🧪 Testes:
```bash
Teste com nc
Teste com cliente IRC real
Teste com mensagens cortadas
Teste de múltiplas conexões simultâneas
```
