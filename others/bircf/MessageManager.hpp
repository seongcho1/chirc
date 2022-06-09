#ifndef __MESSAGE_MANAGER_HPP__
#define __MESSAGE_MANAGER_HPP__

#include "StringUtils.hpp"
#include "Models.hpp"
#include "reply.hpp"
//#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <vector>
#include <map>

class MessageManager {

private:
	std::map<int, User>							reqAuthenticates_;
	std::map<int, User>							users_;
	std::map<int, std::string>			inMessages_;
	std::map<int, std::string>			outMessages_;
	std::map<std::string, Channel>	channels_;
	std::map<std::string, int>			nickFdPair_;

	typedef void(MessageManager::*FuncPtr)(int cs, std::vector<std::string> paramsVec, std::string trailing);

	std::map<std::string, FuncPtr>	functionCallMap_;
	void	registerFunctions();
	void	PASS(int cs, std::vector<std::string> paramsVec, std::string);
	void	NICK(int cs, std::vector<std::string> paramsVec, std::string);
	void	USER(int cs, std::vector<std::string> paramsVec, std::string);
	void	reply(int cs, int code, std::vector<std::string> paramsVec, std::string trailing);
	void	PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
	void	SELFMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
	void	PUBLICMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
	void	TESTMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);


	MessageManager(MessageManager const &) 						{}
	MessageManager &operator=(MessageManager const &) { return *this; }

public:

	MessageManager()	{ registerFunctions(); }
	~MessageManager();
	void	fdClean(int cs);
	std::string 								pass;

	std::map<int, User>					&users() 						{ return users_; }
	std::map<int, User>					&authenticates()		{ return reqAuthenticates_; }
	std::map<int, std::string>	&inMessages() 			{ return inMessages_; }
	std::map<int, std::string>	&outMessages() 			{ return outMessages_; }
	std::size_t									inSize() const 			{ return inMessages_.size(); }
	std::size_t									outSize() const 		{ return outMessages_.size(); }

	void											executeMessages(int cs);
	std::vector<std::string>	splitMessages(int cs, bool bSkipLast = true, bool bClearMessages = true);
	//tried to use terms in https://datatracker.ietf.org/doc/html/rfc2812#section-2.3.1
	void											executeMessage(int cs, std::string message);
	void											srvAccept(int s);
	void											authRead(int cs);
	void											clientRead(int cs);
	void											authWrite(int cs)	{ reqAuthenticates_[cs].clientWrite(outMessages_[cs]); }
	void											clientWrite(int cs)	{ users_[cs].clientWrite(outMessages_[cs]); }
	void											kickUser(int cs);
	bool 											isUniqueNick(int cs, std::string &nick);
	User											&anyUser(int cs);
};

/*
std::ostream& operator<<(std::ostream& out, const MessageManager& c) {
	return out << "MessageManager (map size=" << c.size() << ")";
}
*/

#endif
