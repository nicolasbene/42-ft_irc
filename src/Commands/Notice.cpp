/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 14:39:00 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/22 15:50:52 by nwyseur          ###   ########.fr       */
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

void Server::broadcastToChannelNotice(std::string target, std::string speech, int fd)
{
    Channel broadcast = channels[target];
    // std::cout << RED << "TEST MSG CHANNEL :" << target << " testret" << RESET << std::endl;
    // std::cout << RED << "TEST MSG CHANNEL :" << channels[target].getName() << RESET << std::endl;
    std::vector<User*> banned_users = broadcast.getBannedUsers();
    for (std::vector<User *>::iterator it = banned_users.begin(); it != banned_users.end(); it++)
    {
        if (*it == &users[fd])
        {
            //std::cout << RED << "[Server] User " << users[fd].getUserNickName() << " is banned from channel " << RESET << broadcast.getName();
            return; 
        }
    }

    std::vector<User*> kicked_users = broadcast.getKickedUsers();
    for (std::vector<User *>::iterator it = kicked_users.begin(); it != kicked_users.end(); it++)
    {
        if (*it == &users[fd])
        {
            //std::cout << RED << "[Server] User " << users[fd].getUserNickName() << " is kicked from channel " << RESET << broadcast.getName();
            return; 
        }
    }

    std::vector<User*> channel_users = broadcast.getChannelMembers();
    for (std::vector<User*>::iterator it = channel_users.begin(); it != channel_users.end(); it++)
    {
        // std::cout << RED << "TEST MSG CHANNEL - ENVOIE" << RESET << std::endl;
        std::string toSend = "#" + target;
        if ((*it)->getUserSockId() != fd)
            sendServerRpl((*it)->getUserSockId(), RPL_NOTICE(users[fd].getUserNickName(), users[fd].getUserName(), toSend, speech));
    }

}

void Server::notice(Message message, int fd)
{
    std::string target;
    std::string speech;

    if (message.getParameters().empty())
    {
        //sendServerRpl(fd, ERR_NORECIPIENT(users[fd].getUserNickName()));
        return;
    }
    else
        target = message.getParameters()[0];
    if (message.getTrailing().empty())
    {
        //sendServerRpl(fd, ERR_NOTEXTTOSEND(users[fd].getUserNickName()));
        return;
    }
    else
    {
        speech = message.getTrailing();
        size_t posr = speech.find('\r');
        if (posr != std::string::npos)
            speech = speech.erase(posr, std::string::npos);
        size_t posn = speech.find('\n');
        if (posn != std::string::npos)
            speech = speech.erase(posn, std::string::npos);
    }
    
    if (target[0] == '#')
    {
        // std::cout << YELLOW << "TEST MSG #" << RESET << std::endl;
        std::map<std::string, Channel>::iterator it;
        it = channels.find(target.substr(1));
        if (it == channels.end())
        {
            //sendServerRpl(fd, ERR_NOSUCHNICK(users[fd].getUserNickName(), target));
            return;
        }
        else
        {
            broadcastToChannel(target.substr(1), speech, fd);
            // std::cout << "bonjour" << std::endl;
        }
    }
    else
    {
        // std::cout << YELLOW << "TEST MSG SOLO" << RESET << std::endl;
        std::map<int, User>::iterator itu;
        for (itu = users.begin(); itu != users.end(); ++itu)
        {
            // std::cout << BLUE << "TEST MSG USER - 1" << RESET << std::endl;
            // std::cout << BLUE << "TEST MSG USER TRGT :" << target << " testret" << RESET << std::endl;
            // std::cout << BLUE << "TEST MSG USER NAME :" << itu->second.getUserName() << " testret" << RESET << std::endl;
            // std::cout << BLUE << "TEST MSG USER NICKNAME :" << itu->second.getUserNickName() << " testret" << RESET << std::endl;
            if (itu->second.getUserName() == target || itu->second.getUserNickName() == target)
            {
                // std::cout << BLUE << "TEST MSG USER - 2" << RESET << std::endl;
                sendServerRpl(itu->second.getUserSockId(), RPL_NOTICE(users[fd].getUserNickName(), users[fd].getUserName(), target, speech));
                return;
            }
        }
        std::map<std::string, Channel>::iterator itc;
        itc = channels.find(target);
        if (itc != channels.end())
        {
            // std::cout << BLUE << "TEST MSG USER - 3" << RESET << std::endl;
            //target.insert(0, "#");
            broadcastToChannel(target, speech, fd);
        }
        else
        {
            //sendServerRpl(fd, ERR_NOSUCHNICK(users[fd].getUserNickName(), target));
            return;
        }
    }

}