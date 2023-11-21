/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 16:58:53 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/21 18:32:46 by nwyseur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <poll.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Log.hpp"


void Server::sendInvitation(Message message, int fd)
{
	std::string	client;
	std::string	channel;
	bool		channelExist;

	client = message.getParameters()[0];
	channel = message.getParameters()[1];

	if (client.empty() || channel.empty() || channel.find('#') == channel.npos)
	{
		sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), message.getCommande()));
		return;
	}
	channel.erase(channel.find("#"), 1);
	std::map<std::string, Channel>::iterator it;
	it = channels.find(channel);
	if (it == channels.end())
		channelExist = false;
	else
		channelExist = true;
	
	if (!userExistName(client))
	{
		sendServerRpl(fd, ERR_NOSUCHNICK(users[fd].getUserNickName(), client));
		return;
	}

	if (channelExist == true)
	{
		std::vector<User*> theOneWhoInvite = channels[channel].getChannelMembers();
		size_t i = 0;
		while (i < theOneWhoInvite.size())
		{
			if (*(theOneWhoInvite)[i] == users[fd])
				break;
			i++;
		}
		if (i == theOneWhoInvite.size())
		{
			sendServerRpl(fd, ERR_NOTONCHANNEL(users[fd].getUserNickName(), channel));
			return;
		}

		std::vector<User*> userInvited = channels[channel].getChannelMembers();
		size_t i = 0;
		while (i < userInvited.size())
		{
			if ((userInvited)[i]->getUserNickName() == client)
				break;
			i++;
		}
		if (i == userInvited.size())
		{
			sendServerRpl(fd, ERR_USERONCHANNEL(users[fd].getUserNickName(), client, channel));
			return;
		}
	}

	sendServerRpl(fd, RPL_INVITING(user_id(users[fd].getUserNickName(), users[fd].getUserName()), users[fd].getUserNickName(), client, channel));
}


bool Server::userExistName(std::string user)
{
	std::map<int, User>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
	{
		if (it->second.getUserNickName() == user)
			return (true);
	}
	return (false);
}