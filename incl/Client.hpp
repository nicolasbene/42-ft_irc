#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <string>

class Client {
public:
	Client(int fd) : m_fd(fd) {}
	~Client() {}
	Client(std::string username, std::string nickname, std::string realname, std::string host)
		: m_username(username), m_nickname(nickname), m_realname(realname), m_host(host) {}

	std::string getUsername() const { return m_username; }
	std::string getNickname() const { return m_nickname; }
	std::string getRealname() const { return m_realname; }
	std::string getHost() const { return m_host; }
	int 		getFd() const { return m_fd; }

	void setUsername(std::string username) { m_username = username; }
	void setNickname(std::string nickname) { m_nickname = nickname; }
	void setRealname(std::string realname) { m_realname = realname; }
	void setHost(std::string host) { m_host = host; }

	void handleMessage(std::string message);
	void handleInteraction();

private:
	std::string m_username;
	std::string m_nickname;
	std::string m_realname;
	std::string m_host;
	int 		m_fd;
};

#endif