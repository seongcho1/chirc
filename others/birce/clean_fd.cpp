
#include <stdlib.h>
#include "birce.hpp"

void	clean_fd(t_fd *fd)
{
  fd->type = FD_FREE;
  //fd->fct_read = NULL;
  //fd->fct_write = NULL;
}
