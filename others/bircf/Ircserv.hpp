#ifndef __IRCSERV_H__
#define __IRCSERV_H__

#include "MessageManager.hpp"
#include <sys/select.h>
#include <fcntl.h>
#include <netdb.h>
// #include <queue>
#include <csignal>

class Ircserv {
public:
  int              port;
  int              r;
  int              ircFd;
  fd_set           fdRead;
  fd_set           fdWrite;
  struct timeval    selectInterval;
  MessageManager    messenger;
  // std::queue<User>  timeout;
  std::vector<User> timeout;
  // std::stack<User>  quit;
  //std::stack<User>  passed;

  void              getOpt(int ac, char **av);
  void              srvCreate(int port);
  void              mainLoop();
  void              initFd();
  void              doSelect();
  void              checkFd();
  void              flushCorpse();
  void              disposeCorpse();
};

#endif
