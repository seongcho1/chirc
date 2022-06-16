#ifndef __MESSAGE_MANAGER_H__
#define __MESSAGE_MANAGER_H__

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
  std::map<int, User>             users_;
  std::map<std::string, Channel>  channels_;
  std::map<std::string, int>      nickFdPair_;
  std::map<int, std::string>      replies_;

	typedef void(MessageManager::*FuncPtr)(int cs, std::vector<std::string> paramsVec);

  std::map<std::string, FuncPtr>	functionCallMap_;
  int   initReplies(std::string configFile = "./reply.cfg");
  void  registerFunctions();
  void  PASS(int cs, std::vector<std::string> paramsVec);
  void  NICK(int cs, std::vector<std::string> paramsVec);
  void  USER(int cs, std::vector<std::string> paramsVec);
  void  PING(int cs, std::vector<std::string> paramsVec);
  void  PONG(int cs, std::vector<std::string> paramsVec);

  void  QUIT(int cs, std::vector<std::string> paramsVec);
  void  JOIN(int cs, std::vector<std::string> paramsVec);
  void  PART(int cs, std::vector<std::string> paramsVec);
  void  LIST(int cs, std::vector<std::string> paramsVec);
  void  AWAY(int cs, std::vector<std::string> paramsVec);
  void  KICK(int cs, std::vector<std::string> paramsVec);
  void  MODE(int cs, std::vector<std::string> paramsVec);
  void  NAMES(int cs, std::vector<std::string> paramsVec);
  void  TOPIC(int cs, std::vector<std::string> paramsVec);
  void  INVITE(int cs, std::vector<std::string> paramsVec);

  void  NOTICE(int cs, std::vector<std::string> paramsVec);
  void  PRIVMSG(int cs, std::vector<std::string> paramsVec);
  void  SENDMSG(int cs, std::vector<std::string> paramsVec, bool isNotice = false);
  void  PRIVMSGHelper(int cs, const std::string& msgto, const std::string& msg, bool isNotice);

  std::string  reply(int cs, int code, std::string command, std::vector<std::string>& paramsVec, bool bDirectDelivery = true);
  void  announceToChannel(int cs, std::string title, std::string message, bool withMe = false);
  void  announceToNeighbors(int cs, std::string message, bool withMe = false);
  void  announceOneUser(int cs, std::string message);
  // void  announceToSelf(int cs, std::string message);
  void  modeChannel(int cs, std::vector<std::string> paramsVec);
  void  modeUser(int cs, std::vector<std::string> paramsVec);
  std::string channelMemberToString(std::string channel);

  MessageManager(MessageManager const &) 						{}
  MessageManager &operator=(MessageManager const &) { return *this; }

public:

  MessageManager()	{
    X(-1, initReplies(), (char *)"initReplies");
    registerFunctions();
  }
  ~MessageManager();
  void	fdClean(int cs);
  std::string                 pass;
  int                         ircfd;

  std::map<int, User>         &users()            { return users_; }
  void                        executeMessages(int cs);
  std::vector<std::string>    splitMessages(int cs, bool bSkipLast = true, bool bClearMessages = true);
  void                        executeMessage(int cs, std::string message);
  void                        srvAccept(int s);
  void                        clientRead(int cs);
  void                        clientWrite(int cs)	{ users_[cs].clientWrite(); }
  void                        kickUser(int cs);
  bool                        isUniqueNick(int cs, std::string &nick);
  void                        ping(int cs);
  std::string                 hostmask(int cs) { return users_[cs].hostmask(); }
};

/*
std::ostream& operator<<(std::ostream& out, const MessageManager& c) {
  return out << "MessageManager (map size=" << c.size() << ")";
}
*/


#endif
