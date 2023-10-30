/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:08:32 by nibenoit          #+#    #+#             */
/*   Updated: 2023/10/30 16:00:40 by nibenoit         ###   ########.fr       */
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

#include "Message.hpp"

extern bool g_isRunning;

class Server
{
  public:
	Server(const char* port, const char* password);
	~Server();

	int poll();
	void stop();

  private:
	const char* m_password;
};

#endif