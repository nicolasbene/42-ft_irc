/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:08:32 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/09 17:53:25 by nwyseur          ###   ########.fr       */
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
# include <sstream>

# include "utils.h"
# include "User.hpp"
# include "Channel.hpp"

# define SERVER_NAME "ft_irc"
# define MAX_CONNEXIONS 10
# define MAX_EVENTS 10

class User;
class Channel;

class	Server {
	public:
		// -- Constructors --
		Server(const std::string& port, const std::string& password);

		// -- Destructor --
		~Server();

		// -- Public Functions --
		int		    start();
		int		    poll();
		void		write_logo() const;
		
		int			create_client();
		int 		receive_message(int i);


		// -- Public static functions --
		static bool		is_valid_port(const std::string& port);
		static bool		is_valid_password(const std::string& password);

		// -- Users 
		std::map<int, User> users;
		void addUser(int sockId);

		// -- Channels
		std::map<std::string, Channel> channels;
		void addChannel(const std::string& name, User& channelOperator); 

		// -- Execution
		int executeCommand(char* buffer, int fd);
		void setUserNickName(const std::vector<std::string>& result, int fd);
		void sendPrivateMessage(const std::vector<std::string>& result, int fd);
		void executeJoinOrder(const std::vector<std::string>& result, int fd);


	private:
		// -- Private attributes --
		int						        _sockfd;
		int						        _nb_clients;
		std::string				        _port;
		std::string				        _password;
		std::string 				    _name;
		struct addrinfo			        _hints;
		struct addrinfo*				_servinfo;

		int								_epoll_fd;
		epoll_event						_events[MAX_CONNEXIONS];
		
		std::vector<pollfd>				_client_pfds;

};

#endif
