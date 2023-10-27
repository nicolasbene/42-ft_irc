#include <iostream>
#include "error.h"
#include "server/Server.hpp"

int	main(int argc, char** argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return (1);
	}

	if (strlen(argv[2]) < 8)
	{
		std::cout << "Invalid password" << std::endl;
		return (1);
	}

	Server	server(argv[1], argv[2]);

	if (server.start() == -1)
	{
		std::cout << "Error while starting server" << std::endl;
		return (1);
	}

	


	return (0);
}
