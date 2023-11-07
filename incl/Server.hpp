/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:08:32 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/07 18:29:54 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <poll.h>
# include <netdb.h>
# include <stdlib.h>
# include <vector>
# include <unistd.h>
# include <map>
# include <iomanip>
# include <fstream>
# include <sys/epoll.h>


# define SERVER_NAME "ft_irc"
# define MAX_CONNEXIONS 10
# define MAX_EVENTS 10

# include "ClientManager.hpp"
# include "Message.hpp"

class	Server {
	public:
		// -- Constructors --
		Server(const std::string& port, const std::string& password);

		// -- Destructor --
		~Server();

		// -- Public Functions --
		int		start();
		int		poll();
		void	write_logo() const;
		
		int		create_client();
		int 	processClientData(int i);
		void	logReceivedMessage(const Message& message, const int fd);


		// -- Public static functions --
		static bool		is_valid_port(const std::string& port);
		static bool		is_valid_password(const std::string& password);

	private:
		// -- Private attributes --
		int						        _socket_serveur;
		int						        _nb_clients;
		std::string				        _port;
		std::string				        _password;
		std::string 				    _name;
		
		
		struct addrinfo			        _hints;
		struct addrinfo*				_servinfo;

		int								_epoll_fd;
		epoll_event						_events[MAX_CONNEXIONS];

		ClientManager					_client_manager;
		// std::vector<Channel*>			_channels;
};

#endif
