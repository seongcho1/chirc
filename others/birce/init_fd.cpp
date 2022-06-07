#include "birce.hpp"
#include <string.h>
#include <sys/select.h>

void	init_fd(t_env *e) {

	FD_ZERO(&e->fd_read);
	FD_ZERO(&e->fd_write);

	std::map<int, User *>::iterator uit;
	for (uit = e->commander.userMap().begin(); uit != e->commander.userMap().end(); ++uit) {

		FD_SET(uit->first, &e->fd_read);

		if (e->commander.out_commands()[uit->first].length() > 0) {
			FD_SET(uit->first, &e->fd_write);
		}
	}
}
