#include "Ircserv.hpp"

int	main(int ac, char **av) {
	Ircserv		ircserv;

	ircserv.getOpt(ac, av);
	ircserv.srvCreate(ircserv.port);
	ircserv.mainLoop();

	return 0;
}
