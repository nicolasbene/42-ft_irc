/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:31:07 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/20 15:32:07 by nwyseur          ###   ########.fr       */
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

void Server::setReadTopic(Message message, int fd)
{
    std::string channel = message.getParameters()[0];
	if (channel.empty() || channel.find('#') == channel.npos)
    {
        sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), message.getCommande()));
        return;
    }
    std::map<std::string, Channel>::iterator it;
    it = channels.find(channel);
    if (it == channels.end())
    {
        sendServerRpl(fd, ERR_NOSUCHCHANNEL(users[fd].getUserNickName(), channel));
        return;
    }
    std::vector<User*> itc = channels[channel].getChannelMembers();
    size_t i = 0;
    while (i < itc.size())
    {
        if (*(itc)[i] == users[fd])
            break;
        i++;
    }
    if (i == itc.size())
    {
        sendServerRpl(fd, ERR_NOTONCHANNEL(users[fd].getUserNickName(), channel));
        return;
    }

    std::string topic = message.getTrailing();

    if(topic.empty())
    {
        if (channels[channel].getChannelTopic().empty() == false)
        {
            sendServerRpl(fd, RPL_TOPIC(users[fd].getUserNickName(), channel, topic));
            return;
        }
        else
        {
            sendServerRpl(fd, RPL_NOTOPIC(users[fd].getUserNickName(), channel));
        }
    }
    else
    {
        std::vector<User*> ito = channels[channel].getChannelOperators();
        size_t i = 0;
        while (i < ito.size())
        {
            if (*(ito)[i] == users[fd])
                break;
            i++;
        }
        if (i == ito.size()) // rajouter ici la condition au mode
        {
            sendServerRpl(fd, ERR_CHANOPRIVSNEEDED(users[fd].getUserNickName(), channel));
            return;
        }
        else
        {
            //rajouter trailing vide;
            channels[channel].setChannelTopic(topic);
            std::vector<User*>  channelMembers = channels[channel].getChannelMembers();
            std::vector<User*>::const_iterator member = channelMembers.begin();
            while (member != channelMembers.end())
            {
                sendServerRpl((*member)->getUserSockId(), RPL_TOPIC(users[fd].getUserNickName(), channel, topic));
                member++;
            }
        }
    }
}