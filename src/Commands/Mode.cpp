#include "Server.hpp"


Server::handleMode(Message message, int fd)
{
	User* user = users[fd];
	std::string command = message.getCommande();
	std::cout << "User " << user->getNickName() << " is trying to execute command " << command << std::endl;
	if (!command)
	{
		sendServerRpl(fd, ERR_UNKNOWNCOMMAND(user->getNickName(), command));
		return ;
	}

	if (message.getParameters().empty())
	{
		Log::warning() << "User " << user->getNickName() << " is trying to execute command " << command << " without target" << std::endl;
		sendServerRpl(fd, ERR_NEEDMOREPARAMS(user->getNickName(), command));
		return ;
	}

	Channel* target = channels[message.getParameters()[0]];
	if (!target)
	{
		Log::warning() << "User " << user->getNickName() << " is trying to execute command " << command << " on an unknown channel" << std::endl;
		sendServerRpl(fd, ERR_NOSUCHCHANNEL(user->getNickName(), message.getParameters()[0]));
		return ;
	}

	if (message.getParameters().size() == 1)
	{
		Log::info() << "User " << user->getNickName() << " is trying to execute command " << command << " on channel " << target->getName() << std::endl;
		sendServerRpl(fd, RPL_CHANNELMODEIS(user->getNickName(), target->getName(), target->getMode()));
		return ;
	}

	std::string mode = message.getParameters()[1];
	if (mode[0] != '+' && mode[0] != '-')
	{
		Log::warning() << "User " << user->getNickName() << " is trying to execute command " << command << " with an invalid mode" << std::endl;
		sendServerRpl(fd, ERR_UNKNOWNMODE(user->getNickName(), mode));
		return ;
	}

	if (!target->is_operator(*user))
	{
		Log::warning() << "User " << user->getNickName() << " is trying to execute command " << command << " on channel " << target->getName() << " without being operator" << std::endl;
		sendServerRpl(fd, ERR_CHANOPRIVSNEEDED(user->getNickName(), target->getName()));
		return ;
	}

	bool operand;

	for (std::string::const_iterator it = mode.begin() + 1; it != mode.end(); ++it)
	{
		if (*it == '+' || *it == '-')
			operand = (*it == '+');
		else if (*it == 'o')
			operator_mode(target, user, operand);
		else
		{
			Log::warning() << "User " << user->getNickName() << " is trying to execute command " << command << " with an invalid mode" << std::endl;
			sendServerRpl(fd, ERR_UNKNOWNMODE(user->getNickName(), mode));
			return ;
		}
	}

}

std::string Server::MODE_reply()
{
	std::string reply;
	reply += "MODE " + _target->getName() + " ";
	reply += _target->getMode() + " ";

	return reply;
}

void Server::operator_mode(Channel* target, User* user, bool operand)
{
	//faire un getClient(string nickName) dans serverm utiliser un iterateur sur le vector de user de channel avec getUserNickName()
}

