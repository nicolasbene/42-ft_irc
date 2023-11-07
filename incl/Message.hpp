#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <vector>

class Message {
private:
	std::string rawMessage;
	std::string verb;
	std::vector<std::string> parameters;

public:
	Message(const std::string& raw);

	std::string getVerb() const {
		return verb;
	}

	std::vector<std::string> getParameters() const {
		return parameters;
	}
};

#endif