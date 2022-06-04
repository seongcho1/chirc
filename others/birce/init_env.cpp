
#include <stdlib.h>
#include <sys/resource.h>
#include "bircd.hpp"

void		init_env(t_env *e)
{
  int		i;
  struct rlimit	rlp;

  X(-1, getrlimit(RLIMIT_NOFILE, &rlp), (char *)"getrlimit");
  e->maxfd = rlp.rlim_cur;
  e->fds = (t_fd*)Xv(NULL, malloc(sizeof(*e->fds) * e->maxfd), (char *)"malloc");
  i = 0;
  while (i < e->maxfd)
  {
    clean_fd(&e->fds[i]);
    i++;
  }
  e->commander.link2fds(e->fds, e->maxfd);
}
