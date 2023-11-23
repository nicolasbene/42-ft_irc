#include "Server.hpp"

void Server::handleMode(Message message, int fd)
{
	User& user = users[fd];
	
	// lors de la premiere connexion : getParameters()[0] = user.getUserNickName() et non pas le channel
	if (message.getParameters()[0] == user.getUserNickName())
	{
		// ne rien faire
		return ;
	}

	std::string command = message.getCommande();
	std::cout << "User " << user.getUserNickName() << " is trying to execute command " << command << std::endl;

	if (command.empty())
	{
		sendServerRpl(fd, ERR_UNKNOWNCOMMAND(user.getUserNickName(), command));
		return ;
	}

	if (message.getParameters().empty())
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command " << command << " without target" << std::endl;
		sendServerRpl(fd, ERR_NEEDMOREPARAMS(user.getUserNickName(), command));
		return ;
	}

	// ---------------------------------------------- //
	std::string channel = message.getParameters()[0];
	Log::info() << "channel before = " << channel << std::endl;
	//channel ne peut que commencer par #
	if (channel[0] != '#')
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command " << command << " with an invalid channel name" << std::endl;
		sendServerRpl(fd, ERR_NOSUCHCHANNEL(user.getUserNickName(), channel));
		return ;
	}

    size_t posr = channel.find('\r');//ici
    if (posr != std::string::npos)
    {
        channel = channel.erase(posr, std::string::npos);
    }
    size_t posn = channel.find('\n');
    if (posn != std::string::npos)
    {
        channel = channel.erase(posn, std::string::npos);
    }
	if (channel.empty() || channel.find('#') == channel.npos)
    {
        sendServerRpl(fd, ERR_NEEDMOREPARAMS(users[fd].getUserNickName(), message.getCommande()));
        return;
    }
    channel.erase(channel.find("#"), 1); // ici
	std::cout << "channelName = " << channel << std::endl;

	Channel& target = channels[channel];
	if (target.getName().empty())
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command " << command << " on an unknown channel" << std::endl;
		sendServerRpl(fd, ERR_NOSUCHCHANNEL(user.getUserNickName(), message.getParameters()[0]));
		return ;
	}

	if (message.getParameters().size() == 1)
	{
		Log::info() << "User " << user.getUserNickName() << " is trying to execute command " << command << " on channel " << target.getName() << std::endl;
		sendServerRpl(fd, RPL_CHANNELMODEIS(user.getUserNickName(), target.getName(), target.getChannelMode()));
		return ;
	}

	std::string mode = message.getParameters()[1];
	Log::info() << "mode = " << mode << std::endl;
	if (mode[0] != '+' && mode[0] != '-')
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command " << command << " with an invalid mode" << std::endl;
		sendServerRpl(fd, ERR_UNKNOWNMODE(user.getUserNickName(), mode));
		return ;
	}

	posr = mode.find('\r');//ici
    if (posr != std::string::npos)
    {
        mode = mode.erase(posr, std::string::npos);
    }
    posn = mode.find('\n');
    if (posn != std::string::npos)
    {
        mode = mode.erase(posn, std::string::npos);
    }

	// if (!target->is_operator(*user))
	// {
	// 	Log::warning() << "User " << user.getUserNickName() << " is trying to execute command " << command << " on channel " << target->getName() << " without being operator" << std::endl;
	// 	sendServerRpl(fd, ERR_CHANOPRIVSNEEDED(user.getUserNickName(), target->getName()));
	// 	return ;
	// }

	bool operand;

	//mode = +o

	for (std::string::const_iterator it = mode.begin() + 1; it != mode.end(); ++it)
	{
		if (*it == '+' || *it == '-')
			operand = (*it == '+');
		else if (*it == 'o')
			operator_mode(target, user, operand, message);
		else
		{
			Log::warning() << "User " << user.getUserNickName() << " is trying to execute command " << command << " with an invalid mode" << std::endl;
			sendServerRpl(fd, ERR_UNKNOWNMODE(user.getUserNickName(), mode));
			return ;
		}
	}
	return ;
}

// std::string Server::MODE_reply()
// {
// 	std::string reply;
// 	reply += "MODE " + _target->getName() + " ";
// 	reply += _target->getMode() + " ";

// 	return reply;
// }

void Server::operator_mode(Channel& channel, User& user, bool operand, Message& message)
{
	Log::info() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << std::endl;

	// Vérifiez que message.getParameters() a au moins 3 éléments avant d'accéder à l'index 2
    if (message.getParameters().size() < 3)
    {
        Log::warning() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " but without target user" << std::endl;
        sendServerRpl(user.getUserSockId(), ERR_NEEDMOREPARAMS(user.getUserNickName(), message.getCommande()));
        return ;
    }

	std::string targetUserNickName = message.getParameters()[2];
	Log::info() << "targetUserNickName = " << targetUserNickName << std::endl;
	if (targetUserNickName.empty())
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " but without target user" << std::endl;
		sendServerRpl(user.getUserSockId(), ERR_NEEDMOREPARAMS(user.getUserNickName(), message.getCommande()));
		return ;
	}


	else if (!channel.isUserInMap(users, targetUserNickName))
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " but target user " << targetUserNickName << " is not in channel" << std::endl;

		sendServerRpl(user.getUserSockId(), ERR_USERNOTINCHANNEL(user.getUserNickName(), targetUserNickName, channel.getName()));
		return ;
	}

	else if (operand && !channel.is_operator(users[getUserIdByNickName(targetUserNickName)]))
	{
		Log::info() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " and target user " << targetUserNickName << std::endl;
		channel.setChannelOperator(users[getUserIdByNickName(targetUserNickName)]);
		sendServerRpl(user.getUserSockId(), RPL_UMODEIS(user.getUserNickName(), targetUserNickName, "+o"));
		return ;
	}

	// else if (operand && channel.is_operator(users[users[targetUserNickName].getUserSockId()]))
	// {
	// 	Log::info() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " and target user " << targetUserNickName << std::endl;
	// 	channel.setChannelOperator(users[users[targetUserNickName].getUserSockId()]);
	// 	sendServerRpl(user.getUserSockId(), RPL_UMODEIS(user.getUserNickName(), targetUserNickName, "+o"));
	// 	return ;
	// }
	
	// // else if (!operand && channel.is_operator(users[users[targetUserNickName].getUserSockId()]))
	// // {
	// // 	Log::info() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " and target user " << targetUserNickName << std::endl;
	// // 	channel.removeChannelOperator(users[users[targetUserNickName].getUserSockId()]);
	// // 	sendServerRpl(user.getUserSockId(), RPL_UMODEIS(user.getUserNickName(), targetUserNickName, "-o"));
	// // 	return ;
	// // }

	else
	{
		Log::warning() << "User " << user.getUserNickName() << " is trying to execute command MODE on channel #" << channel.getName() << " with operand " << operand << " and target user " << targetUserNickName << std::endl;
		sendServerRpl(user.getUserSockId(), ERR_CHANOPRIVSNEEDED(user.getUserNickName(), channel.getName()));
		return ;
	}
}

