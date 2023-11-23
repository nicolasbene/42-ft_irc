# include "Server.hpp"

// bool Server::userExistUserName(std::string user)
// {
// 	std::map<int, User>::iterator it;
// 	for (it = users.begin(); it != users.end(); it++)
// 	{
// 		if (it->second.getUserUserName() == user)
// 			return (true);
// 	}
// 	return (false);
// }

// std::string Server::changeUserName(std::string nickNameTochange, std::string NameTochange, int fd)
// {
//     int i = 1;
//     std::string tmp(NameTochange);

//     while (userExistUserName(tmp))
//     {
//         tmp = incrementString(NameTochange, i);
//         i++;
//     }
//     return (tmp)
// }

std::string incrementString(const std::string& baseString, int number) 
{
    std::ostringstream oss;
    oss << number;
    std::string numberString = oss.str();

    std::string result = baseString + numberString;

    return result;
}

std::string Server::changeNickname(std::string nickNameTochange, std::string NameTochange, int fd)
{
    int i = 1;
    std::string tmp(nickNameTochange);

    while (userExistName(tmp))
    {
        tmp = incrementString(nickNameTochange, i);
        i++;
    }
    sendServerRpl(fd, RPL_NICK(nickNameTochange, NameTochange, tmp));
    return (tmp);
}
