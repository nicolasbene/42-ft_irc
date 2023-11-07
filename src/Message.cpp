#include "Message.hpp"

Message::Message(const std::string& raw) : rawMessage(raw) {
	std::string::size_type pos = raw.find(' ');
	if (pos != std::string::npos) {
		verb = raw.substr(0, pos);
		std::string::size_type pos2 = raw.find(' ', pos + 1);
		if (pos2 != std::string::npos) {
			parameters.push_back(raw.substr(pos + 1, pos2 - pos - 1));
			parameters.push_back(raw.substr(pos2 + 1));
		} else {
			parameters.push_back(raw.substr(pos + 1));
		}
	} else {
		verb = raw;
	}
}