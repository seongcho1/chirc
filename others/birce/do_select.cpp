
//#include <stdlib.h>
#include "birce.hpp"

void	do_select(t_env *e)
{
  e->r = select(e->max + 1, &e->fd_read, &e->fd_write, NULL, NULL);
}