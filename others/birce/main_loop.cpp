
#include "birce.hpp"

void	main_loop(t_env *e)
{
  while (1)
  {
    init_fd(e);
    do_select(e);
    check_fd(e);
  }
}
