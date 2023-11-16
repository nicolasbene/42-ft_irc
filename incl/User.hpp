/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgautier <jgautier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:46:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/16 15:21:43 by jgautier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

# include <string>
# include <iostream>
# include "Channel.hpp"
# include "Server.hpp"

class Channel;

class User
{
	public:

		User(void);
		User(int sockId, const std::string& userNickName, const std::string& userName, struct sockaddr_in addrClient);
		~User();
		bool operator!=(User& other) {return (this->_userSockId != other.getUserSockId());}
		bool operator==(User& other) {return (this->_userSockId == other.getUserSockId());}

		void setUserName(const std::string userName);
		void setOldNickName(const std::string userNickName);
		void setNickName(const std::string userNickName);
		int getUserSockId(void);
		std::string getUserName(void);
		std::string getOldNickName(void);
		std::string getUserNickName(void);
		std::string getUserID(void) const {return _userID;}
		void addChannelList(Channel channel);
		bool isChannel(const std::string& name);
		bool getIsConnected(void) const;
		void setIsConnected(bool value);
	

	private:
	int	_userSockId;
	std::string _userName;
	std::string _nickName;
	std::string _oldNickName;
	struct sockaddr_in _addrClient;
	std::string			_userID;
	std::string 			_IPchar;
	bool	_isConnected;
	std::vector<Channel*> _channelList;
	// std::map<std::string, Channel> _channelList;

};

#endif