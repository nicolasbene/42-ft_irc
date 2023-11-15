/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:13:24 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/15 17:37:41 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(void)
{
	return;
}

User::User(int sockId, const std::string& userName, struct sockaddr_in addrClient) : 
		_userSockId(sockId), _userName(userName), _nickName(userName), _oldNickName(userName), _addrClient(addrClient), _isConnected(false) 
{
	_IPchar = std::string(inet_ntoa(addrClient.sin_addr));
	_userID = _nickName + "!~" + _userName + "@" + _IPchar;
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

void User::setUserNickName(const std::string userNickName)
{
	_nickName = userNickName;
	return;
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

void User::addChannelList(Channel channel)
{
	_channelList.push_back(&channel);
	return;
}

int User::getUserSockId(void)
{
	return(this->_userSockId);
}

bool Server::isChannel(const std::string& name)
{
    std::map<std::string, Channel>::iterator it = channels.find(name);
    if (it != channels.end()) 
        return true;
    return false;
}