#include "Server.hpp"

void Server::sendPong(Message msg, int fd)
{
    if (msg.getParameters().empty() || !msg.getParameters()[0].size())
		return (sendServerRpl(fd, ERR_NOORIGIN(user_id(users[fd].getUserNickName(), users[fd].getUserName()), users[fd].getUserNickName())));
	return (sendServerRpl(fd, RPL_PONG(user_id(users[fd].getUserNickName(), users[fd].getUserName()), msg.getParameters()[0])));
}

//Le message PING est utilisé pour tester la présence d'un client actif à l'autre bout de la connexion. Un message PING est envoyé régulièrement si aucune activité n'est détectée sur une connexion. Si la connexion ne répond pas à la commande PING dans un certain délai, la connexion est fermée.

// Tout client qui reçoit un message PING doit répondre au <serveur1> (serveur qui a envoyé le message PING) aussi rapidement que possible, avec un message PONG approprié pour indiquer qu'il est toujours là et actif. Les serveurs ne doivent pas répondre aux commandes PING, mais se fier au PING dans l'autre sens pour indiquer que la connexion est toujours active. Si le paramètre <serveur2> est spécifié, le message PING lui est transmis.