/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:46:17 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/20 14:53:36 by nwyseur          ###   ########.fr       */
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
		int getUserSockId(void);
		std::string getUserName(void);
		std::string getOldNickName(void);
		std::string getUserNickName(void);
		void addChannelList(Channel& channel);

	private:
	int	_userSockId;
	std::string _userName;
	std::string _nickName;
	std::string _oldNickName;
	bool	_isConnected;
	std::vector<Channel*> _channelList;

};

#endif