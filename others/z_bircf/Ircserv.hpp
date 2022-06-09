#ifndef __IRCSERV_HPP__
#define __IRCSERV_HPP__

#include "MessageManager.hpp"
#include <sys/select.h>
#include <fcntl.h>
//#include <sys/socket.h>
#include <netdb.h>
#include <stack>

class	Ircserv {
public:
	int						port;
	int						r;
	int						ircFd;
	fd_set					fdRead;
	fd_set					fdWrite;

	MessageManager	messenger;

	void	getOpt(int ac, char **av);
	void	srvCreate(int port);
	void	mainLoop();
	void	initFd();
	void	doSelect();
	void	checkFd();
	void 	authenticate();
};


void	Ircserv::getOpt(int ac, char **av) {
	switch (ac) {
		case 3:
			messenger.pass = av[2];
			--ac;
		case 2:
			port = atoi(av[1]);
			break;
		default:
			// std::cerr << stderr << "Usage: " << av[0] << " port" << std::endl;
			std::cerr << "Usage: " << av[0] << " PORT(required) PASS(optional)" << std::endl;
			exit(1);
	}
}

void	Ircserv::srvCreate(int port) {
	int			s;
	struct sockaddr_in	sin;
	struct protoent	*pe;
	int yes = 1;

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
}

void	Ircserv::mainLoop() {
	while (42) {
		initFd();
		doSelect();
		checkFd();
		authenticate();
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

	for (uit = messenger.authenticates().begin(); uit != messenger.authenticates().end(); ++uit) {
		FD_SET(uit->first, &fdRead);

		if (messenger.outMessages()[uit->first].length() > 0) {
			FD_SET(uit->first, &fdWrite);
		}
	}
}

void	Ircserv::doSelect() {
	/*
	struct timeval timeout;
	timeout.tv_sec = 3;			//3 second timeout
	timeout.tv_usec = 0;		//0 micro second timeout
	*/

	int		max = ircFd;
	if (!messenger.users().empty()) {
		std::map<int, User>::reverse_iterator urit = messenger.users().rbegin();
		max = urit->first;
	}

	if (!messenger.authenticates().empty()) {
		std::map<int, User>::reverse_iterator urit = messenger.authenticates().rbegin();
		max = MAX(urit->first, max);
	}

	r = select(max + 1, &fdRead, &fdWrite, NULL, NULL); //&timeout); //NULL, 0
	//std::cout << "[select=" << e->r << "]" << std::endl;
}

void	Ircserv::checkFd() {

	if (r <= 0) return;

	//server
	if (FD_ISSET(ircFd, &fdRead)) {
		messenger.srvAccept(ircFd);
		r--;
	}
	//else if (FD_ISSET(e->ircFd, &e->fdWrite))
		//server-to-server

	//clients
	std::map<int, User>::iterator uit = messenger.users().begin();
	for (; uit != messenger.users().end(); ++uit) {
		if (FD_ISSET(uit->first, &fdRead))
			messenger.clientRead(uit->first);

		if (FD_ISSET(uit->first, &fdWrite))
			messenger.clientWrite(uit->first);

		if (FD_ISSET(uit->first, &fdRead) || FD_ISSET(uit->first, &fdWrite))
			r--;
		if (r == 0) break;
	}
}

void Ircserv::authenticate() {
	std::map<int, User>::iterator uit = messenger.authenticates().begin();
	std::stack<User> passed;
	for (; uit != messenger.authenticates().end(); ++uit) {
		if (FD_ISSET(uit->first, &fdRead)) {
			messenger.authRead(uit->first);

			if (uit->second.authenticated == AUTH_LEVEL2)
				passed.push(uit->second);
		}

		if (FD_ISSET(uit->first, &fdWrite))
			messenger.authWrite(uit->first);
	}

	// go from reqAuthenticates_ to users_
	// Because can not erase elements while iterating through the loop
	while (passed.size()) {
		User &user = passed.top();
		messenger.users().insert(std::pair<int, User>(user.fd, user));
		messenger.authenticates().erase(user.fd);

/**/std::cout << user.host << "[" << user.fd << "] transfered to users_ container\n"; // test code

		passed.pop();
	}
}

#endif
