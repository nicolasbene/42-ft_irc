/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:51:44 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/29 22:49:54 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

Server::Server(std::string const &port, std::string const &password)
{
	this->_port = port;
	this->_password = password;
}

Server::~Server(void)
{
}

int Server::start(void)
{
	int					sock;
	struct sockaddr_in	addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return (-1);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(this->_port.c_str()));
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
		return (-1);
	if (listen(sock, 10) == -1)
		return (-1);
	return (0);
}

int Server::stop(void)
{
	return (0);
}

// int Server::loop(void)
// {
	
// }