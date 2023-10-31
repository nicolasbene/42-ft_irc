/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:08:32 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/31 00:06:04 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <netdb.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <map>
#include <vector>

class Server
{
  public:
	Server(const char* port, const char* password);
	~Server();

	int createSocket(const char* port);
	int poll();
	void stop();
	
	void addPollFd(int fd);
	std::vector<pollfd>& getPollfds();
	

  private:
	const char* _password;
	std::vector<pollfd> _fds;
	std::map<int, std::string> _clients;
};

#endif