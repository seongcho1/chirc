#ifndef BIRCE_HPP
#define BIRCE_HPP

#include "fd.hpp"
#include "MessageManager.hpp"
#include <sys/select.h>
#include <fcntl.h>
//#include <sys/socket.h>
#include <netdb.h>

#define Xv(err,res,str)		(SS::x_void(err,res,str,(char *)__FILE__,__LINE__))
#define X(err,res,str)		(SS::x_int(err,res,str,(char *)__FILE__,__LINE__))
//#define MAX(a,b)			((a > b) ? a : b)

class	Ircserv {
  public:
	int						port;
	//char					*passwd;

	int						r;
	int						irc_fd;
	fd_set					fd_read;
	fd_set					fd_write;

	MessageManager			messenger;

	void	init_env();
	void	get_opt(int ac, char **av);
	void	srv_create(int port);
	void	main_loop();
	void	init_fd();
	void	do_select();
	void	check_fd();
};


void	Ircserv::get_opt(int ac, char **av) {
	//need to add passwd
	if (ac != 2) {
		std::cerr << stderr << "Usage: " << av[0] << " port" << std::endl;
		exit(1);
	}
	port = atoi(av[1]);
	//passwd = av[2];
}

void	Ircserv::srv_create(int port) {
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

	irc_fd = s;
}

void	Ircserv::main_loop() {
	while (42) {
		init_fd();
		do_select();
		check_fd();
	}
}

void	Ircserv::init_fd() {
	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);

	//server
	FD_SET(irc_fd, &fd_read);

	//clients
	std::map<int, User>::iterator uit;
	for (uit = messenger.userMap().begin(); uit != messenger.userMap().end(); ++uit) {

		FD_SET(uit->first, &fd_read);

		if (messenger.out_messages()[uit->first].length() > 0) {
			FD_SET(uit->first, &fd_write);
		}
	}
}

void	Ircserv::do_select() {
	/*
	struct timeval timeout;
	timeout.tv_sec = 3;			//3 second timeout
	timeout.tv_usec = 0;		//0 micro second timeout
	*/

	int		max = irc_fd;
	if (!messenger.userMap().empty()) {
		std::map<int, User>::reverse_iterator urit = messenger.userMap().rbegin();
		max = urit->first;
	}

	r = select(max + 1, &fd_read, &fd_write, NULL, NULL); //&timeout); //NULL, 0
	//std::cout << "[select=" << e->r << "]" << std::endl;
}

void	Ircserv::check_fd() {

	if (r <= 0) return;

	//server
	if (FD_ISSET(irc_fd, &fd_read)) {
		messenger.srv_accept(irc_fd);
		r--;
	}
	//else if (FD_ISSET(e->irc_fd, &e->fd_write))
		//server-to-server

	//clients
	std::map<int, User>::iterator uit;
	for (uit = messenger.userMap().begin(); uit != messenger.userMap().end(); ++uit) {
		if (FD_ISSET(uit->first, &fd_read))
			messenger.client_read(uit->first);

		if (FD_ISSET(uit->first, &fd_write))
			messenger.client_write(uit->first);

		if (FD_ISSET(uit->first, &fd_read) || FD_ISSET(uit->first, &fd_write))
			r--;
		if (r == 0) break;
	}
}




#endif
