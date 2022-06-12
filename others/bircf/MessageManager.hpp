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
  // std::map<int, User>          reqAuthenticates_;
  std::map<int, User>             users_;
  std::map<int, std::string>      inMessages_;
  std::map<int, std::string>      outMessages_;
  std::map<std::string, Channel>  channels_;
  std::map<std::string, int>      nickFdPair_;
  std::map<int, std::string>      replies_;

	typedef void(MessageManager::*FuncPtr)(int cs, std::vector<std::string> paramsVec, std::string trailing);

  std::map<std::string, FuncPtr>	functionCallMap_;
  int   initReplies(std::string configFile = "./reply.cfg");
  void  registerFunctions();
  void  PASS(int cs, std::vector<std::string> paramsVec, std::string);
  void  NICK(int cs, std::vector<std::string> paramsVec, std::string);
  void  USER(int cs, std::vector<std::string> paramsVec, std::string);
  void  PONG(int cs, std::vector<std::string>, std::string trailing);

  void  QUIT(int cs, std::vector<std::string>, std::string);
  void  JOIN(int cs, std::vector<std::string> paramsVec, std::string);
  void  PART(int cs, std::vector<std::string> paramsVec, std::string);
  void  LIST(int cs, std::vector<std::string> paramsVec, std::string);
  void  NAMES(int cs, std::vector<std::string> paramsVec, std::string);
  void  KICK(int cs, std::vector<std::string> paramsVec, std::string);
  void  MODE(int cs, std::vector<std::string> paramsVec, std::string);
  void  INVITE(int cs, std::vector<std::string> paramsVec, std::string);
  void  TOPIC(int cs, std::vector<std::string> paramsVec, std::string);

  void  PRIVMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
  void  PRIVMSGHelper(int cs, const std::string& msgto, const std::string& trailing);
  void  SELFMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
  void  PUBLICMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
  void  TESTMSG(int cs, std::vector<std::string> paramsVec, std::string trailing);
  void  reply(int cs, int code, std::string command, std::vector<std::string> paramsVec, std::string trailing);

  std::vector<std::string> namesVec(std::set<int> fds);
  std::string namesVec_TEST(std::set<int> fds);

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

  std::map<int, User>         &users()            { return users_; }
  // std::map<int, User>       &authenticates()   { return reqAuthenticates_; }
  std::map<int, std::string>  &inMessages()       { return inMessages_; }
  std::map<int, std::string>  &outMessages()      { return outMessages_; }
  std::size_t                 inSize() const      { return inMessages_.size(); }
  std::size_t                 outSize() const     { return outMessages_.size(); }

  void                        executeMessages(int cs);
  std::vector<std::string>    splitMessages(int cs, bool bSkipLast = true, bool bClearMessages = true);
  void                        executeMessage(int cs, std::string message);
  void                        srvAccept(int s);
  void                        clientRead(int cs);
  // void                     authRead(int cs);
  // void                     authWrite(int cs)	{ reqAuthenticates_[cs].authWrite(outMessages_[cs]); }
  void                        clientWrite(int cs)	{ users_[cs].clientWrite(outMessages_[cs]); }
  void                        kickUser(int cs);
  bool                        isUniqueNick(int cs, std::string &nick);
  void                        ping(int cs);
  // User                     &anyUser(int cs);
  std::string                 prefix(int cs) { return users_[cs].prefix(); }
};

/*
std::ostream& operator<<(std::ostream& out, const MessageManager& c) {
  return out << "MessageManager (map size=" << c.size() << ")";
}
*/


#endif
