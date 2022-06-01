#include "Ircserv.hpp"
#include <iostream>
#include <csignal>

int main(int ac, char **av)
{

    if (ac != 2) {
		std::cout << "./ircserv <port>" << std::endl;
  		return 1;
    }

    Ircserv irc;

    irc.init(2130706433, atoi(av[1])); //127.0.0.1 6667

	while(42) {
	    irc.doInTheLoop();
	}

    return (0);
}
