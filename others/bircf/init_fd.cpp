#include "birce.hpp"
#include <string.h>
#include <sys/select.h>

void	init_fd(t_env *e) {

	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);

	//server
	FD_SET(e->irc_fd, &e->fd_read);

	//clients
	std::map<int, User>::iterator uit;
	for (uit = e->messenger.userMap().begin(); uit != e->messenger.userMap().end(); ++uit) {

		FD_SET(uit->first, &e->fd_read);

		if (e->messenger.out_messages()[uit->first].length() > 0) {
			FD_SET(uit->first, &e->fd_write);
		}
	}

}
