#include "Ft_irc.hpp"
#include <unistd.h>



int main(int ac, char **av)
{

    if (ac != 2) {
        write(2, "Wrong number of arguments\n", strlen("Wrong number of arguments\n"));
        exit(1);
    }

    Ft_irc irc;


    irc.init(2130706433, atoi(av[1])); //127.0.0.1 6667
    irc.loop();
    return (0);
}
