/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:13:24 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/08 12:54:51 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int sockId, const std::string& userName) : 
		_userSockId(sockId), _userName(userName), _nickName(userName), _isConnected(false)
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

int User::getUserSockId(void)
{
	return(this->_userSockId);
}