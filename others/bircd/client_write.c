
#include <sys/socket.h>
#include <string.h>
#include "bircd.h"

void	client_write(t_env *e, int cs)
{
	send(cs, e->fds[cs].buf_write, strlen(e->fds[cs].buf_write), 0);
	e->fds[cs].buf_write[0] = 0;
}
