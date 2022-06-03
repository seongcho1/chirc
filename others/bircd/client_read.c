
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bircd.h"

void	client_read(t_env *e, int cs)
{
  int	r;
  int	i;

  r = recv(cs, e->fds[cs].buf_read, BUF_SIZE, 0);
  if (r <= 0)
  {
    close(cs);
    clean_fd(&e->fds[cs]);
    printf("client #%d gone away\n", cs);
  }
  else
  {
 	e->fds[cs].buf_read[r] = 0;
	i = 0;
    while (i < e->maxfd)
    {
      if ((e->fds[i].type == FD_CLIENT) && (i != cs))
		sprintf(e->fds[i].buf_write, "[from %d] %s", cs, e->fds[cs].buf_read);
        //send(i, e->fds[cs].buf_read, r, 0);
      i++;
    }
	//sprintf(e->fds[cs].buf_write, "%s", e->fds[cs].buf_read);
  }
}
