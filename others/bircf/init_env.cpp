#include "birce.hpp"
#include <stdlib.h>
#include <sys/resource.h>

void		init_env(t_env *e)
{
	struct rlimit	rlp;

	//maxfd was used for looping of *fds array, but keep it just in case we need it later
	X(-1, getrlimit(RLIMIT_NOFILE, &rlp), (char *)"getrlimit");
	e->maxfd = rlp.rlim_cur;

}
