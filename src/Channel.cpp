/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 15:06:34 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/27 15:00:12 by nwyseur          ###   ########.fr       */
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
	_channelCapacity = 0;
	_channelSymbol = "#";
}

Channel::~Channel()
{
	return;
}

// -- Getter
const std::vector<User*> Channel::getChannelMembers() const
{
	return (this->_channelMembers);
}

const std::vector<User*> Channel::getBannedUsers() const
{
	return (this->_bannedUsers);
}

const std::vector<User*> Channel::getKickedUsers() const
{
	return (this->_kickedUsers);
}

const std::vector<User*> Channel::getChannelOperators() const
{
	return (this->_channelOperators);
}

const std::string Channel::getName() const
{
	return (this->_channelName);
}

const std::string Channel::getPassword() const
{
	return (this->_channelPassword);
}

size_t Channel::getChannelCap() const
{
	return (this->_channelCapacity);
}

const std::string Channel::getSymbol() const
{
	return (this->_channelSymbol);
}

const std::string Channel::getChannelTopic() const
{
	return (this->_channelTopic);
}

// -- Setter

void	Channel::setPassword(std::string pass)
{
	_channelPassword = pass;
}

void	Channel::setChannelCap(int cap)
{
	_channelCapacity = cap;
}

void	Channel::setChannelTopic(std::string topic)
{
	_channelTopic = topic;
}

void Channel::setChannelOperator(User& ops)
{
	_channelOperators.push_back(&ops);
}

// -- Add/Remove
void Channel::addUser(User& user)
{
    _channelMembers.push_back(&user);
}

void Channel::removeUser(User& user)
{
	unsigned long int i = 0;
	while (i < _channelMembers.size())
	{
		if (_channelMembers[i] == &user)
		{
			_channelMembers.erase(_channelMembers.begin() + i);
			return;
		}
		i++;
	}
}

void Channel::addBannedUser(User& user)
{
	_bannedUsers.push_back(&user);
}

void Channel::removeBannedUser(User& user)
{
	unsigned long int i = 0;
	while (i < _bannedUsers.size())
	{
		if (_bannedUsers[i] == &user)
		{
			_bannedUsers.erase(_bannedUsers.begin() + i);
			return;
		}
		i++;
	}
}

void Channel::addKickedUser(User& user)
{
	_kickedUsers.push_back(&user);
}

void Channel::removeKickedUser(User& user)
{
	unsigned long int i = 0;
	while (i < _kickedUsers.size())
	{
		if (_kickedUsers[i] == &user)
		{
			_kickedUsers.erase(_kickedUsers.begin() + i);
			return;
		}
		i++;
	}
}

bool Channel::hasUser(User& user) const
{
	unsigned long int i = 0;
	while (i < _channelMembers.size())
	{
		if (_channelMembers[i] == &user)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

bool Channel::hasOp(User& user) const
{
	unsigned long int i = 0;
	while (i < _channelOperators.size())
	{
		if (_channelOperators[i] == &user)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

bool Channel::hasBanUser(User& user) const
{
	unsigned long int i = 0;
	while (i < _bannedUsers.size())
	{
		if (_bannedUsers[i] == &user)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

bool Channel::hasKickUser(User& user) const
{
	unsigned long int i = 0;
	while (i < _kickedUsers.size())
	{
		if (_kickedUsers[i] == &user)
		{
			return (true);
		}
		i++;
	}
	return (false);
}

std::string Channel::listOfMember() const
{
	std::string toReturn;

	for (size_t i = 0; i < _channelMembers.size() - 1; i++)
	{
		if (i == 0)
			toReturn += "@";
		else
			toReturn += " @";
		toReturn += _channelMembers[i]->getUserNickName();
	}

	return(toReturn);
}