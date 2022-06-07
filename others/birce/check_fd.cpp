
#include "birce.hpp"

void	check_fd(t_env *e)
{
	std::map<int, User *>::iterator uit;
	for (uit = e->commander.userMap().begin(); uit != e->commander.userMap().end(); ++uit) {
		if (FD_ISSET(uit->first, &e->fd_read)) {
			if (uit->second->type() == FD_SERV)
				e->commander.srv_accept(uit->first);
			else if (uit->second->type() == FD_CLIENT)
				e->commander.client_read(uit->first);
		}

		if (FD_ISSET(uit->first, &e->fd_write)) {
			//if (uit->second->type() == FD_SERV)
			//	server-to-server
			if (uit->second->type() == FD_CLIENT)
				e->commander.client_write(uit->first);
		}
		if (FD_ISSET(uit->first, &e->fd_read) || FD_ISSET(uit->first, &e->fd_write))
			e->r--;
	}
}
