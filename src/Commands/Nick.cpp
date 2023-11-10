/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwyseur <nwyseur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 15:12:23 by nwyseur           #+#    #+#             */
/*   Updated: 2023/11/10 17:04:26 by nwyseur          ###   ########.fr       */
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

void Server::setUserNickName(const std::vector<std::string>& result, int fd)
{
    if (result[1] == "")
    {
        sendServerRpl(fd, ERR_NONICKNAMEGIVEN(users[fd].getUserNickName()));
        return;
    }
    for (unsigned long int i = 0; i < result[1].size(); i++)
    {
        if (isalnum(result[1][i]) == 0 && (result[1][i] < 91 && result[1][i] > 95) && result[1][i] != 45 
            && result[1][i] != 123 && result[1][i] != 125)
        {
            sendServerRpl(fd, ERR_ERRONEUSNICKNAME(users[fd].getUserNickName(), result[1]));
            return;
        }
    }
    std::map<int, User>::iterator it;
    for (it = users.begin(); it != users.end(); ++it)
    {
        if (it->second.getUserNickName() == result[1] + "\r\n")
        {
            sendServerRpl(fd, ERR_NICKNAMEINUSE(users[fd].getUserNickName(), result[1]));
            return;
        }
    }
    users[fd].setOldNickName(users[fd].getUserNickName());
    users[fd].setUserNickName(result[1]);
    std::cout << GREEN << "Nickname of user " << users[fd].getUserSockId() << " updated to " << users[fd].getUserNickName() << RESET << std::endl;
    sendServerRpl(fd, RPL_NICK(users[fd].getOldNickName(), users[fd].getUserName(), users[fd].getUserNickName()));
}