/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibenoit <nibenoit@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:13:24 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/15 17:49:28 by nibenoit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void)
{
	return;
}

User::User(int sockId, const std::string& userNickName, const std::string& userName) : 
		_userSockId(sockId), _userName(userName), _nickName(userNickName), _oldNickName(userNickName), _isConnected(false)
{
	return;
}

User::~User()
{
	return;
}

void User::setUserName(const std::string userName)
{
	_userName = userName;
	return;
}

void User::setOldNickName(const std::string userNickName)
{
	_oldNickName = userNickName;
	return;
}

void User::setNickName(const std::string userNickName)
{

	if (userNickName[userNickName.size() - 1] == '\n' && userNickName[userNickName.size() - 2] == '\r')
	{
		std::string substr =userNickName.substr(0, userNickName.size() - 2);
		_nickName = substr;
	}
	else
		_nickName = userNickName;
	return;
}

void User::setHostName(const std::string hostName)
{
	_hostName = hostName;
}

void User::setRealName(const std::string realName)
{
	_realName = realName;
}

std::string User::getUserName(void)
{
	return (this->_userName);
}

std::string User::getUserNickName(void)
{
	return (this->_nickName);
}

std::string User::getOldNickName(void)
{
	return (this->_oldNickName);
}

std::string User::getHostName(void)
{
	return ("localhost");
}

std::string User::getRealName(void)
{
	return ("realname");
}

void User::addChannelList(Channel channel)
{
	_channelList.push_back(&channel);
	return;
}

int User::getUserSockId(void)
{
	return(this->_userSockId);
}