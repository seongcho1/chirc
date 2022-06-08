
#include "birce.hpp"

void	main_loop(t_env *e) {
	while (42) {
		init_fd(e);
		do_select(e);
		check_fd(e);
	}
}
