#include <iostream>
#include <sstream>
#include <string>

class UserInfo {
public:
    std::string nickname;
    std::string username;
    std::string password;
};

void parseUserString(const std::string& userString, UserInfo& userInfo) {
    std::istringstream iss(userString);
    std::string token;

    while (iss >> token) {
        if (token == "NICK" && iss >> userInfo.nickname) {
            // Do nothing, nickname is set
        } else if (token == "USER" && iss >> userInfo.username) {
            // Do nothing, username is set
        } else if (token == "PASS" && iss >> userInfo.password) {
            // Do nothing, password is set
        }
    }
}

int main() {
    std::string inputString = "NICK nicolas USER bob jo";
    UserInfo userInfo;

    parseUserString(inputString, userInfo);

    // Afficher les informations extraites
    std::cout << "Nickname: " << userInfo.nickname << std::endl;
    std::cout << "Username: " << userInfo.username << std::endl;
    std::cout << "Password: " << userInfo.password << std::endl;

    return 0;
}