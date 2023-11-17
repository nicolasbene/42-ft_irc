/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/17 17:03:52 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <poll.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Log.hpp"

Server::Server(const std::string& port, const std::string& password) :
    _sockfd(-1), _nb_clients(0), _port(port),
    _password(password), _name(SERVER_NAME), _hints(), _servinfo(NULL)
{
    //setup date de creation
    time_t      rawtime = time(NULL);
    struct tm   *timeinfo;
    timeinfo = localtime(&rawtime);
	_date = std::string(asctime(timeinfo));

    // Affiche les informations sur le port et le mot de passe
    Log::info() << "Using port: " << port << '\n';
    Log::info() << "Using password: '" << password << "'" << '\n';

    // Initialise la structure _hints pour la fonction getaddrinfo
    memset(&_hints, 0, sizeof(_hints));
    _hints.ai_family = AF_INET;
    _hints.ai_socktype = SOCK_STREAM;
    _hints.ai_flags = AI_PASSIVE;

    // Obtient les détails de connexion du serveur en utilisant getaddrinfo
    if (getaddrinfo(NULL, _port.c_str(), &_hints, &_servinfo) != 0) {
        Log::error() << "Could not get server connection details" << '\n';
        exit(1);
    }
}

Server::~Server()
{
    freeaddrinfo(_servinfo);
    close(_sockfd);
    Log::info() << "Server stopped" << '\n';
}

int Server::start()
{
    // Création, liaison et écoute du socket du serveur
    _sockfd = socket(_servinfo->ai_family, _servinfo->ai_socktype,
        _servinfo->ai_protocol);
    if (_sockfd == -1) {
        Log::error() << "Could not create server socket" << '\n';
        exit(1);
    }
    if (bind(_sockfd, _servinfo->ai_addr, _servinfo->ai_addrlen) == -1) {
        Log::error() << "Could not bind server socket" << '\n';
        exit(1);
    }
    if (listen(_sockfd, MAX_CONNEXIONS) == -1) {
        Log::error() << "Could not listen on server socket" << '\n';
        exit(1);
    }
    Log::info() << "Server started" << '\n';

    return (0);
}

int Server::poll()
{
    // Crée une structure pour le socket du serveur
    epoll_event server_event;
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        Log::error() << "Could not create epoll fd" << '\n';
        exit(1);
    }
    server_event.events = EPOLLIN;
    server_event.data.fd = _sockfd;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _sockfd, &server_event) == -1) {
        Log::error() << "Could not add server fd to epoll" << '\n';
        exit(1);
    }

    while (true) {
        // Ne pas réutiliser num_events, car vous avez déjà une variable i dans la boucle for
        int num_events = epoll_wait(_epoll_fd, _events, MAX_CONNEXIONS, 200);
        if (num_events == -1) 
        {
            perror("epoll_wait");
            exit(1);
        }

        for (int i = 0; i < num_events; i++) {
            int fd = _events[i].data.fd;
            if (fd == _sockfd) {
                create_client();
            } else {
                receive_message(fd);
            }
        }
    }
}

int Server::create_client()
{
    if (_nb_clients < MAX_CONNEXIONS)
    {
        struct sockaddr_storage client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_fd = accept(_sockfd, (struct sockaddr *)&client_addr, &client_addr_size);

        if (client_fd == -1)
        {
            Log::error() << "Could not accept the client connection" << '\n';
            exit(1);
        }

        // Vous devez créer une nouvelle structure epoll_event pour le nouveau client.
        struct epoll_event client_event;
        client_event.data.fd = client_fd;
        client_event.events = EPOLLIN;

        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event) == -1)
        {
            perror("epoll_ctl");
            exit(1);
        }

        //addUser(client_fd);
        ++_nb_clients;
        sleep(1); // ici test
        Log::info() << "Client connected : " << client_fd << '\n';
        message_creation(client_fd);

        // Envoi du code RPL au client
        sendServerRpl(client_fd, RPL_WELCOME(user_id(users[client_fd].getUserNickName(), users[client_fd].getUserName()), users[client_fd].getUserNickName()));
        sendServerRpl(client_fd, RPL_YOURHOST(users[client_fd].getUserNickName(), SERVER_NAME, SERVER_VERSION));
        sendServerRpl(client_fd, RPL_CREATED(users[client_fd].getUserNickName(), this->_date));
        sendServerRpl(client_fd, RPL_MYINFO(users[client_fd].getUserNickName(), SERVER_NAME, SERVER_VERSION, "io", "kost", "k"));
        sendServerRpl(client_fd, RPL_ISUPPORT(users[client_fd].getUserNickName(), "CHANNELLEN=50 NICKLEN=30 TOPICLEN=307"));
    }
    else
    {
        Log::info() << "Client connection refused: Too many clients" << '\n';
    }
    return 0;
}

int Server::message_creation(int fd)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Lire les données du socket
    int num_bytes = recv(fd, buffer, sizeof(buffer), 0);
    if (num_bytes == -1)
    {
        perror("recv");
        exit(1);
    }

    if (num_bytes == 0)
    {
        // Le client s'est déconnecté, vous devez supprimer le descripteur de fichier de epoll.
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
        {
            perror("epoll_ctl");
            exit(1);
        }

        close(fd);
        Log::info() << "Client disconnected" << '\n';
    }
    else
    {
        std::cout << "-Received << " << buffer << std::endl;
        addUser(fd, &buffer[0]);
    }

    return 0;
}

int Server::receive_message(int fd)
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Lire les données du socket
    int num_bytes = recv(fd, buffer, sizeof(buffer), 0);
    if (num_bytes == -1)
    {
        perror("recv");
        exit(1);
    }

    if (num_bytes == 0)
    {
        // Le client s'est déconnecté, vous devez supprimer le descripteur de fichier de epoll.
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1)
        {
            perror("epoll_ctl");
            exit(1);
        }

        close(fd);
        Log::info() << "Client disconnected" << '\n';
    }
    else
    {
        std::cout << "Received << " << buffer << std::endl;
        executeCommand(&buffer[0], fd);
    }

    return 0;
}

int Server::executeCommand(char* buffer, int fd)
{
    std::string str(buffer);

    Message message(str);
    if (message.getCommande() == "NICK")
        setUserNickName(message, fd);
    else if (message.getCommande() == "PRIVMSG")
        sendPrivateMessage(message, fd);
    else if (message.getCommande() == "JOIN")
        executeJoinOrder(message, fd);
<<<<<<< HEAD
    else if (message.getCommande() == "MODE")
        handleModeCommand(message, fd);
=======
    else if (message.getCommande() == "PART")
        partb(message, fd);
>>>>>>> 96c2f39b46e7f84d40a323d9b771df83a578d55e
    else
        std::cout << "-------" << std::endl;
    return (0);
}

void Server::addUser(int sockId, char *buffer) // ici pas satisfait avec le name par defaut
{
    //static int i = 1;
    std::string str(buffer);
    std::string nickName = extractNextWord(str, "NICK");
    std::string userName = extractNextWord(str, "USER");
    users.insert(std::make_pair(sockId, User(sockId, nickName, userName)));
    return;
}

void Server::addChannel(const std::string& name, User& channelOperator)
{
    channels.insert(std::make_pair(name, Channel(name, channelOperator)));
    return;
}

void Server::sendServerRpl(int const fd, std::string reply)
{
	std::istringstream	buf(reply);
	std::string			sended;
	
	send(fd, reply.c_str(), reply.size(), 0);
	while (getline(buf, sended))
	{
		std::cout << "[Server] Message sent to client " << fd << "       >> " << GREEN << sended << RESET << std::endl;
	}
}

bool Server::is_valid_port(const std::string& port)
{
    if (port.empty())
        return false;
    for (std::string::const_iterator it = port.begin(); it != port.end(); it++) {
        if (!isdigit(*it))
            return false;
    }
    int portNum = atoi(port.c_str());
    return portNum >= 1024 && portNum <= 65535;
}

bool Server::is_valid_password(const std::string& password)
{
    if (password.empty() || password.size() > 255)
        return false;
    for (std::string::const_iterator it = password.begin(); it != password.end(); it++) {
        if (isspace(*it) || *it == '\0' || *it == ':')
            return false;
    }
    return true;
}

void Server::write_logo() const
{
    std::ifstream infile("assets/logo.txt");
    std::string line;
    while (std::getline(infile, line))
    {
        std::cout << line << std::endl;
    }
}

std::string Server::getDate() const
{
    return (this->_date);
}