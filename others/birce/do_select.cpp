
//#include <stdlib.h>
#include "birce.hpp"
#include <iostream>
void	do_select(t_env *e) {
	/*
	struct timeval timeout;
	timeout.tv_sec = 3;			//3 second timeout
	timeout.tv_usec = 0;		//0 micro second timeout
	*/

	int		max = e->irc_fd;
	if (!e->commander.userMap().empty()) {
		std::map<int, User>::reverse_iterator urit = e->commander.userMap().rbegin();
		max = urit->first;
	}

	e->r = select(max + 1, &e->fd_read, &e->fd_write, NULL, NULL); //&timeout); //NULL, 0
	//std::cout << "[select=" << e->r << "]" << std::endl;
}
