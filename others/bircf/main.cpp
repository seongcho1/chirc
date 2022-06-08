
#include "Ircserv.hpp"

int	main(int ac, char **av) {
	Ircserv		ircserv;

	ircserv.get_opt(ac, av);
	ircserv.srv_create(ircserv.port);
	ircserv.main_loop();

	return 0;
}
