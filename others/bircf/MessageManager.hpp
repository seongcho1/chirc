#ifndef MessageManager_HPP
#define MessageManager_HPP

#include "StringUtils.hpp"
#include "Models.hpp"
#include "fd.hpp"
//#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <vector>
#include <map>

class MessageManager {

private:
	std::map<int, User>							users_;
	std::map<int, std::string>			in_messages_;
	std::map<int, std::string>			out_messages_;
	std::map<std::string, Channel>	channels_;

	typedef void(MessageManager::*FuncPtr)(int cs, std::vector<std::string> paramsVec, std::string trailing);

	std::map<std::string, FuncPtr>	functionCallMap_;
	void	registerFunctions();
	void	PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
	void	SELFMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
	void	PUBLICMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
	MessageManager(MessageManager const &) 						{}
	MessageManager &operator=(MessageManager const &) { return *this; }

public:

	MessageManager()	{ registerFunctions(); }
	~MessageManager();
	void	clean_fd(int cs);

	std::map<int, User>					&users() 						{ return users_; }
	std::map<int, std::string>	&inMessages() 			{ return in_messages_; }
	std::map<int, std::string>	&outMessages() 		{ return out_messages_; }
	std::size_t									inSize() const 		{ return in_messages_.size(); }
	std::size_t									outSize() const 		{ return out_messages_.size(); }

	int												append_and_execute(int cs, std::string message);
	void											append(int cs, std::string message);
	void											executeMessages(int cs);
	std::vector<std::string>	splitMessages(int cs, bool bSkipLast = true, bool bClearMessages = true);
	//tried to use terms in https://datatracker.ietf.org/doc/html/rfc2812#section-2.3.1
	void											executeMessage(int cs, std::string message);
	void											srvAccept(int s);
	void											clientRead(int cs);
	void											clientWrite(int cs)	{ users_[cs].fdWriteBy(out_messages_[cs]); }
};

/*
std::ostream& operator<<(std::ostream& out, const MessageManager& c) {
	return out << "MessageManager (map size=" << c.size() << ")";
}
*/

#endif
