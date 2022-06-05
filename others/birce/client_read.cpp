#include "birce.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>


void	client_read(t_env *e, int cs)
{
	int		r;
	char	buf_write[BUF_SIZE + 1];

	r = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
	if (r <= 0) {
	close(cs);
	clean_fd(&e->fds[cs]);
	printf("client #%d gone away\n", cs);
	} else {
	e->fds[cs].buf_read[r] = 0;
	sprintf(buf_write, "%s", e->fds[cs].buf_read);

	//e->commander.push_and_execute(cs, std::string(buf_write);
	X(-1, e->commander.push_and_execute(cs, std::string(buf_write)),
		(char *)"e->commander.push_and_execute");
	}
}
