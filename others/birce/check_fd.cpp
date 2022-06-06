
#include "birce.hpp"

void	check_fd(t_env *e)
{
	int	i;

	i = 0;
	while ((i < e->maxfd) && (e->r > 0)) {
		if (FD_ISSET(i, &e->fd_read)) {
			//e->fds[i].fct_read(e, i);
			if (e->fds[i].type == FD_SERV)
				srv_accept(e, i);
			else if (e->fds[i].type == FD_CLIENT)
				client_read(e, i);
		}
		if (FD_ISSET(i, &e->fd_write)) {
			//e->fds[i].fct_write(e, i);
			//if (e->fds[i].type == FD_SERV)
			//	server-to-server
			if (e->fds[i].type == FD_CLIENT)
				client_write(e, i);
		}
		if (FD_ISSET(i, &e->fd_read) || FD_ISSET(i, &e->fd_write))
			e->r--;
		i++;
	}
}
