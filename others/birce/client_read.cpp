#include "birce.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>


void	client_read(t_env *e, int cs)
{
	e->commander.client_read(cs);

	/*
	int		r;
	char	buf_read[BUF_SIZE + 1];

	r = recv(cs, buf_read, BUF_SIZE, 0);
	if (r <= 0) {
		close(cs);
		clean_fd(&e->fds[cs]);
		printf("client #%d gone away\n", cs);
	} else {
		if (r == BUF_SIZE)
			buf_read[r-1] = NEWLINE;
		buf_read[r] = 0;
		X(-1, e->commander.push_and_execute(cs, std::string(buf_read)),
			(char *)"e->commander.push_and_execute");
	}
	*/
}
