#ifndef Ft_irc_HPP
#define Ft_irc_HPP

#include "User.hpp"
#include <iostream>
#include <map>


//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
//#include <netdb.h>

#include <sys/socket.h>
//#include <sys/select.h>
#include <netinet/in.h>

class Ft_irc {

  private:

	struct sockaddr_in			servaddr_;
	int							ircfd_;

	fd_set curr_sock_, cpy_read_, cpy_write_;
	char msg_[42];
	char str_[42*4096], tmp_[42*4096], buf_[42*4096 + 42];


	std::map<int, User *>		userMap;
	//std::map<int, Channel>	channelMap;

	Ft_irc(Ft_irc const &other);
	Ft_irc &operator=(Ft_irc const &other);

  public:

	typedef std::map<int, User *>::iterator iterator;
	typedef std::pair<int, User *> pair;


	Ft_irc();
	Ft_irc(unsigned long addr, unsigned short  port);

	//Ft_irc(Ft_irc const &other);
	//Ft_irc &operator=(Ft_irc const &other);
	~Ft_irc();


	void	init(unsigned long addr, unsigned short  port);
	void	fatal();

	void	loop(); //listen, accept, read, write

	void	sendToAll(int fd, char *str_req);
	void	exchangeMsg(int fd);

	int		getMaxUserFd();
	int		getUserId(int const &userfd);

	void 	connectUser();
	void	addUser(std::string userinfo);
	void	addUser(User user);
	int		addUser(int const &userfd);

	void	delUser(std::string userinfo);
	void	delUser(User user);
	int		delUser(int const &userfd);

	void	sendMsg(std::string nick, std::string msg);
	void	ex_msg(int fd);

	//void launchSpell(std::string str, ATarget const &target);
};


#endif
