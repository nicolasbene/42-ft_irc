/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:12:23 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/16 16:31:16 by nwyseur          ###   ########.fr       */
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

void sendChanInfo(Channel& channel, User& user);
bool atLeastOneAlphaNum(std::string toTest);
void addClientToChannel(Channel& channel, User& client);
std::string getChannelName(std::string strToPars);

void Server::executeJoinOrder(Message message, int fd)
{
    std::string channel_list = message.getParameters()[0];
    std::string keyword_list;
    std::string channel;

    if (message.getParameters().size() >= 2)
        keyword_list = message.getParameters()[1];
    if (atLeastOneAlphaNum(channel_list) == false)
    {
        sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), message.getCommande()));
        return;
    }
    while (atLeastOneAlphaNum(channel_list) == true)
    {
        channel.clear();
        channel = getChannelName(channel_list);

        channel_list.erase(channel_list.find(channel), channel.length());
        std::map<std::string, Channel>::iterator it;
        it = channels.find(channel);
        if (it == channels.end())
        {
            addChannel(channel, users[fd]);
        }
        if (channels[channel].getChannelMembers().size() >= channels[channel].getChannelCap() && channels[channel].getChannelCap() != 0)
        {
            sendServerRpl(fd, ERR_CHANNELISFULL(users[fd].getUserNickName(), '#' + channel));
            continue;
        }
        std::vector<User*>::const_iterator itb;
        for (itb = channels[channel].getBannedUsers().begin(); itb != channels[channel].getBannedUsers().end(); itb++)
        {
            if (*(*itb) == users[fd])
            {
                sendServerRpl(fd, ERR_BANNEDFROMCHAN(users[fd].getUserNickName(), '#' + channel));
                continue;
            }
        }
        std::vector<User*>::const_iterator itk;
        for (itk = channels[channel].getKickedUsers().begin(); itk != channels[channel].getKickedUsers().end(); itk++)
        {
            if (*(*itk) == users[fd])
            {
                sendServerRpl(fd, ERR_BANNEDFROMCHAN(users[fd].getUserNickName(), '#' + channel));
                continue;
            }
        }
        std::vector<User*>::const_iterator iti;
        iti = channels[channel].getChannelMembers().begin();
        while (iti != channels[channel].getChannelMembers().end())
        {
            if (*(*iti) == users[fd])
                break;
            iti++;
        }
        if (iti == channels[channel].getChannelMembers().end())
        {
            addClientToChannel(channels[channel], users[fd]);
        }
        else
		    std::cout << YELLOW << users[fd].getUserNickName() << "already in the channel\n" << RESET;
        sendChanInfo(channels[channel], users[fd]);
    }
}

void Server::sendChanInfo(Channel& channel, User& user)
{
    std::vector<User*>::const_iterator member;
    member = channel.getChannelMembers().begin();

    while (member != channel.getChannelMembers().end())
    {
        sendServerRpl((*member)->getUserSockId(), RPL_JOIN(user_id(user.getUserNickName(), user.getUserName()), channel.getName()));
        sendServerRpl((*member)->getUserSockId(), RPL_NAMREPLY(user.getUserNickName(), channel.getSymbol(), channel.getName(), channel.listOfMember()));
        sendServerRpl((*member)->getUserSockId(), RPL_ENDOFNAMES(user.getUserNickName(), channel.getName()));
        member++;
    }
}

bool atLeastOneAlphaNum(std::string toTest)
{
    for (size_t i = 0; i < toTest.size(); i++)
    {
        if(isalnum(toTest[i]) != 0)
            return(true);
    }
    return (false);
}

void addClientToChannel(Channel& channel, User& client)
{
    channel.addUser(client);
}

std::string getChannelName(std::string strToPars)
{
    std::string toReturn;
    int i = 0;
    while (strToPars[i] && !isalnum(strToPars[i]) && strToPars[i] != '-' && strToPars[i] != '_')
        i++;
    while (strToPars[i] && strToPars[i] != ',')
    {
        toReturn += strToPars[i];
        i++;
    }
    return (toReturn);
    
}


// void Server::executeJoinOrder(Message message, int fd)
// {
//     std::map<std::string, Channel>::iterator it;
//     for (it = channels.begin(); it != channels.end(); it++)
//     {
//         if (it->second.getName() == message.getParameters()[0])
//         {
//             it->second.addUser(users[fd]);
//             users[fd].addChannelList(it->second);
//             std::string channel = it->second.getName();
//             channel.resize(channel.size() - 2);
//             std::string chan = "You entered channel : [" + channel + "]";
//             send(fd, chan.c_str(), chan.size(), 0);
//             return;
//         }
//     }
//     addChannel(message.getParameters()[0], users[fd]);
//     channels[message.getParameters()[0]].addUser(users[fd]);
//     users[fd].addChannelList(channels[message.getParameters()[0]]);
//     std::string createchan = "You created channel : [" + message.getParameters()[0] + "] and entered as operator";
//     send(fd, createchan.c_str(), createchan.size(), 0);
// }