/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgautier <jgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:08:32 by nibenoit          #+#    #+#             */
/*   Updated: 2023/11/23 18:02:28 by jgautier         ###   ########.fr       */
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

# define SERVER_NAME "ft_irc"
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
		int			message_creation(int fd, sockaddr_in addrClient);


		// -- Public static functions --
		static bool		is_valid_port(const std::string& port);
		static bool		is_valid_password(const std::string& password);

		// -- Users 
		std::map<int, User> users;
		// void addUser(int sockId, struct sockaddr_in addrClient);
		void addUser(int sockId, char *buffer, sockaddr_in addrClient);
		int userNameToFd(std::string& user);

		// -- Channels
		std::map<std::string, Channel> channels;
		bool isChannel(const std::string& name);
		void addChannel(const std::string& name, User& channelOperator); 

		// -- Execution
		int executeCommand(char* buffer, int fd);
		void setUserNickName(Message message, int fd);
		void sendPrivateMessage(Message message, int fd);
		void broadcastToChannel(std::string target, std::string speech, int fd);
		void executeJoinOrder(Message message, int fd);
		void	executePart(Message message, int fd);
		void sendChanInfo(Channel& channel, User& user);
		void	sendPong(Message msg, int fd);
		void	executeKick(Message msg, int fd);


		void setReadTopic(Message message, int fd);
		void sendInvitation(Message message, int fd);
		void notice(Message message, int fd);
		void broadcastToChannelNotice(std::string target, std::string speech, int fd);

		// -- Utils
		bool userExistName(std::string user);
		// bool userExistUserName(std::string user);
		std::string changeNickname(std::string nickNameTochange, std::string NameTochange, int fd);
		// std::string changeUserName(std::string NameTochange);


		// -- SendText
		void	sendServerRpl(int const fd, std::string reply);

		// -- unregister Client in channel
		void unregisterClientToChannel(std::string unregisterChannnel, int fd);

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
void printVector(const std::vector<std::string>& vec);
void TrimVectorWhiteSpace(std::vector<std::string> &vec, const std::string& TrimStr);
std::string TrimString(const std::string& str, const std::string& TrimStr);

#endif
