
#include "birce.hpp"

void	check_fd(t_env *e)
{
	if (e->r <= 0) return;

	//server
	if (FD_ISSET(e->irc_fd, &e->fd_read)) {
		e->commander.srv_accept(e->irc_fd);
		e->r--;
	}
	//else if (FD_ISSET(e->irc_fd, &e->fd_write))
		//server-to-server

	//clients
	std::map<int, User>::iterator uit;
	for (uit = e->commander.userMap().begin(); uit != e->commander.userMap().end(); ++uit) {
		if (FD_ISSET(uit->first, &e->fd_read))
			e->commander.client_read(uit->first);

		if (FD_ISSET(uit->first, &e->fd_write))
				e->commander.client_write(uit->first);

		if (FD_ISSET(uit->first, &e->fd_read) || FD_ISSET(uit->first, &e->fd_write))
			e->r--;
		if (e->r == 0) break;
	}
}
