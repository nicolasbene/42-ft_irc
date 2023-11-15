/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:46:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/15 17:49:58 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

# include <string>
# include <iostream>
# include "Channel.hpp"

class Channel;

class User
{
	public:

		User(void);
		User(int sockId, const std::string& userNickName, const std::string& userName);
		~User();
		bool operator!=(User& other) {return (this->_userSockId != other.getUserSockId());}
		bool operator==(User& other) {return (this->_userSockId == other.getUserSockId());}

		void setUserName(const std::string userName);
		void setOldNickName(const std::string userNickName);
		void setNickName(const std::string userNickName);
		void setHostName(const std::string hostName);
		void setRealName(const std::string realName);
		int getUserSockId(void);
		std::string getUserName(void);
		std::string getOldNickName(void);
		std::string getUserNickName(void);
		std::string getHostName(void);
		std::string getRealName(void);
		
		void addChannelList(Channel channel);

	private:
	int	_userSockId;
	std::string _userName;
	std::string _nickName;
	std::string _oldNickName;
	std::string _hostName;
	std::string _realName;
	
	bool	_isConnected;
	std::vector<Channel*> _channelList;

};

#endif