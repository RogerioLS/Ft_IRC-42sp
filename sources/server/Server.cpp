#include "../includes/server/Server.hpp"
#include "../includes/command/CommandHandler.hpp"
#include "../includes/utils/Colors.hpp"
#include "../includes/utils/Utils.hpp"
#include "../includes/parser/Parser.hpp"


Server::Server(char **argv, Debug& debug)
 : _port(std::atoi(argv[1])),
  _password(argv[2]),
  _serverName("irc.42sp"),
  _serverFd(-1),
	_epollFd(-1),
	_gSignalStatus(-1),
	_debug(debug)
{
	instance = this;
	_commandHandler = new CommandHandler(*this, _debug);
	_debug.debugPrint("✅ Server initialized", GREEN);
}

Server::~Server()
{
	delete _commandHandler;
	closeFds();
	_debug.debugPrint("✅ Server destroyed", RED);
}

void Server::setupServer() {
	setupServerSocket();
	setupEpollEvent();
	setupEpollLoop();
	setupClientVector();
}
void Server::setupServerSocket() {
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(getPort());
	setServerFd(socket(AF_INET, SOCK_STREAM, 0));
	if (getServerFd() < 0)
		throw std::runtime_error("Error creating socket");
	if (fcntl(getServerFd(), F_SETFL, O_NONBLOCK) < 0)
		throw std::runtime_error("Error setting socket option O_NONBLOCK");
	if (bind(getServerFd(), (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		throw std::runtime_error("Error at binding.");
	if (listen(getServerFd(), SOMAXCONN) < 0)
		throw std::runtime_error("Error at listening.");
}
void Server::setupEpollEvent() {
	setEpollFd(epoll_create(1));
	if (getEpollFd() < 0)
		throw std::runtime_error("Error creating epool");
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = getServerFd();
	if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, getServerFd(), &ev) < 0)
		throw std::runtime_error("Error epoll listen server fd");
	_eventsVector.reserve(INITIAL_EVENT_VECTOR_SIZE);
	_eventsVector.push_back(ev);
}
void Server::setupEpollLoop() {
	while (getServerRunning() == true) {
		int nfds = epoll_wait(_epollFd, _eventsVector.data(), _eventsVector.size(), 0);
		if (nfds < 0)
			throw std::runtime_error("Error epoll_wait");
		for (int n = 0; n < nfds; ++n) {
			int clientFd = _eventsVector[n].data.fd;
			if (clientFd == getServerFd())
				handleNewClient();
			else
				handleClientRequest(clientFd);
		resizeVector(static_cast<std::size_t>(nfds), _eventsVector);
		resizeVector(_clientsVector.size(), _clientsVector);
		}
	}
}
void Server::setupClientVector() {
	if (_clientsVector.empty())
		_clientsVector.reserve(INITIAL_CLIENT_VECTOR_SIZE);
}
void Server::handleNewClient() {
	sockaddr_in client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	socklen_t client_len = sizeof(client_addr);
	int conn_socket = accept(getServerFd(),(struct sockaddr *) &client_addr, &client_len);
	if (conn_socket == -1)
		std::cerr << YELLOW << "Failed to accept client connection. Retrying..." << RESET << std::endl;
	else {
		if (fcntl(conn_socket, F_SETFL, O_NONBLOCK) < 0) {
			std::cerr << YELLOW << "Failed to set client connection to non block. Retrying..." << RESET << std::endl;
			return;
		}
		epoll_event client_event;
		client_event.events = EPOLLIN | EPOLLET;
		client_event.data.fd = conn_socket;
		std::string clientIp = inet_ntoa(client_addr.sin_addr);
		// uint16_t clientPort = ntohs(client_addr.sin_port);
		//TODO getbuffer from client to validate password and connect if its right
		std::cout << GREEN << "Client connected. " << "fd: " << conn_socket <<  RESET << std::endl;
		// _clientsVector.push_back(Client(conn_socket, clientPort, clientIp));
		if (epoll_ctl(getEpollFd(), EPOLL_CTL_ADD, conn_socket, &client_event) < 0) {
			std::cerr << YELLOW << ("Error epoll listen client fd") << RESET << std::endl;
			return;
		}
		std::cout << BOLD << "Client count: " << getClientCount() <<  RESET << std::endl;
	}
}
void Server::handleClientRequest(int clientFd) {
	std::vector<Client>::iterator it = clientItFromFd(clientFd);
	if (it != _clientsVector.end()) {
		_debug.debugPrint("Client interacted. fd: " + utils::intToString(clientFd), MAGENTA);
		char buffer[1024];
		ssize_t bytesRead = recv(clientFd, buffer, sizeof(buffer) - 1, 0);

		if (bytesRead <= 0) {
			_debug.debugPrint("Client id: " + utils::intToString(it->getClientId()) + " disconnected", YELLOW);
			close(it->getClientFd());
			_clientsVector.erase(it);
		} else {
			buffer[bytesRead] = '\0';
			it->appendClientBuffer(std::string(buffer));
			Parser::appendParsedCommand(*it);

			std::vector<std::string> commands = it->getClientParsedCommand();
			for (size_t i = 0; i < commands.size(); ++i) {
				_commandHandler->executeCommand(*it, commands[i]);
			}
			it->clearParsedCommands();
		}
	}
}
// Getters
int		Server::getPort() const { return _port; }
const std::string& Server::getPassword() const { return _password; }
const std::string& Server::getServerName() const { return _serverName; }
int		Server::getServerFd() const { return _serverFd; }
int		Server::getEpollFd() const { return _epollFd; }
int		Server::getClientCount() const { return _clientsVector.size(); }
int	Server:: getServerRunning() const { return _gSignalStatus; }
Debug& Server::getDebug() { return _debug; }

Channel* Server::getChannelByName(const std::string& name) {
	for (std::vector<Channel>::iterator it = _channelsVector.begin(); it != _channelsVector.end(); ++it) {
		if (it->getName() == name) {
			return &(*it);
		}
	}
	return NULL;
}

Client* Server::getClientByNickname(const std::string& nickname) {
	for (std::vector<Client>::iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
		if (it->getClientNickName() == nickname) {
			return &(*it);
		}
	}
	return NULL;
}

Client* Server::getClientById(int id) {
	for (std::vector<Client>::iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
		if (it->getClientId() == id) {
			return &(*it);
		}
	}
	return NULL;
}

// Setters
void Server::setServerFd(int serverFd) { _serverFd = serverFd; }
void Server::setEpollFd(int epollFd) { _epollFd = epollFd; }
void Server::setServerRunning(int gSignalStatus) { _gSignalStatus = gSignalStatus; }

void Server::createChannel(const std::string& name, Client& client) {
	int channelId = _channelsVector.size() + 1;
	_channelsVector.push_back(Channel(name, channelId, client.getClientId()));
	_debug.debugPrint("Channel " + name + " created by client " + client.getClientNickName(), GREEN);
}

template<typename T>
void Server::resizeVector(std::size_t currSize, std::vector<T>& vectorToResize) {
	if (vectorToResize.size() <= currSize)
		vectorToResize.reserve(vectorToResize.size() * 2);
}
std::vector<Client>::iterator Server::clientItFromFd(int fd) {
	for (std::vector<Client>::iterator it = _clientsVector.begin(); it != _clientsVector.end(); ++it) {
		if (it->getClientFd() == fd)
			return it;
	}
	return _clientsVector.end();
}


std::vector<Client> &Server::getClientsVector() {
	return _clientsVector;
}
void Server::closeFds() {

	if (getServerFd() != -1)
		close(getServerFd());

	if (getEpollFd() != -1)
		close(getEpollFd());

	std::vector<Client>::iterator it = _clientsVector.begin();
	for(std::vector<Client>::iterator ite = _clientsVector.end(); it != ite; it++) {
		int clientFd = it->getClientFd();
		if (clientFd != -1)
			close(clientFd);
	}
}

Server* Server::instance = NULL;
