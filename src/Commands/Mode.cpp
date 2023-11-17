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
#include "Numerical_reply.hpp"
#include "Message.hpp"

void Server::handleModeCommand(Message message, int fd) {
    if (message.getParameters().size() < 2) {
        sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), message.getCommand()));
        return;
    }

    const std::string& channelName = message.getParameters()[0];
    const std::string& modeChange = message.getParameters()[1];

    if (channelName[0] != '#') {
        sendServerRpl(fd, ERR_USERSDONTMATCH(users[fd].getUserNickName()));
        return;
    }

    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        sendServerRpl(fd, ERR_NOSUCHCHANNEL(users[fd].getUserNickName(), channelName));
        return;
    }

    if (!it->second.isOperator(users[fd].getUserNickName())) {
        sendServerRpl(fd, ERR_CHANOPRIVSNEEDED(users[fd].getUserNickName(), channelName));
        return;
    }

    char modeType = modeChange[0];
    char modeFlag = modeChange[1];
    const std::string& targetUser = message.getParameters().size() > 2 ? message.getParameters()[2] : "";

    switch (modeType) {
        case '+':
            switch (modeFlag) {
                case 'o':
                    it->second.addOperator(targetUser);
                    break;
                case 'k':
                    it->second.addKey(targetUser);
                    break;
                case 'l':
                    it->second.addLimit(targetUser);
                    break;
                case 't':
                    it->second.addTopic(targetUser);
                    break;
                case 'i':
                    it->second.addInviteOnly(targetUser);
                    break;
            }
            break;
        case '-':
            switch (modeFlag) {
                case 'o':
                    it->second.removeOperator(targetUser);
                    break;
                case 'k':
                    it->second.removeKey(targetUser);
                    break;
                case 'l':
                    it->second.removeLimit(targetUser);
                    break;
                case 't':
                    it->second.removeTopic(targetUser);
                    break;
                case 'i':
                    it->second.removeInviteOnly(targetUser);
                    break;
            }
            break;
        default:
            sendServerRpl(fd, ERR_UNKNOWNMODE(users[fd].getUserNickName(), modeFlag));
            return;
    }

    sendServerRpl(fd, RPL_MODE(users[fd].getUserNickName(), channelName, modeChange, targetUser));
}
