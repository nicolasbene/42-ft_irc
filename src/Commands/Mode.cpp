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
        sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), message.getCommande()));
        return;
    }

    const std::string& channelName = message.getParameters()[0];
    const std::string& modeChange = message.getParameters()[1];

    std::map<std::string, Channel>::iterator itChannel = channels.begin();
    bool channelFound = false;

    // Parcourir manuellement la map pour rechercher le canal
    for (; itChannel != channels.end(); ++itChannel) {
        if (itChannel->first == channelName) {
            // Canal trouvé
            channelFound = true;
            break;
        }
    }

    if (!channelFound) {
        sendServerRpl(fd, ERR_NOSUCHCHANNEL(users[fd].getUserNickName(), channelName));
        return;
    }

    char modeType = modeChange[0];
    char modeFlag = modeChange[1];
    const std::string& targetUser = message.getParameters().size() > 2 ? message.getParameters()[2] : "";

    // Vérifier si le mode est '+o' (ajout d'un opérateur)
    if (modeType == '+' && modeFlag == 'o') {
        bool userFound = false;
        User targetUserObject;

        // Parcourir manuellement la map pour rechercher l'utilisateur
        std::map<int, User>::iterator itUser = users.begin();
        for (; itUser != users.end(); ++itUser) {
            if (itUser->second.getUserNickName() == targetUser) {
                // Utilisateur trouvé
                userFound = true;
                targetUserObject = itUser->second;
                break;
            }
        }

        // Vérifiez si l'utilisateur a été trouvé
        if (userFound) {
            // Appeler la fonction addOperator de la classe Channel
            itChannel->second.addOperator(targetUserObject, targetUser);
            sendServerRpl(fd, RPL_UMODEIS(users[fd].getUserNickName(), modeChange));
        } else {
            sendServerRpl(fd, ERR_NOSUCHNICK(users[fd].getUserNickName(), targetUser));
        }
    } else {
        // Gérer les autres cas de mode
        sendServerRpl(fd, ERR_UNKNOWNMODE(users[fd].getUserNickName(), modeFlag));
    }
}
