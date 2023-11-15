#include "Client.hpp"
#include <iostream>

void Client::handleMessage(std::string message) {
    // Example: Print the received message
    std::cout << "Received message: " << message << std::endl;
    // You can implement more complex message handling logic here.

}

void Client::handleInteraction() {
	// Example: Send a message to the client
	std::string message = "Hello from the server!";
	// send(message);
	// You can implement more complex interaction logic here.
}
