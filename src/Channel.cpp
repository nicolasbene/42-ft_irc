/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 15:06:34 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/09 17:57:44 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
	return;
}

Channel::Channel(const std::string &name,  User& channelOperator)
{
    _channelName = name;
    _channelOperators.push_back(&channelOperator);
    _channelMembers.push_back(&channelOperator);
}

Channel::~Channel()
{
	return;
}

const std::vector<User*> Channel::getUsers() const
{
	return (this->_channelMembers);
}

const std::string Channel::getName() const
{
	return (this->_channelName);
}

void Channel::addUser(User& user)
{
    _channelMembers.push_back(&user);
}

void Channel::removeUser(User& User)
{
	unsigned long int i = 0;
	while (i < _channelMembers.size())
	{
		if (_channelMembers[i] == &User)
		{
			_channelMembers.erase(_channelMembers.begin() + i);
			return;
		}
		i++;
	}
}

bool Channel::hasUser(User& User) const
{
	unsigned long int i = 0;
	while (i < _channelMembers.size())
	{
		if (_channelMembers[i] == &User)
		{
			return (true);
		}
		i++;
	}
	return (false);
}