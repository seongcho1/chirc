#include "Ircserv.hpp"

  void sigIntHandler(int signum) {
    std::cout << "call sigIntHandler: " << signum << std::endl;
  }

  void sigQuitHandler(int signum) {
    std::cout << "call sigQuitHandler: " << signum << std::endl;
  }

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

  signal(SIGINT, sigIntHandler);
  signal(SIGQUIT, sigQuitHandler);
}

void	Ircserv::srvCreate(int port) {
  int                 s;
  struct sockaddr_in  sin;
  struct protoent     *pe;
  int                 yes = 1;

  pe = (struct protoent*)Xv(NULL, getprotobyname("tcp"), (char *)"getprotobyname");
  s = X(-1, socket(AF_INET, SOCK_STREAM, pe->p_proto), (char *)"socket");

  X(-1, setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)), (char *)"setsockopt");
  X(-1, fcntl(s, F_SETFL, O_NONBLOCK), (char *)"fcntl");

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  X(-1, bind(s, (sockaddr*)&sin, sizeof(sin)), (char *)"bind");
  X(-1, listen(s, 42), (char *)"listen");

  ircFd = s;
  messenger.ircfd = ircFd;

  selectInterval.tv_sec = 1;			//3 second timeout
  selectInterval.tv_usec = 0;		//0 micro second timeout
}

void	Ircserv::mainLoop() {
  while (42) {
    disposeCorpse();
    initFd();
    doSelect();
    checkFd();
    flushCorpse();
  }
}

void Ircserv::disposeCorpse(void) {

  while (!timeout.empty()) {
    int fd = timeout.back().fd;
    std::string nick = timeout.back().nick;
    messenger.users().erase(fd);
    messenger.nickFdPairs().erase(nick);
    close(fd);
    timeout.pop_back();
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

    if (messenger.users()[uit->first].wbuff.length() > 0) {
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

  r = select(max + 1, &fdRead, &fdWrite, NULL, &selectInterval); //&loopInterval); //NULL, 0
}

void	Ircserv::checkFd() {
  if (r < 0) return;
  //server
  if (FD_ISSET(ircFd, &fdRead)) {
    messenger.srvAccept(ircFd);
  }
  //else if (FD_ISSET(e->ircFd, &e->fdWrite))
    //server-to-server

  //clients
  std::map<int, User>::iterator uit = messenger.users().begin();
  time_t now = time(NULL);

  for (; uit != messenger.users().end(); ++uit) {
    if (uit->second.dead < now || uit->second.quit) {
      uit->second.quit = true;
      timeout.push_back(uit->second);
    }
    else {
      if (uit->second.alive < now)
        messenger.ping(uit->first);
      if (FD_ISSET(uit->first, &fdRead))
        messenger.clientRead(uit->first); // << 문제 지점;;;
      if (FD_ISSET(uit->first, &fdWrite))
        messenger.clientWrite(uit->first);
    }
  }
}

void Ircserv::flushCorpse(void) {
  for (int i = 0; i < (int)timeout.size(); ++i) {
    if (!timeout[i].quit) {
      messenger.users()[timeout[i].fd].appendWbuff(std::string().append("timeout").append(NEWLINE));
      messenger.clientWrite(timeout[i].fd);
    }
    messenger.kickUser(timeout[i].fd);
  }
}
