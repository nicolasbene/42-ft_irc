#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
int main(int argc, char** argv)
{

	// Vérifie que le nombre d'arguments est correct.
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	// Vérifie que le numéro de port est valide.
	int port = atoi(argv[1]);
	if (port < 1024 || port > 65535) {
		std::cout << "Invalid port number" << std::endl;
		return 1;
	}

	// Vérifie que le mot de passe est valide.
	if (strlen(argv[2]) < 8) {
		std::cout << "Invalid password" << std::endl;
		return 1;
	}

	// Création de l'adresse du serveur
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	serverAddress.sin_addr.s_addr = inet_addr("hello.com");
	//serverAddress.sin_addr.s_addr = INADDR_ANY;


	// Création du socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		std::cout << "Error while creating socket" << std::endl;
		return 1;
	}

	// Bind le socket à l'adresse du serveur
	if (bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
		std::cout << "Error while binding socket" << std::endl;
		return 1;
	}

	// Met le socket en mode écoute
	if (listen(serverSocket, 1) < 0) {
		std::cout << "Error while listening" << std::endl;
		return 1;
	}

	// Accepte la connexion entrante
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddress, &clientAddressLength);
	if (clientSocket < 0) {
		std::cout << "Error while accepting connection" << std::endl;
		return 1;
	}

	// Envoie le mot de passe au client
	if (send(clientSocket, argv[2], strlen(argv[2]), 0) < 0) {
		std::cout << "Error while sending password" << std::endl;
		return 1;
	}

	// Ferme le socket
	close(clientSocket);
	close(serverSocket);

	return 0;

}