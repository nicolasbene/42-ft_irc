/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:08:32 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/15 20:52:14 by nibenoit         ###   ########.fr       */
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
# include "Message.hpp"

# include "Numerical_reply.hpp"

# define SERVER_NAME "ircserv"
# define SERVER_VERSION "1.1"
# define MAX_CONNEXIONS 10
# define MAX_EVENTS 10

#define RED "\e[1;31m"
#define RESET "\e[0m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"

class User;
class Channel;

class	Server {
	public:
		// -- Constructors --
		Server(const std::string& port, const std::string& password);

		// -- Destructor --
		~Server();

		// -- getter --
		std::string getDate() const;

		// -- Public Functions --
		int		    start();
		int		    poll();
		void		write_logo() const;
		
		int			create_client();
		int 		receive_message(int fd);
		int			message_creation(int fd);


		// -- Public static functions --
		static bool		is_valid_port(const std::string& port);
		static bool		is_valid_password(const std::string& password);

		// -- Users 
		std::map<int, User> users;
		void addUser(int sockId, char *buffer);

		// -- Channels
		std::map<std::string, Channel> channels;
		void addChannel(const std::string& name, User& channelOperator); 

		// -- Execution
		int executeCommand(char* buffer, int fd);
		void setUserNickName(Message message, int fd);
		void sendPrivateMessage(Message message, int fd);
		void broadcastToChannel(std::string target, std::string speech, int fd);
		void executeJoinOrder(Message message, int fd);
		
		void handleWhoisCommand(Message message, int fd);

		

		// -- SendText
		void	sendServerRpl(int const fd, std::string reply);
		void	sendToClient(int const fd, std::string reply);

		void	logReceivedMessage(int fd, Message& message);

	private:
		// -- Private attributes --
		int						        _sockfd;
		int						        _nb_clients;
		std::string				        _port;
		std::string				        _password;
		std::string 				    _name;
		std::string						_date;
		struct addrinfo			        _hints;
		struct addrinfo*				_servinfo;

		int								_epoll_fd;
		epoll_event						_events[MAX_CONNEXIONS];
		
		std::vector<pollfd>				_client_pfds;

};

//utils
std::vector<std::string> mySplit(const std::string& s, char delimiter);
std::string extractNextWord(const std::string& input, const std::string& keyword);

#endif
