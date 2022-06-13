#ifndef __IRCSERV_H__
#define __IRCSERV_H__

#include "MessageManager.hpp"
#include <sys/select.h>
#include <fcntl.h>
#include <netdb.h>
#include <stack>
#include <csignal>

class Ircserv {
public:
  int              port;
  // int              r;
  int              ircFd;
  fd_set           fdRead;
  fd_set           fdWrite;
  struct timeval    selectInterval;
  MessageManager    messenger;
  std::stack<User>  timeout;
  // std::stack<User>  quit;
  //std::stack<User>  passed;

  void              getOpt(int ac, char **av);
  void              srvCreate(int port);
  void              mainLoop();
  void              initFd();
  void              doSelect();
  void              checkFd();
  void              disposeCorpse();
};

// void sigIntHandler(int signum) {
//   std::cout << "call sigIntHandler: " << signum << std::endl;
//   (void)signum;
// }

// void sigQuitHandler(int signum) {
//   std::cout << "call sigQuitHandler: " << signum << std::endl;
//   (void)signum;
// }

void  Ircserv::getOpt(int ac, char **av) {
  switch (ac) {
    case 3:
      messenger.pass = av[2];
      --ac;
    case 2:
      port = atoi(av[1]);
      break;
    default:
      std::cerr << "Usage: " << av[0] << " PORT(required) PASS(optional)" << std::endl;
      exit(1);
  }

  // signal(SIGINT, sigIntHandler);
  // signal(SIGQUIT, sigQuitHandler);
}

void	Ircserv::srvCreate(int port) {
  int                 s;
  struct sockaddr_in  sin;
  struct protoent     *pe;
  int                 yes = 1;

  pe = (struct protoent*)Xv(NULL, getprotobyname("tcp"), (char *)"getprotobyname");
  s = X(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), (char *)"socket");

  X(-1, setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)), (char *)"setsockopt");
  X(-1, fcntl(s, F_SETFL, O_NONBLOCK), (char *)"fcntl");

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  X(-1, bind(s, (struct sockaddr*)&sin, sizeof(sin)), (char *)"bind");
  X(-1, listen(s, 42), (char *)"listen");

  ircFd = s;

  selectInterval.tv_sec = 1;			//3 second timeout
  selectInterval.tv_usec = 0;		//0 micro second timeout
}

void	Ircserv::mainLoop() {
  while (42) {
    initFd();
    doSelect();
    checkFd();
    disposeCorpse();
  }
}

void	Ircserv::initFd() {
  FD_ZERO(&fdRead);
  FD_ZERO(&fdWrite);

  //server
  FD_SET(ircFd, &fdRead);

  //clients
  std::map<int, User>::iterator uit;
  for (uit = messenger.users().begin(); uit != messenger.users().end(); ++uit) {
    FD_SET(uit->first, &fdRead);

    if (messenger.outMessages()[uit->first].length() > 0) {
      FD_SET(uit->first, &fdWrite);
    }
  }
}

void	Ircserv::doSelect() {

  int		max = ircFd;
  if (!messenger.users().empty()) {
    std::map<int, User>::reverse_iterator urit = messenger.users().rbegin();
    max = urit->first;
  }

  // r = select(max + 1, &fdRead, &fdWrite, NULL, NULL); //&loopInterval); //NULL, 0
  // r = select(max + 1, &fdRead, &fdWrite, NULL, &selectInterval); //&loopInterval); //NULL, 0
  select(max + 1, &fdRead, &fdWrite, NULL, &selectInterval); //&loopInterval); //NULL, 0
}

void	Ircserv::checkFd() {
  // if (r <= 0) return;

  //server
  if (FD_ISSET(ircFd, &fdRead)) {
    messenger.srvAccept(ircFd);
    // r--;
  }
  //else if (FD_ISSET(e->ircFd, &e->fdWrite))
    //server-to-server

  //clients
  std::map<int, User>::iterator uit = messenger.users().begin();
  time_t now = time(NULL);
  for (; uit != messenger.users().end(); ++uit) {
    if (uit->second.dead < now || uit->second.quit)
      timeout.push(uit->second);
    else {
      if (uit->second.alive < now)
        messenger.ping(uit->first);

      if (FD_ISSET(uit->first, &fdRead))
        messenger.clientRead(uit->first);
      
      if (FD_ISSET(uit->first, &fdWrite))
        messenger.clientWrite(uit->first);
    }
    // if (FD_ISSET(uit->first, &fdRead) || FD_ISSET(uit->first, &fdWrite))
    //   r--;
    // if (r == 0) break;
  }
}

void Ircserv::disposeCorpse() {
  while (timeout.size()) {
    
    if (!timeout.top().quit) {
      messenger.outMessages()[timeout.top().fd].append("timeout\n");
      messenger.clientWrite(timeout.top().fd);
    }

    messenger.kickUser(timeout.top().fd);
    timeout.pop();
  }
}

#endif
