/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/31 13:22:16 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#include <arpa/inet.h>
#include <netdb.h>

#include <vector>
#include <poll.h> // Inclure la bibliothèque pour utiliser poll()
#include <errno.h> // Inclure la bibliothèque pour la gestion des erreurs

#include <ctime>
#include <sstream>
#include <stdexcept>

#include <fstream>
#include <iostream>

#include "Log.hpp"

Server::Server(const std::string& port, const std::string& password) :
	_sockfd(-1), _nb_clients(0), _port(port),
	_password(password), _name(SERVER_NAME), _hints(), _servinfo(NULL)
{
	// Initialisation du serveur avec le port et le mot de passe
	Log::info() << "using the given port " << port << '\n';
	Log::info() << "using the given password '" << password << "'" << '\n';

	// Initialisation de la structure _hints pour getaddrinfo
	memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port.c_str(), &_hints, &_servinfo) != 0) {
		Log::error() << "could not get the server connection details" << '\n';
		exit(1);
	}
}

Server::~Server()
{
	freeaddrinfo(_servinfo);
}

int	Server::start()
{
	// Création, liaison et écoute du socket du serveur
	_sockfd = socket(_servinfo->ai_family, _servinfo->ai_socktype,
		_servinfo->ai_protocol);
	if (_sockfd == -1) {
		Log::error() << "could not create the server socket" << '\n';
		exit(1);
	}
	if (bind(_sockfd, _servinfo->ai_addr, _servinfo->ai_addrlen) == -1) {
		Log::error() << "could not bind the server socket" << '\n';
		exit(1);
	}
	if (listen(_sockfd, MAX_CONNEXIONS) == -1) {
		Log::error() << "could not listen to the server socket" << '\n';
		exit(1);
	}
	Log::info() << "server started" << '\n';

	return (0);
}

int	Server::poll()
{
	pollfd	server_pfd;

	server_pfd.fd = _sockfd;
	server_pfd.events = POLLIN;
	server_pfd.revents = 0;

	std::vector<pollfd>	client_pfds;
	client_pfds.reserve(MAX_CONNEXIONS);

	while (true) {
		// Utilisation de poll pour surveiller les connexions entrantes
		if (::poll(&server_pfd, 1, -1) == -1) {
			Log::error() << "could not poll the server socket" << '\n';
			exit(1);
		}
		if (server_pfd.revents & POLLIN) {
			// Accepter de nouvelles connexions si possible
			if (_nb_clients < MAX_CONNEXIONS) {
				struct sockaddr_storage	client_addr;
				socklen_t				client_addr_size = sizeof(client_addr);
				int						client_fd = accept(_sockfd,
					(struct sockaddr*)&client_addr, &client_addr_size);
				if (client_fd == -1) {
					Log::error() << "could not accept the client connection" << '\n';
					exit(1);
				}
				client_pfds.push_back(pollfd());
				client_pfds.back().fd = client_fd;
				client_pfds.back().events = POLLIN;
				client_pfds.back().revents = 0;
				++_nb_clients;
				Log::info() << "client connected" << '\n';
			} else {
				Log::info() << "client connection refused: too many clients" << '\n';
			}
		}
		for (int i = 0; i < _nb_clients; ++i) {
			if (client_pfds[i].revents & POLLIN) {
				// Lecture des données reçues des clients
				char	buffer[1024];
				int		bytes_read = recv(client_pfds[i].fd, buffer, 1024, 0);
				if (bytes_read == -1) {
					Log::error() << "could not read from the client socket" << '\n';
					exit(1);
				}
				if (bytes_read == 0) {
					// Si la lecture est vide, le client s'est déconnecté
					close(client_pfds[i].fd);
					client_pfds.erase(client_pfds.begin() + i);
					--_nb_clients;
					Log::info() << "client disconnected" << '\n';
				} else {
					buffer[bytes_read] = '\0';
					Log::info() << "received: " << buffer << '\n';
				}
			}
		}
	}
}

bool    Server::is_valid_port(const std::string& port)
{
	if (port.empty())
		return false;
	for (std::string::const_iterator it = port.begin(); it != port.end(); it++) {
		if (!isdigit(*it))
			return false;
	}
	if (atoi(port.c_str()) < 1024 || atoi(port.c_str()) > 65535)
		return false;
	return true;
}

bool    Server::is_valid_password(const std::string& password)
{
	if (password.size() < 1 || password.size() > 255)
		return false;
	for (std::string::const_iterator it = password.begin(); it != password.end(); it++) {
		if (std::isspace(*it) || *it == '\0' || *it == ':')
			return false;
	}
	return true;
}

void 	Server::write_logo() const
{
	std::string line;
	std::ifstream infile("assets/logo.txt");
	while (std::getline(infile, line))
	{
		std::cout << line << std::endl;
	}	
}